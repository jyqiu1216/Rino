#include "EmbedWindowHS.h"

#include "DlgWebView.h"
#include "DlgLoading.h"
#include "DlgTouchWebView.h"
#include "DlgBetView.h"
//#include "WSMsgHandlerHS.h"
#include "hook/WSMessageId.h"
#include "common/WJCommonMarco.h"
#include "common/WJCommonMarco.h"
#include "common/clientconfig.h"
#include "automation/WJAutomation.h"
#ifdef _DEBUG
#pragma comment(lib, "WJAutomation_d.lib")
#else
#pragma comment(lib, "WJAutomation.lib")
#endif // _DEBUG

#include <ShObjIdl.h>
#include <QPushButton>


void hideTaskIcon(HWND _hWnd)
{
	/* 需要初始化CoInitialize */
	//::CoInitialize(NULL);

	DWORD dwExStyle = GetWindowLong(_hWnd, GWL_EXSTYLE);
	dwExStyle |= WS_EX_TOOLWINDOW;
	SetWindowLong(_hWnd, GWL_EXSTYLE, dwExStyle);

	ITaskbarList* pTaskbarList;
	HRESULT hRes = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList, (void**)&pTaskbarList);
	if (SUCCEEDED(hRes))
	{
		pTaskbarList->HrInit();
		pTaskbarList->DeleteTab(_hWnd);
		pTaskbarList->Release();
	}
}



EmbedWindowHS::EmbedWindowHS(HWND _hWndParent, HWND _hWndTell, const QRect& _rect)
: m_dlgMatch(nullptr)
, m_dlgLoading(nullptr)
, m_webSocketSrv(nullptr)
, m_sockClient(nullptr)
, m_hWndParent(_hWndParent)
, m_anim(nullptr)
{
	//QWidget *widget = new QWidget;
	//widget->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint/* | Qt::WindowMinMaxButtonsHint*/);
	//widget->setGeometry(0, 0, 500, 500);
	//widget->show();
	//::SetParent((HWND)widget->winId(), m_hWndParent);

	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | Qt::Dialog);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->setGeometry(0, 0, 0, 0);

	m_pChildWind = new DlgTouchWebView(this);
	m_pBetView = new CDlgBetView(this);

	m_dlgMatch = new DlgWebView(this, (HWND)m_pChildWind->winId());
	m_dlgMatch->setWindowSize(320, 500);
	m_dlgMatch->setHSHWin(m_hWndParent);

	m_dlgLoading = new DlgLoading(this);

	RECT rtParent;
	::GetWindowRect(m_hWndParent, &rtParent);
	m_dlgLoading->resize(rtParent.right - rtParent.left - 20, rtParent.bottom - rtParent.top - 42);
	m_dlgLoading->move(rtParent.left + 10, rtParent.top + 32);
	m_dlgLoading->setHSWin(_hWndParent);

	m_pBetView->resize(240, 465);
	m_pBetView->move(rtParent.right - 260, rtParent.bottom - 520);
	m_pBetView->setHwnd_Operate((HWND)m_dlgMatch->winId());
	m_pBetView->setHwnd_Tell(_hWndTell);
	m_pBetView->setWind_Init_Station(GetWindowLong((HWND)m_dlgMatch->winId(), GWL_EXSTYLE));

	
	m_pChildWind->resize(120, 200);
	m_pChildWind->setHwnd_Operate((HWND)m_dlgMatch->winId());
	m_pChildWind->setHwnd_Tell(_hWndTell);
	m_pChildWind->setWind_Init_Station(GetWindowLong((HWND)m_dlgMatch->winId(), GWL_EXSTYLE));
	m_pChildWind->setWindowOpacity(0.01);
	hideTaskIcon((HWND)m_dlgMatch->winId());
	hideTaskIcon((HWND)m_dlgLoading->winId());
	hideTaskIcon((HWND)m_pBetView->winId());

	startTimer(1);
	::SetParent((HWND)this->winId(), m_hWndParent);
}

EmbedWindowHS::~EmbedWindowHS()
{
	WJ_SAFE_DELETE(m_dlgMatch);
	WJ_SAFE_DELETE(m_dlgLoading);
	WJ_SAFE_DELETE(m_sockClient);
	WJ_SAFE_DELETE(m_pChildWind);
}


void EmbedWindowHS::initialize(HWND _hWndHS, QUrl& _url, WebSocketServer *pwebSocketSrv)
{
	//m_webView->setHSHwnd(m_hWndHS);
	/*m_webView->load(QUrl("F:/04-Project/05-WJ_GAME/SourceCode_Dev/run/Debug/Resource/H5/HSMatchesAtOnce.html"));
	m_webView->show();*/
	WJ_SAFE_DELETE(m_sockClient);

	m_webSocketSrv = pwebSocketSrv;

	m_dlgMatch->load(QUrl(_url));
	connect(m_dlgMatch, SIGNAL(loadFinished(bool)), this, SLOT(onLoadUrlFinished(bool)));

	//std::string path = QCoreApplication::applicationDirPath().toStdString();
	std::string str_url = clientconfig().GetData()->h5_server_address + "resource/HS_loading.html";
	m_dlgLoading->load(QUrl(str_url.c_str()));
	std::string str_url2 = clientconfig().GetData()->h5_server_address + "resource/HS_stake.html";
	m_pBetView->load(QUrl(str_url2.c_str()));
}

void EmbedWindowHS::onLoadUrlFinished(bool _status)
{
	m_dlgMatch->show();
	m_pChildWind->show();
	m_pBetView->hide();
}

void EmbedWindowHS::timerEvent(QTimerEvent *_event)
{
	HWND hWnd = ::FindWindowA(NULL, "炉石传说");
	if (!hWnd)
	{
		if (m_dlgLoading)
			m_dlgLoading->close();

		if (m_dlgMatch)
			m_dlgMatch->close();

		if (m_pChildWind)
			m_pChildWind->close();

		if (m_pBetView)
			m_pBetView->close();

		this->close();
		return;
	}

	/**
	* \brief:	获取父窗口，根据父窗口位置设置内嵌窗口的位置
	*/
	RECT rtParent;
	if (::GetWindowRect(m_hWndParent, &rtParent))
	{
		int w = rtParent.right - rtParent.left;
		int h = rtParent.bottom - rtParent.top;

		if (m_dlgMatch)
		{
			m_dlgMatch->resize(w*0.25, h*0.6);
			m_dlgMatch->move(rtParent.right - m_dlgMatch->width()*1.02, rtParent.top + m_dlgMatch->height()*0.2);
		}
		if (m_dlgLoading)
		{
			m_dlgLoading->resize(w - 16, h - 40);
			m_dlgLoading->move(rtParent.left + 8, rtParent.top + 32);
		}

		if (m_pChildWind)
		{
			m_pChildWind->resize(m_dlgMatch->width()*0.37, m_dlgMatch->height()*0.46);
			m_pChildWind->move(rtParent.right - m_dlgMatch->width()*0.4, m_dlgMatch->pos().y());
		}

		if (m_pBetView) {

			m_pBetView->resize(w*0.25, h*0.6);
			m_pBetView->move(rtParent.right - m_pBetView->width()*1.01, rtParent.bottom - m_pBetView->height()*1.07);
		}
	}

	int eventId = WJAM::takeEvent();
	if (eventId > 0)
	{
		printf("炉石内嵌窗口接收到自动化模块事件, id[%d].\n", eventId);

		switch (eventId)
		{
		case GAME_EVENT_AM_BEGIN:
		case GAME_EVENT_AM_ADD_FRIEND:
		case GAME_EVENT_AM_ACCEPT_FRIEND:
		{
			QJsonObject sendjsonpart;
			sendjsonpart.insert(QStringLiteral("MSGID"), eventId);
			QJsonDocument documentpack;
			documentpack.setObject(sendjsonpart);
			QString json_json(documentpack.toJson(QJsonDocument::Compact));
			m_webSocketSrv->sendMessage(json_json, web_c_ls_inside_loading);
		}
			
			this->beginLoading();
			break;

		case  GAME_EVENT_AM_END:
			this->endLoading();
			break;

		case  GAME_EVENT_AM_ERROR:
			this->endLoading();
			break;

		case GAME_EVENT_GAME_BEGIN:
			this->startGame();
			break;

		case GAME_EVENT_GAME_END:
			this->endGame();
			break;

		case GAME_EVENT_AM_REQ_CHALLENGE:
		case GAME_EVENT_AM_ACCEPT_CHALLENGE:
		{
			QJsonObject sendjsonpart;
			sendjsonpart.insert(QStringLiteral("MSGID"), eventId);
			QJsonDocument documentpack;
			documentpack.setObject(sendjsonpart);
			QString json_json(documentpack.toJson(QJsonDocument::Compact));
			m_webSocketSrv->sendMessage(json_json, web_c_ls_inside_loading);
		}
		default:
			break;
		}
	}
}

#include <QGraphicsOpacityEffect>
void EmbedWindowHS::beginLoading()
{
	m_dlgMatch->hide();		/* 隐藏匹配页 */
	m_pChildWind->hide();
	m_pBetView->hide();
	m_dlgLoading->show(); 	/* 显示遮罩 */
}

void EmbedWindowHS::endLoading()
{
	m_dlgLoading->hide();	/* 隐藏遮罩加载页 */
	m_pBetView->hide();
	m_dlgMatch->show();		/* 显示匹配页 */
	m_pChildWind->show();
}

void EmbedWindowHS::startGame()
{
	m_dlgLoading->hide();	/* 隐藏遮罩 */
	m_dlgMatch->hide();		/* 隐藏匹配页, 在游戏结束时显示 */

	//押注显示
	m_pBetView->show();
	//30秒后关闭押注页
	m_pBetView->StartTime();
}

void EmbedWindowHS::endGame()
{
	m_dlgMatch->show();
	m_pChildWind->show();

	m_pBetView->KillTime();
}

void EmbedWindowHS::showErrorPage()
{
	m_dlgLoading->hide();	/* 隐藏遮罩 */
	m_dlgMatch->show();	
	m_pChildWind->show();
}

void EmbedWindowHS::onAnimateFinished()
{
	delete m_anim;
	m_anim = nullptr;
}

//设置窗口鼠标穿透(1 穿透 0 不穿透)
void EmbedWindowHS::setWindMousePenetrate(bool bPenetrate)
{ 
	m_pChildWind->setWindMousePenetrate(bPenetrate); 
}
