#ifndef AMTEST_H
#define AMTEST_H

#include <QKeyEvent>
#include <QtWidgets/QMainWindow>
#include "ui_AMTest.h"

#include "websocket/WebSocketServer.h"

class AMTest;
class EmbedWindowHS;

class MyWSMsgHandler : public WSMsgHandler
{
public: 
	MyWSMsgHandler(AMTest* _p) : m_pTest(_p)
	{}

	virtual void onRecvMsg(const QString &_msg)
	{
		printf(_msg.toStdString().c_str());
	}

public:
	AMTest* m_pTest;
};

#include <Windows.h>

class AMTest : public QMainWindow
{
	Q_OBJECT

public:
	AMTest(QWidget *parent = 0);
	~AMTest();



public:
	void timerEvent(QTimerEvent *);
	void initWebSocketSrv(int _port);

public:
	static void addEvent(int _eventId)
	{
		m_listEvent.push_back(_eventId);
	}

	void handleEvent();

	int getCmd() { return m_cmd; ; };
	void setCmd(int _cmd) { m_cmd = _cmd; }

	QString& getParam1() { m_param1 = m_ui.lineEdit_param_1->text(); return m_param1; };
	QString& getParam2() { m_param1 = m_ui.lineEdit_param_2->text(); return m_param2; };
	//WebSocketClient* getWebSocketClient() { return m_webSocketClient; }

	//virtual void enterEvent(QEvent *_event)
	//{
	//	::ShowCursor(true);
	//}
	//virtual void leaveEvent(QEvent *_event)
	//{
	//	//::ShowCursor(TRUE);
	//}


	virtual void keyPressEvent(QKeyEvent *_event)
	{
		int key = _event->key();
		if (Qt::Key_0 == key)
		{
			m_flag = 5555;
		}
		else if (Qt::Key_1 == key)
		{
			m_flag = 0;
		}
	}


	public slots:
	void onActivated(const QString &);
	void onExecute();

	void onClick_test();

	void onClick_reloadLua();

	void onClick_lol_startup();
	void onClick_lol_create();
	void onClick_lol_join();
	void onClick_lol_play();

	void onClick_hs_injectDll();
	void onClick_hs_reqFrient();
	void onClick_hs_reqChallenge();
	void onClick_hs_acceptChallenge();
	void onClick_hs_acceptFriend();
	void onClick_hs_msg2005();
	void onClick_hs_msg2006();
	void onClick_hs_pipei();
	void onClick_hs_endGame();

	void onEmbedWinHSClose();


public:
	int m_flag;

private:
	Ui::MainWindowAMTest m_ui;
	
	int m_cmd;

	QString m_param1;
	QString m_param2;

	QPoint m_coord;

	//WebSocketServer *m_webSocketSrv;

	MyWSMsgHandler *m_msgHandler;

	EmbedWindowHS* m_embedWin;

	static std::list<unsigned int> m_listEvent;

};

#endif // AMTEST_H
