#include "test_client.h"
#include "WJPlatformLogic.h"
#include <windows.h>
#include "qpushbutton.h"
#include "qdebug.h"
#include <QtNetwork/QtNetwork>
#include "clientconfig.h"
#include "message/WJNetPlatformMessage.h"

test_client::test_client(QWidget *parent)
	: QMainWindow(parent)
{
	m_bMakeSocket = false;
	m_runthread = true;
	m_timer = nullptr;

	ui.setupUi(this);

	this->setFixedSize(this->width(), this->height());

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onsocketconnect()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(onsocketRun()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(onsocketClose()));

	//启动处理线程
	std::thread recvThread(std::bind(&test_client::MakeSocketThread, this));
	recvThread.detach();
	//MakeSocketThread();
	clientconfig().initconfig();
}

test_client::~test_client()
{
	clear_socketlogic();
}

//造套接字逻辑线程
int test_client::MakeSocketThread()
{
	try
	{
		int inum = 0;
		while (m_runthread)
		{
			if (inum >= AI_NUM)
			{
				break;
			}

			{
				std::lock_guard <std::mutex> autoLock(m_dataMutex_test_client);
				WJ::WJPlatformLogic* plogic_struct = new WJ::WJPlatformLogic(inum);
				m_vector_logic_struct.push_back(plogic_struct);
			}

			++inum;
		}
	}
	catch (...)
	{
		clear_socketlogic();
	}

	m_bMakeSocket = true;

	return 0;
}

//清理接字逻辑队列
void test_client::clear_socketlogic()
{
	m_runthread = false;
	std::lock_guard <std::mutex> autoLock(m_dataMutex_test_client);
	auto _iter = m_vector_logic_struct.begin();
	while (_iter != m_vector_logic_struct.end())
	{
		(*_iter)->close();
		WJ_SAFE_DELETE((*_iter));
		_iter = m_vector_logic_struct.erase(_iter);
	}
}

//用户操作计时器
void test_client::onTimeRun()
{
	for (UINT i = 0; i < m_vector_logic_struct.size(); ++i)
	{
		m_vector_logic_struct.at(i)->run();
	}
}

//执行连接动作
void test_client::onsocketconnect()
{
	if (!m_bMakeSocket)
	{
		return;
	}

	if (nullptr == m_timer)
	{
		m_timer = new QTimer(this);
		connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeRun()));//关联定时器计满信号和相应的槽函数
	}
	m_timer->stop();
	m_timer->start(1000);//定时器开始计时，其中1000表示1000ms即1秒

	//执行连接操作
	QString qstr = clientconfig().GetData()->hall_server_address;
	ui.lineEdit->setText(qstr);
	char*  ch;
	QByteArray ba = qstr.toLocal8Bit();
	ch = ba.data();

	for (UINT i = 0; i < m_vector_logic_struct.size(); ++i)
	{
		m_vector_logic_struct.at(i)->connect(ch, 0);
	}	
}

//执行发送动作
void test_client::onsocketRun()
{
	//废弃
}

//执行关闭动作
void test_client::onsocketClose()
{
	if (!m_bMakeSocket)
	{
		return;
	}

	if (nullptr != m_timer)
	{
		m_timer->stop();
	}

	for (UINT i = 0; i < m_vector_logic_struct.size(); ++i)
	{
		m_vector_logic_struct.at(i)->close();
	}
}