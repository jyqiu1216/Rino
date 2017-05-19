#pragma once

#include <QObject>
#include <QtWebEngineWidgets/QWebEngineView>

#include <QtCore/QAbstractNativeEventFilter>

class DlgWebView_00000002 : public QWebEngineView/*, public QAbstractNativeEventFilter*/
{
	Q_OBJECT
public:
	DlgWebView_00000002(QWidget *parent = 0);
	~DlgWebView_00000002();

public:
	/**
	* \brief:	设置窗口的宽度和高度，单位像素
	*/
	void setWindowSize(int _w, int _h);


	void setHSHWin(HWND _hWnd);

	///**
	// * \brief paintEvent 背景渲染继承类，通过该类实现背景绘制
	// */
	//virtual void paintEvent(QPaintEvent *);

	///**
	// * \brief timerEventFunction 定时器
	// */
	//void timerEventFunction();


public:

	/* 鼠标进入窗口区域 */
	virtual void enterEvent(QEvent *);

	/* 鼠标离开窗口区域 */
	virtual void leaveEvent(QEvent *);

	//	private slots:
	//	/**
	//	 * @brief onLoadStarted 加载完成开始时进行调用
	//	 */
	//	void onLoadStarted();
	//	/**
	//	 * @brief onLoadProgress 加载过程中进行调用
	//	 * @param progress
	//	 */
	//	void onLoadProgress(int progress);
	//	/**
	//	 * @brief onLoadFinished 加载结束时进行调用
	//	 */
	//	void onLoadFinished(bool);





	//	//Q_SIGNALS:
	//	//    virtual void loadStarted();
	//	//    virtual void loadProgress(int progress);
	//	//    virtual void loadFinished(bool);
	//	//    void titleChanged(const QString& title);
	//	//    void selectionChanged();
	//	//    void urlChanged(const QUrl&);
	//	//    void iconUrlChanged(const QUrl&);
	//	//    void renderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus terminationStatus,
	//	//                             int exitCode);
	//
	//
public:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	HWND m_hWinHS;
};

