// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 WJSERVERMANAGELIB_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// WJSERVERMANAGELIB_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#ifndef _WJ_MAINMANAGE_H_
#define _WJ_MAINMANAGE_H_

#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"
#include "server/comstruct.h"
#include "server/DataLine.h"
#include "server/WJSSocketManage.h"

namespace WJ
{
	/*
	服务器底层直接面对上层接口的部分实际上是一个单线程队列，
	所有的套接字读取、套接字关闭、套接字连接、数据库处理结果、计时器执行事件都在其中
	*/

	//处理类型定义
#define HD_SOCKET_READ					1							//SOCKET 读取事件处理	
#define HD_SOCKET_CLOSE					2							//SOCKET 关闭事件处理
#define HD_SOCKET_CONNECT				3							//SOCKET 连接事件处理
#define HD_DATA_BASE_RESULT				4							//数据库结果处理
#define HD_TIMER_MESSAGE				5							//定时器消息处理

	//SOCKET 有客户端连接成功通知结构定义
	struct SocketConnectLine
	{
		DataLineHead						LineHead;					//队列头
		UINT								uIndex;						//SOCKT 索引
		ULONG								uAccessIP;					//SOCKET IP
		
	};

	//SOCKET 关闭通知结构定义
	struct SocketCloseLine
	{
		DataLineHead						LineHead;					//队列头
		UINT								uIndex;						//SOCKT 索引
		ULONG								uAccessIP;					//SOCKET IP
	};

	//SOCKET 读取通知结构定义
	struct SocketReadLine
	{
		DataLineHead						LineHead;					//队列头
		NetMessageHead						NetMessageHead;				//数据包头
		UINT								uHandleSize;				//数据包处理大小
		UINT								uIndex;						//SOCKET 索引
		ULONG								uAccessIP;					//SOCKET IP
		SocketReadLine()
		{
			memset(this, 0, sizeof(SocketReadLine));
		}
	};

	///数据库结果消息结构定义
	struct DataBaseResultLine
	{
		DataLineHead						LineHead;					///队列头
		UINT								uHandleRusult;				///结果结果
		UINT								uHandleKind;				///处理类型
		UINT								uIndex;						///对象索引

		DataBaseResultLine()
		{
			memset(this, 0, sizeof(DataBaseResultLine));
		}
	};

	//定时器消息结构定义
	struct TimerLine
	{
		DataLineHead						LineHead;					//队列头
		UINT								uTimerID;					//定时器 ID

		TimerLine()
		{
			memset(this, 0, sizeof(TimerLine));
		}
	};
	/*****************************************************************************************************************/
}

#endif //_WJ_MAINMANAGE_H_