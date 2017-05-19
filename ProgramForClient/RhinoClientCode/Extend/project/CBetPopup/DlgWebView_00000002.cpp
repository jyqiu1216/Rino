#include "DlgWebView_00000002.h"

#include <Windows.h>

DlgWebView_00000002::DlgWebView_00000002(QWidget *parent /*= 0*/)
	: QWebEngineView(parent)
	, m_hWinHS(NULL)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | Qt::Dialog);
	setAttribute(Qt::WA_TranslucentBackground, true);
	this->page()->setBackgroundColor(QColor(0, 0, 0, 0));
}

DlgWebView_00000002::~DlgWebView_00000002()
{

}

void DlgWebView_00000002::setWindowSize(int _w, int _h)
{
	m_width = _w;
	m_height = _h;
	this->resize(_w, _h);
}

void DlgWebView_00000002::setHSHWin(HWND _hWnd)
{
	m_hWinHS = _hWnd;
}

void DlgWebView_00000002::enterEvent(QEvent *)
{

}

void DlgWebView_00000002::leaveEvent(QEvent *)
{
	if (m_hWinHS)
	{
		SwitchToThisWindow(m_hWinHS, true);
	}
}
