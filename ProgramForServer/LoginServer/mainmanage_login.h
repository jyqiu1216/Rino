
#ifndef _WJ_mainmanage_login_H_
#define _WJ_mainmanage_login_H_
#include "TeamManage.h"
#include "server/WJServerManagelib.h"
#include "server/WJSSocketManage.h"
#include "LUserInfoManage.h"
#include "Gamelistmanage.h"
#include "ApplyManage.h"
#include "redisDataListManage.h"
#include <map>
#include <vector>
#include<mutex>
#include "server/WJTimerMgr.h"
namespace WJ
{
	class CMainManage_login : public  CWJSSocketManage, public CallBack_Time
	{
	
	public:
		CMainManage_login(ManageInfoStruct *pManageInfoStruct);
		virtual ~CMainManage_login();


		CLUserInfoManage* GetCLUserInfoManage();

		///初始化函数
		virtual bool Init_subclass(const ManageInfoStruct &InitData);
		///取消初始化函数
		virtual bool UnInit_subclass();
		///启动函数
		virtual bool Start_subclass();
		///停止服务
		virtual bool Stop_subclass();

		//计时器事件处理(该函数执行在上层业务主线程中)
		int ProccessTimers();
		// 数据库查询结果处理
		int ProccessRedisResults();

		//UINT uElapse 单位秒
		void SetTimer(TimerID uTimerID, UINT uElapse);

		void KillTimer(TimerID uTimerID);

	private:
		// 网络消息逻辑处理
		virtual bool ProccessNetMessage(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//有新客户端加入
		virtual void OnNewConnection(CWJSSocket * pSocket, void *pUserData);
		///SOCKET 关闭 （必须重载）
		virtual void OnEndConnection(CWJSSocket * pSocket);
		//[客户端角色]连接对方失败
		virtual void OnConnectFailed(int type, void *pUserData)  {  }
		///数据库处理结果 （必须重载）
		virtual bool OnDataBaseResult(DataBaseResultLine * pResultData, UINT tag = 0);
		///定时器消息 （必须重载）
		virtual bool OnTimerMessage(TimerID timerId);


		//批量发送函数
		void SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT uAssistantID, UINT uHandleCode);

		//登陆消息处理
		bool OnSocketRead_hs_mess_login(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//游戏列表消息处理
		bool OnSocketRead_hs_mess_gamelist(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//报名消息处理
		bool OnSocketRead_hs_mess_apply(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//团队消息处理
		bool OnSocketRead_hs_mess_team(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		/////PtoR通信消息//////////
		//房间消息处理
		bool OnSocketRead_pr_mess_roominfo(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//用户消息处理
		bool OnSocketRead_pr_mess_userinfo(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);

		//获得指定比赛下的roomlis，并发送给客户端
		void onSendRoomListForContest(CWJSSocket * pSocket, UINT contest, UINT bMainID, UINT bAssistantID, UINT bHandleCode_part, UINT bHandleCode_finish);

		//发送给团队里面的消息
		//UINT myuserid 排除自己发送
		void TeamSendData(UINT myuserid, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//发送给团队VS里面的消息
		//UINT myuserid 排除自己发送
		void TeamVSSendData(UINT myuserid, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode);


		//redis命令结果 用户登录大厅
		void onRedisRes_Order_h_messid_load_hall(RedisResultLine * pResultData);
		//redis命令结果 报名
		void onRedisRes_Order_h_messid_apply(RedisResultLine * pResultData);
		//redis命令结果 退费
		void onRedisRes_Order_h_messid_apply_restitution(RedisResultLine * pResultData);
		//redis命令结果 创建团队
		void onRedisRes_Order_h_messid_team_create(RedisResultLine * pResultData);
		//redis命令结果 加入团队
		void onRedisRes_Order_h_messid_team_join(RedisResultLine * pResultData);
		//redis命令结果 退出团队
		void onRedisRes_Order_h_messid_team_leave(RedisResultLine * pResultData);
		//redis命令结果 解散团队
		void onRedisRes_Order_h_messid_team_dissolve(RedisResultLine * pResultData);
		//redis命令结果 更新用户数据(娱币、积分)
		void onRedisRes_Order_h_messid_update_userinfo(RedisResultLine * pResultData);
		//测试
		void Test();
	private:
		CWJRedis * m_pRedis;

		//CLGsocketserver m_CLGsocketserver;//针对游戏服务器的通信类
		CLUserInfoManage m_CLUserInfoManage;//用户列表
		CGameList		m_CGameList;//游戏列表
		CApplayManage	m_CApplayManage;//报名管理
		CTeamManage m_CTeamManage;//团队创建管理

		ManageInfoStruct m_ManageInfoStruct;//基础数据(针对游戏服务器的通信类)
		std::vector<int> m_socket_vector;//当前正常给服务器发送消息的GS数目

		CWJTimerMgr   m_timerMgr_h;//计时器

		UINT m_uCheckTime;//检测断线间隔（单位是s）

		UINT m_update_now_playercount;//当前在线人数
	};
}

#endif //_WJ_mainmanage_login_H_
