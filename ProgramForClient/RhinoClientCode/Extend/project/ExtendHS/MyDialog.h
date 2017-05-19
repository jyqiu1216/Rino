#ifndef _MYDIALOG_H_
#define _MYDIALOG_H_

#include <QObject>
#include <QDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QWebEngineView>
#include <QAbstractNativeEventFilter>

#include <Windows.h>

class MyDialog : public /*QDialog*/QWebEngineView, public QAbstractNativeEventFilter
{
	Q_OBJECT
public:
	MyDialog(QWidget *parent) : /*QDialog(parent)*/QWebEngineView(parent)
	{
		this->setParent(parent);
		//this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
		this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Dialog);
		//this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | Qt::Dialog);
		this->setAttribute(Qt::WA_TranslucentBackground, true);
		//this->setAutoFillBackground(true);
		this->page()->setBackgroundColor(QColor(0, 0, 0, 0));
		this->hide();
	}

public:
	void setHSWin(HWND _hWnd)
	{
		m_hWinHS = _hWnd;
	}

public:
	//virtual void paintEvent(QPaintEvent *_event)
	//{
	//	QPainter painter(this);
	//	painter.fillRect(this->rect(), QColor(255, 255, 255, 80));
	//}

	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *) Q_DECL_OVERRIDE
	{
		if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
		{
			MSG* pMsg = reinterpret_cast<MSG*>(message);
			switch (pMsg->message)
			{
			case WM_USER + WM_MOVE:
				break;

			case WM_MOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_KEYDOWN:
			case WM_KEYUP:
				::SendMessage(m_hWinHS, pMsg->message, pMsg->wParam, pMsg->lParam);
				break;

			default:
				break;
			}
		}
		return false;
	}

protected:
	HWND m_hWinHS;
};

#endif // _ZSWEBVIEW_zswebview_H_
