#include "GameDesk.h"
#include "GameRoom.h"
#include "message/WJNetRoomMessage.h"
#include "server/comtimetag.h"
#include "message/WJNetPtoR.h"
#include "common/commonfunction.h"
#include "common/QkLogger.h"
#include "gameserver.h"

namespace WJ
{
    extern CGameServer * g_pGameServer;

	CGameDesk::CGameDesk()
	{
		m_pGameRoom = nullptr;
		m_bPlayGame = false;
		m_bLock = false;
		std::memset(m_szLockPass, 0, sizeof(m_szLockPass));
		std::memset(m_UserEndGame, 0, sizeof(m_UserEndGame));
		m_dwOwnerUserID = 0;
		m_uDeskIndex = 0;
		m_uMaxPeople = 0;
		m_dwBeginTime = 0;
		for (int i = 0; i < MAX_PEOPLE; ++i)
		{
			m_pUserInfo[i] = nullptr;
		}

		m_uGameStation = 0;
	}

	CGameDesk::~CGameDesk()
	{

	}

	///判断游戏桌上的某个玩家是否开始游戏了
	bool CGameDesk::IsPlayGame()
	{
		return m_bPlayGame;
	}

	///用户同意开始
	bool CGameDesk::UserAgreeGame(UINT uDeskStation)
	{
		QK_InfoLog("sdp_test desk 用户同意消息。");
		//在游戏进行中拒绝准备消息
		if (IsPlayGame())
		{
			return true;
		}

		if (uDeskStation >= m_uMaxPeople)
		{
			return false;
		}

		if (nullptr == m_pUserInfo[uDeskStation])
		{
			return false;
		}

		//设置数据
		m_pUserInfo[uDeskStation]->GetUserData()->basedata.bUserState = US_R_Agress;

		//发送同意消息
		//发送游戏同意消息
		RoomMessDate_user_frame_agree UserAgree;
		UserAgree.udeskid = m_uDeskIndex;
		UserAgree.udeskstation = uDeskStation;
		std::memcpy(&UserAgree.userdata, m_pUserInfo[uDeskStation]->GetUserData(), sizeof(UserInfoInRoomStruct));
		SendDeskData(&UserAgree, sizeof(UserAgree), e_gs_mess_game_frame, e_gs_ass_game_frame_agree, 0);
		QK_InfoLog("sdp_test desk 群发用户同意消息。");
		//判断是否可以开始
		if (Sys_CanBeginGame() == true)
		{
			Sys_GameBegin(0);
		}

		return true;
	}

	///处理用户发送结束消息
	bool CGameDesk::Sys_UserEndGame(UINT uDeskStation, void* pData, UINT DataSize)
	{
		QK_InfoLog("sdp_test desk 用户结束消息。");
		if (uDeskStation >= m_uMaxPeople)
		{
			return false;
		}
		if (m_UserEndGame[uDeskStation])
		{
			return false;
		}
		//目前设计只要有一个客户端告知第三方游戏结束，游戏就可以结束
		if (IsPlayGame())
		{
			QK_InfoLog("sdp_test desk 用户结束消息 正在处理。。。");	
			UserEndGame(uDeskStation, pData, DataSize);
			m_UserEndGame[uDeskStation] = true;
			//转发给其他人
			for (UINT i = 0; i < GetMaxPeople(); ++i)
			{
				if (uDeskStation != i)
				{
					SendGameData(i, pData, DataSize, e_gs_mess_game_frame, e_gs_ass_game_frame_end_c, 0);
				}
			}
			//查看是不是所有人都提示结束了
			for (int i = 0; i < m_uMaxPeople;++i)
			{
				if (!m_UserEndGame[i])
				{//还有人没有告诉服务器结束，继续等待
					//启动结束计时器，防止其中有玩家断线不发送结束信息给服务器
					SetTimer(GAME_TIME_GAMEFINISH_WAIT, 10);
					return false;
				}
			}

			GameFinish(GAME_FINISH_TYPE_NORMAL);
		}
		return true;
	}

	/// 获得桌子中第一个没有人的座位索引
	bool CGameDesk::GetDeskNoPlayerStation(UINT &uDeskStation)
	{
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (m_pUserInfo[i] == nullptr)
			{
				uDeskStation = i;
				return true;
			}
		}

		return false;
	}

	/// 获得桌子中有多少人
	UINT CGameDesk::GetDeskPlayerNum()
	{
		UINT iPeopoleCount = 0;
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (m_pUserInfo[i] != nullptr)
			{
				iPeopoleCount++;
			}
		}

		return iPeopoleCount;
	}

	///用户在线状态离开游戏
	bool CGameDesk::UserOnlineStationNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo)
	{
		return true;
	}

	///用户在线状态重回游戏
	bool CGameDesk::UserOnlineStationReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo)
	{
		return true;
	}

	///是否锁定
	bool CGameDesk::IsLock()
	{
		return m_bLock;
	}

	///上锁
	void CGameDesk::LockDesk()
	{
		m_bLock = true;
	}

	///解锁
	void CGameDesk::UnlockDesk()
	{
		m_bLock = false;
	}

	//是否存在该玩家
	bool CGameDesk::IsExistPlayer(CGameUserInfo * pUserInfo)
	{
        if (pUserInfo == nullptr)
        {
            return false;
        }
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (m_pUserInfo[i] != nullptr && m_pUserInfo[i]->GetUserData()->basedata.dwUserID == pUserInfo->GetUserData()->basedata.dwUserID)
			{
				return true;
			}
		}
		return false;
	}

	///初始化函数
	bool CGameDesk::Init(UINT uDeskStation, UINT bMaxPeople, void* pDataManage)
	{
		m_uDeskIndex = uDeskStation;
		m_uMaxPeople = bMaxPeople;
		m_pGameRoom = (CGameRoom*)pDataManage;
		m_bPlayGame = false;
		m_bLock = false;
		std::memset(m_szLockPass, 0, sizeof(m_szLockPass));
		std::memset(m_UserEndGame, 0, sizeof(m_UserEndGame));
		m_dwOwnerUserID = 0;
		for (int i = 0; i < MAX_PEOPLE; ++i)
		{
			m_pUserInfo[i] = nullptr;
		}

		m_uGameStation = 0;
		InitDeskGameStation();
		return true;
	}

	//判断这个座位是否有人坐下了
	bool CGameDesk::bExistPlayerForDeskStation(UINT uDeskStation)
	{
		if (uDeskStation >= m_uMaxPeople)
		{
			return false;
		}

		return (m_pUserInfo[uDeskStation] != nullptr);
	}

	/// 设置桌子中的玩家信息
	void CGameDesk::SetDeskUserInfo(CGameUserInfo * pUserInfo, UINT uDeskStation)
	{
		if (uDeskStation >= m_uMaxPeople)
		{
			return;
		}

		if (nullptr != m_pUserInfo[uDeskStation] && nullptr == pUserInfo)
		{//将本来在桌子内的玩家设置为不在桌子内，肯定该玩家的信息也要设置一下
			m_pUserInfo[uDeskStation]->GetUserData()->uDeskid = ERROR_DESKID;
			m_pUserInfo[uDeskStation]->GetUserData()->uDeskStation = ERROR_DESKSTATION;
		}
		m_pUserInfo[uDeskStation] = pUserInfo;
	}

	//获取用户信息指针
	CGameUserInfo* CGameDesk::GetUserInfo(UINT uDeskStation)
	{
		if (uDeskStation >= m_uMaxPeople)
		{
			return nullptr;
		}

		return m_pUserInfo[uDeskStation];
	}

	//获取最大人数
	const UINT CGameDesk::GetMaxPeople()
	{
		return m_uMaxPeople;
	}

	//获取桌子id
	const UINT CGameDesk::GetDeskIndex()
	{
		return m_uDeskIndex;
	}

	///发送游戏状态
	bool CGameDesk::SendGameStation(UINT uDeskStation, UINT uSocketID, void* pStationData, UINT uSize)
	{
		//发送游戏状态
		return SendGameData(uDeskStation, pStationData, uSize, e_gs_mess_game_frame, e_gs_ass_game_frame_station, 0);
	}

	///发送开始消息
	bool CGameDesk::SendBeginData(void * pData, UINT uSize)
	{
		return SendDeskData(pData, uSize, e_gs_mess_game_frame, e_gs_ass_game_frame_begin, 0);
	}

	///发送结束消息
	bool CGameDesk::SendFinishData(void * pData, UINT uSize)
	{
		return SendDeskData(pData, uSize, e_gs_mess_game_frame, e_gs_ass_game_frame_end_s, 0);
	}

	//发送桌子数据
	bool CGameDesk::SendDeskData(void * pData, UINT uSize, UINT uMainID, UINT uAssID, UINT uHandleCode)
	{
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if ((m_pUserInfo[i] != nullptr) && (m_pUserInfo[i]->GetUserData()->basedata.bUserState != US_R_NetCut))
			{
                g_pGameServer->SendData(m_pUserInfo[i]->GetSocketId(), pData, uSize, uMainID, uAssID, uHandleCode);
			}
		}

		return true;
	}

	bool CGameDesk::SendDeskData(UINT uMainID, UINT uAssID, UINT uHandleCode)
	{
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if ((m_pUserInfo[i] != nullptr) && (m_pUserInfo[i]->GetUserData()->basedata.bUserState != US_R_NetCut))
			{
				g_pGameServer->SendData(m_pUserInfo[i]->GetSocketId(), uMainID, uAssID, uHandleCode);
			}
		}

		return true;
	}

	///发送数据函数
	bool CGameDesk::SendGameData(UINT uDeskStation, UINT uMainID, UINT uAssID, UINT uHandleCode)
	{
		if (uDeskStation < m_uMaxPeople)
		{
			if ((m_pUserInfo[uDeskStation] != nullptr) && (m_pUserInfo[uDeskStation]->GetUserData()->basedata.bUserState != US_R_NetCut))
			{
				UINT uSocketId = m_pUserInfo[uDeskStation]->GetSocketId();
				g_pGameServer->SendData(uSocketId, uMainID, uAssID, uHandleCode);
                return true;
			}
		}

		return false;
	}

	///发送数据函数
	bool CGameDesk::SendGameData(UINT uDeskStation, void * pData, UINT uSize, UINT uMainID, UINT uAssID, UINT uHandleCode)
	{
		if (uDeskStation < m_uMaxPeople)
		{
			if ((m_pUserInfo[uDeskStation] != nullptr) && (m_pUserInfo[uDeskStation]->GetUserData()->basedata.bUserState != US_R_NetCut))
			{
				UINT uSocketIndex = m_pUserInfo[uDeskStation]->GetSocketId();
				g_pGameServer->SendData(uSocketIndex, pData, uSize, uMainID, uAssID, uHandleCode);
                return true;
			}
		}

		return false;
	}

	//批量发送函数
	void CGameDesk::SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT uAssistantID, UINT uHandleCode)
	{
		m_pGameRoom->SendDataBatch(pData, uSize, uMainID, uAssistantID, uHandleCode);
	}

	//玩家是否任何时候都可以离开（意味着一离开游戏就结束了，即游戏状态只有一个）
	bool CGameDesk::SysCanLeaveEveryTime()
	{
		return CanLeaveEveryTime();
	}

	//设置定时器
	bool CGameDesk::SetTimer(UINT uTimerID, int uElapseSeconds)
	{
		UINT globalTimerId = 0;
		if (uTimerID >= TIMER_GAME_TIMERID_MASK && uTimerID < TIMER_GAME_TIMERID_MASK_SYS)
		{
			globalTimerId = ((m_pGameRoom->GetRoom_ManageInfoStruct()->uRoomID << 20) & TIMER_ROOMID_MASK)
				| ((m_uDeskIndex << 8) & TIMER_DESKID_MASK)
				| (uTimerID & TIMER_GAME_TIMERID_MASK_SYS);
		}
		else if (uTimerID < TIMER_GAME_TIMERID_MASK)
		{
			globalTimerId = ((m_pGameRoom->GetRoom_ManageInfoStruct()->uRoomID << 20) & TIMER_ROOMID_MASK)
				| ((m_uDeskIndex << 8) & TIMER_DESKID_MASK)
				| uTimerID;
		}
		else
		{
			return false;
		}

		
		g_pGameServer->SetTimer(globalTimerId, uElapseSeconds);
        return true;
	}

	//删除定时器
	bool CGameDesk::KillTimer(UINT uTimerID)
	{
		UINT globalTimerId = 0;
		if (uTimerID >= TIMER_GAME_TIMERID_MASK && uTimerID < TIMER_GAME_TIMERID_MASK_SYS)
		{
			globalTimerId = ((m_pGameRoom->GetRoom_ManageInfoStruct()->uRoomID << 20) & TIMER_ROOMID_MASK)
				| ((m_uDeskIndex << 8) & TIMER_DESKID_MASK)
				| (uTimerID & TIMER_GAME_TIMERID_MASK_SYS);
		}
		else if (uTimerID < TIMER_GAME_TIMERID_MASK)
		{
			globalTimerId = ((m_pGameRoom->GetRoom_ManageInfoStruct()->uRoomID << 20) & TIMER_ROOMID_MASK)
				| ((m_uDeskIndex << 8) & TIMER_DESKID_MASK)
				| uTimerID;
		}
		else
		{
			return false;
		}

		g_pGameServer->KillTimer(globalTimerId);
		return true;

	}

	//获取房间指针
	const Room_ManageInfoStruct* CGameDesk::GetRoom_ManageInfoStruct()
	{
		return m_pGameRoom->GetRoom_ManageInfoStruct();
	}

	//获取同桌玩家 
	UINT CGameDesk::FillDeskUserInfo(char * pBuffer, UINT uBufferSize, UINT& uindex, bool & bFinish)
	{
		return 0;
	}
	//清理桌子,强行踢人(是否把玩家重新放回排队机中)
	void CGameDesk::ClearDesk(bool bInqueue)
	{
		if (US_ContestRule_team == m_pGameRoom->GetRoom_ManageInfoStruct()->uContestRule)
		{
			bInqueue = false;
		}

		KillTimer(GAME_TIME_GAMEFINISH_WAIT);
		KillTimer(GAME_TIME_AGREE_OUTTIME);
		KillTimer(GAME_TIME_GAMEFINISH_OUTTIME);
		//重置游戏内部数据
		ReSetGameState(m_uGameStation);
		//清理桌子内玩家
		CleanCutGameInfoAll();//先清理断线用户
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (nullptr != m_pUserInfo[i])
			{
				m_pUserInfo[i]->GetUserData()->basedata.bUserState = US_R_Wait;
				m_pUserInfo[i]->GetUserData()->uDeskid = ERROR_DESKID;
				m_pUserInfo[i]->GetUserData()->uDeskStation = ERROR_DESKSTATION;
				if (bInqueue)
				{//继续加入队列
					g_pGameServer->GetCQueueManage(m_pGameRoom->GetRoom_ManageInfoStruct()->uContestid)->Add(m_pUserInfo[i]);
				}
				m_pUserInfo[i] = nullptr;
			}
		}

		std::memset(m_szLockPass, 0, sizeof(m_szLockPass));
		std::memset(m_UserEndGame, 0, sizeof(m_UserEndGame));
		m_bLock = false;
		m_dwOwnerUserID = 0;
		m_bPlayGame = false;
	}
	///游戏结束
	/// @param bCloseFlag  异常结束游戏时的标志，正常结束时为0
	bool CGameDesk::GameFinish(UINT uCloseFlag, UINT uDeskStation)
	{
		if (!m_bPlayGame)
		{
			QK_ErrorLog("sdp_error desk 游戏不在进行中，不处理结束事件。");
			return false;
		}
		QK_InfoLog("sdp_test desk 游戏结束============the end========================。");
		KillTimer(GAME_TIME_GAMEFINISH_WAIT);
		KillTimer(GAME_TIME_GAMEFINISH_OUTTIME);
		UINT uiPlayTime = (UINT)time(NULL) - (UINT)m_dwBeginTime;	//本局游戏时长
		m_bPlayGame = false;
		std::memset(m_UserEndGame, 0, sizeof(m_UserEndGame));
		//游戏内部结算处理
		std::string _Data;
		GameOverSettlement(uCloseFlag, _Data);
		//重置游戏内部数据
		ReSetGameState(m_uGameStation);

		if (US_ContestRule_solo == m_pGameRoom->GetRoom_ManageInfoStruct()->uContestRule)
		{//个人赛通知大厅
			//通知登陆服务器，玩家结束游戏了
			for (UINT i = 0; i < m_uMaxPeople; i++)
			{
				if (nullptr != m_pUserInfo[i])
				{
					struct_data_pr_userinfo_gameover cmd;
					cmd.userid = m_pUserInfo[i]->GetUserData()->basedata.dwUserID;
					QK_InfoLog("sdp_test desk 通知大厅游戏结束。");
					g_pGameServer->SendData(g_pGameServer->GetSockidOfLoginServer(), &cmd, sizeof(struct_data_pr_userinfo_gameover), e_pr_mess_userinfo, e_pr_ass_userinfo_gameover, uCloseFlag);
				}
			}
		}
		else if (US_ContestRule_team == m_pGameRoom->GetRoom_ManageInfoStruct()->uContestRule)
		{//团队赛通知大厅
			//如果是团队赛记得删除该VS记录
			unordered_map<UINT, R_TeamStruct_VS * > * _pTeamStruct_VS = g_pGameServer->GetR_TeamStruct_VS();
			bool bdelVS = false;//是否可以删除vs记录
			for (UINT i = 0; i < m_uMaxPeople; i++)
			{
				if (nullptr != m_pUserInfo[i])
				{
                    auto iter = _pTeamStruct_VS->find(m_pUserInfo[i]->GetUserData()->basedata.teamid);
					if (iter != _pTeamStruct_VS->end())
					{
						for (int j = 0; j < 2; ++j)
						{
                            for (int k = 0; k < iter->second->_data._TeamStruct[j].m_uTemaPlayerNum; ++k)
							{
								if (iter->second->_data._TeamStruct[j].m_Userid[k] == m_pUserInfo[i]->GetUserData()->basedata.dwUserID)
								{
									bdelVS = true;
									break;
								}
							}
							if (bdelVS)
							{
								break;
							}
						}
						if (bdelVS)
						{
							QK_InfoLog("sdp_test desk 通知大厅游戏结束。");
							struct_data_pr_userinfo_team_vs_finish cmd;
							cmd._data = iter->second->_data;
                            g_pGameServer->SendData(g_pGameServer->GetSockidOfLoginServer(), &cmd, sizeof(struct_data_pr_userinfo_team_vs_finish), e_pr_mess_userinfo, e_pr_ass_userinfo_team_vs_finish, 0);
							_pTeamStruct_VS->erase(iter);
							break;
						}
					}
				}

				if (bdelVS)
				{
					break;
				}
			}
		}
		
		//清理桌子内玩家
		CleanCutGameInfoAll();//先清理断线用户
		char data_str[MAX_SEND_SIZE];
		QK_InfoLog("sdp_test desk 群发游戏结束[1]length=%d", _Data.length());
		if (_Data.length()+1 <= sizeof(data_str))
		{
			sprintf_s(data_str, "%s", _Data.c_str());
		}
        else
        {
            QK_ErrorLog("game finish result data is too long : len=%d", _Data.length());
        }
		
		if (GAME_FINISH_TYPE_FLEE == uCloseFlag)
		{//如果是逃跑导致结束的,把逃跑玩家userid传递给所有人
			SendDeskData(&uDeskStation, sizeof(int), e_gs_mess_game_frame, e_gs_ass_game_frame_end_s, uCloseFlag);
		}
		else
		{
			SendDeskData(nullptr, 0, e_gs_mess_game_frame, e_gs_ass_game_frame_end_s, uCloseFlag);
		}

		QK_InfoLog("sdp_test desk 群发游戏结束[2]。uCloseFlag=%d", uCloseFlag);
		//清理掉桌子一切游戏记录
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (nullptr != m_pUserInfo[i])
			{//讲玩家清除在线状态
				g_pGameServer->GetCGameUserInfoManage()->FreeUserEx(m_pUserInfo[i]->GetUserData()->basedata.dwUserID);
			}
		}
		
		ClearDesk(false);

		
		//保存游戏结束信息到数据库
        //CGameServer::m_pRedis->hmset("game_result", );  //todo

		QK_InfoLog("sdp_test", "desk 群发游戏结束[3]。");
		return true;
	}

	///清理所有断线玩家
	bool CGameDesk::CleanCutGameInfoAll()
	{
		for (BYTE i = 0; i < m_uMaxPeople; i++)
		{
			CleanCutGameInfo(i);
		}
		return true;
	}

	//清理断线指定断线玩家资料
	bool CGameDesk::CleanCutGameInfo(UINT bDeskStation)
	{
		/// 如果没找到这个玩家，则返回 false
		if (bDeskStation >= m_uMaxPeople)
		{
			return false;
		}

		if ((m_pUserInfo[bDeskStation] != nullptr) && US_R_NetCut == m_pUserInfo[bDeskStation]->GetUserData()->basedata.bUserState)
		{
			//清理数据
			g_pGameServer->GetCGameUserInfoManage()->FreeUserEx(m_pUserInfo[bDeskStation]->GetUserData()->basedata.dwUserID);
			m_pUserInfo[bDeskStation] = nullptr;
		}

		return false;
	}

	int CGameDesk::MyRand()
	{
		static int innerSeed = GetTickCount();

		return (((innerSeed = innerSeed * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	///用户断线离开
	bool CGameDesk::Sys_UserNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo)
	{
		UserNetCut(uDeskStation, pLostUserInfo);

		if (!SysCanLeaveEveryTime())
		{
			//所有玩家都断线了，就结束游戏
			bool bAllNetCut = true;
			for (int i = 0; i < MAX_PEOPLE; ++i)
			{
				if (nullptr != m_pUserInfo[i] && US_R_NetCut != m_pUserInfo[i]->GetUserData()->basedata.bUserState)
				{
					bAllNetCut = false;
					break;
				}
			}

			if (bAllNetCut)
			{
				QK_InfoLog("sdp_test 所有玩家都断线了，就结束游戏");
				GameFinish(GAME_FINISH_TYPE_FORCE);
			}
		}
		else
		{//随时都可以离开那就结束
			QK_InfoLog("sdp_test 随时都可以离开那就结束!!!!!!!!!!!!!!");
			GameFinish(GAME_FINISH_TYPE_FLEE, uDeskStation);
		}

		return true;
	}

	///用户断线重来
	bool CGameDesk::Sys_UserReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo)
	{
		assert(nullptr != pNewUserInfo);
		m_pUserInfo[uDeskStation] = pNewUserInfo;
		return UserReCome(uDeskStation, pNewUserInfo);
	}

	///游戏开始
	bool CGameDesk::Sys_GameBegin(UINT uBeginFlag)
	{
		QK_InfoLog("sdp_test desk 游戏开始。");
		m_bPlayGame = true;
		m_dwBeginTime = (long int)time(NULL);
		KillTimer(GAME_TIME_AGREE_OUTTIME);
		SetTimer(GAME_TIME_GAMEFINISH_OUTTIME, 60 * 60 * 2);
		if (!GameBegin(uBeginFlag))
		{
			QK_InfoLog("sdp_test 开始失败，就结束游戏");
			return GameFinish(GAME_FINISH_TYPE_FORCE);
		}
		
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (nullptr != m_pUserInfo[i])
			{
				m_pUserInfo[i]->GetUserData()->basedata.bUserState = US_R_Playing;
			}
		}

		QK_InfoLog("sdp_test desk 群发游戏开始消息。");
		SendDeskData(e_gs_mess_game_frame, e_gs_ass_game_frame_begin, 0);
		return true;
	}

	///用户离开游戏桌
	UINT CGameDesk::Sys_UserLeftDesk(UINT uDeskStation, CGameUserInfo * pUserInfo, UINT uLeftCause)
	{
		QK_InfoLog("sdp_test desk 用户离开游戏桌。");
		assert(nullptr != pUserInfo);
		UINT userid = pUserInfo->GetUserData()->basedata.dwUserID;
		bool res = UserLeftDesk(uDeskStation, pUserInfo, uLeftCause);
		pUserInfo->GetUserData()->uDeskid = ERROR_DESKID;
		pUserInfo->GetUserData()->uDeskStation = ERROR_DESKSTATION;
		m_pUserInfo[uDeskStation] = nullptr;

		if (GetDeskPlayerNum() == 0)
		{//没人应该要清理下桌子
			ClearDesk(false);
		}

		SendDeskData(e_gs_mess_game_frame, e_gs_ass_game_frame_user_leave, userid);
		return res;
	}

	///框架消息处理函数
	bool CGameDesk::Sys_HandleFrameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID)
	{
		switch (pNetHead->bAssistantID)
		{
		case e_gs_ass_game_frame_info:
		{
			QK_InfoLog("sdp_test desk 收到玩家请求游戏桌子数据消息。");
			//获取游戏信息
			UINT uSendSize = 0;
			RoomMessDate_deskinfo deskinfo;
			deskinfo.bGameStation = m_uGameStation;///< 游戏状态
			QK_InfoLog("sdp_test desk 回复玩家请求游戏桌子数据消息。");
			//发送游戏信息
			SendGameData(uDeskStation, &deskinfo, sizeof(deskinfo), e_gs_mess_game_frame, e_gs_ass_game_frame_info, 0);

			//发送用户游戏状态
			return Sys_OnGetGameStation(uDeskStation, uSocketID);
		}break;
		case e_gs_ass_game_frame_agree:
		{
			UserAgreeGame(uDeskStation);
		}break;
		case e_gs_ass_game_frame_end_c:
		{
			Sys_UserEndGame(uDeskStation, pData, uSize);
		}break;
		default:
			break;
		}

		return HandleFrameMessage(uDeskStation, pNetHead, pData, uSize, uSocketID);
	}

	///游戏数据包处理函数
	bool CGameDesk::Sys_HandleGameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID)
	{
		return HandleGameMessage(uDeskStation, pNetHead, pData, uSize, uSocketID);
	}

	///定时器消息
	bool CGameDesk::Sys_OnTimer(UINT uTimerID)
	{
		UINT timeId = (uTimerID & (TIMER_GAME_TIMERID_MASK_SYS | TIMER_GAME_TIMERID_MASK));

		if (timeId >= TIMER_GAME_TIMERID_MASK && timeId < TIMER_GAME_TIMERID_MASK_SYS)
		{//系统计时器
			timeId = uTimerID & TIMER_GAME_TIMERID_MASK_SYS;

			if (GAME_TIME_GAMEFINISH_WAIT == timeId
				|| GAME_TIME_GAMEFINISH_OUTTIME == timeId)
			{//超时，发送结束消息
				QK_InfoLog("sdp_test 超时，发送结束消息(%d)", timeId);
				GameFinish(GAME_FINISH_TYPE_FORCE);
				return true;
			}
			else if (GAME_TIME_AGREE_OUTTIME == timeId)
			{
				ClearDesk(true);
			}
		}
		else if (timeId < TIMER_GAME_TIMERID_MASK)
		{//普通计时器
			timeId = uTimerID & TIMER_GAME_TIMERID_MASK;

			OnTimer(timeId);
			return true;
		}

		return false;
	}

	///获取游戏状态信息
	bool CGameDesk::Sys_OnGetGameStation(UINT uDeskStation, UINT uSocketID)
	{
		OnGetGameStation(uDeskStation, uSocketID);
		QK_InfoLog("sdp_test", "desk 告诉该玩家，桌子内其他玩家的数据。");
		//告诉该玩家，桌子内其他玩家的数据
		SendUserListForDesk(m_uDeskIndex, uSocketID, m_pUserInfo[uDeskStation]->GetUserData()->basedata.dwUserID);
		return true;
	}

	//设置房主
	void CGameDesk::SetdwOwnerUserID(UINT uuserid)
	{
		m_dwOwnerUserID = uuserid;
	}

	//获取房主
	UINT CGameDesk::GetdwOwnerUserID()
	{
		return m_dwOwnerUserID;
	}

	//搓桌分配完成
	void CGameDesk::QueueOver()
	{
		SetTimer(GAME_TIME_AGREE_OUTTIME, 30*60);
	}

	//判断是否可以开始游戏
	bool CGameDesk::Sys_CanBeginGame()
	{
		//判断所有人是否同意
		UINT bPeopoleCount = 0;
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (m_pUserInfo[i] != nullptr && US_R_Agress == m_pUserInfo[i]->GetUserData()->basedata.bUserState)
			{
				bPeopoleCount++;
			}
		}

		//判断适合开始条件
		if (bPeopoleCount == m_uMaxPeople)
		{
			return CanBeginGame();
		}

		return false;
	}

	//获得随机数字
	void CGameDesk::GetRandText(std::string &str, int strleng)
	{
		str.clear();
		do
		{
			int a = MyRand() % 10;
			char s[2] = "0";
			_itoa_s(a, s, 2, 10);
			str += s;
			--strleng;
		} while (strleng > 0);
	}

	//获取一个桌子内所有玩家列表,除自己信息外
	UINT CGameDesk::FillDeskUsers(char * pBuffer, UINT uBufferSize, UINT deskid, UINT myuserid, UINT uBeginPos, bool & bFinish, UINT &uCopyCount)
	{
		//拷贝数据
		uCopyCount = 0;//拷贝了几个
		UINT i = uBeginPos;//移动的节点
		if (uBeginPos < m_uMaxPeople)
		{
			int i = 0;
			for (; i<m_uMaxPeople; ++i)
			{
				if (((uCopyCount + 1)*sizeof(UserInfoInRoomStruct)) >= uBufferSize)
				{
					break;
				}

				if (nullptr != m_pUserInfo[i]
					&& myuserid != m_pUserInfo[i]->GetUserData()->basedata.dwUserID)
				{
					std::memcpy(pBuffer + uCopyCount*sizeof(UserInfoInRoomStruct), m_pUserInfo[i]->GetUserData(), sizeof(UserInfoInRoomStruct));
					++uCopyCount;
				}
			}
		}

		//拷贝完成
		bFinish = (i >= m_uMaxPeople);

		return i;
	}

	//向指定玩家更新该玩家所在桌子内所有除自己数据外的用户列表
	void CGameDesk::SendUserListForDesk(UINT deskid, UINT socketindex, UINT myuserid)
	{
		//获取构件名字
		UINT uBeginPos = 0;
		bool bFinish = false;
		char *pBuffer = new char[sizeof(UserInfoInRoomStruct)*m_uMaxPeople];
		assert(pBuffer);
		int uCopyCount = 0;
		for (int i = 0; i < m_uMaxPeople; ++i)
		{
			if (nullptr != m_pUserInfo[i]
				&& myuserid != m_pUserInfo[i]->GetUserData()->basedata.dwUserID)
			{
				std::memcpy(pBuffer + uCopyCount*sizeof(UserInfoInRoomStruct), m_pUserInfo[i]->GetUserData(), sizeof(UserInfoInRoomStruct));
				ZeroMemory(((UserInfoInRoomStruct*)(pBuffer + uCopyCount*sizeof(UserInfoInRoomStruct)))->basedata.password, PASSWORD_MD5_LENGTH);
				++uCopyCount;
			}
		}
		g_pGameServer->SendData(socketindex, pBuffer, uCopyCount*sizeof(UserInfoInRoomStruct), e_gs_mess_user_list, e_gs_ass_user_list_get_desk_userinfo, e_gs_hcid_userlist_finish);
		if (pBuffer)
		{
			delete[]pBuffer;
			pBuffer = NULL;
		}
	}
}