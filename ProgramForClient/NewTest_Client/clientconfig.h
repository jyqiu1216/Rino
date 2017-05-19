#ifndef __WJ_clientconfig_H__
#define __WJ_clientconfig_H__

#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"
class Ccconfig
{
	struct  Ccconfig_data
	{
		std::string hall_server_address;//登陆服务器ip
		std::string web_server_address;//web服务器ip
		UINT hall_server_port;//登陆服务器端口
		UINT AInum;//机器人数目
		UINT AIFirstID;//机器人起始id
		UINT contestid;//加载游戏队列的比赛id
		UINT roomid;//需要进入的房间的房间id
		Ccconfig_data()
		{
			hall_server_address = "127.0.0.1";
			web_server_address = "127.0.0.1";
			hall_server_port = PORT_LS;
			AInum = 200;
			contestid = 27;
			roomid = 756;
		}
	};

public:
	// 获取单例
	static Ccconfig& getInstance();

	//初始化配置文件，检测如果文件不存在就创建文件，并将默认参数写进文件
	void initconfig(){}
	//获取数据
	Ccconfig_data* GetData();
private:
	// 构造函数是私有的
	Ccconfig(void);

	// 析构函数
	virtual ~Ccconfig(void);

private:
	//类拷贝是私有的
	Ccconfig(const Ccconfig &);
	Ccconfig & operator = (const Ccconfig &);

private:
	Ccconfig_data m_data;//数据
};

#define  clientconfig()	Ccconfig::getInstance()

#endif//__WJ_clientconfig_H__