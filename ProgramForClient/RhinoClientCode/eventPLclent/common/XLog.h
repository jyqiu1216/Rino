#pragma once


#include <fstream>
#include <Windows.h>
#include <stdarg.h>

#define LOG_BUF_SIZE	1024*100

/******************************************************************
* @brief： 系统日志类
*/
class XLog
{
public:
	XLog();
	~XLog();


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
	/**
	* @brief： 释放资源
	*/
	void release();


	/**
	* @brief： 初始化： 设置日志路径名
	*/
	void initialize(const char* _logFile);


	/**
	* @brief： 获取系统当前时间 (字符串格式 yyyy-mm-dd hh:mm:ss.mms)
	*/
	char* getSysTimeBuffer();

	
	/**
	* @brief： 输出日志
	*/
	void writeLog(const EnLogLevel& _level, const char* _format, ...);

	bool isOpen() { return mIsOpen; }

	void open() { mIsOpen = true; }

private:
	/**
	* @brief： 文件操作流
	*/
	std::fstream mFSream;

	/**
	* @brief： 字符串缓存
	*/
	char mBuffer[LOG_BUF_SIZE];

	/**
	* @brief:	日志文件是否打开标识
	*/
	bool mIsOpen;

}; // $END$ class SystemLog