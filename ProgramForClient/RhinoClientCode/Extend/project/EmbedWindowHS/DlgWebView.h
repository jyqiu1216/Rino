#ifndef _ZSWEBVIEW_zswebview_H_
#define _ZSWEBVIEW_zswebview_H_

#include <QObject>
#include <QtWebEngineWidgets/QWebEngineView>

#include <QtCore/QAbstractNativeEventFilter>

class DlgWebView : public QWebEngineView/*, public QAbstractNativeEventFilter*/
{
	Q_OBJECT
public:
	DlgWebView(QWidget *parent = 0, HWND hWinTouch = nullptr);
	~DlgWebView();


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
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);


	

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
	HWND m_hWinTouch;//触摸板句柄
};

#endif // _ZSWEBVIEW_zswebview_H_
