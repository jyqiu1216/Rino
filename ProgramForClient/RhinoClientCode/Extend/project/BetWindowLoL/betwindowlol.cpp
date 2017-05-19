#include "betwindowlol.h"
#include "DlgWebView_00000001.h"
#include "DlgLoading_00000001.h"
#include "DlgTouchWebView_00000001.h"
#include "WSMsgHeaderLOL.h"
#include "common/WJCommonMarco.h"
#include "common/clientconfig.h"
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


BetWindowLoL::BetWindowLoL(HWND _hWnd, HWND _hWndTell, const QRect& _rect)
	: m_dlgMatch(nullptr)
	, m_WSMsgHandler(nullptr)
	, m_webSocketSrv(nullptr)
	, m_sockClient(nullptr)
	, m_hWndParent(_hWnd)
	, m_anim(nullptr)
{
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | Qt::Dialog);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->setGeometry(0, 0, 0, 0);

	m_dlgMatch = new DlgWebView(this);
	m_dlgMatch->setWindowSize(280, 700);
	m_dlgMatch->setHSHWin(m_hWndParent);
	m_dlgMatch->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Dialog);
	m_dlgMatch->setAttribute(Qt::WA_TranslucentBackground, true);

	RECT rtParent;

	while (true) 
	{

		::ZeroMemory(&rtParent, sizeof(RECT));
		::GetWindowRect(m_hWndParent, &rtParent);

		int inWidth = rtParent.right - rtParent.left;
		int inHight = rtParent.bottom - rtParent.top;

		//LOGO界面过去了，往下走
		if ((inWidth > 100) && (inHight > 100)) {

			break;
		}

		::Sleep(1000);
	}

	hideTaskIcon((HWND)m_dlgMatch->winId());

	startTimer(1);
	::SetParent((HWND)this->winId(), m_hWndParent);
}

BetWindowLoL::~BetWindowLoL()
{

}

void BetWindowLoL::initialize(HWND _hWndHS, QUrl& _url, int _wsPort)
{
	WJ_SAFE_DELETE(m_webSocketSrv);
	WJ_SAFE_DELETE(m_sockClient);

	m_WSMsgHandler = new WSMsgHandleLOL(this);
	m_webSocketSrv = new WebSocketServer(_wsPort, m_WSMsgHandler);

	//std::string path = QCoreApplication::applicationDirPath().toStdString();
	std::string str_url = clientconfig().GetData()->h5_server_address + "resource/LOL_stake.html";
	m_dlgMatch->load(QUrl(str_url.c_str()));
	connect(m_dlgMatch, SIGNAL(loadFinished(bool)), this, SLOT(onLoadUrlFinished(bool)));

	::SetWindowPos((HWND)m_dlgMatch->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	::SetForegroundWindow((HWND)m_dlgMatch->winId());
}

#include <QGraphicsOpacityEffect>
void BetWindowLoL::beginLoading()
{
	m_dlgMatch->hide();		/* 隐藏匹配页 */
}

void BetWindowLoL::endLoading()
{
	setWindowFlags(Qt::WindowStaysOnTopHint);
	m_dlgMatch->show();		/* 显示匹配页 */

}

void BetWindowLoL::startGame()
{
	m_dlgMatch->hide();		/* 隐藏匹配页, 在游戏结束时显示 */
}

void BetWindowLoL::showErrorPage()
{
	m_dlgMatch->show();
}

void BetWindowLoL::endGame()
{
	m_dlgMatch->show();
}

void BetWindowLoL::timerEvent(QTimerEvent *)
{
	//HWND hWnd = ::FindWindowA(NULL, "League of Legends (TM) Client");
	HWND hWnd = ::FindWindow(NULL, TEXT("PVP.net 客户端"));

	if (!hWnd) 
	{

		if (m_dlgMatch)
			m_dlgMatch->close();

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
			m_dlgMatch->resize(w*0.28, h*0.75);
			m_dlgMatch->move(rtParent.right, rtParent.top + m_dlgMatch->height()*0.2);
		}
	}
}

void BetWindowLoL::onLoadUrlFinished(bool _status)
{
	m_dlgMatch->show();
}

void BetWindowLoL::onAnimateFinished()
{
	delete m_anim;
	m_anim = nullptr;
}

void BetWindowLoL::setWindMousePenetrate(bool bPenetrate)
{
	//m_dlgMatch->setWindMousePenetrate(bPenetrate);
}
