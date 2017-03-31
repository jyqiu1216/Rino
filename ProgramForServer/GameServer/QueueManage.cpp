#include <atltime.h>
#include <algorithm>
#include "QueueManage.h"
#include "GameRoom.h"
#include "message/WJNetRoomMessage.h"
#include "common/commonfunction.h"
#include "gameserver.h"
#include "common/QkLogger.h"
#include "message/WJNetPtoR.h"

namespace WJ
{
    
    extern CGameServer *g_pGameServer;

	///////////////////////////////////////////////////////////////////////////////////
	CQueueManage::CQueueManage()
	{
		m_ucontestid = ERROR_CONTESTID;
		m_uContestRule = US_ContestRule_mission;
	}

	CQueueManage::~CQueueManage()
	{
		DeleteAll();
	}

	void CQueueManage::run()
	{
		m_QueueUserList.sort();

		switch (m_uContestRule)
		{
		case US_ContestRule_solo:
		{
								 run_RoomRule_solo();
		}break;
		case US_ContestRule_team:
		{
								 run_RoomRule_team();
		}break;
		default:
			break;
		}
	}

	//个人solo模式运行程序
	void CQueueManage::run_RoomRule_solo()
	{
		if (2 > m_QueueUserList.size())
		{//至少要两个人
			return;
		}

		//有没有房间、空桌子
		ISysDeskInterface* pDesk = nullptr;
		CGameRoom* pRoom = g_pGameServer->m_roomMgr.GetRoom_ForDeskid(m_ucontestid, &pDesk);
		if (nullptr == pDesk || nullptr == pRoom)
		{//没有房间、空桌子就不用匹配了
			return;
		}

		UINT max_palyers = pDesk->GetMaxPeople();
		if (0 == max_palyers)
		{//无人是不可以的
			return;
		}

		bool bOK = false;
		//人不够就不用匹配了
		while (max_palyers <= m_QueueUserList.size())
		{
			bOK = false;
			pRoom = g_pGameServer->m_roomMgr.GetRoom_ForDeskid(m_ucontestid, &pDesk);
			if (nullptr == pDesk || nullptr == pRoom)
			{
				return;
			}
			//取用户数据塞进桌子
			vector<QueueInfo_t>	QueueUservector;//指定加入房间的人
			QueueUservector.clear();
			auto iter = m_QueueUserList.begin();
			while (iter != m_QueueUserList.end())
			{
				CGameUserInfo * puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfo((*iter).m_dwUserID);
				if (nullptr == puser)
				{//先确认人是否还在线，在线就处理，不在线就丢弃
					m_userid2iter.erase(iter->m_dwUserID);
					iter = m_QueueUserList.erase(iter);
					continue;
				}

				QueueUservector.push_back((*iter));
				m_userid2iter.erase(iter->m_dwUserID);
				iter = m_QueueUserList.erase(iter);
				if (QueueUservector.size() == pDesk->GetMaxPeople())
				{//人数达到了
					bOK = true;
					QK_InfoLog("sdp_test room 分配成功的桌子与玩家数据========= (deskid=%d,%d,%d)。"
						, pDesk->GetDeskIndex(), QueueUservector.at(0).m_dwUserID, QueueUservector.at(1).m_dwUserID);
					break;
				}
			}//while (iter != m_QueueUserList.end())

			if (bOK)
			{//桌子有了，人也找好了，开始通知这些人进入桌子开赛
				for (UINT j = 0; j < QueueUservector.size(); ++j)
				{
					CGameUserInfo * puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfo(QueueUservector.at(j).m_dwUserID);
					//在这里才设置玩家所在房间id
					puser->GetUserData()->uRoomid = pRoom->GetRoom_ManageInfoStruct()->uRoomID;
					struct_data_pr_userinfo_allot_room _cmd;
					_cmd.userid = puser->GetUserData()->basedata.dwUserID;
					_cmd.contestid = puser->GetUserData()->uContestid;
					_cmd.roomid = puser->GetUserData()->uRoomid;
					_cmd.uIndex = puser->GetSocketId();
					QK_InfoLog("gs solo赛 在这里才设置玩家所在房间id。(roomid=%d, userid=%d,contestid=%d,uIndex=%d)", _cmd.roomid, _cmd.userid, _cmd.contestid, _cmd.uIndex);
					g_pGameServer->SendData(g_pGameServer->GetSockidOfLoginServer(), &_cmd, sizeof(_cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_allot_room, 0);

					if (0 == j)
					{//第一个设置的玩家为房主
						pDesk->SetDeskUserInfo(puser, 0);
						pDesk->QueueOver();
						puser->GetUserData()->uDeskStation = 0;
						puser->GetUserData()->uDeskid = pDesk->GetDeskIndex();
						puser->GetUserData()->basedata.bUserState = US_R_GetStation;

						//通知玩家被分配到指定房间的指定座位上，并设置为房主
						QK_InfoLog("sdp_test room 通知玩家被分配到指定房间的指定座位上，并设置为房主。");
						RoomMessDate_queue_allot cmd;
						cmd.bOwerUserid = true;
						cmd.udeskid = pDesk->GetDeskIndex();
						cmd.udeskstation = 0;
						cmd.uuserid = QueueUservector.at(j).m_dwUserID;
						g_pGameServer->SendData(puser->GetSocketId(), &cmd, sizeof(cmd), e_gs_mess_queue, e_gs_ass_queue_allot, 0);
					}//if (0 == j)
					else
					{//非第一个玩家，直接放入空座位上
						UINT udeskstation = 0;
						bool res = pDesk->GetDeskNoPlayerStation(udeskstation);
						if (!res)
						{//查找座位失败
							continue;
						}
						pDesk->SetDeskUserInfo(puser, udeskstation);
						pDesk->QueueOver();
						puser->GetUserData()->uDeskStation = udeskstation;
						puser->GetUserData()->uDeskid = pDesk->GetDeskIndex();
						puser->GetUserData()->basedata.bUserState = US_R_GetStation;

						//通知玩家被分配到指定房间的指定座位上
						QK_InfoLog("sdp_test room 通知玩家被分配到指定房间的指定座位上。");
						RoomMessDate_queue_allot cmd;
						cmd.bOwerUserid = false;
						cmd.udeskid = pDesk->GetDeskIndex();
						cmd.udeskstation = udeskstation;
						cmd.uuserid = QueueUservector.at(j).m_dwUserID;
						g_pGameServer->
							SendData(puser->GetSocketId(), &cmd, sizeof(cmd), e_gs_mess_queue, e_gs_ass_queue_allot, 0);
					}
				}
			}//if (bOK)
			else
			{
				//没有一桌就还回去
				auto iter_vector = QueueUservector.begin();
				while (iter_vector != QueueUservector.end())
				{
					m_QueueUserList.push_front((*iter_vector));
					m_userid2iter.insert(make_pair(iter_vector->m_dwUserID, m_QueueUserList.begin()));
					++iter_vector;
				}
			}
		}
	}

	//组队模式模式运行程序
	void CQueueManage::run_RoomRule_team()
	{
		unordered_map<UINT, R_TeamStruct_VS * > * _pTeamStruct_VS2 = g_pGameServer->GetR_TeamStruct_VS();
		vector<R_TeamStruct_VS *> vector_VS;

		auto _it = _pTeamStruct_VS2->begin();
		while (_it != _pTeamStruct_VS2->end())
		{
			assert(_it->second);
			if (_it->second->_E_R_TeamStruct_VS_station == E_R_TeamStruct_VS_station_wait)
			{//先收集可匹配的团队
				vector_VS.push_back(_it->second);
			}
			++_it;
		}

		if (0 == vector_VS.size())
		{//没有团队信息
			return;
		}

		//有没有房间、空桌子
		ISysDeskInterface* pDesk = nullptr;
		CGameRoom* pRoom = g_pGameServer->m_roomMgr.GetRoom_ForDeskid(m_ucontestid, &pDesk);
		if (nullptr == pDesk || nullptr == pRoom)
		{//没有房间、空桌子就不用匹配了
			return;
		}

		bool bOK = false;
		//人不够就不用匹配了
		while (0 < vector_VS.size())
		{
			bOK = false;
			pRoom = g_pGameServer->m_roomMgr.GetRoom_ForDeskid(m_ucontestid, &pDesk);
			if (nullptr == pDesk || nullptr == pRoom)
			{
				return;
			}
			//接着把团队放进桌子
			R_TeamStruct_VS _VS_data;
			UINT players_count = 0;//两个团队人数的总和
			auto it = vector_VS.begin(), end = vector_VS.end();
			while (it != end)
			{
				players_count = (*it)->_data._TeamStruct[0].m_uTemaPlayerNum + (*it)->_data._TeamStruct[1].m_uTemaPlayerNum;
				if (players_count > pDesk->GetMaxPeople())
				{//两个团队人数不能加起来大于桌子人数
					it = vector_VS.erase(it);
					continue;
				}

				bool bOnLine = true;//是否两组团队中的玩家都在线
				for (int j = 0; j < 2; ++j)
				{
					for (int k = 0; k < (*it)->_data._TeamStruct[j].m_uTemaPlayerNum; ++k)
					{
						CGameUserInfo * puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfo((*it)->_data._TeamStruct[j].m_Userid[k]);
						if (nullptr == puser)
						{//只要团队VS里面有一个人不在线就不能开始
							bOnLine = false;
							break;
						}
					}

					if (!bOnLine)
					{
						break;
					}
				}

				if (!bOnLine)
				{//查找下一个团队vs节点
					it = vector_VS.erase(it);
					continue;
				}

				//人数达到了
				bOK = true;
				(*it)->_E_R_TeamStruct_VS_station = E_R_TeamStruct_VS_station_playing;
				_VS_data = *(*it);

				it = vector_VS.erase(it);
				break;
			}

			if (bOK)
			{//桌子有了，人也找好了，开始通知这些人进入桌子开赛
				std::vector<UINT> vec_userid;//用户id队列
				for (int j = 0; j < 2; ++j)
				{
					for (int k = 0; k < _VS_data._data._TeamStruct[j].m_uTemaPlayerNum; ++k)
					{
						vec_userid.push_back(_VS_data._data._TeamStruct[j].m_Userid[k]);
					}
				}

				for (UINT j = 0; j < vec_userid.size(); ++j)
				{
					CGameUserInfo * puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfo(vec_userid.at(j));
					if (nullptr == puser)
					{
						continue;
					}

					//在这里才设置玩家所在房间id
					puser->GetUserData()->uRoomid = pRoom->GetRoom_ManageInfoStruct()->uRoomID;
					struct_data_pr_userinfo_allot_room _cmd;
					_cmd.userid = puser->GetUserData()->basedata.dwUserID;
					_cmd.contestid = puser->GetUserData()->uContestid;
					_cmd.roomid = puser->GetUserData()->uRoomid;
					_cmd.uIndex = puser->GetSocketId();
					QK_InfoLog("gs 团队赛 在这里才设置玩家所在房间id。(roomid=%d, userid=%d,contestid=%d,uIndex=%d)", _cmd.roomid, _cmd.userid, _cmd.contestid, _cmd.uIndex);
					g_pGameServer->SendData(g_pGameServer->GetSockidOfLoginServer(), &_cmd, sizeof(_cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_allot_room, 0);

					if (0 == j)
					{//第一个设置的玩家为房主
						pDesk->SetDeskUserInfo(puser, 0);
						pDesk->QueueOver();
						puser->GetUserData()->uDeskStation = 0;
						puser->GetUserData()->uDeskid = pDesk->GetDeskIndex();
						puser->GetUserData()->basedata.bUserState = US_R_GetStation;

						//通知玩家被分配到指定房间的指定座位上，并设置为房主
						QK_InfoLog("sdp_test room 通知玩家被分配到指定房间的指定座位上，并设置为房主。");
						RoomMessDate_queue_allot cmd;
						cmd.bOwerUserid = true;
						cmd.udeskid = pDesk->GetDeskIndex();
						cmd.udeskstation = 0;
						cmd.uuserid = vec_userid.at(j);
						g_pGameServer->SendData(puser->GetSocketId(), &cmd, sizeof(cmd), e_gs_mess_queue, e_gs_ass_queue_allot, 0);
					}//if (0 == j)
					else
					{//非第一个玩家，直接放入空座位上
						UINT udeskstation = 0;
						bool res = pDesk->GetDeskNoPlayerStation(udeskstation);
						if (!res)
						{//查找座位失败
							continue;
						}
						pDesk->SetDeskUserInfo(puser, udeskstation);
						pDesk->QueueOver();
						puser->GetUserData()->uDeskStation = udeskstation;
						puser->GetUserData()->uDeskid = pDesk->GetDeskIndex();
						puser->GetUserData()->basedata.bUserState = US_R_GetStation;

						//通知玩家被分配到指定房间的指定座位上
						QK_InfoLog("sdp_test room 通知玩家被分配到指定房间的指定座位上。");
						RoomMessDate_queue_allot cmd;
						cmd.bOwerUserid = false;
						cmd.udeskid = pDesk->GetDeskIndex();
						cmd.udeskstation = udeskstation;
						cmd.uuserid = vec_userid.at(j);
						g_pGameServer->SendData(puser->GetSocketId(), &cmd, sizeof(cmd), e_gs_mess_queue, e_gs_ass_queue_allot, 0);
					}
				}//if (bOK)
			}//if (0 == uplayernum)
		}//for (UINT i = 0; i < ucount; ++i)
	}

	//添加玩家信息到队列中
	bool CQueueManage::Add(CGameUserInfo* pGameUserInfo)
	{
		if (nullptr == pGameUserInfo)
		{
			return false;
		}

		QueueInfo_t _t;
		_t.m_dwUserID = pGameUserInfo->GetUserData()->basedata.dwUserID;
		_t.m_uContestid = pGameUserInfo->GetUserData()->uContestid;
		_t.m_EnterTime = CTime::GetCurrentTime().GetTime();// 记录进入排队机的时间，用于判断排了多长时间的队

		//添加普通玩家队列
		//判断该玩家是否已添加
        auto iter = m_userid2iter.find(_t.m_dwUserID);
		if (iter == m_userid2iter.end())//未添加的玩家添加到排队列表
		{
            auto insertPos = m_QueueUserList.insert(m_QueueUserList.end(), _t);
            m_userid2iter.insert(make_pair(_t.m_dwUserID, insertPos));
			return true;
		}
		QK_ErrorLog("sdp_err room 不能将玩家重复加入到排队队列里面(%d)。", _t.m_dwUserID);
		return false;
	}

	//删除队列中指定玩家信息
	void CQueueManage::Delete(CGameUserInfo* pGameUserInfo)
	{
        if (nullptr == pGameUserInfo)
        {
            return;
        }
		auto iter = m_userid2iter.find(pGameUserInfo->GetUserData()->basedata.dwUserID);
		if (iter != m_userid2iter.end())
		{
			m_QueueUserList.erase(iter->second);
            m_userid2iter.erase(iter);
		}
	}

	//删除队列中所有玩家信息
	void CQueueManage::DeleteAll()
	{
		m_QueueUserList.clear();
        m_userid2iter.clear();
	}
}