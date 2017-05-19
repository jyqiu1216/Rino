#include "cbetpopup.h"
#include "DlgWebView_00000002.h"
#include "WSMsgHeaderBetPopup.h"
#include "common/WJCommonMarco.h"
#include "common/clientconfig.h"
#include <ShObjIdl.h>
#include <QPushButton>

void hideTaskIcon(HWND _hWnd)
{
	/* ÐèÒª³õÊ¼»¯CoInitialize */
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


CBetPopup::CBetPopup(HWND _hWnd, HWND _hWndTell, const QRect& _rect)
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

	m_dlgMatch = new DlgWebView_00000002(this);
	m_dlgMatch->setWindowSize(361, 264);
	m_dlgMatch->setHSHWin(m_hWndParent);
	m_dlgMatch->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Dialog);
	m_dlgMatch->setAttribute(Qt::WA_TranslucentBackground, true);
	
	hideTaskIcon((HWND)m_dlgMatch->winId());
	::SetParent((HWND)this->winId(), m_hWndParent);
}

CBetPopup::~CBetPopup()
{

}

#include <fstream>
using namespace std;
void CBetPopup::initialize(HWND _hWndHS, QUrl& _url, int _wsPort)
{
	WJ_SAFE_DELETE(m_webSocketSrv);
	WJ_SAFE_DELETE(m_sockClient);

	m_WSMsgHandler = new WSMsgHandleBetPopup(this);
	m_webSocketSrv = new WebSocketServer(_wsPort, m_WSMsgHandler);

	//std::string path = QCoreApplication::applicationDirPath().toStdString();
	std::string str_url = clientconfig().GetData()->h5_server_address + "resource/LOL_success.html";
	m_dlgMatch->load(QUrl(str_url.c_str()));
	connect(m_dlgMatch, SIGNAL(loadFinished(bool)), this, SLOT(onLoadUrlFinished(bool)));

	RECT rtParent;
	::GetWindowRect(m_hWndParent, &rtParent);

	int inParentWidth = rtParent.right - rtParent.left;
	int inParentHigth = rtParent.bottom - rtParent.top;

	RECT rtMyWnd;
	::GetWindowRect((HWND)m_dlgMatch->winId(), &rtMyWnd);

	int inMyWidth = rtMyWnd.right - rtMyWnd.left;
	int inMyHigth = rtMyWnd.bottom - rtMyWnd.top;

	int inXPos = rtParent.right - inMyWidth;
	int inYPos = rtParent.bottom - inMyHigth - (::GetSystemMetrics(SM_CYSCREEN) - ::GetSystemMetrics(SM_CYFULLSCREEN));

	::SetWindowPos((HWND)m_dlgMatch->winId(), HWND_TOPMOST, inXPos, inYPos, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
	::SetForegroundWindow((HWND)m_dlgMatch->winId());

	startTimer(60 * 1000);
}

#include <QGraphicsOpacityEffect>
void CBetPopup::beginLoading()
{
	m_dlgMatch->hide();		/* Òþ²ØÆ¥ÅäÒ³ */
}

void CBetPopup::endLoading()
{
	m_dlgMatch->show();		/* ÏÔÊ¾Æ¥ÅäÒ³ */
}

void CBetPopup::startGame()
{
	m_dlgMatch->hide();		/* Òþ²ØÆ¥ÅäÒ³, ÔÚÓÎÏ·½áÊøÊ±ÏÔÊ¾ */
}

void CBetPopup::showErrorPage()
{
	m_dlgMatch->show();
}

void CBetPopup::endGame()
{
	m_dlgMatch->show();
}

void CBetPopup::timerEvent(QTimerEvent *)
{
	if (NULL != m_dlgMatch) {

		m_dlgMatch->close();
	}
}

void CBetPopup::onLoadUrlFinished(bool _status)
{
	m_dlgMatch->show();
}

void CBetPopup::onAnimateFinished()
{
	delete m_anim;
	m_anim = nullptr;
}

void CBetPopup::setWindMousePenetrate(bool bPenetrate)
{

}
