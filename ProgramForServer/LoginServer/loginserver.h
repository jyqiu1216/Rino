#ifndef LOGINSERVER_H
#define LOGINSERVER_H

#include <list>
#include <mutex>


#include "mainmanage_login.h"
#include "balancesocket.h"
#include "Redis\WJRedis.h"

class LoginServer
{

public:
	LoginServer();
	~LoginServer();
	void StatServer();
	//关闭服务器
	void StopServer();

private:
	WJ::CMainManage_login *m_pMainManage_login;
	UINT m_uConnectCount;//当前连接数
	UINT m_uConnectCount_gs;//当前gs连接数
	UINT m_uOnlineCount;//当前在线人数
	UINT m_uOnlineCount_gs;//当前gs有效连接数
	std::string m_ApplyInfo_str;//当前个人赛信息
	std::string m_TeamInfo_str;//当前团队赛信息
	WJ::ManageInfoStruct m_ManageInfoStruct;//大厅启动的基础数据
	CBalanceSocket *m_pBalanceSocket;//与中心服务器的连接
};

#endif // LOGINSERVER_H
