#ifndef __WJ_SocketThread_H__
#define __WJ_SocketThread_H__


#include <string>
#include <vector>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include "common/WJBaseType.h"
#include "common/WJCommonMarco.h"
#include "message/WJNetMessageHead.h"
#include "WJSocketEventDelegate.h"
#include "WJClientSocket_load.h"
#include "WJSocketMessage.h"
namespace WJ 
{
	/*********************************************************
	*
	*						类说明
	*
	*********************************************************/
	class WJCLIENTSOCKETLOGIC_EXPORT WJSocketThread : public IClientSocketService_load
	{
		typedef std::deque<WJSocketMessage*>		WJSocketMessageQueue;

	public:
		WJSocketThread(ISocketEventDelegate* delegate);
		virtual ~WJSocketThread(void);

	public:
		///网络读取消息
		virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket_load * pClientSocket);
		///网络连接消息
		virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket_load * pClientSocket);
		///网络关闭消息
		virtual bool OnSocketCloseEvent();

	public:
		//J_PROPERTY_PASS_BY_REF(std::string, Tag, tag)//使用该通信进程的标记值，方便排错

		/*
		* 打开服务端连接
		*/
		bool openWithIp(const char* ip, int port);

		/*
		* 打开服务端连接
		*/
		bool openWithHost(const char* host, int port);

		/*
		* 关闭服务端连接
		*/
		bool close();
		
		/*
		*是否已连接
		*/
		bool connected();
		
		BYTE GetSocketState();
		// 发送数据
		int sendData(void * pData, UINT uSize, UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		// 发送数据
		int sendData(UINT bMainID, UINT bAssistantID, UINT bHandleCode);

		/*
		*转换器，对收集到的套接字底层状态消息进行响应
		*/
		void transform_socket(std::function<void(WJSocketMessage* socketMessage)> func);
		//WJSocket* getWJSocket(){ return &_socket; }
		CTCPClientSocket_load* getWJSocket(){ return m_pCTCPClientSocket; }

	private:

		//WJSocket/***/					_socket;//套接字
		CTCPClientSocket_load*				m_pCTCPClientSocket;//套接字
		std::mutex						_dataMutex;//互斥量，方便线程对互斥量上锁
		WJSocketMessageQueue/***/		_socketMessageQueue;//套接字底层状态消息队列

		ISocketEventDelegate*			_delegate;
	};

};

#endif	//__WJ_SocketThread_H__
