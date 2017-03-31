#include "GameRoom.h"
#include "server/comtimetag.h"
#include "message/WJNetRoomMessage.h"
#include "common/WJUserInfoBaseStruct.h"
#include "message/WJNetPtoR.h"
#include "common/commonfunction.h"
#include "common/QkLogger.h"
#include "GameServer.h"
#include "gsconfig.h"
#include "server/WJSSocketManage.h"

using namespace std;

namespace WJ
{
	extern CGameServer * g_pGameServer;

	CGameRoom::CGameRoom()
	{
	}

	CGameRoom::~CGameRoom()
	{
	}

	const Room_ManageInfoStruct* CGameRoom::GetRoom_ManageInfoStruct()
	{
		return &m_initRoomData;
	}

	CGameDeskManage* CGameRoom::GetCGameDeskManage()
	{
		return &m_GameDeskManage;
	}

	void CGameRoom::Init_Room(Room_ManageInfoStruct &initRoomData)
	{
		m_initRoomData = initRoomData;
	}

	///初始化函数
	bool CGameRoom::Init_subclass()
	{
		m_GameDeskManage.InitGameDesk(m_initRoomData.uDeskCount, m_initRoomData.uDeskPeople, &m_initRoomData, this);

		return true;
	}

	///取消初始化函数
	bool CGameRoom::UnInit_subclass()
	{
		return true;
	}

	///启动函数
	bool CGameRoom::Start_subclass()
	{
		return true;
	}

	///停止服务
	bool CGameRoom::Stop_subclass()
	{
		return true;
	}

	//更新房间信息给登陆服务器
	void CGameRoom::UpdateInfoToLS()
	{
		struct_data_pr_roominfo_update cmd;
		cmd.roomid = m_initRoomData.uRoomID;
		cmd.gameid = m_initRoomData.uGameID;
		cmd.contestid = m_initRoomData.uContestid;
		cmd.uOlineCount = g_pGameServer->GetCGameUserInfoManage()->GetGameRoomCount(m_initRoomData.uRoomID);
		int copyLen = min(sizeof(cmd.strIP), gsconfig()->ip_for_client.size());
		memcpy(cmd.strIP, gsconfig()->ip_for_client.c_str(), copyLen);
		cmd.strIP[copyLen] = '\0';
		cmd.uport = gsconfig()->port_for_client;
		std::memcpy(cmd.roomname, m_initRoomData.cRoomName, sizeof(cmd.roomname));

		g_pGameServer->SendData(g_pGameServer->GetSockidOfLoginServer(),
								&cmd, 
								sizeof(cmd), 
								e_pr_mess_roominfo, 
								e_pr_ass_roominfo_update, 
								0);
	}

	///SOCKET 关闭 （必须重载）
	bool CGameRoom::OnSocketClose(CWJSSocket *pSocket)
	{
		QK_InfoLog("sdp_test room 游戏服务器 OnSocketClose ");
		if (pSocket->GetType() == E_CONN_Client)
		{
			CGameUserInfo *puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfoEx(pSocket->GetSockID());
			if (nullptr != puser)
			{//必须是在线玩家才能进行处理				
				//从排队机中删除
				if (US_ContestRule_solo == m_initRoomData.uContestRule)
				{
					g_pGameServer->GetCQueueManage(m_initRoomData.uContestid)->Delete(puser);
				}

				UINT deskid = puser->GetUserData()->uDeskid;
				UINT uDeskStation = puser->GetUserData()->uDeskStation;
				if (ERROR_DESKID != deskid)
				{
					ISysDeskInterface* pdesk = m_GameDeskManage.At(deskid);
					if (nullptr != pdesk)
					{
						if (pdesk->IsPlayGame())
						{//在游戏中，则注册断线用户
							CGameUserInfo* pNetCutUser = g_pGameServer->GetCGameUserInfoManage()->RegNetCutUser(pSocket->GetSockID());
							if (nullptr != pNetCutUser)
							{//通知游戏内部玩家断线
								QK_InfoLog("sdp_tes room 通知游戏内部玩家断线。");
								pdesk->Sys_UserNetCut(uDeskStation, pNetCutUser);
								return true;
							}
						}
						else
						{//游戏未开始，则是直接离开桌子
							QK_InfoLog("sdp_test room 游戏未开始，则是直接离开桌子。");
							pdesk->Sys_UserLeftDesk(uDeskStation, puser);
						}
					}
				}

				if (US_ContestRule_solo == m_initRoomData.uContestRule)
				{
					//通知登陆服务器，玩家没有在该房间排队游戏
					struct_data_pr_userinfo_unplay cmd;
					cmd.userid = puser->GetUserData()->basedata.dwUserID;
					QK_InfoLog("sdp_test room 通知登陆服务器，玩家没有在该房间排队游戏。");
					g_pGameServer->SendData(g_pGameServer->GetSockidOfLoginServer(), &cmd, sizeof(struct_data_pr_userinfo_unplay), e_pr_mess_userinfo, e_pr_ass_userinfo_unplay, 0);
				}
				else if (US_ContestRule_team == m_initRoomData.uContestRule)
				{//团队赛，要告诉vs中的其他玩家
					g_pGameServer->RTeamVSSendData(puser->GetUserData()->basedata.dwUserID, nullptr, 0, e_gs_mess_user_action, e_gs_ass_user_action_team_out, puser->GetUserData()->basedata.dwUserID);
				}

				//完全清理玩家数据
				g_pGameServer->GetCGameUserInfoManage()->FreeUser(pSocket->GetSockID());

			}
			else
			{
				QK_InfoLog("sdp_err CGameRoom::OnSocketClose uSocketId=%d", pSocket->GetSockID());
			}
		}
		else if (pSocket->GetType() == E_CONN_LoginServer)
		{
			//不处理
		}

		return true;
	}


	///定时器消息 （必须重载）
	bool CGameRoom::OnTimerMessage(UINT uTimerID)
	{
		UINT deskId = (uTimerID & TIMER_DESKID_MASK) >> 8;
		if(deskId < m_GameDeskManage.GetCount())
		{
			m_GameDeskManage.At(deskId)->Sys_OnTimer(uTimerID);
		}
		else
		{
			return false;
		}

		return true;
	}

	//用户列表消息
	bool CGameRoom::OnSocketRead(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		switch (pNetHead->bMainID)
		{
		case e_gs_mess_user_list:
		{//用户列表消息
									return OnSocketRead_user_list(pSocket, pNetHead, pData, uSize);
		}break;
		case e_gs_mess_user_action:
		{//用户动作消息
									  return OnSocketRead_user_action(pSocket, pNetHead, pData, uSize);
		}break;
		case e_gs_mess_room:
		{//房间信息
							   return OnSocketRead_room(pSocket, pNetHead, pData, uSize);	   
		}break;
		case e_gs_mess_queue:
		{//排队消息
								return OnSocketRead_queue(pSocket, pNetHead, pData, uSize);
		}break;
		case e_gs_mess_game_frame:
		{//游戏框架消息
									 return OnSocketRead_game_frame(pSocket, pNetHead, pData, uSize);
		}break;
		case e_gs_mess_game_notify:
		{//游戏普通消息
									  return OnSocketRead_game_notify(pSocket, pNetHead, pData, uSize);
		}break;
		default:
			break;
		}

		return false;
	}

	//用户列表消息
	bool CGameRoom::OnSocketRead_user_list(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		switch (pNetHead->bAssistantID)
		{
		default:
			break;
		}

		return true;
	}

	//用户动作消息
	bool CGameRoom::OnSocketRead_user_action(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		//switch (pNetHead->bAssistantID)
		//{
		//default:
		//	break;
		//}

		return true;
	}

	//房间信息
	bool CGameRoom::OnSocketRead_room(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		switch (pNetHead->bAssistantID)
		{
		case e_gs_ass_room_chat:
		{
			RoomMessDate_room_chat* pCmd = (RoomMessDate_room_chat*)pData;
			if (nullptr == pCmd)
			{
				return false;
			}

			CGameUserInfo *puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfoEx(pSocket->GetSockID());
			if (nullptr == puser)
				return true;

			if (e_gs_hcid_gs_room_chat_for_room == pNetHead->bHandleCode)
			{//广播到房间内, 广播操作尽量少发
                SendDataBatch(pData, uSize, pNetHead->bMainID, pNetHead->bAssistantID, pNetHead->bHandleCode);
				return true;
			}
			else if (e_gs_hcid_gs_room_chat_for_desk == pNetHead->bHandleCode)
			{//组播到桌子
			UINT deskid = puser->GetUserData()->uDeskid;
				if (ERROR_DESKID != deskid)
				{
					ISysDeskInterface* pdesk = m_GameDeskManage.At(deskid);
					if (nullptr != pdesk)
					{
						pdesk->SendDeskData(pData, uSize, pNetHead->bMainID, pNetHead->bAssistantID, pNetHead->bHandleCode);
						return true;
					}
				}
			}
			else if (e_gs_hcid_gs_room_chat_for_player == pNetHead->bHandleCode)
			{//单播给个人									   
				CGameUserInfo* to_puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfo(pCmd->to_username);
				if (nullptr != to_puser)
				{
                    g_pGameServer->SendData(to_puser->GetSocketId(), pData, uSize, pNetHead->bMainID, pNetHead->bAssistantID, pNetHead->bHandleCode);
					return true;
				}
			}

			return true;
		}break;
		default:
			break;
		}

		return true;
	}

	//排队消息
	bool CGameRoom::OnSocketRead_queue(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		switch (pNetHead->bAssistantID)
		{
		default:
			break;
		}

		return true;
	}

	//游戏框架消息
	bool CGameRoom::OnSocketRead_game_frame(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		QK_InfoLog("sdp_test room 游戏框架消息");
		CGameUserInfo *puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfoEx(pSocket->GetSockID());
		if (nullptr == puser)
			return true;

		if (ERROR_DESKID == puser->GetUserData()->uDeskid || ERROR_DESKSTATION == puser->GetUserData()->uDeskStation)
		{//没有在桌子里面
			QK_InfoLog("sdp_test room 没有在桌子里面(%d,%d)。", puser->GetUserData()->uDeskid, puser->GetUserData()->uDeskStation);
			return false;
		}

		ISysDeskInterface* pdesk = m_GameDeskManage.At(puser->GetUserData()->uDeskid);
		if (nullptr == pdesk)
		{//桌子异常
			QK_InfoLog("sdp_test room 桌子异常。");
			return false;
		}

		return pdesk->Sys_HandleFrameMessage(puser->GetUserData()->uDeskStation, pNetHead, pData, uSize, pSocket->GetSockID());
	}

	//游戏普通消息
	bool CGameRoom::OnSocketRead_game_notify(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		CGameUserInfo *puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfoEx(pSocket->GetSockID());
		if (nullptr == puser)
			return true;

		if (ERROR_DESKID == puser->GetUserData()->uDeskid || ERROR_DESKSTATION == puser->GetUserData()->uDeskStation)
		{//没有在桌子里面
			return false;
		}

		ISysDeskInterface* pdesk = m_GameDeskManage.At(puser->GetUserData()->uDeskid);
		if (nullptr == pdesk)
		{//桌子异常
			return false;
		}

		return pdesk->Sys_HandleGameMessage(puser->GetUserData()->uDeskStation, pNetHead, pData, uSize, pSocket->GetSockID());
	}

	//批量发送函数
	void CGameRoom::SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT uAssistantID, UINT uHandleCode)
	{
		std::unordered_map<UINT, GameUserStruct*>* pMap = g_pGameServer->GetCGameUserInfoManage()->GetGameUserStruct_map();
		auto iter = pMap->begin(), end = pMap->end();
		for (; iter != end; ++iter)
		{
			if (usertype_online == iter->second->usertype
				&& m_initRoomData.uRoomID == iter->second->userinfo.GetUserData()->uRoomid)
			{
				g_pGameServer->SendData(iter->second->userinfo.GetSocketId(), pData, uSize, uMainID, uAssistantID, uHandleCode);
			}
		}
	}
}