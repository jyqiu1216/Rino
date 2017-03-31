#include "gameserver.h"
#include "gsconfig.h"
#include "common/QkLogger.h"
#include "message/WJNetRoomMessage.h"
#include "message/WJNetPtoR.h"

using namespace std;

namespace WJ
{
    CWJRedis * CGameServer::m_pRedis = NULL;

    CGameServer::CGameServer(unsigned int maxSocketNum)
	    : CWJSSocketManage(maxSocketNum)
    {
		m_redisIp = gsconfig()->redis_address;
		m_redisPass = gsconfig()->redis_pass;
		m_nRedisPort = gsconfig()->redis_port;
	    m_pRedis = NULL;
		m_nListenPort = gsconfig()->port_for_client;
        m_sockidOfLoginServer = INVALID_SOCKET_ID;
		m_uCheckTime = 180;
    }

    CGameServer::~CGameServer()
    {
	    StopServer();
	    if (m_pRedis)
	    {
		    delete m_pRedis;
		    m_pRedis = NULL;
	    }

		auto it = m_teamid2team_vs.begin(), end = m_teamid2team_vs.end();
		for (; it != end; ++it)
		{
			if (it->second)
			{
				WJ_SAFE_DELETE(it->second);
			}
		}
		m_teamid2team_vs.clear();
    }

    void CGameServer::StartServer()
    {
	    if (m_pRedis == NULL)
	    {	
		    m_pRedis = new CWJRedis(m_redisIp, m_redisPass, m_nRedisPort, 4);
		    assert(m_pRedis);
		    m_pRedis->Init();
	    }
	    Start_WJSSocketManage(1);
		QK_InfoLog("gs 监听 = (%s:%d)", gsconfig()->ip_for_client.c_str(), gsconfig()->port_for_client);
	    AsyncListen(gsconfig()->ip_for_client, gsconfig()->port_for_client, E_CONN_Client);
        if (!m_roomMgr.LoadRoomInfoFromDb())
        {
            QK_ErrorLog("CGameServer::StartServer :  load room list failed");
        }

		SetTimer(E_R_TIME_ID_ReconnectLoginServe, 2);
		SetTimer(E_R_TIME_ID_LoginServerKeepalive, 5);
		SetTimer(E_R_TIME_ID_RoomQueue, 3);
		SetTimer(E_R_TIME_ID_HEART, 10);
    }

    void CGameServer::StopServer()
    {
	    Stop_WJSSocketManage();
	    if (m_pRedis)
	    {
		    m_pRedis->StopWorkers();
	    }

		m_teamid2team_vs.clear();

		m_contestid2Queue.clear();
		KillTimer(E_R_TIME_ID_ReconnectLoginServe);
		KillTimer(E_R_TIME_ID_LoginServerKeepalive);
		KillTimer(E_R_TIME_ID_RoomQueue);
		KillTimer(E_R_TIME_ID_HEART);
    }

	CGameUserInfoManage* CGameServer::GetCGameUserInfoManage()
	{
		return &m_UserInfoManage;
	}

	CQueueManage* CGameServer::GetCQueueManage(UINT contestid)
	{
		auto iter = m_contestid2Queue.find(contestid);
		
		if (iter != m_contestid2Queue.end())
		{
			return &(iter->second);
		}
		return nullptr;
	}

	unordered_map<UINT, R_TeamStruct_VS * > * CGameServer::GetR_TeamStruct_VS()
	{
		return &m_teamid2team_vs;
	}

    int CGameServer::ProccessTimers()
    {
        return m_timerMgr.ProcessTimerEvents();
    }

    int CGameServer::ProccessRedisResults()
    {
		list<const CRedisCmdResultBase*>  results;
	    m_pRedis->ProcessDbResults(results);
	    int count = 0;
	    while (!results.empty())
	    {
			const CRedisCmdResultBase* pCmdRes = results.front();
		    results.pop_front();
		    switch(pCmdRes->m_cmdId)
		    {
			    // case load_user:
			    //{
			    // .....
			    //}
			    // case ..:
			    //{
			    // .....
			    //}
		    }
		    delete pCmdRes;
		    ++count;
	    }
	    return count;
    }

	void CGameServer::OnNewConnection(CWJSSocket * pSocket, void *pUserData)
    {
        if (pSocket->GetType() == E_CONN_LoginServer)
        {
			QK_InfoLog("连接ls成功！");
            m_sockidOfLoginServer = pSocket->GetSockID();
        }
		else if (pSocket->GetType() == E_CONN_Client)
		{
			//发送连接成功消息
			Net_ConnectSuccess Message;
			Message.bLessVer = (BYTE)MAKELONG(3, 4);
			Message.bReserve[0] = 0;
			Message.bMaxVer = (BYTE)MAKELONG(3, 4);
			Message.bReserve[1] = 0;
			//此处把hSocket进行加密，由客户端解密并在包头的bReserve字段中发过来校验。其中SECRET_KEY为密钥由客户自己管理在
			Message.i64CheckCode = (int)pSocket->GetSocket() * 23 + SECRECTKEY;
			pSocket->SendData(&Message, sizeof(Message), NET_M_CONNECT, ASS_NET_CONNECT_2, 0);
		}
    }

    void CGameServer::OnEndConnection(CWJSSocket * pSocket)
    {
	    //m_roomMgr.On
		if (pSocket->GetType() == E_CONN_Client)
		{
			//OnSocketClose
			QK_InfoLog("client  disconnect");
			CGameUserInfo *puser = m_UserInfoManage.GetOnLineUserInfoEx(pSocket->GetSockID());
			if (nullptr == puser)
				return;

			auto iter = m_contestid2Queue.find(puser->GetUserData()->uContestid);
			if (iter != m_contestid2Queue.end())
			{//将玩家从排队机中清除
				iter->second.Delete(puser);
			}

			CGameRoom *pRoom = m_roomMgr.GetRoom(puser->GetUserData()->uRoomid);
			if (pRoom != nullptr)
			{//在房间中就去房间里面处理
				QK_InfoLog("client  OnEndConnection 房间id=%d", puser->GetUserData()->uRoomid);
				pRoom->OnSocketClose(pSocket);
			}
			else
			{//未在房间内，直接清除掉该玩家信息
				QK_InfoLog("client  OnEndConnection 玩家没在房间中");
				m_UserInfoManage.FreeUser(pSocket->GetSockID());
			}
		}
	    else if (pSocket->GetType() == E_CONN_LoginServer)
	    {   //准备重连
			QK_InfoLog("disconnect from loginserver, prepare reconnect");
			m_sockidOfLoginServer = INVALID_SOCKET_ID;
	    }
    }

	void CGameServer::OnConnectFailed(int type, void *pUserData)
	{
		QK_InfoLog("connect login server fail type=%d, pUserData=%p", type, pUserData);
	}

	// static
	bool CGameServer::OnTimerMessage(TimerID timerId)
	{
		UINT roomId = (timerId & TIMER_ROOMID_MASK) >> 20;
		if (roomId != 0)  //游戏内计时器
		{
			CGameRoom *pRoom = m_roomMgr.GetRoom(roomId);
			if (pRoom)
			{
				pRoom->OnTimerMessage(timerId);
			}
		}
		else
		{
			if (timerId == E_R_TIME_ID_ReconnectLoginServe)
			{
				OnReconnectLoginServerTimer();
			}
			else if (timerId == E_R_TIME_ID_LoginServerKeepalive)
			{
				//OnKeepaliveLoginServerTimer();
				m_roomMgr.UpdateRoomInfoToLoginServer();
			}
			else if (timerId == E_R_TIME_ID_RoomQueue)
			{
				auto iter = m_contestid2Queue.begin();
				while (iter != m_contestid2Queue.end())
				{
					iter->second.run();
					++iter;
				}
			}
			else if (timerId == E_R_TIME_ID_HEART)
			{
				UINT uNowTime = (UINT)time(NULL);	//当前时间

				std::unordered_map<UINT, GameUserStruct*>*p = m_UserInfoManage.GetGameUserStruct_map();
				auto iter = p->begin(), end = p->end();
				int i = 0;
				for (; iter != end; ++iter, ++i)
				{//查看在线玩家是否有心跳
					if (usertype_online == iter->second->usertype)
					{
						CWJSSocket* pSocket = getCWJSSocket(iter->second->userinfo.GetSocketId());
						if (nullptr!=pSocket)
						{
							UINT itag = uNowTime - pSocket->GetKeep();
							if (itag > m_uCheckTime)
							{
								QK_WarnLog("游戏服务器 (uNowTime = %d, m_Keep = %d)时间差=%d 大于 %d,则断开该客户连接 socketindex = %d", uNowTime, pSocket->GetKeep(), itag, m_uCheckTime, iter->second->userinfo.GetSocketId());
								CloseSocket(pSocket->GetSockID());
							}
						}
					}
				}
			}
		}

		return true;
	}

    void CGameServer::OnReconnectLoginServerTimer()
    {
        if (m_sockidOfLoginServer == INVALID_SOCKET_ID)
        {
			QK_InfoLog("连接ls = (%s:%d)", gsconfig()->loginserver_address.c_str(), PORT_LS_TO_GS);
			AsyncConnect(gsconfig()->loginserver_address.c_str(), PORT_LS_TO_GS, E_CONN_LoginServer, (void*)123);
        }
    }

    void CGameServer::OnKeepaliveLoginServerTimer()
    {
	    if (m_sockidOfLoginServer != INVALID_SOCKET_ID)
	    { //发送保活包
		    SendData(m_sockidOfLoginServer, NET_M_CONNECT, ASS_NET_CONNECT_1, 0);
	    }
    }

	//UINT uElapse 单位秒
	void CGameServer::SetTimer(TimerID uTimerID, UINT uElapse)
	{
		CWJTimer _Timer;
		_Timer.m_timerID = uTimerID;
		_Timer.m_interval = uElapse; // seconds
		_Timer.m_pCallBack_Time = this;
		m_timerMgr.AddTimer(&_Timer);
	}

	void CGameServer::KillTimer(TimerID uTimerID)
	{
		m_timerMgr.DelTimer(uTimerID);
	}

    /**
     * @param 
     *    pData 可能为NULL , 此时表示只收到了一个 NetMessageHead 头
     */
    bool CGameServer::ProccessNetMessage(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
    {
		if (pNetHead->bMainID != NET_M_CONNECT && pNetHead->bAssistantID != ASS_NET_CONNECT_1)
		{
			QK_InfoLog("gs ProccessNetMessage(bMainID=%d,bAssistantID=%d,bHandleCode=%d)。", pNetHead->bMainID, pNetHead->bAssistantID, pNetHead->bHandleCode);
		}

		//效验网络测试数据
		if ((pNetHead->bMainID == NET_M_CONNECT) && (pNetHead->bAssistantID == ASS_NET_CONNECT_1))
		{
			//心跳消息暂不处理
			return true;
		}

        if (pSocket->GetType() == E_CONN_Client)  //来自客户端的消息
        {
			return ProccessNetMessage_player(pSocket, pNetHead, pData, uSize);
        }
        else if (pSocket->GetType() == E_CONN_LoginServer) //来自LoginServer的消息
        {
			return ProccessNetMessage_ls(pSocket, pNetHead, pData, uSize);
        }

		return true;
    }

	//处理玩家客户端消息
	bool CGameServer::ProccessNetMessage_player(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		UINT roomid = ERROR_ROOMID;
		if (e_gs_mess_user_action == pNetHead->bMainID && e_gs_ass_user_action_login == pNetHead->bAssistantID)
		{//登陆

			QK_InfoLog("sdp_test room 收到登陆房间消息。");
			RoomMessDate_user_action_login* p = (RoomMessDate_user_action_login*)pData;
			if (nullptr == p || uSize != sizeof(RoomMessDate_user_action_login))
			{
				QK_WarnLog("收到登陆房间消息,数据为空");
				return false;
			}

			CGameUserInfo *puser2 = m_UserInfoManage.GetOnLineUserInfo(p->uuserid);
			if (nullptr != puser2)
			{//已经是在线用户，拒绝登陆
				QK_WarnLog("已经是在线用户，拒绝登陆（nickName=%s,dwUserID=%d）"
					, puser2->GetUserData()->basedata.nickName, puser2->GetUserData()->basedata.dwUserID);
				return false;
			}

			CGameUserInfo *pNetCutUser = m_UserInfoManage.GetNetCutUser(p->uuserid);
			if (nullptr != pNetCutUser)
			{//断线重回
				UINT iRoomid = pNetCutUser->GetUserData()->uRoomid;
				if (0 == iRoomid)
				{
					QK_WarnLog("断线重回 获取roomid错误（%d,%d）", iRoomid, pNetCutUser->GetUserData()->uRoomid);
					return false;
				}
				CGameUserInfo* puser = m_UserInfoManage.ActiveUser(pNetCutUser->GetUserData(), pSocket->GetSockID());
				if (nullptr == puser)
				{//注册在线用户异常
					QK_WarnLog("sdp_test room 房间登陆人数已达上限,注册在线用户异常。");
					return false;
				}
				puser->GetUserData()->basedata.bUserState = US_R_Playing;
				QK_InfoLog("sdp_test room 房间数据给客户端。");

				CGameRoom* pRoom = m_roomMgr.GetRoom(iRoomid);
				if (nullptr == pRoom)
				{
					QK_WarnLog("房间不存在。");
					return false;
				}

				pSocket->SendData((Room_ManageInfoStruct*)pRoom->GetRoom_ManageInfoStruct(), sizeof(Room_ManageInfoStruct), e_gs_mess_room, e_gs_ass_room_info, 0);
				QK_InfoLog("sdp_test room 登陆房间成功—断线重回。");
				pSocket->SendData(puser->GetUserData(), sizeof(UserInfoInRoomStruct), e_gs_mess_user_action, e_gs_ass_user_action_login, e_gs_hcid_login_suc_recome);

				//通知游戏内部，玩家断线重回了
				ISysDeskInterface* pdesk = pRoom->GetCGameDeskManage()->At(puser->GetUserData()->uDeskid);
				assert(pdesk);
				pdesk->Sys_UserReCome(puser->GetUserData()->uDeskid, puser);

				RTeamVSSendData(puser->GetUserData()->basedata.dwUserID, nullptr, 0, e_gs_mess_user_action, e_gs_ass_user_action_team_come, puser->GetUserData()->basedata.dwUserID);
			}
			else
			{
				struct_data_pr_userinfo_login cmd;
				cmd.uuserid = p->uuserid;
				cmd.contestid = p->contestid;
				cmd.uIndex = pSocket->GetSockID();
				SendData(GetSockidOfLoginServer(), &cmd, sizeof(cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_login, 0);
			}
			return true;
		}
		else if (e_gs_mess_user_action == pNetHead->bMainID && e_gs_ass_user_action_login_cancel == pNetHead->bAssistantID)
		{//玩家取消登录
			RoomMessDate_user_action_login_cancel* p = (RoomMessDate_user_action_login_cancel*)pData;
			if (nullptr == p || uSize != sizeof(RoomMessDate_user_action_login_cancel))
			{
				QK_WarnLog("收到取消登录消息,数据为空");
				return false;
			}

			QK_InfoLog("sdp_test room 玩家取消登录(%d)。", p->uuserid);
			CGameUserInfo *puser = m_UserInfoManage.GetOnLineUserInfoEx(pSocket->GetSockID());
			if (nullptr != puser)
			{//玩家在线
				roomid = puser->GetUserData()->uRoomid;
				//if (roomid == ERROR_ROOMID)
				//{//没有在房间里面
				//	auto iter = m_contestid2Queue.find(puser->GetUserData()->uContestid);
				//	if (iter != m_contestid2Queue.end())
				//	{//将玩家从排队机中清除
				//		iter->second.Delete(puser);
				//	}

				//	m_UserInfoManage.FreeUser(pSocket->GetSockID());
				//	SendData(pSocket->GetSockID(), nullptr, 0, e_gs_mess_user_action, e_gs_ass_user_action_login_cancel, e_gs_hcid_login_cancel_suc_);
				//}
				//else
				//{//在房间中
				//	QK_WarnLog("玩家已经在房间内玩了，不接受取消登录消息！(%d)", p->uuserid);
				//	SendData(pSocket->GetSockID(), nullptr, 0, e_gs_mess_user_action, e_gs_ass_user_action_login_cancel, e_gs_hcid_login_cancel_err_);
				//}

				auto iter = m_contestid2Queue.find(puser->GetUserData()->uContestid);
				if (iter != m_contestid2Queue.end())
				{//将玩家从排队机中清除
					iter->second.Delete(puser);
				}

				CGameRoom *pGameRoom = m_roomMgr.GetRoom(roomid);
				if (nullptr != pGameRoom)
				{
					ISysDeskInterface* pDesk = pGameRoom->GetCGameDeskManage()->At(puser->GetUserData()->uDeskid);
					if (nullptr != pDesk)
					{
						if (pDesk->IsPlayGame())
						{//游戏正在进行中
							pDesk->GameFinish(GAME_FINISH_TYPE_FORCE);
							m_UserInfoManage.FreeUser(pSocket->GetSockID());
							return true;
						}
						else
						{
							pDesk->Sys_UserLeftDesk(puser->GetUserData()->uDeskStation, puser);
						}
					}
				}
				
				m_UserInfoManage.FreeUser(pSocket->GetSockID());
				SendData(pSocket->GetSockID(), nullptr, 0, e_gs_mess_user_action, e_gs_ass_user_action_login_cancel, e_gs_hcid_login_cancel_suc_);
			}
			else
			{//玩家可能不在这个服务器内、可能未登录过、可能处于断线状态。
				puser = m_UserInfoManage.GetNetCutUser(p->uuserid);
				if (nullptr != puser)
				{
					QK_WarnLog("断线状态不可能收到取消登录消息！(%d)", p->uuserid);
					return false;
				}
				else
				{
					SendData(pSocket->GetSockID(), nullptr, 0, e_gs_mess_user_action, e_gs_ass_user_action_login_cancel, e_gs_hcid_login_cancel_suc_);
				}
			}

			return true;
		}
		else //登陆后，可直接拿到roomid
		{
			CGameUserInfo *puser = m_UserInfoManage.GetOnLineUserInfoEx(pSocket->GetSockID());
			if (nullptr != puser)
			{
				roomid = puser->GetUserData()->uRoomid;
			}
		}

		if (roomid == ERROR_ROOMID) //找roomid
		{
			QK_ErrorLog("roomid == %d", ERROR_ROOMID);
			return false;
		}
		CGameRoom *pGameRoom = m_roomMgr.GetRoom(roomid);
		if (pGameRoom)
		{
			pGameRoom->OnSocketRead(pSocket, pNetHead, pData, uSize);
		}
		
		return true;
	}

	//处理ls发来的消息
	bool CGameServer::ProccessNetMessage_ls(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		switch (pNetHead->bMainID)
		{
		case e_pr_mess_roominfo:
		{
								   OnSocketRead_pr_mess_roominfo(pSocket, pNetHead, pData, uSize);
								   break;
		}
		case e_pr_mess_userinfo:
		{
								   OnSocketRead_pr_mess_userinfo(pSocket, pNetHead, pData, uSize);
								   break;
		}
		default:
			break;
		}

		return true;
	}

	//房间消息处理
	bool CGameServer::OnSocketRead_pr_mess_roominfo(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		/*	switch (pNetHead->bAssistantID)
		{
		case e_pr_ass_userinfo_login:
		{

		return true;
		}break;
		default:
		{
		break;
		}
		}*/

		return true;
	}

	//用户消息处理
	bool CGameServer::OnSocketRead_pr_mess_userinfo(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		switch (pNetHead->bAssistantID)
		{
		case e_pr_ass_userinfo_login:
		{//玩家登录游戏服务器
										if (e_pr_hcid_login_allow == pNetHead->bHandleCode)
										{
											QK_InfoLog("sdp_test room 收到大厅确认玩家登录游戏服务器 允许消息。");

											struct_data_pr_userinfo_login_res_ok* p = (struct_data_pr_userinfo_login_res_ok*)pData;

											if (nullptr == p || sizeof(struct_data_pr_userinfo_login_res_ok) != uSize) //tell client
											{
												return true;
											}

											CWJSSocket* pSocket = getCWJSSocket(p->uIndex);
											if (nullptr == pSocket)
											{
												return true;
											}

											//if (nullptr != pSocket->GetUserObj())
											//{//已经干扰到正常玩家了
											//	QK_ErrorLog("sdp_err room 已经干扰到正常玩家了1。");
											//	SendData(p->uIndex, nullptr, 0, e_gs_mess_user_action, e_gs_ass_user_action_login, e_gs_hcid_login_err_);
											//	return true;
											//}

											CGameUserInfo *puser = m_UserInfoManage.GetOnLineUserInfoEx(p->uIndex);
											if (nullptr != puser)
											{//已经干扰到正常玩家了
												QK_ErrorLog("sdp_err room 已经干扰到正常玩家了3。");
												SendData(p->uIndex, nullptr, 0, e_gs_mess_user_action, e_gs_ass_user_action_login, e_gs_hcid_login_err_);
												return true;
											}

											CGameUserInfo *puser2 = m_UserInfoManage.GetOnLineUserInfo(p->data.basedata.dwUserID);
											if (nullptr != puser2)
											{//已经干扰到正常玩家了
												QK_ErrorLog("sdp_err room 已经干扰到正常玩家了3。");
												SendData(p->uIndex, nullptr, 0, e_gs_mess_user_action, e_gs_ass_user_action_login, e_gs_hcid_login_err_);
												return true;
											}

											CGameUserInfo *puser3 = m_UserInfoManage.GetNetCutUser(p->data.basedata.dwUserID);
											if (nullptr != puser3)
											{//已经干扰到正常玩家了
												QK_ErrorLog("sdp_err room 已经干扰到正常玩家了4。");
												SendData(p->uIndex, nullptr, 0, e_gs_mess_user_action, e_gs_ass_user_action_login, e_gs_hcid_login_err_);
												return true;
											}

											//连接已经中断，则无需处理
											CWJSSocket *pClientSocket = getCWJSSocket(p->uIndex);
											if (pClientSocket->IsConnect())
											{//在线
												//登陆成功
												UserInfoInRoomStruct user;
												std::memcpy(&user.basedata, &p->data.basedata, sizeof(UserInfoBaseStruct));
												user.uContestid = p->contestid;//玩家所在比赛id
												CGameUserInfo *puser2 = m_UserInfoManage.ActiveUser(&user, p->uIndex);
												ContestInfo_locad* pContestinfo = GetContInfo_locad(p->contestid);
												if (nullptr == puser2
													|| nullptr == pContestinfo)
												{
													SendData(p->uIndex, nullptr, 0, e_gs_mess_user_action, e_gs_ass_user_action_login, e_gs_hcid_login_err_);
													QK_ErrorLog("sdp_err room 不应该发生此错误(用户(%d)注册不成功，比赛找不到(%d))。", p->uIndex, p->contestid);
													return true;
												}
												if (US_ContestRule_solo == pContestinfo->uContestRule)
												{//加入排队机队列
													auto iter = m_contestid2Queue.find(p->contestid);
													if (iter != m_contestid2Queue.end())
													{
														iter->second.Add(puser2);
													}													
												}
												else if (US_ContestRule_team == pContestinfo->uContestRule)
												{//团队赛，要告诉vs中的其他玩家
													//
												}

												QK_InfoLog("sdp_test room 房间同意玩家登陆消息。");
												pClientSocket->SendData(puser2->GetUserData(), sizeof(UserInfoInRoomStruct), e_gs_mess_user_action, e_gs_ass_user_action_login, e_gs_hcid_login_suc_);
											}
										}
										else if (e_pr_hcid_login_unallow == pNetHead->bHandleCode)
										{
											QK_InfoLog("sdp_test room 收到大厅确认玩家登录游戏服务器 拒绝消息。");
											struct_data_pr_userinfo_login_res_err* p = (struct_data_pr_userinfo_login_res_err*)pData;
											if (nullptr == p || sizeof(struct_data_pr_userinfo_login_res_err) != uSize)
											{
												return true;
											}

											//登陆失败
											QK_InfoLog("sdp_test room 游戏服务器拒绝玩家登陆消息。");
											SendData(p->uIndex, nullptr, 0, e_gs_mess_user_action, e_gs_ass_user_action_login, e_gs_hcid_login_err_);
										}

										return true;
		}break;
		case e_pr_ass_userinfo_apply_restitution://咨询是否玩家可以退费
		{
													 QK_InfoLog("sdp_test room 收到大厅咨询是否玩家可以退费消息。");
													 struct_data_pr_userinfo_apply_restitution* p = (struct_data_pr_userinfo_apply_restitution*)pData;
													 if (nullptr == p || sizeof(struct_data_pr_userinfo_apply_restitution) != uSize)
													 {
														 return true;
													 }

													 bool bAllow = true;//是否允许退费
													 do
													 {
														 CGameUserInfo *puser = m_UserInfoManage.GetOnLineUserInfo(p->userid);
														 if (nullptr == puser)
														 {
															 puser = m_UserInfoManage.GetNetCutUser(p->userid);
														 }
														 if (nullptr != puser)
														 {
															 UINT uDeskid = puser->GetUserData()->uDeskid;
															 CGameRoom* pRoom = m_roomMgr.GetRoom(puser->GetUserData()->uRoomid);
															 if (pRoom != nullptr)
															 {
																 return true;
															 }

															 if (ERROR_DESKID != uDeskid)
															 {
																 ISysDeskInterface* pdesk = pRoom->GetCGameDeskManage()->At(uDeskid);
																 if (nullptr != pdesk)
																 {
																	 if (pdesk->IsPlayGame())
																	 {//游戏已经开始,不能退费
																		 bAllow = false;
																		 break;
																	 }

																	 switch (pRoom->GetRoom_ManageInfoStruct()->uContestRule)
																	 {
																	 case US_ContestRule_solo:
																	 {//个人赛
																								 UINT uDeskStation = puser->GetUserData()->uDeskStation;
																								 //从排队机中删除
																								 auto iter = m_contestid2Queue.find(pRoom->GetRoom_ManageInfoStruct()->uContestid);
																								 if (iter != m_contestid2Queue.end())
																								 {
																									 iter->second.Delete(puser);
																								 }
																								 //该桌子座位上的人清理掉
																								 pdesk->SetDeskUserInfo(nullptr, uDeskStation);
																	 }break;
																	 case US_ContestRule_team:
																	 {//组队赛
																								 //如果是队长就解散该队伍																																																
																	 }break;
																	 default:
																		 break;
																	 }
																 }
															 }
															 else
															 {//还没分配桌子
																 if (US_ContestRule_solo == pRoom->GetRoom_ManageInfoStruct()->uContestRule)
																 {//从排队机中删除
																	 auto iter = m_contestid2Queue.find(pRoom->GetRoom_ManageInfoStruct()->uContestid);
																	 if (iter != m_contestid2Queue.end())
																	 {
																		 iter->second.Delete(puser);
																	 }
																 }
															 }
														 }
													 } while (0);

													 //返回消息
													 struct_data_pr_userinfo_apply_restitution cmd;
													 cmd.userid = p->userid;
													 QK_InfoLog("sdp_test room 回复大厅是否玩家可以退费（bAllow=%d）。", bAllow);
													 SendData(GetSockidOfLoginServer(), &cmd, sizeof(cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_apply_restitution
														 , bAllow ? e_pr_hcid_apply_restitution_suc : e_pr_hcid_apply_restitution_err);
		}break;
		case e_pr_ass_userinfo_team_start:
		{
											 TeamStruct_VS* p = (TeamStruct_VS*)pData;
											 if (nullptr == p || sizeof(TeamStruct_VS) != uSize)
											 {
												 return true;
											 }

											 R_TeamStruct_VS * p_R_TeamStruct_VS = new R_TeamStruct_VS;
											 std::memcpy(&p_R_TeamStruct_VS->_data, p, sizeof(TeamStruct_VS));
											 auto retPair = m_teamid2team_vs.insert(make_pair(p_R_TeamStruct_VS->_data.teamid, p_R_TeamStruct_VS));
											 if (!retPair.second)
											 {
												 WJ_SAFE_DELETE(p_R_TeamStruct_VS);
											 }
		}break;
		default:
		{
				   break;
		}
		}

		return true;
	}

	//发送给团队VS里面的消息
	//UINT myuserid 排除自己发送
	void CGameServer::RTeamVSSendData(UINT myuserid, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{
		CGameUserInfo *pUserInfo = m_UserInfoManage.GetOnLineUserInfo(myuserid);
		if (nullptr == pUserInfo)
		{
			return;
		}

		ContestInfo_locad* pCont = GetContInfo_locad(pUserInfo->GetUserData()->uContestid);
		if (nullptr == pCont)
		{
			return;
		}

		if (US_ContestRule_team == pCont->uContestRule)
		{//团队赛，要告诉vs中的其他玩家
			bool bInVS = false;//玩家是否在团队赛VS中
			R_TeamStruct_VS * pR_TeamStruct_VS = nullptr;
			//auto iter = m_R_TeamStruct_VS.begin();
			auto iter = m_teamid2team_vs.find(pUserInfo->GetUserData()->basedata.teamid);
			if (iter != m_teamid2team_vs.end() && iter->second != nullptr)
			{
				for (int j = 0; j < 2; ++j)
				{
					for (int k = 0; k < iter->second->_data._TeamStruct[j].m_uTemaPlayerNum; ++k)
					{
						if (iter->second->_data._TeamStruct[j].m_Userid[k] == myuserid)
						{
							bInVS = true;
							pR_TeamStruct_VS = iter->second;
							break;
						}
					}
					if (bInVS)
					{
						break;
					}
				}
			}

			if (bInVS)
			{//在vs中						
				for (int j = 0; j < 2; ++j)
				{
					for (int k = 0; k < pR_TeamStruct_VS->_data._TeamStruct[j].m_uTemaPlayerNum; ++k)
					{
						if (pR_TeamStruct_VS->_data._TeamStruct[j].m_Userid[k] == myuserid)
						{
							continue;
						}
						CGameUserInfo* puser2 = m_UserInfoManage.GetOnLineUserInfo(pR_TeamStruct_VS->_data._TeamStruct[j].m_Userid[k]);
						if (nullptr != puser2)
						{//要告诉vs中的其他玩家,有人回来了
							SendData(puser2->GetSocketId(), pData, uBufLen, bMainID, bAssistantID, bHandleCode);
						}
					}
				}
			}
		}
	}

	//添加比赛
	void CGameServer::AddContInfo_locad(UINT contestid, ContestInfo_locad &data)
	{
		if (nullptr == GetContInfo_locad(contestid))
		{
			m_contestid2info.insert(make_pair(contestid, data));
			//添加比赛的同时添加排队机模块
			CQueueManage cmd;
			cmd.m_ucontestid = contestid;
			cmd.m_uContestRule = data.uContestRule;
			m_contestid2Queue.insert(make_pair(contestid, cmd));
		}
	}

	ContestInfo_locad* CGameServer::GetContInfo_locad(UINT contestid)
	{
		auto it = m_contestid2info.find(contestid);
		if (it != m_contestid2info.end())
		{
			return &(it->second);
		}
		return nullptr;
	}
}