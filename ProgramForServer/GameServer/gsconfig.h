#ifndef __WJ_clientconfig_H__
#define __WJ_clientconfig_H__

#include <string>
#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"

class Cgsconfig
{
#define CONFIG_CONTEST_MAXNUM 100
	struct  Cgsconfig_data
	{
		std::string loginserver_address;//登陆服务器ip
		std::string ip_for_client; // 给客户端直连的ip
        int port_for_client;
        int max_client_count;
		std::string redis_address;//redis服务器ip
		int redis_port;//redis端口
		std::string redis_pass;//redis密码
		UINT contestid[CONFIG_CONTEST_MAXNUM];//加载房间队列的比赛id
		Cgsconfig_data()
		{
			loginserver_address = "127.0.0.1";
            port_for_client = 9000;
            max_client_count = 10000;
			redis_address = "118.178.25.88";
			redis_port = 6380;
			redis_pass = "XnDj";
			std::memset(contestid, 0, sizeof(contestid));
		}
	};

public:
	// 获取单例
	static Cgsconfig& getInstance();

	//初始化配置文件，检测如果文件不存在就创建文件，并将默认参数写进文件
	void initconfig();
	//获取数据
	const Cgsconfig_data* GetData();
private:
	// 构造函数是私有的
	Cgsconfig(void);

	// 析构函数
	virtual ~Cgsconfig(void);

private:
	//类拷贝是私有的
	Cgsconfig(const Cgsconfig &);
	Cgsconfig & operator = (const Cgsconfig &);

private:
	Cgsconfig_data m_data;//数据
};

#define  gsconfig()	Cgsconfig::getInstance().GetData()

#endif//__WJ_clientconfig_H__