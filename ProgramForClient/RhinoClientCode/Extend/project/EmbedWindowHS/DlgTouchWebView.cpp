#include "DlgTouchWebView.h"

#include <Windows.h>
#include "common/LoadWindowsMess.h"

DlgTouchWebView::DlgTouchWebView(QWidget *parent)
	: QWebEngineView(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | Qt::Dialog);
	setAttribute(Qt::WA_TranslucentBackground, true);
	this->page()->setBackgroundColor(QColor(255, 0, 255, 255));

	m_bAllowMouseThrough = true;//一开始肯定是已经展开了
}

DlgTouchWebView::~DlgTouchWebView()
{
}

//设置窗口鼠标穿透(1 穿透 0 不穿透)
void DlgTouchWebView::setWindMousePenetrate(bool bPenetrate)
{
	m_bAllowMouseThrough = !bPenetrate;
	if (bPenetrate)
	{
		if (nullptr != m_hwnd_tell)
			::SendMessage(m_hwnd_tell, WM_GAME_MESS_2019, 0, 0);

		SetWindowLong(m_hwnd_operate, GWL_EXSTYLE, m_wind_init_station | WS_EX_TRANSPARENT | WS_EX_LAYERED);
	}
	else
	{
		if (nullptr != m_hwnd_tell)
			::SendMessage(m_hwnd_tell, WM_GAME_MESS_2019, 1, 0);

		SetWindowLong(m_hwnd_operate, GWL_EXSTYLE, m_wind_init_station);
	}
}

bool DlgTouchWebView::nativeEvent(const QByteArray &eventType, void *message, long *result)
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
			setWindMousePenetrate(m_bAllowMouseThrough);
			break;

		default:
			break;
		}
	}
	return false;

}
