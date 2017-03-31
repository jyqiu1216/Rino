#ifndef __WJ_clientconfig_H__
#define __WJ_clientconfig_H__

#include "json/json.h"
#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"
class Clsconfig
{
	struct  Clsconfig_data
	{
		bool redis_on_off;//redis服务是否开启(true,false)
		std::string redis_address;//redis服务器ip
		int redis_port;//redis端口
		std::string redis_pass;//redis密码
		int iMaxVirtualCount;//虚拟用户最大人数
		int iMinVirtualCount;//虚拟用户最小人数
		Clsconfig_data()
		{
			redis_on_off = true;
			redis_address = "0.0.0.0";
			redis_port = 6380;
			redis_pass = "XnDj";
			iMaxVirtualCount = 800;
			iMinVirtualCount = 700;
		}
	};

public:
	// 获取单例
	static Clsconfig& getInstance();

	//初始化配置文件，检测如果文件不存在就创建文件，并将默认参数写进文件
	void initconfig();
	//获取数据
	const Clsconfig_data* GetData();
private:
	// 构造函数是私有的
	Clsconfig(void);

	// 析构函数
	virtual ~Clsconfig(void);

private:
	//类拷贝是私有的
	Clsconfig(const Clsconfig &);
	Clsconfig & operator = (const Clsconfig &);

private:
	Clsconfig_data m_data;//数据
};

#define  lsconfig()	Clsconfig::getInstance()

#endif//__WJ_clientconfig_H__
