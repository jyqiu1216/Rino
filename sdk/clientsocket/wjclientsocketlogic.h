#ifndef WJCLIENTSOCKETLOGIC_H
#define WJCLIENTSOCKETLOGIC_H

#include <list>

#ifdef _UNUSE_QT
#else
#include <QtCore>
#endif

#include "WJSocketMessageDelegate.h"
#include "WJSocketEventDelegate.h"
#include "WJSocketSelector.h"
#include "WJSocketThread.h"
#include "WJSocketMessage.h"

namespace WJ
{
#ifdef _UNUSE_QT
	class WJCLIENTSOCKETLOGIC_EXPORT WJClientSocketLogic : public ISocketEventDelegate
#else
	class WJCLIENTSOCKETLOGIC_EXPORT WJClientSocketLogic : public QObject, ISocketEventDelegate
#endif
	{
	public:
		WJClientSocketLogic(ISocketMessageDelegate* delegate, const std::string& tag);
		virtual ~WJClientSocketLogic();

		typedef WJSocketSelector<std::string> WJSocketMessageSelectorHandler;

		/*
		* 套接字跟踪已经暂时完成，计时器恢复
		*/
		//virtual void onReadComplete() override;
	public:
		//所需的初始化
		bool init();
		//是否连接成功
		bool connected();

	public:
		BYTE GetSocketState();

		//连接服务器
		bool openWithIp(const CHAR* ip, INT port);

		//连接服务器
		bool openWithHost(const CHAR* host, INT port);

		//关闭套接字
		bool close();

		//发送数据
		INT send(UINT MainID, UINT AssistantID, void* object, INT objectSize);
		//发送数据
		INT send(UINT MainID, UINT AssistantID, UINT uHandleCode, void* object, INT objectSize);
		//增加消息回调的函数绑定
		void addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector);

		//移除消息回调的函数绑定
		void removeEventSelector(UINT MainID, UINT AssistantID);

		//发送心跳消息
		void heartBeat();

		//重置数据
		void resetData();

		int gethWindow();
		void sethWindow(int hWindow);
		//计时器
		void OnTimeMesage(int itimeid);
	private:
		///WINDOW 消息循环线程
		void WindowMsgThread();

		///设定定时器
		bool SetTimer(UINT uTimerID, UINT uElapse);
		///清除定时器
		bool KillTimer(UINT uTimerID);

		//主动发送心跳包
		void socketHeartBeatSend();

#ifdef _UNUSE_QT

#else
		Q_OBJECT

	private slots:
#endif
		/*
		* 网络消息通知处理函数
		*/
		void socketDataDispatch();
	private:
		WJSocketThread*					_socketThread;//套接字线程

		WJSocketMessageSelectorHandler*	_selectorQueue;//消息回调的函数绑定队列

		ISocketMessageDelegate*			_delegate;//上层网络操作类接口父类

		//是否可以处理消息队列
		//bool _bReadComplete;

		int		m_hWindow;					///窗口句柄

	};
}


#endif // WJCLIENTSOCKETLOGIC_H
