#include "lsconfig.h"
#include <io.h>
Clsconfig& Clsconfig::getInstance()
{
	static Clsconfig cconfig;
	return cconfig;
}

Clsconfig::Clsconfig()
{

}

Clsconfig::~Clsconfig()
{

}

//初始化配置文件，检测如果文件不存在就创建文件，并将默认参数写进文件
void Clsconfig::initconfig()
{
	bool bExit = false;
	std::string strFile = "lsconfig.json";
	if ((_access(strFile.c_str(), 0)) != -1)
	{
		bExit = true;
	}
	if (bExit)
	{
		FILE*fp = fopen(strFile.c_str(), "rb");// localfile文件名       
		fseek(fp, 0L, SEEK_END); /* 定位到文件末尾 */
		int nLen = ftell(fp); /* 得到文件大小 */
		char *pData = new char[nLen + 1]; /* 根据文件大小动态分配内存空间 */
		if (NULL == pData)
		{
			fclose(fp);
			return;
		}
		memset(pData, 0, nLen + 1);

		fseek(fp, 0L, SEEK_SET); /* 定位到文件开头 */
		fread(pData, nLen, 1, fp); /* 一次性读取全部文件内容 */
		std::string strJsonString = pData;
		if (pData)
		{
			delete[]pData;
		}
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(strJsonString, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
		{

			Json::Value val = root["SysSet"];
			{//redis服务是否开启(true,false)
				m_data.redis_on_off = val["redis_on_off"].asBool();
			}
			{//redis服务器ip
				m_data.redis_address = val["redis_address"].asString();
			}
			{//redis端口
				m_data.redis_port = val["redis_port"].asInt();
			}
			{//redis密码
				m_data.redis_pass = val["redis_pass"].asString();
			}
			{//虚拟用户最大人数
				m_data.iMaxVirtualCount = val["MaxVirtualCount"].asInt();
			}
			{//虚拟用户最小人数
				m_data.iMinVirtualCount = val["MinVirtualCount"].asInt();
			}
		}
	}
	else
	{
		std::string str = "{\n"
			"\"SysSet\" :\n"
			"	{\n"
			"	\"redis_on_off\": true,\n"
			"	\"redis_address\": \"118.178.25.88\",\n"
			"	\"redis_port\": 6380,\n"
			"	\"redis_pass\": \"XnDj\",\n"
			"	\"MaxVirtualCount\": 800,\n"
			"	\"MinVirtualCount\": 700 \n"
			"	}\n"
			"}\n";
		FILE *fp = fopen(strFile.c_str(), "wb");
		if (fp)
		{
			fwrite(str.c_str(), str.length(), 1, fp);
			fclose(fp);
		}
	}
}

//获取数据
const Clsconfig::Clsconfig_data* Clsconfig::GetData()
{
	static bool bInit = false;
	if (!bInit)
	{
		initconfig();
		bInit = true;
	}
	return &m_data;
}
