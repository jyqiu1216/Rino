#pragma once

#ifndef DLGBETVIEW_H
#define DLGBETVIEW_H

#include <QObject>
#include <QtWebEngineWidgets/QWebEngineView>

#include <QtCore/QAbstractNativeEventFilter>


class CDlgBetView : public QWebEngineView {

	Q_OBJECT
private:
protected:
public:
	CDlgBetView(QWidget *parent = 0);
	~CDlgBetView();

	void setHwnd_Operate(HWND hwnd_operate){ m_hwnd_operate = hwnd_operate; }
	void setHwnd_Tell(HWND hwnd_tell){ m_hwnd_tell = hwnd_tell; }
	void setWind_Init_Station(long lwind_init_station){ m_wind_init_station = lwind_init_station; }
	void StartTime();
	void KillTime();

protected:
	virtual void timerEvent(QTimerEvent *);

public:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

	//设置窗口鼠标穿透(1 穿透 0 不穿透)
	void setWindMousePenetrate(bool bPenetrate);
	int m_TimeId;
private:
	HWND m_hwnd_operate;//被操作的窗口句柄
	long m_wind_init_station;//被操作的窗口创建窗口的时候初始状态（鼠标不可穿透）
	HWND m_hwnd_tell;//被通知去传递消息给H5的窗口句柄
};

#endif