
#include "WJMainWindow.h"
#include <QMessageBox>
#include <QPainter>
#include "WebSocketMessInfo.h"
#include "resource.h"
#include <QtWin>
#include <tlhelp32.h>
#include "common/LoadWindowsMess.h"
#include "../websocket/DataDrocessing.h"
#include "clientsocket/commonfunction_c.h"
#include "common/WJGameInfo.h"

/* 炉石传说注入窗口 DLL */
#include "hook/EmbedWindowHS.h"
#ifdef _DEBUG
#pragma comment(lib, "EmbedWindowHS_d.lib")
#pragma comment(lib, "BetWindowLoL_d.lib")
#pragma comment(lib, "CBetPopup_d.lib")
#else
#pragma comment(lib, "EmbedWindowHS.lib")
#pragma comment(lib, "BetWindowLoL.lib")
#pragma comment(lib, "CBetPopup.lib")
#endif // _DEBUG

CWJMainWindow* g_pWJMainWindow = nullptr;

CWJMainWindow::CWJMainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ZSQtMouseDrag(this)
	, ui(new Ui::WJMainWindow)
	, press(false)
{
	box = new QMbox;

	ui->setupUi(this);
	m_pWXView = nullptr;
	m_pGameFinishView = nullptr;
	m_pRegistrationView = nullptr;
	setWindowOpacity(1);
	m_pEmbedWindowHS = nullptr;
	m_pBetWindowLoL = nullptr;
	m_BetIdLoL = 0;
	m_BetIdHs = 0;
	m_LolSoloId = 0;
	m_pBetPopup = nullptr;
	m_pLoginView = nullptr;
	m_pGameTaskLOL = nullptr;


	Qt::WindowFlags flag = (Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	setWindowFlags(flag);
	setAttribute(Qt::WA_TranslucentBackground, true);
	resize(1280, 720);
	//resize(QApplication::desktop()->availableGeometry().size());//全屏窗口
	//H5主窗口初始化
	m_pMainView = new WJWebView(this);
	m_pMainView->page()->setBackgroundColor(QColor(0, 0, 0, 255));
	m_pMainView->setParent(this);
	m_pMainView->setAttribute(Qt::WA_DeleteOnClose);
	m_pMainView->show();
	//载入大厅页
	std::string str_url = clientconfig().GetData()->h5_server_address + "index.html";
	openUrl(QUrl(QString(str_url.c_str())));

	QObject::connect(this, SIGNAL(signa_linkland(bool)), ClientLink(), SLOT(slot_landlink(bool)));
	//设置图标
	HICON hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
	QPixmap image = QtWin::fromHICON(hIcon);
	QIcon icon = QIcon(image);
	setWindowIcon(icon);

	//设置系统托盘图标
	m_ptrayIcon = new QSystemTrayIcon(this);
	m_ptrayIcon->setIcon(icon);
	m_ptrayIcon->setToolTip(QString::fromUtf8("RhinoClient"));
	m_ptrayIcon->show();

	//创建托盘右键菜单
	m_pSNSAction = new QAction(QString::fromLocal8Bit("个人中心 (&R)"), this);
	connect(m_pSNSAction, SIGNAL(triggered()), this, SLOT(taskEvent()));
	m_pSetAction = new QAction(QString::fromLocal8Bit("设置 (&I)"), this);
	connect(m_pSetAction, SIGNAL(triggered()), this, SLOT(setEvent()));
	m_pQuitAction = new QAction(QString::fromLocal8Bit("退出 (&Q)"), this);
	connect(m_pQuitAction, SIGNAL(triggered()), this, SLOT(hide1()));
	//connect(m_pQuitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	//创建右键弹出菜单
	m_pTrayIconMenu = new QMenu(this);
	m_pTrayIconMenu->addAction(m_pSNSAction);
	m_pTrayIconMenu->addAction(m_pSetAction);
	m_pTrayIconMenu->addSeparator();
	m_pTrayIconMenu->addAction(m_pQuitAction);
	m_ptrayIcon->setContextMenu(m_pTrayIconMenu);
	//添加鼠标响应
	connect(m_ptrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(mouseEvent(QSystemTrayIcon::ActivationReason)));

	m_timer_update_players = new QTimer(this);
	bool bres = connect(m_timer_update_players, SIGNAL(timeout()), this, SLOT(onTime_update_players()));//关联定时器计满信号和相应的槽函数
	m_timer_update_players->start(3000);//定时器开始计时，其中1000表示1000ms即1秒
}
//托盘右键操作呼出个人中心
void CWJMainWindow::taskEvent()
{
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_3002);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
	showNormal();
}
//托盘右键操作呼出设置页面
void CWJMainWindow::setEvent()
{
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_3003);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
	showNormal();
}
void CWJMainWindow::MsgBoxGeneral(QString title, bool bClose)
{
	box->setcontent(title);
	box->show();
}
void CWJMainWindow::MsgBoxSignal(QString title, QString okBtn, QString noBtn)
{
	box->setcontent(title);
	box->SetTextOKBtn(okBtn);
	box->SetTextNOBtn(noBtn);
	box->show();
}
void CWJMainWindow::DoMsgBoxGeneral()
{
	WJ_gameid_10000001::CMyHallLogic* hall = (WJ_gameid_10000001::CMyHallLogic*)(CommonData()->GetMyHallLogic(e_gameid_10000001));
	UINT dwUin = hall->gameInfo.dwUin;
	QString str = QStringLiteral("您将使用账号艾欧尼亚-向东走10步参与任务【任务名称】祝您游戏愉快") + QString("%1").arg(dwUin);
	MsgBoxGeneral(str);
}
void CWJMainWindow::DoMsgBoxSignal()
{
	QString title = QStringLiteral("您登录的账号不属于您的绑定账号使用该账号不计入比赛结果，请登录您选择的参赛账号或至设置中心更换绑定");
	QString okBtn = QStringLiteral("前往设置中心");
	QString noBtn = QStringLiteral("更换账号");
	MsgBoxSignal(title, okBtn, noBtn);
}
//窗口最小化后删除任务栏图标
//void CWJMainWindow::changeEvent(QEvent* event)
//{
//	if (event->type() == QEvent::WindowStateChange)
//	{
//		if (windowState() & Qt::WindowMinimized)
//		{
//			QTimer::singleShot(0, this, SLOT(hide()));
//			m_ptrayIcon->show();
//		}
//	}
//}
//退出程序后删除托盘图标
void CWJMainWindow::hide1()
{
	
	m_ptrayIcon->hide();

	ExitMonitorProcess();

	close();
}

//结束服务进程
void CWJMainWindow::ExitMonitorProcess()
{
	try {

		HANDLE hProcess = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (INVALID_HANDLE_VALUE == hProcess) {

			return;
		}

		PROCESSENTRY32 process = { 0 };
		process.dwSize = sizeof(PROCESSENTRY32);

		BOOL bNext = ::Process32Next(hProcess, &process);

		while (bNext) {
		
			//找到服务进程干掉
			if (::wcscmp(process.szExeFile, TEXT("RhinoMonitor.exe")) == 0) {

				HANDLE hTmProecss = ::OpenProcess(PROCESS_TERMINATE, FALSE, process.th32ProcessID);

				if (NULL != hTmProecss) {

					::TerminateProcess(hTmProecss, 0);
					::CloseHandle(hTmProecss);
				}
			}

			bNext = ::Process32Next(hProcess, &process);
		}

		::CloseHandle(hProcess);
	}
	catch (...) {

	}
}

//鼠标事件
void CWJMainWindow::mouseEvent(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
		//单击托盘图标
		
		//WJ_SAFE_QPostMessage(WJMainWindow(), WM_HALL_MESS_1012, 0, 0);
		//setWindowFlags(Qt::X11BypassWindowManagerHint);
		
		SystemLog()->LogToFile("单击托盘图标");
		showNormal();
		raise();
		activateWindow();
		//DoMsgBoxGeneral();
		//DoMsgBoxSignal();
		break;
	default:
		break;
	}
}

bool CWJMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	if (eventType == "windows_generic_MSG")
	{
		PMSG msg = (PMSG)message;
		if (msg->message > WM_GAME_MESS)
		{//此为游戏或房间内消息
			return RoomLogic()->Sys_OnMess(eventType, message, result);
		}
		switch (msg->message)
		{
		case WM_COPYDATA:
			break;
		case WM_HALL_MESS_1006:
		case WM_HALL_MESS_1007:
		{
								  RoomLogic()->Sys_OnJoinRoom(true);
		}break;
		case WM_HALL_MESS_1002:
		{
			emit signa_linkland(false);
		}break;
		case WM_HALL_MESS_1004:
		{
			emit signa_linkland(true);
		}break;
		case WM_HALL_MESS_1003:
		{
			emit signa_linkland(false);
		}break;
		case WM_HALL_MESS_1013:
		{
			showNormal();
		}break;
		case WM_HALL_MESS_1014:
		{
			RoomLogic()->Sys_OnDisposeErr(e_gameid_10000001);
			QJsonObject sendjsonpart;
			sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_3013);
			QJsonDocument documentpack;
			documentpack.setObject(sendjsonpart);
			QString json_json(documentpack.toJson(QJsonDocument::Compact));
			WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

		}break;
		default:
			break;
		}

		IBaseMyHallLogic* p = (IBaseMyHallLogic*)CommonData()->GetMyHallLogic(CommonData()->m_NowGame);
		if (nullptr != p)
		{
			p->OnMess(eventType, message, result);
		}
	}
	return false;
}

CWJMainWindow *CWJMainWindow::getInstance()
{
	if (nullptr == g_pWJMainWindow)
	{
		g_pWJMainWindow = new CWJMainWindow();
	}
	return g_pWJMainWindow;
}
CWJMainWindow::~CWJMainWindow()
{
	delete ui;
	if (nullptr != g_pWJMainWindow)
	{
		delete g_pWJMainWindow;
		g_pWJMainWindow = nullptr;
	}

	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("~WJMainWindow()");
}

void CWJMainWindow::openUrl(QUrl& url)
{
	m_pMainView->load(url);
}

/*
* 定时更新玩家请求
*/
void CWJMainWindow::onTime_update_players()
{
	if (WJ::PlatformLogic().isLogin())
	{
		WJ::PlatformLogic().sendData(WJ::e_hs_mess_gamelist, WJ::e_hs_ass_gamelist_onlien_count, nullptr, 0);
	}
}

//显示微信登陆窗口
void CWJMainWindow::onShowWXwind(bool bShow)
{
	if (nullptr != m_pWXView)
	{
		m_pWXView->hide();
		m_pWXView->stop();
		m_pWXView->close();
		WJ_SAFE_DELETE(m_pWXView);
	}

	if (bShow)
	{//显示就是创建窗口
		m_pWXView = new QWebEngineView(this);
		if (nullptr != m_pWXView)
		{
			std::string str_url = clientconfig().GetData()->h5_server_address + "resource/weichatlogin.html";
			m_pWXView->load(QUrl(QString(str_url.c_str())));
			m_pWXView->page()->setBackgroundColor(QColor(0, 0, 0, 0));
			m_pWXView->resize(330, 480);
			m_pWXView->setAttribute(Qt::WA_DeleteOnClose);
			m_pWXView->move(475, 120);
			m_pWXView->show();
		}
	}
}

//显示内嵌游戏结束窗口
void CWJMainWindow::onShowGameFinishwind(bool bShow)
{
	if (nullptr != m_pGameFinishView)
	{
		m_pGameFinishView->hide();
		m_pGameFinishView->stop();
		m_pGameFinishView->close();
		WJ_SAFE_DELETE(m_pGameFinishView);
		m_pGameFinishView = nullptr;
	}

	if (bShow)
	{//显示就是创建窗口
		m_pGameFinishView = new QWebEngineView(this);
		if (nullptr != m_pGameFinishView)
		{
			m_pGameFinishView->load(QUrl(CommonData()->currentpath + "/Resource/H5/statisticsPages.html"));
			m_pGameFinishView->page()->setBackgroundColor(QColor(0, 0, 0, 0));
			m_pGameFinishView->resize(900 + 2, 560 + 2);
			m_pGameFinishView->setAttribute(Qt::WA_DeleteOnClose);
			m_pGameFinishView->move(210, 150);
			m_pGameFinishView->show();
		}
	}
}
//是否显示注册页面
void CWJMainWindow::showRegistrationPage(bool bShow)
{
	if (nullptr != m_pRegistrationView)
	{
		m_pRegistrationView->hide();
		m_pRegistrationView->stop();
		m_pRegistrationView->close();
		WJ_SAFE_DELETE(m_pRegistrationView);
	}

	if (bShow)
	{//显示就是创建窗口
		m_pRegistrationView = new QWebEngineView(this);
		if (nullptr != m_pRegistrationView)
		{
			std::string str_url = clientconfig().GetData()->h5_server_address + "resource/register.html";
			m_pRegistrationView->load(QUrl(QString(str_url.c_str())));
			m_pRegistrationView->page()->setBackgroundColor(QColor(0, 0, 0, 0));
			//m_pRegistrationView->resize(670 + 2, 420 + 2);
			m_pRegistrationView->resize(640, 440);
			m_pRegistrationView->setAttribute(Qt::WA_DeleteOnClose);
			m_pRegistrationView->move(320, 140);
			m_pRegistrationView->show();
		}
	}
}
//是否显示普通登录窗口
void CWJMainWindow::onShowLogin(bool bShow)
{
	if (nullptr != m_pLoginView)
	{
		m_pLoginView->hide();
		m_pLoginView->stop();
		m_pLoginView->close();
		WJ_SAFE_DELETE(m_pLoginView);
	}

	if (bShow)
	{//显示就是创建窗口
		m_pLoginView = new QWebEngineView(this);
		if (nullptr != m_pLoginView)
		{
			std::string str_url = clientconfig().GetData()->h5_server_address + "resource/login.html";
			m_pLoginView->load(QUrl(QString(str_url.c_str())));
			m_pLoginView->page()->setBackgroundColor(QColor(0, 0, 0, 0));
			m_pLoginView->resize(640, 390);
			m_pLoginView->setAttribute(Qt::WA_DeleteOnClose);
			m_pLoginView->move(320, 165);
			m_pLoginView->show();
		}
	}
}
//是否显示LOL任务赛结果页窗口
void CWJMainWindow::onShowGameTaskLOL(bool bShow)
{
	if (nullptr != m_pGameTaskLOL)
	{
		m_pGameTaskLOL->hide();
		m_pGameTaskLOL->stop();
		m_pGameTaskLOL->close();
		WJ_SAFE_DELETE(m_pGameTaskLOL);
	}

	if (bShow)
	{//显示就是创建窗口
		m_pGameTaskLOL = new QWebEngineView(this);
		if (nullptr != m_pGameTaskLOL)
		{
			std::string str_url = clientconfig().GetData()->h5_server_address + "resource/lolResult.html";
			m_pGameTaskLOL->load(QUrl(QString(str_url.c_str())));
			m_pGameTaskLOL->page()->setBackgroundColor(QColor(0, 0, 0, 0));
			m_pGameTaskLOL->resize(1034, 635);
			m_pGameTaskLOL->setAttribute(Qt::WA_DeleteOnClose);
			m_pGameTaskLOL->move(123, 42);
			m_pGameTaskLOL->show();
		}
	}
}
void CWJMainWindow::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
}
void CWJMainWindow::resizeEvent(QResizeEvent* size)
{
	m_pMainView->resize(size->size());
}

void CWJMainWindow::closeEvent(QCloseEvent *event)
{
	SystemLog()->LogToFile("犀牛平台退出!");
	//hide1();
	QEvent::Type t = event->type();
	SystemLog()->LogToFile("event退出原因:%d", t);

	try
	{
		m_pMainView->stop();
		m_pMainView->close();

		if (nullptr != m_pLoginView)
		{
			m_pLoginView->stop();
			m_pLoginView->close();
		}

		if (nullptr != m_pLoginView)
		{
			m_pLoginView->stop();
			m_pLoginView->close();
		}
		if (nullptr != m_pRegistrationView)
		{
			m_pRegistrationView->stop();
			m_pRegistrationView->close();
		}
		if (nullptr != m_pGameFinishView)
		{
			m_pGameFinishView->stop();
			m_pGameFinishView->close();
		}
		HWND hWnd = ::FindWindowA(NULL, "炉石传说");
		if (nullptr != m_pEmbedWindowHS && hWnd)
		{
			m_pEmbedWindowHS->close();
			delete m_pEmbedWindowHS;
		}
		m_pEmbedWindowHS = nullptr;

		if (nullptr != m_pBetWindowLoL)
		{
			m_pBetWindowLoL->close();
			delete m_pBetWindowLoL;
		}
		m_pBetWindowLoL = nullptr;

		if (nullptr != m_pBetPopup) {

			m_pBetPopup->close();
			delete m_pBetPopup;

		}
		m_pBetPopup = nullptr;

		ClientLink()->close();
	}
	catch (...)
	{
		///
	}
}

void CWJMainWindow::setbageweb()
{
	m_pMainView->setbageweb();
}