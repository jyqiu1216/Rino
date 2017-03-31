#include "loginserver.h"
#include "server/comstruct.h"
#include "lsconfig.h"
#include "server/MD5.h"
#include "common/QkLogger.h"

LoginServer::LoginServer()
{
	WJ::Init_WinSock();
	m_uConnectCount = 0;
	m_uConnectCount_gs = 0;
	m_uOnlineCount = 0;
	m_uOnlineCount_gs = 0;
	m_ApplyInfo_str.clear();
	m_TeamInfo_str.clear();

	//初始化套接字
	int imaxcount = PORT_LS_COUNT;

	m_ManageInfoStruct.bRun_SocketLib = true;
	m_ManageInfoStruct.bRun_DataSQLLib = false;
	m_ManageInfoStruct.uMaxCount = imaxcount;
	m_ManageInfoStruct.uListenPort = PORT_LS;

	m_ManageInfoStruct.bRun_Redis = lsconfig().GetData()->redis_on_off;
	m_ManageInfoStruct.uRedis_port = lsconfig().GetData()->redis_port;
	sprintf_s(m_ManageInfoStruct.uRedis_ip, "%s", lsconfig().GetData()->redis_address.c_str());
	sprintf_s(m_ManageInfoStruct.uRedis_Pass, "%s", lsconfig().GetData()->redis_pass.c_str());
	m_pMainManage_login = new WJ::CMainManage_login(&m_ManageInfoStruct);
	//QK_WarnLog("aaa");
	//m_pBalanceSocket = new CBalanceSocket("127.0.0.1", 3258);
	//bool bRet = false;
	//do
	//{
	//	bRet = m_pBalanceSocket->ConnectBalanceServer();
	//	if (bRet == false)
	//	{
	//		Sleep(1000);
	//	}
	//} while (bRet == false);
}

LoginServer::~LoginServer()
{
	WJ:: UnInit_WinSock();
}


//启动服务器
void LoginServer::StatServer()
{
	if (m_pMainManage_login)
	{
		m_pMainManage_login->Start_subclass();
		while (true)
		{
			if (0 == m_pMainManage_login->ProcessNetEvents()
			&& 0 == m_pMainManage_login->ProccessRedisResults()
			&& 0 == m_pMainManage_login->ProccessTimers())
			{
				Sleep(1);
			}
		}
	}
}
//关闭服务器
void LoginServer::StopServer()
{
	if (m_pMainManage_login)
	{
		m_pMainManage_login->Stop_subclass();
	}
}
