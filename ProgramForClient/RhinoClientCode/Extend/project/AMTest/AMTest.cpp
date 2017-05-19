#include "AMTest.h"


#include "AMTestFunc.h"


int g_pos_x = 0;
int g_pos_y = 0;



#include <process.h>



#include "hook/HookManager.h"

/* 炉石传说注入窗口 DLL */
#include "hook/EmbedWindowHS.h"
#ifdef _DEBUG
#pragma comment(lib, "EmbedWindowHS_d.lib")
#else
#pragma comment(lib, "EmbedWindowHS.lib")
#endif // _DEBUG


#include "hook/HookEventParam.h"

std::list<unsigned int> AMTest::m_listEvent;

AMTest::AMTest(QWidget *parent)
: QMainWindow(parent)
//, m_webSocketSrv(nullptr)
, m_cmd(-1)
, m_embedWin(nullptr)
, m_flag(0)
{
	m_ui.setupUi(this);

	//ui.comboBox->addItem(CMD_TEST);
	//ui.comboBox->addItem(CMD_RELOAD_LUA);
	//ui.comboBox->addItem(CMD_LOL_STARTUP);
	//ui.comboBox->addItem(CMD_LOL_CREATEROOM);
	//ui.comboBox->addItem(CMD_LOL_JOINROOM);
	//ui.comboBox->addItem(CMD_LOL_PLAY);
	//ui.comboBox->addItem(CMD_HS_STARTUP);
	//ui.comboBox->addItem(CMD_HS_PLAY);
	//ui.comboBox->addItem(CMD_HS_SendMSG);


	connect(m_ui.comboBox, SIGNAL(activated(const QString &)), this, SLOT(onActivated(const QString &)));

	connect(m_ui.pushButton_test, SIGNAL(clicked()), this, SLOT(onClick_test()));
	connect(m_ui.pushButton_reloadLua, SIGNAL(clicked()), this, SLOT(onClick_reloadLua()));
	connect(m_ui.pushButton_lol_startup, SIGNAL(clicked()), this, SLOT(onClick_lol_startup()));
	connect(m_ui.pushButton_lol_create, SIGNAL(clicked()), this, SLOT(onClick_lol_create()));
	connect(m_ui.pushButton_lol_join, SIGNAL(clicked()), this, SLOT(onClick_lol_join()));
	connect(m_ui.pushButton_lol_play, SIGNAL(clicked()), this, SLOT(onClick_lol_play()));

	connect(m_ui.pushButton_hs_injectDll, SIGNAL(clicked()), this, SLOT(onClick_hs_injectDll()));
	connect(m_ui.pushButton_hs_reqFrient, SIGNAL(clicked()), this, SLOT(onClick_hs_reqFrient()));
	connect(m_ui.pushButton_hs_reqChallenge, SIGNAL(clicked()), this, SLOT(onClick_hs_reqChallenge()));
	connect(m_ui.pushButton_hs_acceptChallenge, SIGNAL(clicked()), this, SLOT(onClick_hs_acceptChallenge()));
	connect(m_ui.pushButton_hs_acceptFriend, SIGNAL(clicked()), this, SLOT(onClick_hs_acceptFriend()));
	connect(m_ui.pushButton_hs_msg_2005, SIGNAL(clicked()), this, SLOT(onClick_hs_msg2005()));
	connect(m_ui.pushButton_hs_msg_2006, SIGNAL(clicked()), this, SLOT(onClick_hs_msg2006()));
	connect(m_ui.pushButton_hs_pipei, SIGNAL(clicked()), this, SLOT(onClick_hs_pipei()));
	connect(m_ui.pushButton_hs_end, SIGNAL(clicked()), this, SLOT(onClick_hs_endGame()));


	m_ui.lineEdit_param_1->setText(QString::fromLocal8Bit("叶落的季节#51207"));
	//m_ui.lineEdit_param_1->setText(QString::fromLocal8Bit("晓洛晨曦#5175"));
	m_ui.lineEdit_param_2->setText("TestDll.dll");

	if (!WJAM::initLua("./luaScript/main.out"))
	{
		printf("=====> 加载 LUA 脚本失败!\n");
	}
	else
	{
		printf("=====> 加载 LUA 脚本成功!\n");
	}


	HANDLE hThread = (HANDLE)_beginthread(ThreadProc, 0, this);

	//m_imageRecognition.startThread(this);

	startTimer(1);

	m_msgHandler = new MyWSMsgHandler(this);
	initWebSocketSrv(12341);
}

AMTest::~AMTest()
{

}

void AMTest::handleEvent()
{
	for (auto iter = m_listEvent.begin(); iter != m_listEvent.end(); ++iter)
	{
		switch (*iter)
		{
		case 1:
		{
				  OutputDebugStringA("创建炉石内嵌窗口\n");
				  HWND hWnd = ::FindWindowA(NULL, "炉石传说");
				  if (hWnd)
				  {
					  /* 初始化页面， 指定页面URL */
					  std::string path = QCoreApplication::applicationDirPath().toStdString();
					  if (m_embedWin)
					  {
						  delete m_embedWin;
						  m_embedWin = nullptr;
					  }
					  m_embedWin = new EmbedWindowHS(hWnd, NULL, QRect(0, 0, 0, 0));
					  m_embedWin->initialize(NULL, QUrl((path + "/Resource/H5/HSMatchesAtOnce.html").c_str()), 12351);
					  //QObject::connect(m_embedWin, SIGNAL(close()), this, SLOT(onEmbedWinHSClose()));
					  //}
				  }
				  else
				  {
					  printf("炉石传说窗口未启动!!!\n");
				  }
		}

			break;

		default:
			break;
		}
	}
	m_listEvent.clear();
}

void AMTest::timerEvent(QTimerEvent *)
{
	bool flag = (m_cmd == -1);

	m_ui.pushButton_test->setEnabled(flag);

	m_ui.pushButton_reloadLua->setEnabled(flag);
	m_ui.pushButton_lol_startup->setEnabled(flag);
	m_ui.pushButton_lol_create->setEnabled(flag);
	m_ui.pushButton_lol_join->setEnabled(flag);
	m_ui.pushButton_lol_play->setEnabled(flag);

	m_ui.pushButton_hs_injectDll->setEnabled(flag);
	m_ui.pushButton_hs_reqFrient->setEnabled(flag);
	m_ui.pushButton_hs_reqChallenge->setEnabled(flag);
	m_ui.pushButton_hs_acceptChallenge->setEnabled(flag);
	m_ui.pushButton_hs_acceptFriend->setEnabled(flag);

	handleEvent();
}

void AMTest::initWebSocketSrv(int _port)
{
	//if (m_webSocketSrv)
	//{
	//	delete m_webSocketSrv;
	//	m_webSocketSrv = nullptr;
	//}

	//m_webSocketSrv = new WebSocketServer(_port, m_msgHandler, NULL);
}


void AMTest::onExecute()
{
}

void AMTest::onActivated(const QString &text)
{

	//else if (0 == text.compare(LOL_STARTUP))
	//{
	//}	
}



void AMTest::onClick_test()
{
	m_cmd = CMD_TEST;

	AMTest::addEvent(1);

	std::string playerName;
	WJAM::startupHS(0, "", playerName);

	m_param1 = m_ui.lineEdit_param_1->text();
	m_param2 = m_ui.lineEdit_param_2->text();

	//m_HSMgr.recognition(path.toLocal8Bit().data());
}

void AMTest::onEmbedWinHSClose()
{

}

void AMTest::onClick_reloadLua()
{
	if (!WJAM::initLua("./luaScript/main.out"))
	{
		printf("=====> 加载 LUA 脚本失败!\n");
	}
}

void AMTest::onClick_lol_startup()
{
	m_cmd = CMD_LOL_STARTUP;
}
void AMTest::onClick_lol_create()
{
	m_cmd = CMD_LOL_CREATEROOM;
}
void AMTest::onClick_lol_join()
{
	m_cmd = CMD_LOL_JOINROOM;
}
void AMTest::onClick_lol_play()
{
	m_cmd = CMD_LOL_PLAY;
}
void AMTest::onClick_hs_injectDll()
{
	m_cmd = CMD_HS_STARTUP;
}
void AMTest::onClick_hs_reqFrient()
{
	m_cmd = CMD_HS_REQ_FRIEND;	m_param1 = m_ui.lineEdit_param_1->text();
}
void AMTest::onClick_hs_acceptFriend()
{
	m_cmd = CMD_HS_ACCEPT_FRIEND;
}
void AMTest::onClick_hs_reqChallenge()
{
	m_cmd = CMD_HS_REQ_CHALLENGE;
}
void AMTest::onClick_hs_acceptChallenge()
{
	m_cmd = CMD_HS_ACCEPT_CHALLENGE;
}

void AMTest::onClick_hs_msg2005()
{
	m_embedWin->beginLoading();
}
void AMTest::onClick_hs_msg2006()
{
	m_embedWin->startGame();
};
void AMTest::onClick_hs_pipei()
{
	m_embedWin->beginLoading();
	m_cmd = CMD_HS_REQ_FRIEND;
}
void AMTest::onClick_hs_endGame()
{
	m_embedWin->endGame();
}