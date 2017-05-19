#ifndef _ZSWEBVIEW_zswebview_H_
#define _ZSWEBVIEW_zswebview_H_

#include <QObject>

#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QtWebChannel/QtWebChannel>
//#include "webchannelextern.h"


//#define QT_H5_EXTEND_CLAZZ_NAME "qtWebExtend"
#define QT_H5_EXTEND_CLAZZ_NAME "bridge"

class WebChannelExt;

class WJWebView :public QWebEngineView
{
	Q_OBJECT
public:
	explicit WJWebView(QWidget *parent = 0);
	~WJWebView();

	void setbageweb();
	void replyFinished(QNetworkReply* reply);
	void setwebsize();
	void on_readyRead();

	//背景渲染继承类，通过该类实现背景绘制
	virtual void paintEvent(QPaintEvent *);
	//定时器
	void timerEventFunction();

	private slots:
	//加载完成开始时进行调用
	void onLoadStarted();
	//加载过程中进行调用
	void onLoadProgress(int progress);
	//加载结束时进行调用
	void onLoadFinished(bool);
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
	/**
	* @brief 背景图片
	*/
	//QPixmap m_backgroundPix;

	//QPoint oldPos;
	bool press;
	QWebChannel* m_pChannel;
	WebChannelExt* m_webChannelExt;
};

#endif // _ZSWEBVIEW_zswebview_H_
