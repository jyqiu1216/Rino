
#include "mainmanage_login.h"
#include <vector>
#include "loginserver.h"
#include "message/WJNetPlatformMessage.h"
#include "message/WJNetPtoR.h"
#include "server/comtimetag.h"
#include "Redis/WJR_agreement_hall.h"
#include "common/commonfunction.h"
#include "common/WJGameInfo.h"
#include "common\QkLogger.h"
#include "lsconfig.h"
#include "message/WJNetRoomMessage.h"

enum ClientType
{
	ePCClient,//pc客户端
	eGameServer,//游戏服务器
};
namespace WJ
{
	CMainManage_login::CMainManage_login(ManageInfoStruct *pManageInfoStruct):CWJSSocketManage(70000)
	{
		m_pRedis = NULL;
		m_ManageInfoStruct = *pManageInfoStruct;
		m_uCheckTime = 180;
		m_update_now_playercount = 0;
	}

	CMainManage_login::~CMainManage_login()
	{
		UnInit_subclass();
	}

	CLUserInfoManage* CMainManage_login::GetCLUserInfoManage()
	{
		return &m_CLUserInfoManage;
	}

	///初始化函数
	bool CMainManage_login::Init_subclass(const ManageInfoStruct &InitData)
	{
		m_CLUserInfoManage.Init_CLUserInfoManage(InitData.uMaxCount);
		m_pRedis = new WJ::CWJRedis(lsconfig().GetData()->redis_address,
			lsconfig().GetData()->redis_pass,
			lsconfig().GetData()->redis_port,
			8);
		assert(m_pRedis);

		return true;
	}
	///取消初始化函数
	bool CMainManage_login::UnInit_subclass()
	{
		m_CLUserInfoManage.UnInit_CLUserInfoManage();
		m_CGameList.clear();
		m_CApplayManage.clear();
		m_socket_vector.clear();
		return true; 
	}
	///启动函数
	bool CMainManage_login::Start_subclass()
	{
		Init_subclass(m_ManageInfoStruct);
		// 启动时 获取游戏列表
		m_CGameList.InitLoadFromDB(m_pRedis);
		// 注意要先启动socketmanage 然后监听端口，否则，监听线程将不能循环 。。。。。
		Start_WJSSocketManage(1);

		QK_InfoLog("ls 监听 = (0.0.0.0:%d)", m_ManageInfoStruct.uListenPort);
		AsyncListen("0.0.0.0", m_ManageInfoStruct.uListenPort, ePCClient);

		QK_InfoLog("ls (与gs)监听 = (0.0.0.0:%d)", PORT_LS_TO_GS);
		AsyncListen("0.0.0.0", PORT_LS_TO_GS, eGameServer);

		SetTimer(E_P_TIME_ID_TEAM_VS, 4);
		SetTimer(E_P_TIME_ID_UPDATE_ONLINE_COUNT, 10);
		SetTimer(E_P_TIME_ID_HEART, 10);
		SetTimer(E_P_TIME_ID_UPDATE_USER_INFO, 5);
		return true;
	}
	///停止服务
	bool CMainManage_login::Stop_subclass()
	{
		Stop_WJSSocketManage();
		if (m_pRedis)
		{
			m_pRedis->StopWorkers();
		}
		m_CLUserInfoManage.clear();
		m_CGameList.clear();
		m_CApplayManage.clear();
		m_socket_vector.clear();

		KillTimer(E_P_TIME_ID_TEAM_VS);
		KillTimer(E_P_TIME_ID_UPDATE_ONLINE_COUNT);
		KillTimer(E_P_TIME_ID_HEART);

		m_CTeamManage.delAll();
		return true;
	}

	//UINT uElapse 单位秒
	void CMainManage_login::SetTimer(TimerID uTimerID, UINT uElapse)
	{
		CWJTimer _Timer;

		_Timer.m_timerID = uTimerID;
		_Timer.m_interval = uElapse; // seconds
		_Timer.m_pCallBack_Time = this;
		m_timerMgr_h.AddTimer(&_Timer);
	}

	void CMainManage_login::KillTimer(TimerID uTimerID)
	{
		m_timerMgr_h.DelTimer(uTimerID);
	}

	int CMainManage_login::ProccessTimers()
	{
		return m_timerMgr_h.ProcessTimerEvents();
	}

	int CMainManage_login::ProccessRedisResults()
	{
		list<const CRedisCmdResultBase*>  results;
		m_pRedis->ProcessDbResults(results);
		int count = 0;
		while (!results.empty())
		{
			const CRedisCmdResultBase* pCmdRes = results.front();
			assert(pCmdRes);
			results.pop_front();
			if (pCmdRes->m_cmdType == E_TRANSACTION)
			{
				CRedisTransactionResult *pTrans = (CRedisTransactionResult*)pCmdRes;
				assert(pTrans);
				switch (pTrans->m_cmdId)
				{
					case Redis_Order_h_messid_load_hall:
					{
						CTransaction_load_hall *pTransObj = (CTransaction_load_hall *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_load_hall(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_apply:
					{
						CTransaction_apply *pTransObj = (CTransaction_apply *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_apply(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_apply_restitution:
					{
						CTransaction_apply_restitution *pTransObj = (CTransaction_apply_restitution *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_apply_restitution(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_team_create:
					{
						CTransaction_team_create *pTransObj = (CTransaction_team_create *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_team_create(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_team_join:
					{
						CTransaction_team_join *pTransObj = (CTransaction_team_join *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_team_join(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_team_leave:
					{
						CTransaction_team_leave *pTransObj = (CTransaction_team_leave *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_team_leave(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_team_dissolve:
					{
						CTransaction_team_dissolve *pTransObj = (CTransaction_team_dissolve *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_team_dissolve(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_update_userinfo:
					{
																 CTransaction_Update_Userinfo *pTransObj = (CTransaction_Update_Userinfo *)pTrans->m_pArg;
																 auto _iter = pTransObj->m_out_vector.begin();
																 while (_iter != pTransObj->m_out_vector.end())
																 {
																	 onRedisRes_Order_h_messid_update_userinfo(&(*_iter));
																	 ++_iter;
																 }																 
																 WJ_SAFE_DELETE(pTransObj);
					}break;
					default:
						break;
				}
			}
			delete pCmdRes;
			++count;
		}
		return count;
	}

	void CMainManage_login::OnNewConnection(CWJSSocket * pNewTCPSocket, void *pUserData)
	{
		UINT uIndex = pNewTCPSocket->GetSockID();
		pNewTCPSocket->SetKeep();

		// 这里可以做一些统计，根据需求自己添加代码
		int nTag = pNewTCPSocket->GetType();
		if (nTag == ePCClient)
		{

		}
		else if (nTag == eGameServer)
		{

		}

		//发送连接成功消息
		Net_ConnectSuccess Message;
		Message.bLessVer = (BYTE)MAKELONG(3, 4);
		Message.bReserve[0] = 0;
		Message.bMaxVer = (BYTE)MAKELONG(3, 4);
		Message.bReserve[1] = 0;
		//此处把hSocket进行加密，由客户端解密并在包头的bReserve字段中发过来校验。其中SECRET_KEY为密钥由客户自己管理在
		Message.i64CheckCode = (int)uIndex * 23 + SECRECTKEY;
		pNewTCPSocket->SendData(&Message, sizeof(Message), NET_M_CONNECT, ASS_NET_CONNECT_2, 0);

		QK_InfoLog("sdp_test 发送连接成功消息");
	}

	bool CMainManage_login::ProccessNetMessage(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		// 底层socket管理类调用此虚函数  
		if (nullptr == pSocket || nullptr == pNetHead)
		{
			return false;
		}

		//	ULONG uAccessIP =1 ;
		UINT uIndex = pSocket->GetSockID();
		UINT tag = pSocket->GetType();

		//if (pNetHead->bMainID != NET_M_CONNECT && pNetHead->bAssistantID != ASS_NET_CONNECT_1)
		//{
		//	QK_InfoLog("ls ProccessNetMessage(bMainID=%d,bAssistantID=%d,bHandleCode=%d)。", pNetHead->bMainID, pNetHead->bAssistantID, pNetHead->bHandleCode);
		//	QK_InfoLog("ls ProccessNetMessage(uIndex=%d,tag=%d)。", uIndex, tag);
		//}

		//效验网络测试数据
		if ((pNetHead->bMainID == NET_M_CONNECT) && (pNetHead->bAssistantID == ASS_NET_CONNECT_1))
		{
			//心跳消息暂不处理
			return true;
		}

		if (tag == ePCClient)
		{
			switch (pNetHead->bMainID)
			{
			case e_hs_mess_login:
			{
									return OnSocketRead_hs_mess_login(pSocket, pNetHead, pData, uSize);
			}break;
			case e_hs_mess_gamelist:
			{
									   return OnSocketRead_hs_mess_gamelist(pSocket, pNetHead, pData, uSize);
			}break;
			case e_hs_mess_apply:
			{
									return OnSocketRead_hs_mess_apply(pSocket, pNetHead, pData, uSize);
			}break;
			case e_hs_mess_team:
			{
								   return OnSocketRead_hs_mess_team(pSocket, pNetHead, pData, uSize);
			}break;
			default:
				return false;
			}
		}
		else if (tag == eGameServer)
		{
			switch (pNetHead->bMainID)
			{
			case e_pr_mess_roominfo:
			{
									   return OnSocketRead_pr_mess_roominfo(pSocket, pNetHead, pData, uSize);
			}break;
			case e_pr_mess_userinfo:
			{
									   return OnSocketRead_pr_mess_userinfo(pSocket, pNetHead, pData, uSize);
			}break;
			default:
				return false;
			}
		}

		return true;
	}

	//登陆消息处理
	bool CMainManage_login::OnSocketRead_hs_mess_login(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		UINT uIndex = pSocket->GetSockID();
		switch (pNetHead->bAssistantID)
		{
		case e_hs_ass_login:
		{//玩家普通登陆
							   QK_InfoLog("sdp_test hall 收到玩家普通登陆消息。");
							   struct_data_hs_mess_login* pCmd = (struct_data_hs_mess_login*)pData;
							   if (nullptr == pCmd || uSize != sizeof(struct_data_hs_mess_login))
							   {
								   return false;
							   }

							   CLoginUserInfo* puserold = m_CLUserInfoManage.GetOnLineUserInfo(pCmd->username);
							   if (nullptr != puserold)
							   {//玩家已经在线
								   QK_InfoLog("sdp_test hall 玩家已经在线。");
								   pSocket->SendData(e_hs_mess_login, e_hs_ass_login, e_hs_hcid_login_err_online);
								   return true;
							   }

							   //进行事务
							   CTransaction_load_hall *pLoadHall = new CTransaction_load_hall;
							   assert(pLoadHall);
							   pLoadHall->m_in.uIndex = uIndex;
							   std::memcpy(&pLoadHall->m_in.data, pCmd, sizeof(struct_data_hs_mess_login));
							   m_pRedis->do_transaction(Redis_Order_h_messid_load_hall,
								   pSocket->GetSockID(),
								   onRedis_Order_h_messid_load_hall,
								   pLoadHall);

							   return true;
		}break;
		case e_hs_ass_login_WX:
		{//微信登陆
							   QK_InfoLog("sdp_test hall 收到玩家微信登陆消息。");
							   struct_data_hs_mess_login_WX* pCmd = (struct_data_hs_mess_login_WX*)pData;
							   if (nullptr == pCmd || uSize != sizeof(struct_data_hs_mess_login_WX))
							   {
								   QK_InfoLog("sdp_test hall 收到玩家微信登陆消息 err1。");
								   return false;
							   }
							   QK_InfoLog("sdp_test hall 用户登录大厅wx userid=%d,username=%s,wx_key=%s", pCmd->userid, pCmd->username, pCmd->wx_key);
							   CLoginUserInfo* puserold = m_CLUserInfoManage.GetOnLineUserInfo(pCmd->userid);
							   if (nullptr != puserold)
							   {//玩家已经在线
								   QK_InfoLog("sdp_test hall 玩家已经在线。");
								   pSocket->SendData(e_hs_mess_login, e_hs_ass_login, e_hs_hcid_login_err_online);
								   return true;
							   }

							   //进行事务
							   CTransaction_load_hall *pLoadHall = new CTransaction_load_hall;
							   assert(pLoadHall);
							   pLoadHall->m_in.uIndex = uIndex;
							   pLoadHall->m_in.bWX = true;
							   std::memcpy(&pLoadHall->m_in.data2, pCmd, sizeof(struct_data_hs_mess_login_WX));
							   m_pRedis->do_transaction(Redis_Order_h_messid_load_hall,
								   pSocket->GetSockID(),
								   onRedis_Order_h_messid_load_hall,
								   pLoadHall);
							   return true;
		}break;
		case e_hs_ass_login_c_finish:
		{
										QK_InfoLog("sdp_test hall 收到客户端告诉服务器玩家登陆完成消息。");
										CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
										if (nullptr == puser)
										{
											QK_InfoLog("sdp_test hall 玩家不存在。");
											return true;
										}

										//检查玩家是否在报名中，但是没有加入房间
										struct_applynode data;
										bool bexit_apply = m_CApplayManage.at(puser->GetUserData()->basedata.dwUserID, data);
										if (bexit_apply)
										{
											if (data.bAllot)
											{//在房间里
												struct_data_hs_mess_apply_login_recome_playing cmd;
												cmd.ucontestid = data.ucontestid;
												cmd.roomid = data.uroomid;
												const ComRoomInfo* pRoom = m_CGameList.GetComRoomInfo(cmd.roomid);
												const ComContestInfo* pC = m_CGameList.GetComContestInfo(pRoom->contestid);
												if (nullptr != pRoom)
												{
													std::memcpy(cmd.strIP, pRoom->strIP, sizeof(cmd.strIP));
													cmd.uport = pRoom->uport;
													cmd.ereaid = pRoom->ereaid;
													cmd.gameid = pRoom->gameid;
													cmd.uContestKind = pC->uContestKind;//比赛场次类型
												}
												MyDebugString("sdp_test", "hall 玩家断线后回来发现已经加入房间，发送该消息给客户端。ereaid=%d, roomid=%d（%s:%d）", cmd.ereaid, cmd.roomid, cmd.strIP, cmd.uport);
												pSocket->SendData(&cmd, sizeof(struct_data_hs_mess_apply_login_recome_playing), e_hs_mess_apply, e_hs_ass_apply_login_recome_playing, 0);
											}
											else
											{//还没在房间里
												QK_InfoLog("sdp_test hall 玩家断线后回来发现还未加入房间，发送该消息给客户端。");
												onSendRoomListForContest(pSocket, data.ucontestid, e_hs_mess_apply, e_hs_ass_apply_login_recome_apply, e_hs_hcid_login_recome_apply_roomlist_part, e_hs_hcid_login_recome_apply_roomlist_finish);
											}
										}
										else
										{//告诉客户端，玩家并没有报名任何比赛
											QK_InfoLog("sdp_test hall 告诉客户端，玩家并没有报名任何比赛。");
											pSocket->SendData( e_hs_mess_apply, e_hs_ass_unapply, 0);
										}
										return true;
		}break;
		case e_gs_ass_login_c_force_out:
		{
										   //直接断开套接字
										   OutputDebugStringA("sdp_test CloseSocket(21)");
										   pSocket->CloseSocket();
										   return true;
		}break;
		default:
		{
				   break;
		}
		}

		return true;
	}

	//获得指定比赛下的roomlis，并发送给客户端
	void CMainManage_login::onSendRoomListForContest(CWJSSocket * pSocket, UINT contest, UINT bMainID, UINT bAssistantID, UINT bHandleCode_part, UINT bHandleCode_finish)
	{
		//获取构件名字
		UINT uBeginPos = 0;
		bool bFinish = false;
		char bBuffer[1500] = { 0 };
		UINT uCopyCount = 0;
		UINT uCount = m_CGameList.Getroom_nodeCount();
		do
		{
			UINT uCopyPos = m_CGameList.FillGameRoom(bBuffer, sizeof(bBuffer), contest, uBeginPos, bFinish, uCopyCount);
			uBeginPos += uCopyPos;

			if (0 == uCount)
			{
				bFinish = true;
			}

			if (bFinish)
			{
				pSocket->SendData(bBuffer, uCopyCount*sizeof(ComRoomInfo), bMainID, bAssistantID, bHandleCode_finish);
			}
			else
			{
				pSocket->SendData(bBuffer, uCopyCount*sizeof(ComRoomInfo), bMainID, bAssistantID, bHandleCode_part);
			}
			--uCount;
		} while (bFinish == false);
	}

	//游戏列表消息处理
	bool CMainManage_login::OnSocketRead_hs_mess_gamelist(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		switch (pNetHead->bAssistantID)
		{
		case e_hs_ass_gamelist_game:
		{//获取游戏	
									   //获取构件名字
									   UINT uBeginPos = 0;
									   bool bFinish = false;
									   char bBuffer[1500] = { 0 };
									   UINT uCopyCount = 0;
									   UINT uCount = m_CGameList.Getgame_nodeCount();
									   do
									   {
										   UINT uCopyPos = m_CGameList.FillGameName(bBuffer, sizeof(bBuffer), uBeginPos, bFinish, uCopyCount);
										   uBeginPos += uCopyPos;

										   if (0 == uCount)
										   {
											   bFinish = true;
										   }

										   if (bFinish)
										   {
											   //OutputDebugStringA("sdp_test2 获取游戏(bFinish)");
											   pSocket->SendData(bBuffer, uCopyCount*sizeof(ComGameinfo), e_hs_mess_gamelist, e_hs_ass_gamelist_game, e_hs_hcid_gamelist_finish);
										   }
										   else
										   {
											   //OutputDebugStringA("sdp_test2 获取游戏(bunFinish)");
											   pSocket->SendData(bBuffer, uCopyCount*sizeof(ComGameinfo), e_hs_mess_gamelist, e_hs_ass_gamelist_game, e_hs_hcid_gamelist_part);
										   }
										   --uCount;
									   } while (bFinish == false);

									   return true;
		}break;
		case e_hs_ass_gamelist_erea:
		{//	获取区服
									   struct_data_hs_ass_gamelist_erea* pCmd = (struct_data_hs_ass_gamelist_erea*)pData;
									   if (nullptr == pCmd)
									   {
										   return false;
									   }

									   //获取构件名字
									   UINT uBeginPos = 0;
									   bool bFinish = false;
									   char bBuffer[1500] = { 0 };
									   UINT uCopyCount = 0;
									   UINT uCount = m_CGameList.Geterea_nodeCount();
									   do
									   {
										   UINT uCopyPos = m_CGameList.FillErea(bBuffer, sizeof(bBuffer), pCmd->gameid, uBeginPos, bFinish, uCopyCount);
										   uBeginPos += uCopyPos;

										   if (0 == uCount)
										   {
											   bFinish = true;
										   }

										   if (bFinish)
										   {
											   //OutputDebugStringA("sdp_test2 获取区服(bFinish)");
											  pSocket->SendData(bBuffer, uCopyCount*sizeof(ComEreainfo), e_hs_mess_gamelist, e_hs_ass_gamelist_erea, e_hs_hcid_gamelist_finish);
										   }
										   else
										   {
											   //OutputDebugStringA("sdp_test2 获取区服(bunFinish)");
											   pSocket->SendData(bBuffer, uCopyCount*sizeof(ComEreainfo), e_hs_mess_gamelist, e_hs_ass_gamelist_erea, e_hs_hcid_gamelist_part);
										   }

										   --uCount;
									   } while (bFinish == false);

									   return true;
		}break;
		case e_hs_ass_gamelist_contest:
		{//	获取比赛
										  struct_data_hs_ass_gamelist_contest* pCmd = (struct_data_hs_ass_gamelist_contest*)pData;
										  if (nullptr == pCmd)
										  {
											  return false;
										  }

										  //获取构件名字
										  UINT uBeginPos = 0;
										  bool bFinish = false;
										  char bBuffer[1500] = { 0 };
										  UINT uCopyCount = 0;
										  UINT uCount = m_CGameList.Getcontest_nodeCount();
										  do
										  {
											  UINT uCopyPos = m_CGameList.FillGameContest(bBuffer, sizeof(bBuffer), pCmd->ereaid, uBeginPos, bFinish, uCopyCount);
											  uBeginPos += uCopyPos;

											  if (0 == uCount)
											  {
												  bFinish = true;
											  }

											  if (bFinish)
											  {
												  //OutputDebugStringA("sdp_test2 获取比赛(bFinish)");
												  pSocket->SendData(bBuffer, uCopyCount*sizeof(ComContestInfo), e_hs_mess_gamelist, e_hs_ass_gamelist_contest, e_hs_hcid_gamelist_finish);
											  }
											  else
											  {
												  //OutputDebugStringA("sdp_test2 获取比赛(bunFinish)");
												  pSocket->SendData(bBuffer, uCopyCount*sizeof(ComContestInfo), e_hs_mess_gamelist, e_hs_ass_gamelist_contest, e_hs_hcid_gamelist_part);
											  }
											  --uCount;
										  } while (bFinish == false);

										  return true;
		}break;
		case e_hs_ass_gamelist_contest_online:
		{//获取比赛节点(单节点数据)
												 struct_data_hs_ass_gamelist_contest_online* pCmd = (struct_data_hs_ass_gamelist_contest_online*)pData;
												 if (nullptr == pCmd)
												 {
													 return false;
												 }

												 //获得比赛数据
												 const ComContestInfo*p = m_CGameList.GetComContestInfo(pCmd->contestid);
												 if (nullptr == p)
												 {
													 return true;
												 }

												 ComContestInfo cmd;
												 std::memcpy(&cmd, p, sizeof(ComContestInfo));
												 pSocket->SendData( &cmd, sizeof(ComContestInfo), e_hs_mess_gamelist, e_hs_ass_gamelist_contest, e_hs_hcid_gamelist_finish);
												 return true;
		}break;
		case e_hs_ass_gamelist_room:
		{//获取房间									  
									   struct_data_hs_ass_gamelist_room* pCmd = (struct_data_hs_ass_gamelist_room*)pData;
									   if (nullptr == pCmd)
									   {
										   return false;
									   }

									   onSendRoomListForContest(pSocket, pCmd->contestid, e_hs_mess_gamelist, e_hs_ass_gamelist_room, e_hs_hcid_gamelist_part, e_hs_hcid_gamelist_finish);

									   return true;
		}break;
		case e_hs_ass_gamelist_onlien_count:
		{
				pSocket->SendData(nullptr, 0, e_hs_mess_gamelist, e_hs_ass_gamelist_onlien_count, m_update_now_playercount);
				return true;
		}break;
		default:
		{
				   return false;
		}
		}

		return true;
	}

	//报名消息处理
	bool CMainManage_login::OnSocketRead_hs_mess_apply(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		UINT uIndex = pSocket->GetSockID();
		switch (pNetHead->bAssistantID)
		{
		case e_hs_ass_apply:
		{
							   QK_InfoLog("sdp_test hall 玩家请求报名。");
							   struct_data_hs_mess_apply* p = (struct_data_hs_mess_apply*)pData;
							   if (nullptr == p || uSize != sizeof(struct_data_hs_mess_apply))
							   {
								   return false;
							   }

							   //检测是否人不存在
							   CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
							   if (nullptr == puser)
							   {
								   QK_InfoLog("sdp_test hall err 玩家不存在。");
								   return false;
							   }

							   if (!puser->bCanChangeUserStation(US_P_Applying))
							   {
								   QK_InfoLog("sdp_test hall err 玩家不能处于比赛。%d", puser->GetUserData()->basedata.bUserState);
								   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_unknown);
								   return true;
							   }

							   //检测比赛是否存在
							   const ComContestInfo* pContest = m_CGameList.GetComContestInfo(p->ucontestid);
							   if (nullptr == pContest)
							   {
								   QK_InfoLog("sdp_test hall err 比赛不存在（%d）。", p->ucontestid);
								   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_contest_inexistence);
								   return true;
							   }
							   //检测比赛是否有房间
							   if (!m_CGameList.ExistRoomForContest(p->ucontestid))
							   {
								   QK_InfoLog("sdp_test hall err 比赛没有房间。");
								  SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_room_inexistence);
								   return true;
							   }

							   //已报名，未结束
							   if (m_CApplayManage.bExist(puser->GetUserData()->basedata.dwUserID))
							   {
								   QK_InfoLog("sdp_test hall err 玩家已报名，未结束。");
								   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_applyed);
								   return true;
							   }

							   QK_InfoLog("sdp_test hall 报名费(userid=%d,uContestKind=%d,i64Money=%I64d,dwPoint=%d,_i64cost=%I64d)。"
								   , puser->GetUserData()->basedata.dwUserID
								   , pContest->uContestKind
								   , puser->GetUserData()->basedata.i64Money
								   , puser->GetUserData()->basedata.dwPoint
								   , pContest->_i64cost);
							   if (US_ContestKind_free == pContest->uContestKind)
							   {
								   struct_applynode cmd;
								   cmd.ucontestid = p->ucontestid;
								   cmd.userid = puser->GetUserData()->basedata.dwUserID;
								   m_CApplayManage.add(cmd);
								   QK_InfoLog("sdp_test hall 报名成功。");
								   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_suc);
		
							   }
							   else if (US_ContestRule_happy == pContest->uContestKind)
							   {//检测报名费够不够(娱币)
								   if (puser->GetUserData()->basedata.i64Money < pContest->_i64cost)
								   {
									   QK_InfoLog("sdp_test hall err 报名费够不够(娱币)。");
									   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_nomoney);
									   return true;
								   }						   
								   
								   puser->SetUserState(US_P_Applying);
								   /*
								   RRLine_c_apply rediscommand;
								   rediscommand.changemoney = pContest->_i64cost;
								   rediscommand.uContestKind = pContest->uContestKind;
								   rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
								   rediscommand.ucontestid = p->ucontestid;
								   m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_apply), uIndex);
								   */
								   CTransaction_apply *pTransApply = new CTransaction_apply;
								   assert(pTransApply);
								   pTransApply->m_in.uIndex = uIndex;
								   pTransApply->m_in.changemoney = pContest->_i64cost;
								   pTransApply->m_in.uContestKind = pContest->uContestKind;
								   pTransApply->m_in.userid = puser->GetUserData()->basedata.dwUserID;
								   pTransApply->m_in.ucontestid = p->ucontestid;
								   m_pRedis->do_transaction(Redis_Order_h_messid_apply,
									   uIndex,
									   onRedis_Order_h_messid_apply,
									   pTransApply);
							   }
							   else if (US_ContestRule_point == pContest->uContestKind)
							   {//检测报名费够不够(积分)
								   if (puser->GetUserData()->basedata.dwPoint < pContest->_i64cost)
								   {
									   QK_InfoLog("sdp_test hall err 报名费够不够(积分)。");
									   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_nomoney);
									   return true;
								   }
								   puser->SetUserState(US_P_Applying);
								   /*
								   puser->SetUserState(US_P_Applying);
								   RRLine_c_apply rediscommand;
								   rediscommand.changepoint = pContest->_i64cost;
								   rediscommand.uContestKind = pContest->uContestKind;
								   rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
								   rediscommand.ucontestid = p->ucontestid;
								   m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_apply), uIndex);
								   */
								   CTransaction_apply *pTransApply = new CTransaction_apply;
								   assert(pTransApply);
								   pTransApply->m_in.uIndex = uIndex;
								   pTransApply->m_in.changepoint = pContest->_i64cost;
								   pTransApply->m_in.uContestKind = pContest->uContestKind;
								   pTransApply->m_in.userid = puser->GetUserData()->basedata.dwUserID;
								   pTransApply->m_in.ucontestid = p->ucontestid;
								   m_pRedis->do_transaction(Redis_Order_h_messid_apply,
									   uIndex,
									   onRedis_Order_h_messid_apply,
									   pTransApply);
							   }

							   return true;
		}break;
		case e_hs_ass_apply_restitution:
		{
										   QK_InfoLog("sdp_test hall 收到玩家退费请求。");
										   CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
										   if (nullptr == puser)
										   {
											   QK_InfoLog("sdp_test hall 退费中，玩家不存在，错误。");
											   return false;
										   }

										   if (!puser->bCanChangeUserStation(US_P_unApplying))
										   {
											   QK_InfoLog("sdp_test hall err 玩家不能处于退费。%d", puser->GetUserData()->basedata.bUserState);
											   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply_restitution, e_hs_hcid_apply_restitution_err);
											   return true;
										   }

										   struct_applynode data;
										   if (!m_CApplayManage.at(puser->GetUserData()->basedata.dwUserID, data))
										   {
											   QK_InfoLog("sdp_test hall 告诉客户端，玩家并没有报名任何比赛。");
											   //告诉客户端，玩家并没有报名任何比赛					
											  SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_unapply, 0);
											   return true;
										   }

										   UINT usocketindex;										   
										   if (m_CGameList.GetRoomSocketID(data.uroomid, usocketindex))
										   {//已经加入房间，要询问房间
											   QK_InfoLog("sdp_test hall 已经加入房间，要询问房间。");
											   struct_data_pr_userinfo_apply_restitution cmd;
											   cmd.userid = puser->GetUserData()->basedata.dwUserID;
											   SendData(usocketindex, &cmd, sizeof(cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_apply_restitution, 0);
										   }
										   else
										   {//没在房间，则直接同意
											   //检测比赛是否存在
											   const ComContestInfo* pContest = m_CGameList.GetComContestInfo(data.ucontestid);
											   if (nullptr != pContest)
											   {//钱退给玩家
												   puser->SetUserState(US_P_unApplying);
												   /*
												   RRLine_c_apply_restitution rediscommand;
												   rediscommand.changepoint = pContest->_i64cost;
												   rediscommand.uContestKind = pContest->uContestKind;
												   rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
												   rediscommand.ucontestid = data.ucontestid;
												   QK_InfoLog("sdp_test hall 有比赛但没在房间，则查询redis处理退费。");
												   m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_apply_restitution), uIndex);
												   */
												   CTransaction_apply_restitution *pTransResti = new CTransaction_apply_restitution;
												   assert(pTransResti);
												   pTransResti->m_in.uIndex = uIndex;
												   pTransResti->m_in.changepoint = pContest->_i64cost;
												   pTransResti->m_in.uContestKind = pContest->uContestKind;
												   pTransResti->m_in.userid = puser->GetUserData()->basedata.dwUserID;
												   pTransResti->m_in.ucontestid = data.ucontestid;
												   QK_InfoLog("sdp_test hall 有比赛但没在房间，则查询redis处理退费。");
												   m_pRedis->do_transaction(Redis_Order_h_messid_apply_restitution,
													   uIndex,
													   onRedis_Order_h_messid_apply_restitution,
													   pTransResti);

											   }
											   else
											   {
												   QK_InfoLog("sdp_test hall 没有这比赛，则拒绝退费。");
												   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply_restitution, e_hs_hcid_apply_restitution_err);
											   }											   
										   }

										   return true;
		}break;
		default:
		{
				   break;
		}
		}

		return false;
	}

	//团队消息处理
	bool CMainManage_login::OnSocketRead_hs_mess_team(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		UINT uIndex = pSocket->GetSockID();
		switch (pNetHead->bAssistantID)
		{
		case e_hs_ass_team_create:
		{//创建团队
									 QK_InfoLog("sdp_test hall 收到玩家创建团队请求。");
									 struct_data_hs_mess_team_create* p = (struct_data_hs_mess_team_create*)pData;
									 if (nullptr == p || uSize != sizeof(struct_data_hs_mess_team_create))
									 {
										 return false;
									 }

									 //检测是否人不存在
									 CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
									 if (nullptr == puser)
									 {
										 return false;
									 }

									 if (!puser->bCanChangeUserStation(US_P_CreateTeaming))
									 {
										 QK_InfoLog("sdp_test hall err 玩家不能处于创建团队。%d", puser->GetUserData()->basedata.bUserState);
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_unknown);
										 return true;
									 }

									 //检测比赛是否存在
									 const ComContestInfo* pContest = m_CGameList.GetComContestInfo(p->ucontestid);
									 if (nullptr == pContest)
									 {
										 QK_InfoLog("sdp_test hall err 比赛不存在。");
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_contest_inexistence);
										 return true;
									 }

									 //比赛模式不对
									 if (US_ContestRule_team != pContest->uContestRule)
									 {
										 QK_InfoLog("sdp_test hall err 比赛模式不是团队赛（%d）。", pContest->uContestRule);
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_contest_rule);
										 return true;
									 }

									 //检测比赛是否有房间
									 if (!m_CGameList.ExistRoomForContest(p->ucontestid))
									 {
										 QK_InfoLog("sdp_test hall err 比赛没有房间。");
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_room_inexistence);
										 return true;
									 }

									 //已报名，未结束
									 if (m_CApplayManage.bExist(puser->GetUserData()->basedata.dwUserID))
									 {
										 QK_InfoLog("sdp_test hall err 玩家已报名，未结束。");
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_applyed);
										 return true;
									 }

									 TeamStruct _TeamStruct;
									 if (m_CTeamManage.find(puser->GetUserData()->basedata.dwUserID, _TeamStruct))
									 {//已在团队中，未解散
										 QK_InfoLog("sdp_test hall err 已在团队中，未解散。");
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_exist);
										 return true;
									 }

									 LLONG _i64cost = 0;
									 if (TeamPayType_AA == p->m_TeamPayType)
									 {
										 _i64cost = pContest->_i64cost / (UINT)pContest->uRoomRule;
									 }
									 else if (TeamPayType_Captain == p->m_TeamPayType)
									 {
										 _i64cost = pContest->_i64cost;
									 }
									 else
									 {
										 QK_InfoLog("sdp_test hall err 支付类型未知(%d)。", p->m_TeamPayType);
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_PayType);
										 return true;
									 }

									 if (US_ContestKind_free == pContest->uContestKind)
									 {
										 TeamStruct cmd;
										 cmd.m_uCaptain_userid = puser->GetUserData()->basedata.dwUserID;
										 cmd.m_TeamPayType = p->m_TeamPayType;
										 cmd.m_uContest = p->ucontestid;
										 cmd.m_uTemaPlayerNum = (UINT)pContest->uRoomRule;
										 if (!m_CTeamManage.add(cmd))
										 {//已创建，未解散
											 QK_InfoLog("sdp_test hall err 已创建，未解散2。");
											SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_exist);
											 return true;
										 }

										 QK_InfoLog("sdp_test hall 创建团队成功。");
										 SendData(uIndex, &cmd, sizeof(struct_data_hs_mess_team_create_res), e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_suc);
									 }
									 else if (US_ContestRule_happy == pContest->uContestKind)
									 {//检测创建团队费够不够(娱币)
										 if (puser->GetUserData()->basedata.i64Money < _i64cost)
										 {
											 QK_InfoLog("sdp_test hall err 创建团队费够不够(娱币)。");
											 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_nomoney);
											 return true;
										 }

										 QK_InfoLog("sdp_test hall 开始执行redis操作。");
										 puser->SetUserState(US_P_CreateTeaming);
										 /*
										 RRLine_c_team_create rediscommand;
										 rediscommand.changemoney = _i64cost;
										 rediscommand.uContestKind = pContest->uContestKind;
										 rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
										 rediscommand.ucontestid = p->ucontestid;
										 rediscommand.m_TeamPayType = p->m_TeamPayType;
										 rediscommand.uRoomRule = pContest->uRoomRule;
										 m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_team_create), uIndex);
										 */
										 CTransaction_team_create *pTransTeamCreat = new CTransaction_team_create;
										 assert(pTransTeamCreat);
										 pTransTeamCreat->m_in.uIndex = uIndex;
										 pTransTeamCreat->m_in.changemoney = _i64cost;
										 pTransTeamCreat->m_in.uContestKind = pContest->uContestKind;
										 pTransTeamCreat->m_in.userid = puser->GetUserData()->basedata.dwUserID;
										 pTransTeamCreat->m_in.ucontestid = p->ucontestid;
										 pTransTeamCreat->m_in.m_TeamPayType = p->m_TeamPayType;
										 pTransTeamCreat->m_in.uRoomRule = pContest->uRoomRule;
										 m_pRedis->do_transaction(Redis_Order_h_messid_team_create,
											 uIndex,
											 onRedis_Order_h_messid_team_create,
											 pTransTeamCreat);
									 }
									 else if (US_ContestRule_point == pContest->uContestKind)
									 {//检测创建团队费够不够(积分)
										 if (puser->GetUserData()->basedata.dwPoint < _i64cost)
										 {
											 QK_InfoLog("sdp_test hall err 创建团队费够不够(积分)。");
											SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_nomoney);
											 return true;
										 }

										 QK_InfoLog("sdp_test hall 开始执行redis操作。");
										 puser->SetUserState(US_P_CreateTeaming);
										 /*
										 RRLine_c_team_create rediscommand;
										 rediscommand.changepoint = _i64cost;
										 rediscommand.uContestKind = pContest->uContestKind;
										 rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
										 rediscommand.ucontestid = p->ucontestid;
										 rediscommand.m_TeamPayType = p->m_TeamPayType;
										 rediscommand.uRoomRule = pContest->uRoomRule;
										 m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_team_create), uIndex);
										 */
										 CTransaction_team_create *pTransTeamCreat = new CTransaction_team_create;
										 assert(pTransTeamCreat);
										 pTransTeamCreat->m_in.uIndex = uIndex;
										 pTransTeamCreat->m_in.changepoint = _i64cost;
										 pTransTeamCreat->m_in.uContestKind = pContest->uContestKind;
										 pTransTeamCreat->m_in.userid = puser->GetUserData()->basedata.dwUserID;
										 pTransTeamCreat->m_in.ucontestid = p->ucontestid;
										 pTransTeamCreat->m_in.m_TeamPayType = p->m_TeamPayType;
										 pTransTeamCreat->m_in.uRoomRule = pContest->uRoomRule;
										 m_pRedis->do_transaction(Redis_Order_h_messid_team_create,
											 uIndex,
											 onRedis_Order_h_messid_team_create,
											 pTransTeamCreat);
									 }
		}break;
		case e_hs_ass_team_join:
		{//加入团队
									QK_InfoLog("sdp_test hall 收到玩家加入团队请求。");
									struct_data_hs_mess_team_join* p = (struct_data_hs_mess_team_join*)pData;
									if (nullptr == p || uSize != sizeof(struct_data_hs_mess_team_join))
									{
										return false;
									}

									//检测是否人不存在
									CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
									if (nullptr == puser)
									{
										return false;
									}

									if (!puser->bCanChangeUserStation(US_P_AddTeaming))
									{
										QK_InfoLog("sdp_test hall err 玩家不能处于加入团队。%d", puser->GetUserData()->basedata.bUserState);
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_unknown);
										return true;
									}

									//检测比赛是否存在
									const ComContestInfo* pContest = m_CGameList.GetComContestInfo(p->ucontestid);
									if (nullptr == pContest)
									{
										QK_InfoLog("sdp_test hall err 比赛不存在。");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_contest_inexistence);
										return true;
									}

									//比赛模式不对
									if (US_ContestRule_team != pContest->uContestRule)
									{
										QK_InfoLog("sdp_test hall err 比赛模式不是团队赛（%d）。", pContest->uContestRule);
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_contest_rule);
										return true;
									}

									//已报名，未结束
									if (m_CApplayManage.bExist(puser->GetUserData()->basedata.dwUserID))
									{
										QK_InfoLog("sdp_test hall err 玩家已报名，未结束。");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_applyed);
										return true;
									}

									TeamStruct _TeamStruct;
									if (m_CTeamManage.find(puser->GetUserData()->basedata.dwUserID, _TeamStruct))
									{//已在团队中，未解散
										QK_InfoLog("sdp_test hall err 已在团队中，未解散。");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_exist);
										return true;
									}

									std::memset(&_TeamStruct, 0, sizeof(TeamStruct));

									LLONG _i64cost = 0;
									if (!m_CTeamManage.find(p->m_Team_password, _TeamStruct))
									{//团队暗号错误
										QK_InfoLog("sdp_test hall err 团队暗号错误。");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_password);
										return true;
									}
									else
									{
										if (TeamPayType_AA == _TeamStruct.m_TeamPayType)
										{
											_i64cost = pContest->_i64cost / (UINT)pContest->uRoomRule;
										}
										else if (TeamPayType_Captain == _TeamStruct.m_TeamPayType)
										{
											_i64cost = 0;
										}
									}
																		
									if (US_ContestKind_free == pContest->uContestKind)
									{										
										if (m_CTeamManage.bFull(_TeamStruct.m_Team_password))
										{//人数已满，加入失败
											QK_InfoLog("sdp_test hall err 人数已满，加入失败。");
											SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_full);
											return true;
										}

										QK_InfoLog("sdp_test hall 加入团队成功。");
										m_CTeamManage.userAdd(puser->GetUserData()->basedata.dwUserID, _TeamStruct.m_Team_password);
										std::memset(&_TeamStruct, 0, sizeof(TeamStruct));
										m_CTeamManage.find(p->m_Team_password, _TeamStruct);
										SendData(uIndex, &_TeamStruct, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
										TeamSendData(puser->GetUserData()->basedata.dwUserID, &_TeamStruct, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
									}
									else if (US_ContestRule_happy == pContest->uContestKind)
									{//检测加入团队费够不够(娱币)
										if (0 == _i64cost)
										{
											if (m_CTeamManage.bFull(_TeamStruct.m_Team_password))
											{//人数已满，加入失败
												QK_InfoLog("sdp_test hall err 人数已满，加入失败2。");
												SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_full);
												return true;
											}

											QK_InfoLog("sdp_test hall 加入团队成功2。");
											m_CTeamManage.userAdd(puser->GetUserData()->basedata.dwUserID, _TeamStruct.m_Team_password);
											std::memset(&_TeamStruct, 0, sizeof(TeamStruct));
											m_CTeamManage.find(p->m_Team_password, _TeamStruct);
											SendData(uIndex, &_TeamStruct, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
											TeamSendData(puser->GetUserData()->basedata.dwUserID, &_TeamStruct, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
										}
										else
										{
											if (puser->GetUserData()->basedata.i64Money < _i64cost)
											{
												QK_InfoLog("sdp_test hall err 加入团队费够不够(娱币)。");
												SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_nomoney);
												return true;
											}

											QK_InfoLog("sdp_test hall 开始执行redis操作。");
											if (m_CTeamManage.userAdd(puser->GetUserData()->basedata.dwUserID, _TeamStruct.m_Team_password))
											{
												puser->SetUserState(US_P_AddTeaming);
												/*
												RRLine_c_team_join rediscommand;
												rediscommand.changemoney = _i64cost;
												rediscommand.uContestKind = pContest->uContestKind;
												rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
												rediscommand.ucontestid = pContest->contestid;
												std::memcpy(rediscommand.m_Team_password, p->m_Team_password, sizeof(rediscommand.m_Team_password));
												m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_team_join), uIndex);
												*/
												CTransaction_team_join *pTransTeamJoin = new CTransaction_team_join;
												assert(pTransTeamJoin);
												pTransTeamJoin->m_in.uIndex = uIndex;
												pTransTeamJoin->m_in.changemoney = _i64cost;
												pTransTeamJoin->m_in.uContestKind = pContest->uContestKind;
												pTransTeamJoin->m_in.userid = puser->GetUserData()->basedata.dwUserID;
												pTransTeamJoin->m_in.ucontestid = pContest->contestid;
												std::memcpy(pTransTeamJoin->m_in.m_Team_password, p->m_Team_password, sizeof(pTransTeamJoin->m_in.m_Team_password));
												m_pRedis->do_transaction(Redis_Order_h_messid_team_join,
													uIndex,
													onRedis_Order_h_messid_team_join,
													pTransTeamJoin);
											}
										}										
									}
									else if (US_ContestRule_point == pContest->uContestKind)
									{//检测加入团队费够不够(积分)
										if (0 == _i64cost)
										{
											if (m_CTeamManage.bFull(_TeamStruct.m_Team_password))
											{//人数已满，加入失败
												QK_InfoLog("sdp_test hall err 人数已满，加入失败3。");
												SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_full);
												return true;
											}

											QK_InfoLog("sdp_test hall 加入团队成功3。");
											m_CTeamManage.userAdd(puser->GetUserData()->basedata.dwUserID, _TeamStruct.m_Team_password);
											std::memset(&_TeamStruct, 0, sizeof(TeamStruct));
											m_CTeamManage.find(p->m_Team_password, _TeamStruct);
											SendData(uIndex, &_TeamStruct, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
											TeamSendData(puser->GetUserData()->basedata.dwUserID, &_TeamStruct, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
										}
										else
										{
											if (puser->GetUserData()->basedata.dwPoint < _i64cost)
											{
												QK_InfoLog("sdp_test hall err 加入团队费够不够(积分)。");
												SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_nomoney);
												return true;
											}

											QK_InfoLog("sdp_test hall 开始执行redis操作。");
											if (m_CTeamManage.userAdd(puser->GetUserData()->basedata.dwUserID, _TeamStruct.m_Team_password))
											{//先加进去，等结果出来，再判断失败就删除也不迟
												puser->SetUserState(US_P_AddTeaming);
												/*
												RRLine_c_team_join rediscommand;
												rediscommand.changepoint = _i64cost;
												rediscommand.uContestKind = pContest->uContestKind;
												rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
												rediscommand.ucontestid = pContest->contestid;
												std::memcpy(rediscommand.m_Team_password, p->m_Team_password, sizeof(rediscommand.m_Team_password));
												m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_team_join), uIndex);
												*/
												CTransaction_team_join *pTransTeamJoin = new CTransaction_team_join;
												assert(pTransTeamJoin);
												pTransTeamJoin->m_in.uIndex = uIndex;
												pTransTeamJoin->m_in.changepoint = _i64cost;
												pTransTeamJoin->m_in.uContestKind = pContest->uContestKind;
												pTransTeamJoin->m_in.userid = puser->GetUserData()->basedata.dwUserID;
												pTransTeamJoin->m_in.ucontestid = pContest->contestid;
												std::memcpy(pTransTeamJoin->m_in.m_Team_password, p->m_Team_password, sizeof(pTransTeamJoin->m_in.m_Team_password));
												m_pRedis->do_transaction(Redis_Order_h_messid_team_join,
													uIndex,
													onRedis_Order_h_messid_team_join,
													pTransTeamJoin);
											}
										}										
									}
		}break;
		case e_hs_ass_team_leave:
		{//退出团队									
									QK_InfoLog("sdp_test hall 收到玩家退出团队请求。");
									struct_data_hs_mess_team_leave* p = (struct_data_hs_mess_team_leave*)pData;
									if (nullptr == p || uSize != sizeof(struct_data_hs_mess_team_leave))
									{
										return false;
									}

									//检测是否人不存在
									CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
									if (nullptr == puser)
									{
										return false;
									}

									if (!puser->bCanChangeUserStation(US_P_LeaveTeaming))
									{
										QK_InfoLog("sdp_test hall err 玩家不能处于退出团队。%d", puser->GetUserData()->basedata.bUserState);
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_unknown);
										return true;
									}

									//检测比赛是否存在
									const ComContestInfo* pContest = m_CGameList.GetComContestInfo(p->ucontestid);
									if (nullptr == pContest)
									{
										QK_InfoLog("sdp_test hall err 比赛不存在。");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_contest_inexistence);
										return true;
									}

									//比赛模式不对
									if (US_ContestRule_team != pContest->uContestRule)
									{
										QK_InfoLog("sdp_test hall err 比赛模式不是团队赛（%d）。", pContest->uContestRule);
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_contest_rule);
										return true;
									}

									//已报名，未结束
									if (m_CApplayManage.bExist(puser->GetUserData()->basedata.dwUserID))
									{
										QK_InfoLog("sdp_test hall err 玩家已报名，未结束。");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_applyed);
										return true;
									}

									TeamStruct _TeamStruct;
									bool bfind = m_CTeamManage.find(puser->GetUserData()->basedata.dwUserID, _TeamStruct);
									if (bfind && _TeamStruct.m_uCaptain_userid == puser->GetUserData()->basedata.dwUserID)
									{//已在团队中，且自己是队长，自己不能退自己创建的团队
										QK_InfoLog("sdp_test hall err 已在团队中，且自己是队长，自己不能退自己创建的团队。");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_created);
										return true;
									}

									//判断玩家是不是所在的团队已经开始vs了
									for (int i = 0; i < m_CTeamManage.Count_VS(); ++i)
									{
										TeamStruct_VS _TeamStruct_VS;
										m_CTeamManage.at_vs(i, _TeamStruct_VS);
										if (0 == strcmp(_TeamStruct_VS._TeamStruct[0].m_Team_password, p->m_Team_password)
											|| 0 == strcmp(_TeamStruct_VS._TeamStruct[1].m_Team_password, p->m_Team_password))
										{
											QK_InfoLog("sdp_test hall 团队正在vs,不能离开。");
											SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_playing);
											return true;
										}
									}

									TeamStruct _TeamStruct2;
									bool bfind2 = m_CTeamManage.find(p->m_Team_password, _TeamStruct2);
									if (0 != strcmp(p->m_Team_password, _TeamStruct.m_Team_password)
										|| 0 != strcmp(p->m_Team_password, _TeamStruct2.m_Team_password)
										)
									{//如果用户传来的暗号有问题，就拒绝
										QK_InfoLog("sdp_test hall err 如果用户传来的暗号有问题，就拒绝,团队暗号错误。");
										bfind2 = false;
									}

									LLONG _i64cost = 0;
									if (!bfind2)
									{//团队暗号错误
										QK_InfoLog("sdp_test hall err 团队暗号错误。");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_password);
										return true;
									}
									else
									{
										if (TeamPayType_AA == _TeamStruct.m_TeamPayType)
										{
											_i64cost = pContest->_i64cost / (UINT)pContest->uRoomRule;
										}
										else if (TeamPayType_Captain == _TeamStruct.m_TeamPayType)
										{
											_i64cost = 0;
										}
									}

									bool bUnNeedMoney = false;
									if (US_ContestKind_free == pContest->uContestKind)
									{
										bUnNeedMoney = true;										
									}
									else if (US_ContestRule_happy == pContest->uContestKind)
									{//检测加入团队费够不够(娱币)
										if (0 == _i64cost)
										{
											bUnNeedMoney = true;
										}
									}
									else if (US_ContestRule_point == pContest->uContestKind)
									{//检测加入团队费够不够(积分)
										if (0 == _i64cost)
										{
											bUnNeedMoney = true;
										}
									}
									else
									{//其他情况不处理
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_unknown);
										return true;
									}

									if (bUnNeedMoney)
									{
										QK_InfoLog("sdp_test hall 退出团队成功。");
										struct_data_hs_mess_team_leave_res cmd;
										cmd.userid = puser->GetUserData()->basedata.dwUserID;
										m_CTeamManage.userDel(puser->GetUserData()->basedata.dwUserID, _TeamStruct.m_Team_password);
										SendData(uIndex, &cmd, sizeof(struct_data_hs_mess_team_leave_res), e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_suc);
										UINT uTemaPlayerNum;//队伍人数
										UINT Userid[TEAM_USER_NUM];//每个人的userid
										m_CTeamManage.GetUsers(p->m_Team_password, Userid, uTemaPlayerNum);
										for (int i = 0; i < uTemaPlayerNum; ++i)
										{
											CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(Userid[i]);
											if (nullptr != puser)
											{
												SendData(puser->GetSocketIndex(), &cmd, sizeof(struct_data_hs_mess_team_leave_res), e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_suc);
											}
										}
									}
									else
									{
										QK_InfoLog("sdp_test hall err 开始执行redis操作。");	
										puser->SetUserState(US_P_LeaveTeaming);
										/*
										RRLine_c_team_leave rediscommand;
										if (US_ContestRule_happy == pContest->uContestKind)
										{//加入团队费(娱币)
											rediscommand.changemoney = _i64cost;
										}
										if (US_ContestRule_point == pContest->uContestKind)
										{//加入团队费(积分)
											rediscommand.changepoint = _i64cost;
										}
										rediscommand.uContestKind = pContest->uContestKind;
										rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
										rediscommand.ucontestid = pContest->contestid;
										std::memcpy(rediscommand.m_Team_password, p->m_Team_password, sizeof(rediscommand.m_Team_password));										
										m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_team_leave), uIndex);
										*/
										CTransaction_team_leave *pTransTeamLeave = new CTransaction_team_leave;
										assert(pTransTeamLeave);
										pTransTeamLeave->m_in.uIndex = uIndex;
										if (US_ContestRule_happy == pContest->uContestKind)
										{//加入团队费(娱币)
											pTransTeamLeave->m_in.changemoney = _i64cost;
										}
										if (US_ContestRule_point == pContest->uContestKind)
										{//加入团队费(积分)
											pTransTeamLeave->m_in.changepoint = _i64cost;
										}
										pTransTeamLeave->m_in.uContestKind = pContest->uContestKind;
										pTransTeamLeave->m_in.userid = puser->GetUserData()->basedata.dwUserID;
										pTransTeamLeave->m_in.ucontestid = pContest->contestid;
										std::memcpy(pTransTeamLeave->m_in.m_Team_password, p->m_Team_password, sizeof(pTransTeamLeave->m_in.m_Team_password));
										m_pRedis->do_transaction(Redis_Order_h_messid_team_leave,
											uIndex,
											onRedis_Order_h_messid_team_leave,
											pTransTeamLeave);
									}
		}break;
		case e_hs_ass_team_dissolve:
		{//解散团队
									   QK_InfoLog("sdp_test hall 收到玩家解散团队请求。");
									   struct_data_hs_mess_team_dissolve* p = (struct_data_hs_mess_team_dissolve*)pData;
									   if (nullptr == p || uSize != sizeof(struct_data_hs_mess_team_dissolve))
									   {
										   return false;
									   }

									   //检测是否人不存在
									   CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
									   if (nullptr == puser)
									   {
										   return false;
									   }

									   if (!puser->bCanChangeUserStation(US_P_DissolveTeaming))
									   {
										   QK_InfoLog("sdp_test hall err 玩家不能处于解散团队。%d", puser->GetUserData()->basedata.bUserState);
										   SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_unknown);
										   return true;
									   }

									   //检测比赛是否存在
									   const ComContestInfo* pContest = m_CGameList.GetComContestInfo(p->ucontestid);
									   if (nullptr == pContest)
									   {
										   QK_InfoLog("sdp_test hall err 比赛不存在。");
										   SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_contest_inexistence);
										   return true;
									   }

									   //比赛模式不对
									   if (US_ContestRule_team != pContest->uContestRule)
									   {
										   QK_InfoLog("sdp_test hall err 比赛模式不是团队赛（%d）。", pContest->uContestRule);
										   SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_contest_rule);
										   return true;
									   }

									   //已报名，未结束
									   if (m_CApplayManage.bExist(puser->GetUserData()->basedata.dwUserID))
									   {
										   QK_InfoLog("sdp_test hall err 玩家已报名，未结束。");
										  SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_applyed);
										   return true;
									   }

									   TeamStruct _TeamStruct;
									   bool bfind = m_CTeamManage.find(puser->GetUserData()->basedata.dwUserID, _TeamStruct);
									   if (!bfind || _TeamStruct.m_uCaptain_userid != puser->GetUserData()->basedata.dwUserID)
									   {//解散团队失败_自己并没有创建团队
										   QK_InfoLog("sdp_test hall err 解散团队失败_自己并没有创建团队。");
										   SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_uncreated);
										   return true;
									   }

									   //判断玩家是不是所在的团队已经开始vs了
									   for (int i = 0; i < m_CTeamManage.Count_VS(); ++i)
									   {
										   TeamStruct_VS _TeamStruct_VS;
										   m_CTeamManage.at_vs(i, _TeamStruct_VS);
										   if (0 == strcmp(_TeamStruct_VS._TeamStruct[0].m_Team_password, p->m_Team_password)
											   || 0 == strcmp(_TeamStruct_VS._TeamStruct[1].m_Team_password, p->m_Team_password))
										   {
											   QK_InfoLog("sdp_test hall 团队正在vs,不能解散。");
											   SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_playing);
											   return true;
										   }
									   }

									   TeamStruct _TeamStruct2;
									   bool bfind2 = m_CTeamManage.find(p->m_Team_password, _TeamStruct2);
									   if (0 != strcmp(p->m_Team_password, _TeamStruct.m_Team_password)
										   || 0 != strcmp(p->m_Team_password, _TeamStruct2.m_Team_password)
										   )
									   {//如果用户传来的暗号有问题，就拒绝
										   QK_InfoLog("sdp_test hall err 如果用户传来的暗号有问题，就拒绝,团队暗号错误。");
										   bfind2 = false;
									   }

									   LLONG _i64cost = 0;
									   if (!bfind2)
									   {//团队暗号错误
										   QK_InfoLog("sdp_test hall err 团队暗号错误。");
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_password);
										   return true;
									   }
									   else
									   {
										   if (TeamPayType_AA == _TeamStruct.m_TeamPayType)
										   {
											   _i64cost = pContest->_i64cost / (UINT)pContest->uRoomRule;
										   }
										   else if (TeamPayType_Captain == _TeamStruct.m_TeamPayType)
										   {
											   _i64cost = 0;
										   }
									   }

									   bool bUnNeedMoney = false;
									   if (US_ContestKind_free == pContest->uContestKind)
									   {
										   bUnNeedMoney = true;
									   }
									   else if (US_ContestRule_happy == pContest->uContestKind)
									   {//检测加入团队费够不够(娱币)
										   if (0 == _i64cost)
										   {
											   bUnNeedMoney = true;
										   }
									   }
									   else if (US_ContestRule_point == pContest->uContestKind)
									   {//检测加入团队费够不够(积分)
										   if (0 == _i64cost)
										   {
											   bUnNeedMoney = true;
										   }
									   }
									   else
									   {//其他情况不处理
										   SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_unknown);
										   return true;
									   }

									   if (bUnNeedMoney)
									   {
										   QK_InfoLog("sdp_test hall 解散团队成功。");
										   UINT uTemaPlayerNum;//队伍人数
										   UINT Userid[TEAM_USER_NUM];//每个人的userid
										   m_CTeamManage.GetUsers(p->m_Team_password, Userid, uTemaPlayerNum);
										   for (int i = 0; i < uTemaPlayerNum; ++i)
										   {//通知所有人解散团队
											   CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(Userid[i]);
											   if (nullptr != puser)
											   {
												   SendData(puser->GetSocketIndex(), nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_suc);
											   }
										   }
										   m_CTeamManage.del(p->m_Team_password);
									   }
									   else
									   {
										   QK_InfoLog("sdp_test hall 开始执行redis操作。");
										   puser->SetUserState(US_P_DissolveTeaming);
										   /*
										   RRLine_c_team_dissolve rediscommand;
										   for (int i = 0; i < _TeamStruct.m_uTemaPlayerNum; ++i)
										   {
											   if (US_ContestRule_happy == pContest->uContestKind)
											   {//加入团队费(娱币)
												   rediscommand.changemoney[i] = _i64cost;
											   }
											   if (US_ContestRule_point == pContest->uContestKind)
											   {//加入团队费(积分)
												   rediscommand.changepoint[i] = _i64cost;
											   }
											   rediscommand.userid[i] = _TeamStruct.m_Userid[i];
										   }

										   rediscommand.uContestKind = pContest->uContestKind;
										   rediscommand.m_uTemaPlayerNum = _TeamStruct.m_uTemaPlayerNum;
										   rediscommand.ucontestid = pContest->contestid;
										   std::memcpy(rediscommand.m_Team_password, p->m_Team_password, sizeof(rediscommand.m_Team_password));
										   m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_team_dissolve), uIndex);
										   */
										   CTransaction_team_dissolve *pTransTeamDis = new CTransaction_team_dissolve;
										   assert(pTransTeamDis);
										   pTransTeamDis->m_in.uIndex = uIndex;
										   for (int i = 0; i < _TeamStruct.m_uTemaPlayerNum; ++i)
										   {
											   if (US_ContestRule_happy == pContest->uContestKind)
											   {//加入团队费(娱币)
												   pTransTeamDis->m_in.changemoney[i] = _i64cost;
											   }
											   if (US_ContestRule_point == pContest->uContestKind)
											   {//加入团队费(积分)
												   pTransTeamDis->m_in.changepoint[i] = _i64cost;
											   }
											   pTransTeamDis->m_in.userid[i] = _TeamStruct.m_Userid[i];
										   }

										   pTransTeamDis->m_in.uContestKind = pContest->uContestKind;
										   pTransTeamDis->m_in.m_uTemaPlayerNum = _TeamStruct.m_uTemaPlayerNum;
										   pTransTeamDis->m_in.ucontestid = pContest->contestid;
										   std::memcpy(pTransTeamDis->m_in.m_Team_password, p->m_Team_password, sizeof(pTransTeamDis->m_in.m_Team_password));
										   m_pRedis->do_transaction(Redis_Order_h_messid_team_dissolve,
											   uIndex,
											   onRedis_Order_h_messid_team_dissolve,
											   pTransTeamDis);
									   }
		}break;
		case e_hs_ass_team_start:
		{//队长发起开始请求
									struct_data_hs_mess_team_start* p = (struct_data_hs_mess_team_start*)pData;
									if (nullptr == p || uSize != sizeof(struct_data_hs_mess_team_start))
									{
										return false;
									}

									//检测是否人不存在
									CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
									if (nullptr == puser)
									{
										return false;
									}

									//检测比赛是否存在
									const ComContestInfo* pContest = m_CGameList.GetComContestInfo(p->ucontestid);
									if (nullptr == pContest)
									{
										return true;
									}

									TeamStruct _TeamStruct;
									if (!m_CTeamManage.find(p->m_Team_password, _TeamStruct))
									{//不存在团队
										return true;
									}

									if (!m_CTeamManage.bFull(p->m_Team_password))
									{//人还没齐
										return true;
									}

									if (_TeamStruct.m_Userid[0] != puser->GetUserData()->basedata.dwUserID)
									{//不是队长，不处理
										return true;
									}

									//设置同意匹配
									m_CTeamManage.setButVS(p->m_Team_password, true);									
		}break;
		default:
		{
				   break;
		}
		}
		return true;
	}

	//房间消息处理
	bool CMainManage_login::OnSocketRead_pr_mess_roominfo(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		UINT uIndex = pSocket->GetSockID();
		switch (pNetHead->bAssistantID)
		{
		case e_pr_ass_roominfo_update:
		{//已连接上的房间服务器在给大厅更新房间数据
										 //QK_InfoLog("ls 已连接上的房间服务器在给大厅更新房间数据。");
										 struct_data_pr_roominfo_update* p = (struct_data_pr_roominfo_update*)pData;
										 if (nullptr == p || uSize != sizeof(struct_data_pr_roominfo_update))
										 {
											 return false;
										 }

										 ComRoomInfo_node cmd;
										 cmd.usocketindex = uIndex;
										 std::memcpy(&cmd.room_node, p, sizeof(ComRoomInfo));
										 //QK_InfoLog("ls 已连接上的房间服务器在给大厅更新房间数据roomid=%d,(%s:%d)", cmd.room_node.roomid, cmd.room_node.strIP, cmd.room_node.uport);
										 m_CGameList.addroom_node(cmd);

										 bool bexist = false;
										 auto iter = m_socket_vector.begin();
										 while (iter != m_socket_vector.end())
										 {
											 if ((*iter) == uIndex)
											 {
												 bexist = true;
												 break;
											 }
											 ++iter;
										 }

										 if (!bexist)
										 {//存在就不处理了
											 m_socket_vector.push_back(uIndex);
										 }
		}break;
		default:
		{
				   break;
		}
		}

		return true;
	}

	//用户消息处理
	bool CMainManage_login::OnSocketRead_pr_mess_userinfo(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		UINT uIndex = pSocket->GetSockID();
		switch (pNetHead->bAssistantID)
		{
		case e_pr_ass_userinfo_login:
		{
										QK_InfoLog("sdp_test hall 游戏服务器告知登陆服务器，有玩家登录游戏服务器，请求确认。");
										struct_data_pr_userinfo_login* p = (struct_data_pr_userinfo_login*)pData;
										if (nullptr == p || uSize != sizeof(struct_data_pr_userinfo_login))
										{
											return false;
										}

										struct_data_pr_userinfo_login_res_err cmd;
										cmd.userid = p->uuserid;
										cmd.uIndex = p->uIndex;
										struct_data_pr_userinfo_login_res_ok cmd2;
										cmd2.uIndex = p->uIndex;
										cmd2.contestid = p->contestid;
										const ComContestInfo* pComContestInfo = m_CGameList.GetComContestInfo(p->contestid);
										if (nullptr == pComContestInfo)
										{

											QK_WarnLog("sdp_test hall 大厅告诉游戏服务器，不同意该玩家登录游戏服务器，比赛不存在。(%d)", p->contestid);
											SendData(uIndex, &cmd, sizeof(cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_login, e_pr_hcid_login_unallow);
											return true;
										}

										if (US_ContestRule_solo == pComContestInfo->uContestRule)
										{//个人solo赛
											struct_applynode data;
											bool bRes = m_CApplayManage.at(p->uuserid, data);
											QK_InfoLog("sdp_test hall 大厅做出判断(userid=%d,bRes=%d,bAllot=%d)", p->uuserid, bRes, data.bAllot);
											if (!bRes || data.ucontestid != p->contestid)
											{
												QK_WarnLog("sdp_test hall 大厅告诉游戏服务器，不同意该玩家登录游戏服务器，个人solo赛 玩家没在该比赛中(%d, %d)。"
													, data.ucontestid, p->contestid);
												SendData(uIndex, &cmd, sizeof(cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_login, e_pr_hcid_login_unallow);
												return true;
											}
										}
																				
										CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(p->uuserid);
										if (nullptr == puser)
										{
											QK_WarnLog("sdp_test hall 大厅告诉游戏服务器，不同意该玩家登录游戏服务器，玩家不在线。(%d)", p->uuserid);
											SendData(uIndex, &cmd, sizeof(cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_login, e_pr_hcid_login_unallow);
											return true;
										}
										QK_InfoLog("sdp_test hall 登陆服务器同意该玩家登录游戏服务器。");
										std::memcpy(&cmd2.data, puser->GetUserData(), sizeof(UserInfoInHallStruct));
										SendData(uIndex, &cmd2, sizeof(cmd2), e_pr_mess_userinfo, e_pr_ass_userinfo_login, e_pr_hcid_login_allow);
		}break;
		case e_pr_ass_userinfo_allot_room:
		{//游戏服务器告知登陆服务器，有玩家加入该房间，请求确认	
										QK_InfoLog("sdp_test hall 游戏服务器告知登陆服务器，有玩家加入该房间，请求确认。");
										struct_data_pr_userinfo_allot_room* p = (struct_data_pr_userinfo_allot_room*)pData;
										if (nullptr == p || uSize != sizeof(struct_data_pr_userinfo_allot_room))
										{
											return false;
										}

										const ComContestInfo* pComContestInfo = m_CGameList.GetComContestInfo(p->contestid);
										const ComRoomInfo* pComRoomInfo = m_CGameList.GetComRoomInfoEx(p->roomid);
										if (nullptr == pComContestInfo
											|| nullptr == pComRoomInfo)
										{
											return false;
										}

										if (US_ContestRule_solo == pComContestInfo->uContestRule)
										{//个人solo赛
											struct_applynode data;
											bool bRes = m_CApplayManage.at(p->userid, data);
											QK_InfoLog("sdp_test hall 大厅做出判断(userid=%d,bRes=%d,bAllot=%d)", p->userid, bRes, data.bAllot);
											if (bRes)
											{//达到条件就允许登陆房间
												m_CApplayManage.AllotRoom(p->userid, p->roomid);
											}
										}
										else if (US_ContestRule_team == pComContestInfo->uContestRule)
										{//团队赛

										}

										return true;
		}break;
		case e_pr_ass_userinfo_apply_restitution:
		{//游戏服务器告诉大厅玩家是否可以退费
													QK_InfoLog("sdp_test hall 游戏服务器告诉大厅玩家是否可以退费。");
													struct_data_pr_userinfo_apply_restitution* p = (struct_data_pr_userinfo_apply_restitution*)pData;
													if (nullptr == p || uSize != sizeof(struct_data_pr_userinfo_apply_restitution))
													{//数据包异常
														return false;
													}

													struct_applynode data;
													if (!m_CApplayManage.at(p->userid, data))
													{//该玩家没报名
														return true;
													}

													CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(p->userid);

													if (e_pr_hcid_apply_restitution_suc == pNetHead->bHandleCode)
													{//退费成功
														//检测比赛是否存在
														const ComContestInfo* pContest = m_CGameList.GetComContestInfo(data.ucontestid);
														if (nullptr != pContest)
														{//钱退给玩家
															if (nullptr != puser)
																puser->SetUserState(US_P_unApplying);
															/*
															RRLine_c_apply_restitution rediscommand;
															rediscommand.changepoint = pContest->_i64cost;
															rediscommand.uContestKind = pContest->uContestKind;
															rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
															rediscommand.ucontestid = data.ucontestid;
															QK_InfoLog("sdp_test hall 有比赛但没在房间，则查询redis处理退费。");
															m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_apply_restitution), puser->GetSocketIndex());
															*/
															CTransaction_apply_restitution *pTransApplyResti = new CTransaction_apply_restitution;
															assert(pTransApplyResti);
															pTransApplyResti->m_in.uIndex = (nullptr != puser) ? puser->GetSocketIndex() : ERROR_SOCKETID;
															pTransApplyResti->m_in.changepoint = pContest->_i64cost;
															pTransApplyResti->m_in.uContestKind = pContest->uContestKind;
															pTransApplyResti->m_in.userid = p->userid;
															pTransApplyResti->m_in.ucontestid = data.ucontestid;
															m_pRedis->do_transaction(Redis_Order_h_messid_apply_restitution,
																(nullptr != puser) ? puser->GetSocketIndex() : ERROR_SOCKETID,
																onRedis_Order_h_messid_apply_restitution,
																pTransApplyResti);
														}
													}
													else if (e_pr_hcid_apply_restitution_err == pNetHead->bHandleCode)
													{//退费失败
														QK_InfoLog("sdp_test hall 退费失败。");
														if (nullptr != puser)
															SendData(puser->GetSocketIndex(), nullptr, 0, e_hs_mess_apply, e_hs_ass_apply_restitution, e_hs_hcid_apply_restitution_err);
													}
		}break;
		case e_pr_ass_userinfo_unplay:
		{//玩家没有在房间里面玩，比赛继续有效
										 QK_InfoLog("sdp_test hall 游戏服务器告诉大厅玩家没有在房间里面玩，比赛继续有效。");
										 struct_data_pr_userinfo_unplay* p = (struct_data_pr_userinfo_unplay*)pData;
										 if (nullptr == p || uSize != sizeof(struct_data_pr_userinfo_unplay))
										 {
											 return false;
										 }
										 struct_applynode data;
										 if (!m_CApplayManage.at(p->userid, data))
										 {//该玩家没报名
											 return true;
										 }

										 CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(p->userid);

										 if (data.bAllot)
										 {
											 //设置玩家继续等待比赛
											 m_CApplayManage.unAllotRoom(p->userid);
											 //离开房间就退费
											 struct_applynode data;
											 if (m_CApplayManage.at(p->userid, data))
											 {//有报名
												 UINT usocketindex;
												 if (!m_CGameList.GetRoomSocketID(data.uroomid, usocketindex))
												 {//没在房间，则继续退赛
													 //检测比赛是否存在
													 const ComContestInfo* pContest = m_CGameList.GetComContestInfo(data.ucontestid);
													 if (nullptr != pContest)
													 {//钱退给玩家
														 /*
														 RRLine_c_apply_restitution rediscommand;
														 rediscommand.changepoint = pContest->_i64cost;
														 rediscommand.uContestKind = pContest->uContestKind;
														 rediscommand.userid = p->userid;
														 rediscommand.ucontestid = data.ucontestid;
														 QK_InfoLog("sdp_test hall 有比赛但没在房间，则查询redis处理退费。");
														 m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_apply_restitution), ERROR_SOCKETID);
														 */
														 CTransaction_apply_restitution *pTransApplyResti = new CTransaction_apply_restitution;
														 assert(pTransApplyResti);
														 pTransApplyResti->m_in.uIndex = (nullptr != puser) ? puser->GetSocketIndex() : ERROR_SOCKETID;
														 pTransApplyResti->m_in.changepoint = pContest->_i64cost;
														 pTransApplyResti->m_in.uContestKind = pContest->uContestKind;
														 pTransApplyResti->m_in.userid = p->userid;
														 pTransApplyResti->m_in.ucontestid = data.ucontestid;
														 QK_InfoLog("sdp_test hall 有比赛但没在房间，则查询redis处理退费。");
														 m_pRedis->do_transaction(Redis_Order_h_messid_apply_restitution,
															 (nullptr != puser) ? puser->GetSocketIndex() : ERROR_SOCKETID,
															 onRedis_Order_h_messid_apply_restitution,
															 pTransApplyResti);
													 }
												 }
											 }
										 }
										 return true;
		}break;
		case e_pr_ass_userinfo_gameover:
		{//游戏服务器通知大厅，玩家已经结束游戏
										   QK_InfoLog("sdp_test hall 游戏服务器通知大厅，玩家已经结束游戏。");
										   struct_data_pr_userinfo_gameover* p = (struct_data_pr_userinfo_gameover*)pData;
										   if (nullptr == p || uSize != sizeof(struct_data_pr_userinfo_gameover))
										   {
											   QK_WarnLog("sdp_test hall 游戏服务器通知大厅，玩家已经结束游戏。err-----------------!");
											   return false;
										   }
										   QK_InfoLog("sdp_test hall 游戏服务器通知大厅，玩家已经结束游戏2222222。");
										   struct_applynode data;
										   bool bexist = m_CApplayManage.at(p->userid, data);
										   if (false == bexist)
										   {//该玩家不存在，不用处理
											   QK_WarnLog("sdp_err hall 游戏服务器通知大厅，玩家已经结束游戏,该玩家不存在，不用处理。");
											   return true;
										   }
										   CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(p->userid);

										   const ComContestInfo* pContestInfo = m_CGameList.GetComContestInfo(data.ucontestid);
										   MyDebugString("sdp_test", "hall 结束游戏判断退费问题（%d,%d,%d,%d）", p->userid, data.ucontestid, nullptr != pContestInfo, pNetHead->bHandleCode);
										   if (nullptr != pContestInfo && GAME_FINISH_TYPE_FORCE == pNetHead->bHandleCode)
										   {
											   //钱退给玩家
											   CTransaction_apply_restitution *pTransApplyResti = new CTransaction_apply_restitution;
											   assert(pTransApplyResti);
											   pTransApplyResti->m_in.uIndex = (nullptr != puser) ? puser->GetSocketIndex() : ERROR_SOCKETID;
											   pTransApplyResti->m_in.changepoint = pContestInfo->_i64cost;
											   pTransApplyResti->m_in.uContestKind = pContestInfo->uContestKind;
											   pTransApplyResti->m_in.userid = p->userid;
											   pTransApplyResti->m_in.ucontestid = data.ucontestid;
											   QK_InfoLog("sdp_test hall 有比赛但没在房间，则查询redis处理退费。");
											   m_pRedis->do_transaction(Redis_Order_h_messid_apply_restitution,
												   (nullptr != puser) ? puser->GetSocketIndex() : ERROR_SOCKETID,
												   onRedis_Order_h_messid_apply_restitution,
												   pTransApplyResti);
											   return true;
										   }
										   QK_InfoLog("sdp_test hall 游戏服务器通知大厅，玩家已经结束游戏33333333333。");
										   //设置玩家从报名队列中删除
										   bool bres = m_CApplayManage.del(p->userid);
										   QK_InfoLog("sdp_err hall 设置玩家从报名队列中删除(userid=%d, bres=%d)。", p->userid, bres);
										
										   return true;
		}break;
		case e_pr_ass_userinfo_team_vs_finish:
		{//游戏服务器通知大厅，团队已经结束游戏
												 QK_InfoLog("sdp_test hall 游戏服务器通知大厅，团队已经结束游戏。");
												 struct_data_pr_userinfo_team_vs_finish* p = (struct_data_pr_userinfo_team_vs_finish*)pData;
												 if (nullptr == p || uSize != sizeof(struct_data_pr_userinfo_team_vs_finish))
												 {
													 return false;
												 }

												 for (int i = 0; i < m_CTeamManage.Count_VS(); ++i)
												 {
													 TeamStruct_VS _TeamStruct_VS;
													 m_CTeamManage.at_vs(i, _TeamStruct_VS);
													 if (0 == strcmp(_TeamStruct_VS._TeamStruct[0].m_Team_password, p->_data._TeamStruct[0].m_Team_password)
														 && 0 == strcmp(_TeamStruct_VS._TeamStruct[1].m_Team_password, p->_data._TeamStruct[1].m_Team_password)
														 && _TeamStruct_VS._TeamStruct[0].m_uCaptain_userid == p->_data._TeamStruct[0].m_uCaptain_userid
														 && _TeamStruct_VS._TeamStruct[1].m_uCaptain_userid == p->_data._TeamStruct[1].m_uCaptain_userid)
													 {
														 QK_InfoLog("sdp_test hall 团队使用完毕，清除相关数据。");
														 m_CTeamManage.del(_TeamStruct_VS._TeamStruct[0].m_Team_password);
														 m_CTeamManage.del(_TeamStruct_VS._TeamStruct[1].m_Team_password);
														 m_CTeamManage.del_VS(i);
														 break;
													 }
												 }

												 return true;
		}break;
		default:
		{
				   break;
		}
		}

		return true;
	}
	void CMainManage_login::OnEndConnection(CWJSSocket * pSocket)
	{
		int tag = pSocket->GetType();
		UINT uSocketIndex = pSocket->GetSockID();
		QK_InfoLog("sdp_test hall OnSocketClose uSocketIndex = %d ,tag=%d", uSocketIndex, tag);
		if (tag == ePCClient)
		{
			char cip[30] = { 0 };
			char str[MAX_PATH] = { 0 };
			//数据
			UINT userid = 0;
			char username[NAME_LENGTH] = { 0 };
			CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uSocketIndex);
			if (nullptr != puser)
			{
				userid = puser->GetUserData()->basedata.dwUserID;
				//////////////////////////////////////////
				struct_applynode data;
				if (m_CApplayManage.at(userid, data))
				{//该玩家有报名。
					if (!data.bAllot)
					{//且没有进房间
						//检测比赛是否存在
						const ComContestInfo* pContest = m_CGameList.GetComContestInfo(data.ucontestid);
						if (nullptr != pContest)
						{//钱退给玩家
							/*
							RRLine_c_apply_restitution rediscommand;
							rediscommand.changepoint = pContest->_i64cost;
							rediscommand.uContestKind = pContest->uContestKind;
							rediscommand.userid = userid;
							rediscommand.ucontestid = data.ucontestid;
							QK_InfoLog("sdp_test hall 有比赛但没在房间，则查询redis处理退费。");
							m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_apply_restitution), ERROR_SOCKETID);
							*/
							CTransaction_apply_restitution *pTransApplyResti = new CTransaction_apply_restitution;
							assert(pTransApplyResti);
							pTransApplyResti->m_in.uIndex = INVALID_SOCKET_ID;
							pTransApplyResti->m_in.changepoint = pContest->_i64cost;
							pTransApplyResti->m_in.uContestKind = pContest->uContestKind;
							pTransApplyResti->m_in.userid = userid;
							pTransApplyResti->m_in.ucontestid = data.ucontestid;
							m_pRedis->do_transaction(Redis_Order_h_messid_apply_restitution,
								uSocketIndex,
								onRedis_Order_h_messid_apply_restitution,
								pTransApplyResti);

						}
					}					
				}
				//////////////////////////////////////////////////////////
				std::memcpy(username, puser->GetUserData()->basedata.szName, sizeof(username));				
				m_CLUserInfoManage.FreeUser(uSocketIndex);
				sprintf_s(str, "disconnect!IP:%s, index=%d, player=(%d,%s)", cip, uSocketIndex, userid, username);
			}
			else
			{
				LUserStruct* pLUserStruct = m_CLUserInfoManage.GetLUserStructEx_TEST(uSocketIndex);	
				if (nullptr != pLUserStruct)
					sprintf_s(str, "disconnect!IP:%s, index=%d, player=(%d), usertype=%d", cip, uSocketIndex, userid, pLUserStruct->usertype);
				else
					sprintf_s(str, "disconnect!IP:%s, index=%d, player=(%d), usertype=null", cip, uSocketIndex, userid);
				QK_InfoLog("sdp_test %s", std::string(str).c_str());
			}

		
		}
		else if (tag == eGameServer)
		{			
			
			
		}
		return; 
	}
	///数据库处理结果 （必须重载）
	bool CMainManage_login::OnDataBaseResult(DataBaseResultLine * pResultData, UINT tag)
	{
		if (tag == e_UserDataTag_def)
		{

		}
		else if (tag == e_UserDataTag_LGsocketserver)
		{

		}

		return true; 
	}

	double random(double start, double end)
	{
		double _min = 0.0;
		double _max = 0.0;
		_min = start >= end ? end : start;
		_max = start >= end ? start : end;
		return _min + (_max - _min)*rand() / (RAND_MAX + 1.0);

		srand((unsigned)time(NULL));
	}

	///定时器消息  注意 这里还没有进行处理，需要根据业务进行处理（不知道业务知识）
	bool CMainManage_login::OnTimerMessage(TimerID timerId)
	{
		switch (timerId)
		{
		case E_P_TIME_ID_HEART:
		{
							 UINT uNowTime = (UINT)time(NULL);	//当前时间

							 map<long, LUserStruct*>* pMap = m_CLUserInfoManage.GetLUserStruct_map();
							 auto iter = pMap->begin(), end = pMap->end();
							 for (; iter != end; ++iter)
							 {//查看在线玩家是否有心跳
								 if (usertype_online == iter->second->usertype)
								 {
									 CWJSSocket* pSocket = getCWJSSocket(iter->second->userinfo.GetSocketIndex());
									 if (nullptr != pSocket)
									 {
										 UINT itag = uNowTime - pSocket->GetKeep();
										 if (itag > m_uCheckTime)
										 {
											 QK_WarnLog("登录服务器 (uNowTime = %d, m_Keep = %d)时间差=%d 大于 %d,则断开该客户连接 socketindex = %d", uNowTime, pSocket->GetKeep(), itag, m_uCheckTime, iter->second->userinfo.GetSocketIndex());
											 CloseSocket(pSocket->GetSockID());
										 }
									 }
								 }
							 }
		}break;
		case E_P_TIME_ID_UPDATE_ONLINE_COUNT:
		{
											static int irandcount = 0;
											if ((rand() % 10 > 5) || irandcount == 0)
											{
												irandcount = int(random(lsconfig().GetData()->iMinVirtualCount, lsconfig().GetData()->iMaxVirtualCount));
											}
											m_update_now_playercount = m_CLUserInfoManage.GetOnLineUserCount() + irandcount;											
		}break;
		case  E_P_TIME_ID_UPDATE_USER_INFO:
		{
											  CTransaction_Update_Userinfo *pCmd = new CTransaction_Update_Userinfo;
											  assert(pCmd);
											  pCmd->m_in.uIndex = 0;
											  m_pRedis->do_transaction(Redis_Order_h_messid_update_userinfo,
												  0,
												  onRedis_Order_h_messid_update_userinfo,
												  pCmd);
		}break;
		case E_P_TIME_ID_TEAM_VS:
		{
							   std::vector<TeamStruct> _vector = m_CTeamManage.getTeamStruct_list();
							   auto _iter_vector = _vector.begin();
							   while (_iter_vector != _vector.end())
							   {
								   std::vector<ComRoomInfo_node> room_node;
								   m_CGameList.GetComRoomList(room_node, (*_iter_vector).m_uContest);
								   for (auto iter = room_node.begin(); iter != room_node.end(); ++iter)
								   {
									   bool bexist = false;
									   {
										   auto _iter = m_socket_vector.begin();
										   while (_iter != m_socket_vector.end())
										   {
											   if ((*_iter) == (*iter).usocketindex)
											   {//必须是有效的房间
												   bexist = true;
												   break;
											   }
											   ++_iter;
										   }
									   }									   

									   if (bexist && ((*iter).uTeamCount * (*_iter_vector).m_uTemaPlayerNum + ((*_iter_vector).m_uTemaPlayerNum * 2) <= PORT_GS_COUNT))
									   {//匹配只需要一个房间,且组团人数不能超过游戏服务器人数上限
										   TeamStruct_VS _TeamStruct_VS;
										   bool bres = m_CTeamManage.add_VS((*_iter_vector).m_Team_password, _TeamStruct_VS, (*iter).room_node.roomid);
										   if (bres)
										   {//通知房间
											   SendData((*iter).usocketindex, &_TeamStruct_VS, sizeof(TeamStruct_VS), e_pr_mess_userinfo, e_pr_ass_userinfo_team_start, 0);
											   struct_data_hs_hcid_team_start_suc cmd;
											   cmd.data = _TeamStruct_VS;
											   std::memcpy(&cmd.strIP, (*iter).room_node.strIP, sizeof(cmd.strIP));
											   cmd.uport = (*iter).room_node.uport;
											   //通知两个团队的所有玩家，匹配成功
											   for (int i = 0; i < 2;++i)
											   {
												   for (int j = 0; j < _TeamStruct_VS._TeamStruct[i].m_uTemaPlayerNum; ++j)
												   {
													   CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(_TeamStruct_VS._TeamStruct[i].m_Userid[j]);
													   if (nullptr != puser)
													   {
														   SendData(puser->GetSocketIndex(), &cmd, sizeof(struct_data_hs_hcid_team_start_suc), e_hs_mess_team, e_hs_ass_team_start, e_hs_hcid_team_start_suc);
													   }
												   }
											   }												
										   }
										   break;
									   }
								   }								   

								   ++_iter_vector;
							   }
		}break;
		default:
			break;
		}
		return true;
	}
	
	//redis命令结果 用户登录大厅
	void CMainManage_login::onRedisRes_Order_h_messid_load_hall(RedisResultLine * pResultData)
	{
		RRLine_s_load_hall* pCmd = (RRLine_s_load_hall*)pResultData;

		//连接已经中断，则无需处理
		CWJSSocket* psocket = getCWJSSocket(pCmd->uIndex);
		if (nullptr != psocket && psocket->IsConnect())
		{
			CLoginUserInfo* p = nullptr;
			p = m_CLUserInfoManage.GetOnLineUserInfoEx(pCmd->uIndex);
			if (nullptr != p)
			{//发现该套接字id下存在玩家
				QK_WarnLog("sdp_test hall 登陆大厅错误 发现该套接字id下存在玩家。");
				pResultData->uHandleRusult = RRLine_s_load_hall::e_login_err_online;
			}

			p = m_CLUserInfoManage.GetOnLineUserInfo(pCmd->_data.basedata.dwUserID);
			if (nullptr != p)
			{//发现该userid下存在玩家
				QK_WarnLog("sdp_test hall 登陆大厅错误 发现该userid下存在玩家。");
				pResultData->uHandleRusult = RRLine_s_load_hall::e_login_err_online;
			}

			switch (pResultData->uHandleRusult)
			{
			case RRLine_s_load_hall::e_login_err_unexist:
			{//玩家不存在
															QK_WarnLog("sdp_test hall 登陆大厅错误 玩家不存在。");
															SendData(pCmd->uIndex, e_hs_mess_login, e_hs_ass_login, e_hs_hcid_login_err_unexist);
			}break;
			case RRLine_s_load_hall::e_login_err_password:
			{//密码错误
															 QK_WarnLog("sdp_test hall 登陆大厅错误 密码错误。");
															 SendData(pCmd->uIndex, e_hs_mess_login, e_hs_ass_login, e_hs_hcid_login_err_password);
			}break;
			case RRLine_s_load_hall::e_login_err_online:
			{//玩家已经存在
															 QK_WarnLog("sdp_test hall 登陆大厅错误 玩家已经存在。");
															 SendData(pCmd->uIndex, e_hs_mess_login, e_hs_ass_login, e_hs_hcid_login_err_online);
			}break;
			case RRLine_s_load_hall::e_login_suc:
			{//成功
													QK_InfoLog("sdp_test hall 登陆大厅成功。");
													//注册在线用户
													CLoginUserInfo* puser = m_CLUserInfoManage.ActiveUser(&pCmd->_data, pCmd->uIndex);
													if (nullptr == puser)
													{//注册在线用户失败，不能进入大厅
														CloseSocket(pCmd->uIndex);
														return;
													}
													SendData(pCmd->uIndex, &pCmd->_data, sizeof(struct_data_hs_hcid_login_suc), e_hs_mess_login, e_hs_ass_login, e_hs_hcid_login_suc);							
			}break;
			default:
				break;
			}
		}
	}


	//redis命令结果 报名
	void CMainManage_login::onRedisRes_Order_h_messid_apply(RedisResultLine * pResultData)
	{
		RRLine_s_apply* p = (RRLine_s_apply*)pResultData;

		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(pResultData->uIndex);
		if (nullptr != puser)
		{
			puser->SetUserState(US_P_Wait);
			if (!p->bSuc)
			{
				QK_InfoLog("sdp_test hall 报名费不够。");
				SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_nomoney);
				
				return;
			}

			if (US_ContestRule_happy == p->uContestKind)
			{
				puser->GetUserData()->basedata.i64Money -= p->changemoney;
			}
			else if (US_ContestRule_point == p->uContestKind)
			{
				puser->GetUserData()->basedata.dwPoint -= p->changepoint;
			}
		}
		
		struct_applynode cmd;
		cmd.ucontestid = p->ucontestid;
		cmd.userid = p->userid;
		m_CApplayManage.add(cmd);
		QK_InfoLog("sdp_test hall 报名成功。");	
		if (nullptr != puser)
		{
			SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_suc);
		}

	}

	//redis命令结果 创建团队
	void CMainManage_login::onRedisRes_Order_h_messid_team_create(RedisResultLine * pResultData)
	{
		RRLine_s_team_create* p = (RRLine_s_team_create*)pResultData;

		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(pResultData->uIndex);
		if (nullptr != puser)
		{
			puser->SetUserState(US_P_Wait);
			if (!p->bSuc)
			{
				QK_InfoLog("sdp_test hall 创建费不够。");
				SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_nomoney);
				return;
			}

			if (US_ContestRule_happy == p->uContestKind)
			{
				puser->GetUserData()->basedata.i64Money -= p->changemoney;
			}
			else if (US_ContestRule_point == p->uContestKind)
			{
				puser->GetUserData()->basedata.dwPoint -= p->changepoint;
			}
		}

		TeamStruct cmd;
		cmd.m_uCaptain_userid = p->userid;
		cmd.m_TeamPayType = p->m_TeamPayType;
		cmd.m_uContest = p->ucontestid;
		cmd.m_uTemaPlayerNum = (UINT)p->uRoomRule;

		m_CTeamManage.add(cmd);

		QK_InfoLog("sdp_test hall 创建团队成功。");
		if (nullptr != puser)
		{
			SendData(pResultData->uIndex, &cmd, sizeof(struct_data_hs_mess_team_create_res), e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_suc);
		}
	}

	//redis命令结果 加入团队
	void CMainManage_login::onRedisRes_Order_h_messid_team_join(RedisResultLine * pResultData)
	{
		RRLine_s_team_join* p = (RRLine_s_team_join*)pResultData;

		if (!p->bSuc)
		{
			QK_WarnLog("sdp_test hall 创建费不够。");
			m_CTeamManage.userDel(p->userid, p->m_Team_password);//一定要记得删除该玩家，因为事先已经添加
		}

		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(pResultData->uIndex);
		if (nullptr != puser)
		{
			puser->SetUserState(US_P_Wait);
			if (!p->bSuc)
			{
				SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_nomoney);
				return;
			}

			if (US_ContestRule_happy == p->uContestKind)
			{
				puser->GetUserData()->basedata.i64Money -= p->changemoney;
			}
			else if (US_ContestRule_point == p->uContestKind)
			{
				puser->GetUserData()->basedata.dwPoint -= p->changepoint;
			}
		}

		TeamStruct cmd;

		m_CTeamManage.find(p->m_Team_password, cmd);

		QK_InfoLog("sdp_test hall 加入团队成功。");
		
		UINT uTemaPlayerNum;//队伍人数
		UINT Userid[TEAM_USER_NUM];//每个人的userid
		m_CTeamManage.GetUsers(p->m_Team_password, Userid, uTemaPlayerNum);
		for (int i = 0; i < uTemaPlayerNum; ++i)
		{
			CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(Userid[i]);
			if (nullptr != puser)
			{
				SendData(puser->GetSocketIndex(), &cmd, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
			}
		}
	}

	//redis命令结果 退出团队
	void CMainManage_login::onRedisRes_Order_h_messid_team_leave(RedisResultLine * pResultData)
	{
		RRLine_s_team_leave* p = (RRLine_s_team_leave*)pResultData;

		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(pResultData->uIndex);
		if (!p->bSuc)
		{//失败就不再往下走了，并且只有当玩家在线才通知对方
			if (nullptr != puser)
			{
				QK_WarnLog("sdp_test hall 退出团队失败。");
				SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_money);	
				puser->SetUserState(US_P_Wait);
			}
			return;
		}

		QK_InfoLog("sdp_test hall 退出团队成功。");
		struct_data_hs_mess_team_leave_res cmd;
		cmd.userid = p->userid;
		m_CTeamManage.userDel(p->userid, p->m_Team_password);
		
		if (nullptr != puser)
		{
			if (US_ContestRule_happy == p->uContestKind)
			{
				puser->GetUserData()->basedata.i64Money += p->changemoney;
			}
			else if (US_ContestRule_point == p->uContestKind)
			{
				puser->GetUserData()->basedata.dwPoint += p->changepoint;
			}
			//只有人在线的时候才会通知该玩家操作成功
			SendData(pResultData->uIndex, &cmd, sizeof(struct_data_hs_mess_team_leave_res), e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_suc);
			puser->SetUserState(US_P_Wait);
		}
		
		UINT uTemaPlayerNum;//队伍人数
		UINT Userid[TEAM_USER_NUM];//每个人的userid
		m_CTeamManage.GetUsers(p->m_Team_password, Userid, uTemaPlayerNum);
		for (int i = 0; i < uTemaPlayerNum; ++i)
		{
			CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(Userid[i]);
			if (nullptr != puser)
			{
				SendData(puser->GetSocketIndex(), &cmd, sizeof(struct_data_hs_mess_team_leave_res), e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_suc);
			}
		}
	}

	//redis命令结果 更新用户数据(娱币、积分)
	void CMainManage_login::onRedisRes_Order_h_messid_update_userinfo(RedisResultLine * pResultData)
	{
		RRLine_s_Update_Userinfo* p = (RRLine_s_Update_Userinfo*)pResultData;

		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(p->iuserid);
		if (nullptr != puser)
		{
			if (1 == p->itype)
			{//积分
				puser->GetUserData()->basedata.dwPoint = p->dwPoint;
			}
			else if (2 == p->itype)
			{//娱币
				puser->GetUserData()->basedata.i64Money = p->i64Money;
			}
		}
		return;
	}

	void CMainManage_login::onRedisRes_Order_h_messid_team_dissolve(RedisResultLine * pResultData)
	{
		RRLine_s_team_dissolve* p = (RRLine_s_team_dissolve*)pResultData;

		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(p->userid[0]);
		if (!p->bSuc)
		{//失败就不再往下走了，并且只有当玩家在线才通知对方
			if (nullptr != puser)
			{
				QK_InfoLog("sdp_test hall 解散团队失败。");
				SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_money);
				puser->SetUserState(US_P_Wait);
			}
			return;
		}

		UINT uTemaPlayerNum;//队伍人数
		UINT Userid[TEAM_USER_NUM];//每个人的userid
		m_CTeamManage.GetUsers(p->m_Team_password, Userid, uTemaPlayerNum);
		for (int i = 0; i < uTemaPlayerNum; ++i)
		{//通知所有人解散团队
			CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(Userid[i]);
			if (nullptr != puser)
			{
				if (US_ContestRule_happy == p->uContestKind)
				{
					puser->GetUserData()->basedata.i64Money += p->changemoney[i];
				}
				else if (US_ContestRule_point == p->uContestKind)
				{
					puser->GetUserData()->basedata.dwPoint += p->changepoint[i];
				}

				SendData(puser->GetSocketIndex(), nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_suc);
			}
		}
		m_CTeamManage.del(p->m_Team_password);

		if (nullptr != puser)
		{
			puser->SetUserState(US_P_Wait);
		}
		QK_InfoLog("sdp_test hall 解散团队成功。");
	}

	//redis命令结果 退费
	void CMainManage_login::onRedisRes_Order_h_messid_apply_restitution(RedisResultLine * pResultData)
	{
		RRLine_s_apply_restitution* p = (RRLine_s_apply_restitution*)pResultData;
		
		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(pResultData->uIndex);
		if (nullptr != puser)
		{
			if (!p->bSuc)
			{
				QK_WarnLog("sdp_test hall 查询redis退费失败。");
				SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply_restitution, e_hs_hcid_apply_restitution_err);
				puser->SetUserState(US_P_Wait);
				return;
			}

			if (US_ContestRule_happy == p->uContestKind)
			{
				puser->GetUserData()->basedata.i64Money += p->changemoney;
			}
			else if (US_ContestRule_point == p->uContestKind)
			{
				puser->GetUserData()->basedata.dwPoint += p->changepoint;
			}

			SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply_restitution, e_hs_hcid_apply_restitution_suc);
		}
		QK_InfoLog("sdp_test hall redis查询同意退费。");
		m_CApplayManage.del(p->userid);
		if (nullptr != puser)
		{
			puser->SetUserState(US_P_Wait);
		}
	}

	//发送给团队里面的消息
	//UINT myuserid 排除自己发送
	void CMainManage_login::TeamSendData(UINT myuserid, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{
		TeamStruct _TeamStruct;
		if (!m_CTeamManage.find(myuserid, _TeamStruct))
		{
			return;
		}

		for (int k = 0; k < _TeamStruct.m_uTemaPlayerNum; ++k)
		{
			if (_TeamStruct.m_Userid[k] == myuserid)
			{
				continue;
			}
			CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(_TeamStruct.m_Userid[k]);
			if (nullptr != puser)
			{//要告诉vs中的其他玩家,有人回来了
				SendData(puser->GetSocketIndex(), pData, uBufLen, bMainID, bAssistantID, bHandleCode);
			}
		}
	}

	//发送给团队VS里面的消息
	//UINT myuserid 排除自己发送
	void CMainManage_login::TeamVSSendData(UINT myuserid, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{//团队赛，要告诉vs中的其他玩家
		UINT ucount_vs = m_CTeamManage.Count_VS();
		TeamStruct_VS in_TeamStruct_VS;
		bool bInVS = false;//玩家是否在团队赛VS中
		for (int i = 0; i < ucount_vs; i++)
		{
			TeamStruct_VS _TeamStruct_VS;
			m_CTeamManage.at_vs(i, _TeamStruct_VS);
			for (int j = 0; j < 2; ++j)
			{
				for (int k = 0; k < _TeamStruct_VS._TeamStruct[j].m_uTemaPlayerNum; ++k)
				{
					if (_TeamStruct_VS._TeamStruct[j].m_Userid[k] == myuserid)
					{
						bInVS = true;
						in_TeamStruct_VS = _TeamStruct_VS;
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
				for (int k = 0; k < in_TeamStruct_VS._TeamStruct[j].m_uTemaPlayerNum; ++k)
				{
					if (in_TeamStruct_VS._TeamStruct[j].m_Userid[k] == myuserid)
					{
						continue;
					}
					CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(in_TeamStruct_VS._TeamStruct[j].m_Userid[k]);
					if (nullptr != puser)
					{//要告诉vs中的其他玩家,有人回来了
						SendData(puser->GetSocketIndex(), pData, uBufLen, bMainID, bAssistantID, bHandleCode);
					}
				}
			}
		}
	}

	//测试
	void CMainManage_login::Test()
	{
		//测试用，临时创建游戏列表，但房间节点需要真实的游戏服务器与登陆服务器通信
		{//游戏
			ComGameinfo cmd[2];
			cmd[0].gameid = 10000001;
			sprintf_s(cmd[0].gamename, "英雄联盟");
			m_CGameList.addgame_node(cmd[0]);

			cmd[1].gameid = 10000002;
			sprintf_s(cmd[1].gamename, "炉石传说");
			m_CGameList.addgame_node(cmd[1]);
		}

		{//区服
		ComEreainfo cmd[5];
		cmd[0].ereaid = 0;
		sprintf_s(cmd[0].ereaname, "艾欧尼亚");
		cmd[0].gameid = 10000001;
		m_CGameList.adderea_node(cmd[0]);

		cmd[1].ereaid = 1;
		sprintf_s(cmd[1].ereaname, "祖安");
		cmd[1].gameid = 10000001;
		m_CGameList.adderea_node(cmd[1]);

		cmd[2].ereaid = 2;
		sprintf_s(cmd[2].ereaname, "巨神峰");
		cmd[2].gameid = 10000001;
		m_CGameList.adderea_node(cmd[2]);

		cmd[3].ereaid = 3;
		sprintf_s(cmd[3].ereaname, "你大爷");
		cmd[3].gameid = 10000002;
		m_CGameList.adderea_node(cmd[3]);

		cmd[4].ereaid = 4;
		sprintf_s(cmd[4].ereaname, "往死里打");
		cmd[4].gameid = 10000002;
		m_CGameList.adderea_node(cmd[4]);
	}

		{//比赛
			ComContestInfo cmd[6];
			cmd[0].contestid = 1;
			cmd[0].gameid = 10000001;
			cmd[0].uContestRule = US_ContestRule_solo;
			cmd[0].uContestKind = US_ContestRule_happy;
			sprintf_s(cmd[0].contestname, "个人solo1v1标准赛");
			sprintf_s(cmd[0].cContestDescribe, "个人solo1v1标准赛，非常有意思，赶紧来玩！");
			cmd[0].ereaid = 0;
			cmd[0]._i64cost = 1000;
			cmd[0].award = 1;
			m_CGameList.addcontest_node(cmd[0]);

			cmd[1].contestid = 2;
			cmd[1].gameid = 10000001;
			cmd[1].uContestRule = US_ContestRule_solo;
			cmd[1].uContestKind = US_ContestRule_happy;
			sprintf_s(cmd[1].contestname, "个人solo2v2标准赛");
			sprintf_s(cmd[1].cContestDescribe, "个人solo2v2标准赛，非常有意思，赶紧来玩！");
			cmd[1].ereaid = 0;
			cmd[1]._i64cost = 1000;
			cmd[1].award = 2;
			m_CGameList.addcontest_node(cmd[1]);

			cmd[2].contestid = 3;
			cmd[2].gameid = 10000001;
			cmd[2].uContestRule = US_ContestRule_solo;
			cmd[2].uContestKind = US_ContestRule_happy;
			sprintf_s(cmd[2].contestname, "个人solo标准赛");
			sprintf_s(cmd[2].cContestDescribe, "个人solo标准赛，非常有意思，赶紧来玩！");
			cmd[2].ereaid = 1;
			cmd[2]._i64cost = 1000;
			cmd[2].award = 3;
			m_CGameList.addcontest_node(cmd[2]);

			cmd[3].contestid = 4;
			cmd[3].gameid = 10000001;
			cmd[3].uContestRule = US_ContestRule_solo;
			cmd[3].uContestKind = US_ContestRule_happy;
			sprintf_s(cmd[3].contestname, "个人solo5v5大师赛");
			sprintf_s(cmd[3].cContestDescribe, "个人solo5v5大师赛，非常有意思，赶紧来玩！");
			cmd[3].ereaid = 2;
			cmd[3]._i64cost = 1000;
			cmd[3].award = 1;
			m_CGameList.addcontest_node(cmd[3]);

			cmd[4].contestid = 5;
			cmd[4].gameid = 10000002;
			cmd[4].uContestRule = US_ContestRule_solo;
			cmd[4].uContestKind = US_ContestRule_happy;
			sprintf_s(cmd[4].contestname, "个人solo1v1二货赛");
			sprintf_s(cmd[4].cContestDescribe, "个人solo1v1二货赛，非常有意思，赶紧来玩！");
			cmd[4].ereaid = 3;
			cmd[4]._i64cost = 1000;
			cmd[4].award = 100;
			m_CGameList.addcontest_node(cmd[4]);

			cmd[5].contestid = 6;
			cmd[5].gameid = 10000002;
			cmd[5].uContestRule = US_ContestRule_solo;
			cmd[5].uContestKind = US_ContestRule_happy;
			sprintf_s(cmd[5].contestname, "个人solo1v1大师赛");
			sprintf_s(cmd[5].cContestDescribe, "个人solo1v1大师赛，非常有意思，赶紧来玩！");
			cmd[5].ereaid = 4;
			cmd[5]._i64cost = 1000;
			cmd[5].award = 100;
			m_CGameList.addcontest_node(cmd[5]);
		}
	}

	//批量发送函数
	void CMainManage_login::SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT uAssistantID, UINT uHandleCode)
	{
		map<long, LUserStruct*>* pMap = m_CLUserInfoManage.GetLUserStruct_map();
		auto iter = pMap->begin(), end = pMap->end();
		for (; iter != end; ++iter)
		{
			if (usertype_online == iter->second->usertype)
			{
				SendData(iter->second->userinfo.GetSocketIndex(), pData, uSize, uMainID, uAssistantID, uHandleCode);
			}
		}
	}

}
