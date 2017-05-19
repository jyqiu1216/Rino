#ifndef _MYDIALOG_H_
#define _MYDIALOG_H_

#include <QObject>
#include <QDialog>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtCore/QAbstractNativeEventFilter>

#include <Windows.h>

class DlgLoading : public /*QDialog*/QWebEngineView, public QAbstractNativeEventFilter
{
	Q_OBJECT
public:
	DlgLoading(QWidget *parent) : /*QDialog(parent)*/QWebEngineView(parent)
	{
		this->setParent(parent);
		//this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | Qt::Dialog);
		this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Dialog);
		this->setAttribute(Qt::WA_TranslucentBackground, true);
		this->page()->setBackgroundColor(QColor(0, 0, 0, 0));
		this->hide();



		//this->setCursor(Qt::ForbiddenCursor);
		//static HCURSOR hcur = ::LoadCursor(NULL, IDC_CROSS);
		//::SetCursor(hcur);
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
