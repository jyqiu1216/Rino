#ifndef _WJMainWindow_H
#define _WJMainWindow_H

#include "ui_WJMainWindow.h"
#include "WebChannelExt.h"
#include "WJWebView.h"
#include "common\Common.h"
#include <QMainWindow>
#include <QEvent>
#include <QtWebEngineWidgets>
#include <QtWebChannel/QtWebChannel>
#include "../hook/EmbedWindowHS.h"
#include "../Extend/project/BetWindowLoL/betwindowlol.h"
//#include "../Extend/project/EmbedWindowHS/betwindowhs.h"
#include "../Extend/project/CBetPopup/cbetpopup.h"
#define DEFAULT_WINDOW_WIDTH    1200
#define DEFAULT_WINDOW_HEIGHT   800

#include "zsqtmousedrag.h"

class webpopuppag;
class WebChannelExt;
class BetWindowHs;

class CWJMainWindow : public QMainWindow, public ZSQtMouseDrag/*, public QAbstractNativeEventFilter*/
{
    Q_OBJECT

signals :
	void signa_linkland(bool);

public:
	explicit CWJMainWindow(QWidget *parent = 0);
	~CWJMainWindow();
	static CWJMainWindow* getInstance();
	void setbageweb();
	void showResultPage();
	//是否显示注册页面
	void showRegistrationPage(bool bShow);

	//窗口最小化后删除任务栏图标
	//void changeEvent(QEvent* event);
public slots:
    //鼠标事件
	void mouseEvent(QSystemTrayIcon::ActivationReason reason);
	//退出程序后删除托盘图标
	void hide1();
	//退出时结束服务进程
	void ExitMonitorProcess();
	//托盘右键操作呼出设置页面
	void setEvent();
	//托盘右键操作呼出个人中心
	void taskEvent();
	// 常规消息弹窗
	void MsgBoxGeneral(QString title, bool bClose = false);
	// 带信号消息弹窗
	void MsgBoxSignal(QString title, QString okBtn, QString noBtn);
	// 调用常规消息弹窗
	void DoMsgBoxGeneral();
	// 调用带信号消息弹窗
	void DoMsgBoxSignal();
	/*
	* 定时更新玩家请求
	*/
	void onTime_update_players();
public:

	//主窗口地址跳转
	void openUrl(QUrl& url);
	//显示微信登陆窗口
	void onShowWXwind(bool bShow);
	//显示内嵌游戏结束窗口
	void onShowGameFinishwind(bool bShow);
	//是否显示普通登录窗口
	void onShowLogin(bool bShow);
	//是否显示LOL任务赛结果页窗口
	void onShowGameTaskLOL(bool bShow);
protected:
    //背景渲染继承类，通过该类实现背景绘制
    virtual void paintEvent(QPaintEvent *);
	virtual void resizeEvent(QResizeEvent* size);
	virtual void closeEvent(QCloseEvent *event);

	//显示任务栏右下角状态图标
	//BOOL ShowICON(bool bShow);
	//处理windows系统消息，主要处理 WM_COPYDATA
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);
public:
    //main H5 主窗口						//收到QQUIN之后发起任务状态查询（先取游戏结果再比对）
    WJWebView* m_pMainView;
	//内嵌微信登陆窗口
	QWebEngineView* m_pWXView;
	//注册窗口
	QWebEngineView* m_pRegistrationView;
	webpopuppag* m_webpopuppag;
	//炉石内嵌QT窗口
	EmbedWindowHS* m_pEmbedWindowHS;
	//LOL押注QT窗口
	BetWindowLoL* m_pBetWindowLoL;
	//SOLO赛ID
	int m_LolSoloId;
	//LOL押注ID
	int	m_BetIdLoL;
	//炉石押注ID
	int m_BetIdHs;
	//押注结果显示页
	CBetPopup* m_pBetPopup;
	QSystemTrayIcon* m_ptrayIcon;
	//内嵌游戏结束窗口
	QWebEngineView* m_pGameFinishView;
	QTimer *m_timer_update_players;		//更新在线玩家数计时器
	//普通登录窗口
	QWebEngineView* m_pLoginView;
	//LOL任务赛结果页窗口
	QWebEngineView* m_pGameTaskLOL;
private:

	QTimer*	m_timerQueryMission;		//用于查询任务完成状态的定时器
	
	QMenu*	m_pTrayIconMenu;			//托盘右键弹出菜单
	QAction*	m_pSNSAction;			//个人中心
	QAction*	m_pSetAction;			//设置
	QAction*	m_pQuitAction;			//退出
	
friend class CWebChannelExtern;
    // 背景图片
    QPixmap m_backgroundPix;
    QPoint oldPos;
    bool press;
	Ui::WJMainWindow *ui;
	QMbox* box;							//登陆LOL大厅右下角弹窗

};

#define  WJMainWindow()	CWJMainWindow::getInstance()

#endif // _WJMainWindow_H
