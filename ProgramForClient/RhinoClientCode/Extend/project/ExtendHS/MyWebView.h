#ifndef _ZSWEBVIEW_zswebview_H_
#define _ZSWEBVIEW_zswebview_H_

#include <QObject>
#include <QtWebEngineWidgets>

class CWebChannelExtern;


class MyWebView : public QWebEngineView
{
	Q_OBJECT
public:
	explicit MyWebView(QWidget *parent = 0);
	~MyWebView();

	void setHSHwnd(HWND _hWnd);

	virtual void setWindowSize(int _x, int _y, int _w, int _h);

	/**
	 * @brief paintEvent 背景渲染继承类，通过该类实现背景绘制
	 */
	virtual void paintEvent(QPaintEvent *);

	/**
	 * @brief timerEventFunction 定时器
	 * @param event
	 */
	void timerEventFunction();


	private slots:
	/**
	 * @brief onLoadStarted 加载完成开始时进行调用
	 */
	void onLoadStarted();
	/**
	 * @brief onLoadProgress 加载过程中进行调用
	 * @param progress
	 */
	void onLoadProgress(int progress);
	/**
	 * @brief onLoadFinished 加载结束时进行调用
	 */
	void onLoadFinished(bool);


	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	

	//Q_SIGNALS:
	//    virtual void loadStarted();
	//    virtual void loadProgress(int progress);
	//    virtual void loadFinished(bool);
	//    void titleChanged(const QString& title);
	//    void selectionChanged();
	//    void urlChanged(const QUrl&);
	//    void iconUrlChanged(const QUrl&);
	//    void renderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus terminationStatus,
	//                             int exitCode);


protected:
	// setup the channel
	//QWebChannel* m_pChannel;

	//CWebChannelExtern* m_pWebchannelimpl;
	/**
	 * @brief 背景图片
	 */
	QPixmap m_backgroundPix;

	int m_x;
	int m_y;
	int m_width;
	int m_height;

	HWND m_hWinHS;
};

#endif // _ZSWEBVIEW_zswebview_H_
