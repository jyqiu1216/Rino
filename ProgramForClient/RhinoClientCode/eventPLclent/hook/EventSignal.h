// MemeryShare.h

#ifndef _EVENTSIGNAL_eventsignal_H_
#define _EVENTSIGNAL_eventsignal_H_
#include <Windows.h>
#include "macrodefinition.h"
#include <vector>
#include "ShareMemory.h"
#define MEMERY_DEFAULT_SIZE		(0xA00000)

#define EVENT_SERVER_DEFAULT_NAME	TEXT("ZSEvent_Signal_Server_Name")
#define EVENT_CLIENT_DEFAULT_NAME	TEXT("ZSEvent_Signal_Client_Name")
#define EVENT_EXIT_DEFAULT_NAME		TEXT("ZSEvent_Signal_Exit_Name")

#define WANGJING_PLATFORM_SERVER_SHARE_MEMORY_NAME		TEXT("WANGJING_PLATFORM_SERVER_SHARE_MEMORY_NAME")
#define WANGJING_PLATFORM_CLIENT_SHARE_MEMORY_NAME		TEXT("WANGJING_PLATFORM_CLIENT_SHARE_MEMORY_NAME")

#define EVENT_EXIT_INDEX 0
#define EVENT_SERVER_INDEX 1
#define EVENT_CLIENT_INDEX 2

#include "common/XLog.h"

class EventSignal {
public:
	EventSignal();
	~EventSignal();

public:
	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   initEventSignal   初始化信号管理类
	* @version: V1.0.0.1
	* @param:	LPCTSTR serverEventName	服务端名称
	* @param:	LPCTSTR clientEventName	客户端名称
	* @param:	LPCTSTR exitEventName	退出事件，该事件用去其他调用位置关闭本服务端/客户端的问题
	* @param:	IDataRecive* pDataRecive 事件接收处理函数
	* @param:	bool isServer = true	是否为服务端，默认为服务端
	* @return:  BOOL
	**/

	BOOL initEventSignal(LPCTSTR serverEventName, LPCTSTR clientEventName, LPCTSTR exitEventName,
		IDataRecive* pDataRecive, bool isServer = true);
	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   Start   创建线程，开启监听
	* @version: V1.0.0.1
 	* @return:  BOOL
	**/
	BOOL StartThread();

	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   SendEvent   发送通知事件，同时发送需要发送的数据与大小
	* @version: V1.0.0.1
	* @return:  BOOL
	**/
	BOOL SendEvent(LPVOID pData, size_t size);


	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   CallExit   退出函数
	* @version: V1.0.0.1
	* @return:  BOOL
	**/

	BOOL CallExit();

	void SetAutoSendMode(bool _flag) { m_isAutoSend = _flag; };

	XLog& getLog() { return m_log; };

	static UINT __stdcall  mainLoop(void* r);
protected:

	//事件数据
	std::vector<HANDLE> m_eventArray; 
	//线程 句柄
	HANDLE m_threadHandle = NULL;

	//主要用于客户端向服务端发送的共享区域
	CShareMemory* m_pServerSM;
	//主要用于服务端向客户端发送数据的共享区域
	CShareMemory* m_pClientSM;
	//是否为服务端？
	bool m_isServer;

	bool m_isAutoSend;

	XLog m_log;
};



#endif  /*_EVENTSIGNAL_eventsignal_H_*/
