#include "DlgBetView.h"

#include <Windows.h>
#include "common/LoadWindowsMess.h"

CDlgBetView::CDlgBetView(QWidget *parent)
{
	m_TimeId = 0;
	this->setParent(parent);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | Qt::Dialog);
	setAttribute(Qt::WA_TranslucentBackground, true);
	this->page()->setBackgroundColor(QColor(0, 0, 0, 0));
}

CDlgBetView::~CDlgBetView()
{

}

void CDlgBetView::StartTime()
{
	m_TimeId = startTimer(20000);
}

void CDlgBetView::KillTime()
{
	killTimer(m_TimeId);
	this->reload();
	this->hide();
}

void CDlgBetView::timerEvent(QTimerEvent *)
{
	this->reload();
	this->close();
}

bool CDlgBetView::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	if (eventType == "windows_generic_MSG")
	{
		PMSG msg = (PMSG)message;
		static int a = 0;
		++a;
		switch (msg->message)
		{
		case WM_LBUTTONDOWN:
			OutputDebugStringA("sdp_test c DlgWebView::nativeEvent2");
			break;

		default:
			break;
		}
	}
	return false;
}

void CDlgBetView::setWindMousePenetrate(bool bPenetrate)
{

}
