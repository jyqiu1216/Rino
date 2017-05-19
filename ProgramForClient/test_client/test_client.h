#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H

#include <mutex>
#include <vector>
#include <QtWidgets/QMainWindow>
#include "ui_test_client.h"
#include "WJPlatformLogic.h"

class test_client : public QMainWindow
{
	Q_OBJECT

public:
	test_client(QWidget *parent = 0);
	~test_client();

private:
	Ui::test_clientClass ui;
	std::vector<WJ::WJPlatformLogic*> m_vector_logic_struct;
	bool m_bMakeSocket;//是否制造逻辑完成
	bool m_runthread;//是否执行线程
	std::mutex m_dataMutex_test_client;			//批发同步锁互斥量，方便线程对互斥量上锁

	QTimer *m_timer;//运行计时器

private slots:
	//执行连接动作
	void onsocketconnect();
	//执行发送动作
	void onsocketRun();
	//执行关闭动作
	void onsocketClose();

	//连接数 大厅
	//连接数 房间
	//登陆数 大厅
	//登陆数 房间
	//报名人数
	//游戏人数
	//空闲人数

	//造套接字逻辑线程
	int MakeSocketThread();
	//清理接字逻辑队列
	void clear_socketlogic();

	//用户操作计时器
	void onTimeRun();
};

#endif // TEST_CLIENT_H
