#pragma once


#include "hook/HookD3D9.h"
#include "WebSocket/WebSocketServer.h"


#include "MyWebView.h"
#include "MyDialog.h"
#include "ui_mainwindow.h"
#include "hook/HookEventParam.h"

class WSMsgHandlerHS;


class MainWindow : public QDialog, public IDataRecive, public EventSignal, public QAbstractNativeEventFilter/* : public QMainWindow*/ /**/
{
	Q_OBJECT

public:
	MainWindow(HWND _hWndParent);
	~MainWindow();

public:
	/**
	* \brief:	移动注入窗口（）
	*/
	void moveMyWindow();

	void beginAutomation();
	void endAutomation();

	/* 系统事件拦截处理函数 */
public:
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *) Q_DECL_OVERRIDE
	{
		if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
		{
			MSG* pMsg = reinterpret_cast<MSG*>(message);
			switch (pMsg->message)
			{
			case WM_MOVE + WM_USER:
				this->moveMyWindow();
				break;

			case WM_USER + 0x0100:
				this->beginAutomation();
				break;

			case WM_USER + 0x0101:
				this->endAutomation();
				break;

			default:
				break;
			}
		}
		return false;
	}


	MyDialog* getCoverWin()
	{
		return m_dlgCover;
	}

	void setWebSockClient(QWebSocket* _ws)
	{
		m_sockClient = _ws;
	}

public:
	virtual void DoData(void* pData, size_t size);
	virtual void SendData(void* pData, size_t maxSize);

	virtual void timerEvent(QTimerEvent *_event);

	void end() { m_isExitFlag = true; };



private:
	/**
	* \brief:	初始化d3d
	*/
	bool initD3D();


	public slots:
	void onLoadUrlFinished(bool);
	void onTimer();


public:
	
	/**
	* \brief:	webSocket 服务
	*/
	WebSocketServer *m_webSocketSrv;

	/**
	* \brief:	webSocket 服务端回调处理类
	*/
	WSMsgHandlerHS *m_WSMsgHandler;

	/**
	* \brief:	连接成功的客户端通讯 webSocket
	*/
	QWebSocket *m_sockClient;


	/**
	* \brief:	共享内存传递参数
	*/
	ShareParamHS m_shareParam;

	MyDialog *m_dlgCover;
	MyWebView *m_webView;
	//QWebEngineView* m_webView;

	/**
	* \brief:	父窗体（炉石）的窗口句柄
	*/
	HWND m_hWndParent;


	QPoint m_ptTick;

	bool m_isHooked;

	/**
	* \brief:	关闭标识
	*/
	bool m_isExitFlag;

	int m_eventId;
};
