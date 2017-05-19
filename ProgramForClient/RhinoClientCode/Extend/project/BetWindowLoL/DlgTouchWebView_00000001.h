#ifndef _DlgTouchWebView_H_
#define _DlgTouchWebView_H_

#include <QObject>
#include <QtWebEngineWidgets/QWebEngineView>

#include <QtCore/QAbstractNativeEventFilter>

class DlgTouchWebView : public QWebEngineView
{
	Q_OBJECT
public:
	DlgTouchWebView(QWidget *parent = 0);
	~DlgTouchWebView();

	void setHwnd_Operate(HWND hwnd_operate){ m_hwnd_operate = hwnd_operate; }
	void setHwnd_Tell(HWND hwnd_tell){ m_hwnd_tell = hwnd_tell; }
	void setWind_Init_Station(long lwind_init_station){ m_wind_init_station = lwind_init_station; }
public:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

	//设置窗口鼠标穿透(1 穿透 0 不穿透)
	void setWindMousePenetrate(bool bPenetrate);
private:
	bool m_bAllowMouseThrough;//运行鼠标穿透

	HWND m_hwnd_operate;//被操作的窗口句柄
	long m_wind_init_station;//被操作的窗口创建窗口的时候初始状态（鼠标不可穿透）
	HWND m_hwnd_tell;//被通知去传递消息给H5的窗口句柄
};

#endif // _DlgTouchWebView_H_
