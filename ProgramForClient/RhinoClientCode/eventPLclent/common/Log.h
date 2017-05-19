#pragma once

#include <fstream>
#include <Windows.h>
#include <stdarg.h>
#include <string>
class QString;
//class CSystemLog;

//系统日志类，  （该类为单实例类）
class CSystemLog
{
public:

	// 构造函数私有化，通过getInstance()来获取实例指针
	CSystemLog();

public:
	~CSystemLog();

public:
	enum EnLogLevel
	{
		E_LOG_DEBUG,	// 调试信息
		E_LOG_NOTICE,	// 提示信息
		E_LOG_WARNING,	// 警告
		E_LOG_ERROR,	// 错误
		E_LOG_FATAL,	// 致命错误
	};

public:

	static CSystemLog* getInstance();

	// 释放资源
	void release();	
	
	// 初始化： 创建并打开当天的日志文件
	void initialize();
	
	// 获取系统当前时间 (字符串格式 yyyy-mm-dd hh:mm:ss.mms)	
	char* getSysTimeBuffer();
		
	// 格式化输出日志到文件(buf有限制)	
	void LogToFile(char* format, ...);

	// 非格式化输出日志到文件(buf无限制)
	void LogToFile(const char* buf);


	// 非格式化输出日志到文件,str要使用前要转成utf8编码
	void LogToFile(std::string str);

#ifdef _UNUSE_QT

#else
	// 非格式化输出日志到文件,str要使用前要转成utf8编码
	void LogToFile(QString str);
#endif

	// 格式化输出日志到调试器(buf有限制)
	void LogToDebug(char* lpPre, char* format, ...);

private:
	//Utf8ToGb2312
	char* U2G(const char* utf8);
	//GB2312到UTF-8的转换
	char* G2U(const char* gb2312);
	int IsTextUTF8(const char* str, ULONGLONG length);
private:
	// 文件操作流	
	std::fstream mFSream;
	
	// 字符串缓存	
#define BUFFERSIZE 1024 * 1000
	char mBuffer[BUFFERSIZE];

}; // $END$ class CSystemLog

#define SystemLog() CSystemLog::getInstance()

//DEBUG 调试日志
#define LOG_DEBUG(...)		\
if (SystemLog()->isOpen()) \
{ \
	SystemLog()->LogToFile(CSystemLog::E_LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);	\
}

//NITICE 提示日志
#define LOG_NITICE(...)		\
if (SystemLog()->isOpen()) \
{ \
	SystemLog()->LogToFile(CSystemLog::E_LOG_NOTICE, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);	\
}

//	WARNING 告警日志
#define LOG_WARNING(...)		\
if (SystemLog()->isOpen()) \
{ \
	SystemLog()->LogToFile(CSystemLog::E_LOG_WARNING, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);	\
}

//	ERROR 错误日志
#define LOG_ERROR(...)		\
if (SystemLog()->isOpen()) \
{ \
	SystemLog()->LogToFile(CSystemLog::E_LOG_ERROR, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);	\
}

//	FATAL 致命错误日志
#define LOG_FATAL(...)		\
if (SystemLog()->isOpen()) \
{ \
	SystemLog()->LogToFile(CSystemLog::E_LOG_FATAL, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);	\
}