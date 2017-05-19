

#include <SDKDDKVer.h>
#include <Windows.h>

extern HMODULE g_hModule;

#include "MainWindow.h"
#include "hook/HookEventParam.h"
#include "common/timemanager.h"





#include "WindowMsgHandlerHS.h"


#define WH_SHARE_MEMORY_HS		TEXT("WH_SHARE_MEMORY_HS")
#define SHARE_MEM_HS_SERVER		TEXT("SHARE_MEM_HS_SERVER")	
#define SHARE_MEM_HS_CLIENT		TEXT("SHARE_MEM_HS_CLIENT")	

//#include "hook/timemanager.h"


class WSMsgHandlerHS : public WSMsgHandler
{
public:
	WSMsgHandlerHS(MainWindow* _mainWin)
		: m_mainWin(_mainWin)
		, m_sockClient(NULL)
	{
	}

	virtual void onRecvMsg(const QString &_msg)
	{
		QWebSocket *pSocket = qobject_cast<QWebSocket *>(sender());

		// {"MSGID":"2000"}

		char buf[128] = { 0 };
		sprintf_s(buf, "ExtendHS [WSMsgHandlerHS]: 收到 Web Socket 消息: %s", _msg.toStdString().c_str());
		OutputDebugStringA("buf");

		QJsonParseError jsonError;
		QJsonDocument jsonDoc = QJsonDocument::fromJson(_msg.toLocal8Bit(), &jsonError);
		if (jsonError.error != QJsonParseError::NoError)
		{
			OutputDebugStringA("ExterndHS [WSMsgHandlerHS::onRecvMsg]: JSON parser error.");
			return;
		}

		if (!jsonDoc.isObject())
		{
			OutputDebugStringA("ExterndHS [WSMsgHandlerHS::onRecvMsg]: JSON data error: is not json object.");
			return;
		}

		QJsonObject obj = jsonDoc.object();
		if (!obj.contains("MSGID"))
		{
			OutputDebugStringA("ExterndHS [WSMsgHandlerHS::onRecvMsg]: JSON data error: [MSGID] is not exist.");
			return;
		}

		int msgId = obj["MSGID"].toInt();

		switch (msgId)
		{
		case 2005:
			m_mainWin->beginAutomation();
			break;

		case 2006:
			m_mainWin->endAutomation();
			break;

		default:
			break;
		}
	}

	virtual void callbackConn(QWebSocket *_socket, bool _isSrv)
	{
		//m_mainWin->setWebSockClient(_socket);
		m_sockClient = _socket;
	}

	virtual void callbackDisConn(QWebSocket *_socket, bool _isSrv)
	{
		m_sockClient = nullptr;
	}

	/**
	*	消息格式：{"MSGID":"2000"}
	*/
	void sendMsg(QString& _msg, QWebSocket* _websocket = nullptr)
	{
		QJsonObject jsonRoot;
		jsonRoot.insert("MSGID", _msg);

		QJsonDocument jsonDoc;
		jsonDoc.setObject(jsonRoot);

		QString msg = jsonDoc.toJson(QJsonDocument::Compact);

		if (_websocket)
		{
			_websocket->sendTextMessage(msg);
		}
		else if (m_sockClient)
		{
			m_sockClient->sendTextMessage(msg);
		}

		std::string str = "ExtendHS [WSMsgHandlerHS::sendMsg]: ";
		str += msg.toStdString();
		OutputDebugStringA(str.c_str());
	}

public:
	MainWindow *m_mainWin;
	QWebSocket *m_sockClient;
};


#define AM_SHARE_MEM_HS_SERVER		TEXT("AM_SHARE_MEM_HS_SERVER")	
#define AM_SHARE_MEM_HS_CLIENT		TEXT("AM_SHARE_MEM_HS_CLIENT")	


#include "MyDialog.h"

#include "automation/AMUtil.h"

MainWindow::MainWindow(HWND _hWndParent)
	: m_hWndParent(_hWndParent)
	, m_webSocketSrv(NULL)
	, m_sockClient(NULL)
	, m_webView(NULL)
	, m_isHooked(false)
	, m_isExitFlag(false)
	, m_eventId(-1)
{
	/* 设置窗口的风格：无边框，背景透明 */
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	/* 这个窗口作为WEB VIEW的父载体（由于该窗口不能实现透明效果，所以将其大小设置为0,0,即不可见） */
	this->resize(0, 0);
	this->move(0, 0);


	if (m_hWndParent)
	{
		CShareMemory* shareMem = new CShareMemory;
		shareMem->OpenShareFile(WH_SHARE_MEMORY_HS);
		shareMem->ReadFromShareMemory(sizeof(m_shareParam), &m_shareParam);

		/* 初始化匹配窗体 */
		m_webView = new MyWebView(this);
		m_webView->setHSHwnd(m_hWndParent);
		m_webView->load(QUrl(m_shareParam.urlMatch));
		connect(m_webView, SIGNAL(loadFinished(bool)), this, SLOT(onLoadUrlFinished(bool)));

		/* 初始化游戏加载遮罩窗体 */
		m_dlgCover = new MyDialog(this);
		m_dlgCover->setHSWin(m_hWndParent);
		m_dlgCover->load(QUrl(m_shareParam.urlCover));

		/* 设置窗体位置 */
		this->moveMyWindow();
	}

	/* 显示窗口 */
	this->show();

	/* 将其设置为炉石窗口的子窗口 */
	::SetParent((HWND)this->winId(), m_hWndParent);


	/* 初始化WEB SOCKET通讯 */
	m_WSMsgHandler = new WSMsgHandlerHS(this);
	m_webSocketSrv = new WebSocketServer(m_shareParam.wsPort_Hook, m_WSMsgHandler, NULL);


	/* 初始化D3D HOOK */
	initD3D();
	AMUtil::outputDebugStr("Init D3D END.");


	/* 开启定时器 */
	startTimer(1);

	/* 心跳定时器 */
	QTimer* timer = new QTimer;
	connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	timer->start(3000);


	/* 初始化共享内存处理线程 */
	if (this->initEventSignal(AM_SHARE_MEM_HS_SERVER, AM_SHARE_MEM_HS_CLIENT, EVENT_EXIT_DEFAULT_NAME, this, false))
	{
		this->StartThread();
		OutputDebugStringA("ExtendHS : initEventSignal successful.");
		//m_captureInit = true;
	}
	else
	{
		OutputDebugStringA("ExtendHS : initEventSignal failed.");
	}
}

MainWindow::~MainWindow()
{
	//delete m_webView;
	//m_webView = nullptr;
	//m_captureInfo.type = CAPTURE_PROCESS_EXIT;
	//SendEvent(&m_captureInfo, sizeof(CaptureInfo));
	CallExit();
}


bool MainWindow::initD3D()
{
	if (!CHookD3D9::PrepareRealApiEntry())
	{
		OutputDebugStringA("ExtendHS [MainWindow::initD3D]: PrepareRealApiEntry error.");
		return false;
	}

	if (!CHookD3D9::ms_pHookD3D9->InitD3D9())
	{
		OutputDebugStringA("ExtendHS [MainWindow::initD3D]: InitD3D9 error.");
		return false;
	}

	OutputDebugStringA("ExtendHS [MainWindow::initD3D]: DoHook BEGIN.");
	CHookD3D9::DoHook();
	OutputDebugStringA("ExtendHS [MainWindow::initD3D]: DoHook END.");
	return true;
}


void MainWindow::moveMyWindow()
{
	RECT rtParent;
	::GetWindowRect(m_hWndParent, &rtParent);

	if (m_webView)
	{
		m_webView->setWindowSize(rtParent.right /*- rtParent.left*/ - 400 - 18, rtParent.top + 40, 400, 580/*rtParent.bottom - rtParent.top - 58 - 10 - 30*/);
		//m_webView->setWindowSize(60, 500, 420, 580/*rtParent.bottom - rtParent.top - 58 - 10 - 30*/);
	}

	if (m_dlgCover)
	{
		m_dlgCover->move(rtParent.left, rtParent.top + 30);
		m_dlgCover->resize(rtParent.right - rtParent.left, rtParent.bottom - rtParent.top - 30);
	}

}

void MainWindow::beginAutomation()
{
	m_dlgCover->show();	/* 隐藏匹配页 */
	m_webView->hide();	/* 显示遮罩 */

	/* 隐藏鼠标 */
	::ShowCursor(FALSE);

	m_dlgCover->repaint();
	m_dlgCover->update();
	::UpdateWindow(m_hWndParent);
	
}

void MainWindow::endAutomation()
{
	m_dlgCover->hide();	/* 隐藏遮罩 */
	m_webView->hide();	/* 隐藏匹配页, 在游戏结束时显示 */

	/* 显示鼠标鼠标 */
	::ShowCursor(TRUE);
	::UpdateWindow(m_hWndParent);
}

void MainWindow::onLoadUrlFinished(bool _status)
{
	m_webView->show();
	OutputDebugStringA("ExtendHS [MainWindow::onLoadUrlFinished]: WebEngine open successful.");
}


void MainWindow::onTimer()
{
	if (m_isHooked)
	{
		CaptureInfo& capInfo = CHookD3D9::getInstance()->m_captureInfo;

		static EventParamHS param;
		param.id = E_EVENT_ID_HS_HOOK_HEARTBEAT;
		param.data.captInfo.captureId = capInfo.mCaptureId;
		param.data.captInfo.format = capInfo.mFormat;
		param.data.captInfo.tick = capInfo.mHeartBeat;
		param.data.captInfo.height = capInfo.mHeight;
		param.data.captInfo.HUDLocation = capInfo.mHUDLocation;
		param.data.captInfo.isHooked = capInfo.mIsHOOKed;
		param.data.captInfo.offsetX = capInfo.mOffsetX;
		param.data.captInfo.offsetY = capInfo.mOffsetY;
		param.data.captInfo.sharedHandle = capInfo.mSharedHandle;
		param.data.captInfo.width = capInfo.mWidth;
		strcpy_s(param.data.captInfo.data, capInfo.pData);
		param.data.captInfo.processId = capInfo.processId;

		//SendEvent(&param, sizeof(EventParamHS));
		//OutputDebugStringA("HS HOOK ON TIMER!");
	}

	if (m_eventId > 0)
	{
		switch (m_eventId)
		{
		case 2001:
			OutputDebugStringA("ExtendHS [MainWindow::onTimer]: 处理 EventSignal 信号, 发送消息2001");
			m_WSMsgHandler->sendMsg(QString::number(m_eventId));
			break;

		case 2002:
			OutputDebugStringA("ExtendHS [MainWindow::onTimer]: 处理 EventSignal 信号, 发送消息2002");
			m_WSMsgHandler->sendMsg(QString::number(m_eventId));
			break;

		case 2003:
			OutputDebugStringA("ExtendHS [MainWindow::onTimer]: 处理 EventSignal 信号, 发送消息2003");
			m_WSMsgHandler->sendMsg(QString::number(m_eventId));
			break;

		case 2004:
			OutputDebugStringA("ExtendHS [MainWindow::onTimer]: 处理 EventSignal 信号, 发送消息2004");
			m_WSMsgHandler->sendMsg(QString::number(m_eventId));
			break;

		default:
			break;
		}

		m_eventId = 0;
	}

	//if (CHookD3D9::getInstance()->m_captureInit)
	//{
	//	OutputDebugStringA("onTimer");
	//	CHookD3D9::getInstance()->m_captureInfo.type = CAPTURE_HEARTBEAT;
	//	CHookD3D9::getInstance()->m_captureInfo.mHeartBeat = CTimeManager::getInstance()->getCurrentTime();

	//	//通知程序端进行接收数据
	//	SendEvent(&CHookD3D9::getInstance()->m_captureInfo, sizeof(CaptureInfo));
	//	//SendEvent(&g_eventParam, sizeof(g_eventParam));
	//}
}


void MainWindow::timerEvent(QTimerEvent *_event)
{
	if (m_webView)
		m_webView->repaint();

	/* 等待HOOK成功结束的系统事件 */
	if (0 == ::WaitForSingleObject(CHookD3D9::getInstance()->m_eventId_HookEnd, 10))
	{
		CaptureInfo& capInfo = CHookD3D9::getInstance()->m_captureInfo;

		char buf[128] = { 0 };
		sprintf_s(buf, "HOOK SUCCESSFUL:  Capture Info: W:%d, H:%d", capInfo.mWidth, capInfo.mHeight);
		OutputDebugStringA(buf);


		// 通知程序端进行接收数据，
		EventParamHS param;
		param.id = E_EVENT_ID_HS_HOOK_SUCCESS;
		param.data.captInfo.captureId = capInfo.mCaptureId;
		param.data.captInfo.format = capInfo.mFormat;
		param.data.captInfo.tick = capInfo.mHeartBeat;
		param.data.captInfo.height = capInfo.mHeight;
		param.data.captInfo.HUDLocation = capInfo.mHUDLocation;
		param.data.captInfo.isHooked = capInfo.mIsHOOKed;
		param.data.captInfo.offsetX = capInfo.mOffsetX;
		param.data.captInfo.offsetY = capInfo.mOffsetY;
		param.data.captInfo.sharedHandle = capInfo.mSharedHandle;
		param.data.captInfo.width = capInfo.mWidth;
		strcpy_s(param.data.captInfo.data, capInfo.pData);
		param.data.captInfo.processId = capInfo.processId;

		this->SendEvent(&param, sizeof(EventParamHS));
		OutputDebugStringA("ExtendHS : SendEvent E_EVENT_ID_HS_HOOK_SUCCESS");

		/* 保存HOOK状态 */
		m_isHooked = true;
	}

	//CHookD3D9::getInstance()->m_captureInfo.type = CAPTURE_HEARTBEAT;
	//CHookD3D9::getInstance()->m_captureInfo.mHeartBeat = CTimeManager::getInstance()->getCurrentTime();
	////通知程序端进行接收数据
	//this->SendEvent(&CHookD3D9::getInstance()->m_captureInfo, sizeof(CaptureInfo));




	BOOL isReadyShibie;
	//当前炉石的图片--
	//Mat imageMat;

	//MSG msg;
	//while (GetMessage(&msg, NULL, 0, 0))

	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}


	////if (CHookD3D9::getInstance()->m_pSharedSurface9)
	////{
	////	
	////
	////	CHookD3D9::getInstance()->m_captureInfo.type = CAPTURE_HOOK_SUCCESS;
	////	//通知程序端进行接收数据
	////	SendEvent(&CHookD3D9::getInstance()->m_captureInfo, sizeof(CaptureInfo));
	////	//g_eventParam.capInfo = CHookD3D9::getInstance()->m_captureInfo;
	////	//SendEvent(&g_eventParam, sizeof(g_eventParam));
	////}

	//if (0 == WaitForSingleObject(g_subThreadEvent, 10))
	//{
	//	//事件到来 
	//	isReadyShibie = TRUE;
	//	CHookD3D9::getInstance()->CreateShareTexture();
	//	//imageMat.create(desc.Width, desc.Height, CV_8UC4); 
	//	//cvCreateImage
	//}

	//if (isReadyShibie == TRUE)
	//{
	//}
}


//void MainWindow::onRecvMsg(const QString &_msg)
//{
//	QWebSocket *pSocket = qobject_cast<QWebSocket *>(sender());
//	qDebug() << "Recv MSG------:" << _msg;
//	//m_webSocketClient->sendMsg(_msg);
//
//	//EventParamHS param;
//	//param.id = E_EVENT_ID_HS_HOOK_AM_;
//
//	//this->SendEvent(&param, sizeof(EventParamHS));
//}


void MainWindow::DoData(void* pData, size_t size)
{
	int x = 0;
	memcpy(&x, pData, sizeof(int));
	m_eventId = x;
	AMUtil::outputDebugStr("ExtendHS [MainWindow::DoData]: 收到事件, 发送消息至H5 web socket", "", NULL);
}


void MainWindow::SendData(void* pData, size_t maxSize)
{
	//CaptureInfo &capInfo = CHookD3D9::getInstance()->m_captureInfo;
	//capInfo.mHeartBeat = CTimeManager::getInstance()->getCurrentTime();
	//capInfo.type = CAPTURE_HEARTBEAT;
	////写入心跳和相关处理，包括事件类型
	//memcpy(pData, &capInfo, sizeof(CaptureInfo));
}



