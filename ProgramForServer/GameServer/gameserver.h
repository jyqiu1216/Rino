#pragma  once

#include <mutex>
#include "common/WJBaseType.h"
#include "server/comstruct.h"
#include "server/WJSSocketManage.h"
#include "redis/WJRedis.h"
#include "RoomManage.h"
#include "server/WJTimerMgr.h"
#include "server/comtimetag.h"

enum E_DB_BUSID //数据库
{
	E_DB_BUSID_query_roomlist = 1
};

// 连接类型
enum E_CONN_TYPE
{
	E_CONN_LoginServer = 1,//连接登录服务器
	E_CONN_Client = 2//连接用户
};



namespace WJ
{
	struct ContestInfo_locad
	{
		char cContestName[CONTESTNAME_LENGTH];//比赛名
		ENUM_ContestRule uContestRule = (ENUM_ContestRule)0;//比赛模式
		ENUM_ContestKind uContestKind = (ENUM_ContestKind)0;//比赛场次类型
		LLONG _i64cost = 0;//报名费用
		ENUM_AwardType awardType = (ENUM_AwardType)0;//奖励类型
		UINT award = 0;//奖励

		ContestInfo_locad()
		{
			std::memset(cContestName, 0, sizeof(cContestName));
		}
	};

	class CGameServer : public CWJSSocketManage, public CallBack_Time
	{
	public:
		CGameServer(unsigned int maxSocketNum);
		~CGameServer();
		void StartServer();
		void StopServer();

		static CWJRedis * GetRedis() { return m_pRedis; }
        //计时器事件处理(该函数执行在上层业务主线程中)
        int ProccessTimers();
		// 数据库查询结果处理(该函数执行在上层业务主线程中)
		int ProccessRedisResults();
		// 网络消息逻辑处理
		virtual bool ProccessNetMessage(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		virtual void OnNewConnection(CWJSSocket * pSocket, void *pUserData);
		virtual void OnEndConnection(CWJSSocket * pSocket);
		virtual void OnConnectFailed(int type, void *pUserData);

		virtual bool OnTimerMessage(TimerID timerId);

		//处理玩家客户端消息
		bool ProccessNetMessage_player(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//处理ls发来的消息
		bool ProccessNetMessage_ls(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);

        void OnReconnectLoginServerTimer();
		void OnKeepaliveLoginServerTimer();

		//UINT uElapse 单位秒
		void SetTimer(TimerID uTimerID, UINT uElapse);

		void KillTimer(TimerID uTimerID);

		UINT GetSockidOfLoginServer() { return m_sockidOfLoginServer; }
		CGameUserInfoManage* GetCGameUserInfoManage();
		CQueueManage* GetCQueueManage(UINT contestid);
		std::unordered_map<UINT, R_TeamStruct_VS * >* GetR_TeamStruct_VS();

		//发送给团队VS里面的消息
		//UINT myuserid 排除自己发送
		void RTeamVSSendData(UINT myuserid, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode);

		//添加比赛
		void AddContInfo_locad(UINT contestid, ContestInfo_locad &data);
		//获得比赛数据
		ContestInfo_locad* GetContInfo_locad(UINT contestid);
	private:
		/////PtoR通信消息////////// 来自loginserver
		//房间消息处理
		bool OnSocketRead_pr_mess_roominfo(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//用户消息处理///// 来自loginserver
		bool OnSocketRead_pr_mess_userinfo(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);

	public:
		static  CWJRedis *m_pRedis;
		CRoomManage   m_roomMgr;//房间管理器

	private:
		std::string   m_redisIp;
		std::string   m_redisPass;
		int           m_nRedisPort;
		int           m_nListenPort;
        CWJTimerMgr   m_timerMgr;//计时器
		
		UINT          m_sockidOfLoginServer;//与登录服务器连接的套接字id
		CGameUserInfoManage m_UserInfoManage;	//用户管理

		std::unordered_map<UINT, R_TeamStruct_VS * >  m_teamid2team_vs; // 键是团队id，值是团队比赛结构体指针. 房间内存在的团队比赛
		std::unordered_map<UINT, ContestInfo_locad> m_contestid2info;  //比赛id和他的比赛数据对应关系
		std::unordered_map<UINT, CQueueManage>	m_contestid2Queue;//键是比赛id，值是对应的排队机
		UINT m_uCheckTime;//检测断线间隔（单位是s）
	};

}

