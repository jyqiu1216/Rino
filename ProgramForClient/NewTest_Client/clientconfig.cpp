#include "clientconfig.h"
#include "clientsocket/commonfunction_c.h"
#include <windows.h>
Ccconfig& Ccconfig::getInstance()
{
	static Ccconfig cconfig;
	return cconfig;
}

Ccconfig::Ccconfig()
{
	std::string strPath = GetAppPath() + "\\aiset.ini";
	//读取设置值
	char str[200] = {0};
	//登陆服务器
	GetPrivateProfileStringA("config", "hall_server_address", "127.0.0.1", str, MAX_PATH, strPath.c_str());
	m_data.hall_server_address = str;
	//机器人数目
	GetPrivateProfileStringA("config", "AInum", "0", str, MAX_PATH, strPath.c_str());
	m_data.AInum = atoi(str);

	//机器人起始id
	GetPrivateProfileStringA("config", "AIFirstID", "0", str, MAX_PATH, strPath.c_str());
	m_data.AIFirstID = atoi(str);

	//比赛id
	std::memset(str, 0, sizeof(str));
	GetPrivateProfileStringA("config", "contestid", "0", str, MAX_PATH, strPath.c_str());
	m_data.contestid = atoi(str);

	//房间id
	std::memset(str, 0, sizeof(str));
	GetPrivateProfileStringA("config", "roomid", "0", str, MAX_PATH, strPath.c_str());
	m_data.roomid = atoi(str);
}

Ccconfig::~Ccconfig()
{

}

//获取数据
Ccconfig::Ccconfig_data* Ccconfig::GetData()
{
	return &m_data;
}