#include "gsconfig.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "json/json.h"
#include "common/QkLogger.h"

Cgsconfig& Cgsconfig::getInstance()
{
	static Cgsconfig cconfig;
	return cconfig;
}

Cgsconfig::Cgsconfig()
{

}

Cgsconfig::~Cgsconfig()
{

}

/* 配置文件格式
{
"SysSet" :
	{
	"loginserver_address": "127.0.0.1",
	"ip_for_client": "192.168.0.86",
    "port_for_client": 9000,
    "max_client_count": 50000,
	"redis_address": "118.178.25.88",
	"redis_port": 6380,
	"redis_pass": "XnDj",
	"contestid0": 10,
	"contestid1": 11
	//...
	}
}
*/
//初始化配置文件，检测如果文件不存在就创建文件，并将默认参数写进文件
void Cgsconfig::initconfig()
{
	FILE *file = fopen("gsconfig.json", "rb");
	if (!file)
	{
		QK_ErrorLog("Cgsconfig::initconfig  error:%s", strerror(errno));
		return;
	}
	int nFileLen = 0;
	fseek(file, 0, SEEK_END);
	nFileLen = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *fileContent = new char[nFileLen + 1];
	fread(fileContent, nFileLen, 1, file);
	fileContent[nFileLen] = '\0';
	fclose(file);

	Json::Reader reader;
	Json::Value  root;
	if (!reader.parse(fileContent, fileContent + nFileLen, root))
	{
		QK_ErrorLog("Cgsconfig::initconfig  json parse error, make sure gsconfig.json have legal json string");
		if (fileContent)
		{
			delete[]fileContent;
		}
		return;
	}
	//root.toStyledString();
    if (root["SysSet"].isObject())
    {
        Json::Value & sett = root["SysSet"];
        m_data.loginserver_address = sett["loginserver_address"].asString();
		m_data.ip_for_client = sett["ip_for_client"].asString();
        m_data.port_for_client = sett["port_for_client"].asInt();
        m_data.max_client_count = sett["max_client_count"].asInt();
		m_data.redis_address = sett["redis_address"].asString();
        m_data.redis_port = sett["redis_port"].asInt();
		m_data.redis_pass = sett["redis_pass"].asString();
		for (INT i = 0; i < CONFIG_CONTEST_MAXNUM;++i)
		{
			//加载房间队列的比赛id
			char str[64] = {0};
			sprintf_s(str, sizeof(str), "contestid%d", i);
			if (! sett[str].isNull())
			{
                m_data.contestid[i] = sett[str].asInt();
			}			
		}
    }
	if (fileContent)
	{
		delete[]fileContent;
	}
}

//获取数据
const Cgsconfig::Cgsconfig_data* Cgsconfig::GetData()
{
	static bool bInit = false;
	if (!bInit)
	{
		initconfig();
		bInit = true;
	}
	return &m_data;
}