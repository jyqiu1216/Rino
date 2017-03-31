//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved
#pragma once

#ifndef _WJ_WJException_H_
#define _WJ_WJException_H_
#include "../common/WJBaseType.h"



#include <WinBase.h>
#include <Winuser.h>
#include <stdlib.h>
namespace WJ
{
	///***********************************************************************************************///
	///系统错误
#define EX_UNKNOWN_ERROR			0						///未知错误
#define EX_INIT_ERR					1						///初始化失败
#define EX_HEAP_MEM_LOW				2						///堆内存不足
#define EX_IOCP_ERR					3						///完成端口错误

	///数据库错误
#define EX_DATA_CONNECT				100						///数据库连接错误
#define EX_DATA_USER				101						///数据库名字错误
#define EX_DATA_PASS				102						///数据库密码错误
#define EX_DATA_EXEC				103						///数据库执行错误

	///网络错误
#define EX_SOCKET_CREATE_ERR		200						///网络建立错误
#define EX_SOCKET_BIND_ERR			201						///网络绑定错误
#define EX_SOCKET_LISTEN_ERR		202						///网络监听错误
#define EX_SOCKET_SEND_ERR			203						///网络发送错误
#define EX_SOCKET_RECV_ERR			204						///网络接受错误
#define EX_SOCKET_PACKET_ERR		205						///网络数据包异常

	///服务错误
#define EX_SERVICE_START			300						///服务启动
#define EX_SERVICE_PAUSE			301						///服务暂停
#define EX_SERVICE_STOP				302						///服务停止
#define EX_SERVICE_BUSY				303						///服务繁忙
#define EX_SERVICE_UNKNOW			304						///未知服务信息

	///自定义错误

	///***********************************************************************************************///

	///异常类
	class WJSERVERMANAGELIB_API CWJException
	{
		///变量定义
	protected:
		UINT					m_uErrorCode;					///错误码
		bool					m_bAutoDelete;					///是否自动删除
		char					m_szMessage[255];				///错误信息
		static bool				m_bShowError;					///是否显示错误

		///函数定义
	public:
		///构造函数
		inline CWJException(char * szErrorMessage, UINT uErrorCode = EX_UNKNOWN_ERROR, bool bAutoDelete = true);
		///析构函数
		virtual ~CWJException(void);

		///功能函数
	public:
		///获取错误代码
		virtual UINT GetErrorCode() const;
		///获取错误信息
		virtual char const * GetErrorMessage(char * szBuffer, int iBufLength) const;
		///删除函数
		virtual bool Delete();
		///设置是否显示错误
		static bool ShowErrorMessage(bool bShowError);
	};
}

#endif//_WJ_WJException_H_