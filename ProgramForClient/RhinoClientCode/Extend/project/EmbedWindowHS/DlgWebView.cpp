#include "DlgWebView.h"

#include <Windows.h>

DlgWebView::DlgWebView(QWidget *parent, HWND hWinTouch)
	: QWebEngineView(parent)
	, m_hWinHS(NULL)
	, m_hWinTouch(hWinTouch)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | Qt::Dialog);
	setAttribute(Qt::WA_TranslucentBackground, true);
	this->page()->setBackgroundColor(QColor(0, 0, 0, 0));
}

DlgWebView::~DlgWebView()
{
}

void DlgWebView::setWindowSize(int _w, int _h)
{
	m_width = _w; 
	m_height = _h; 
	this->resize(_w, _h);
};


void DlgWebView::setHSHWin(HWND _hWnd)
{
	m_hWinHS = _hWnd;
}
//
//void MyWebView::paintEvent(QPaintEvent *)
//{
//	//QPainter painter(this);
//	//painter.drawPixmap(0, 0, m_backgroundPix);//绘制图像
//	//painter.fillRect(this->rect(), QColor(255,255,0,50));
//}
//
//void MyWebView::timerEventFunction()
//{
//	//qDebug("timer event");
//}
//

//void MyWebView::onLoadStarted()
//{
//	//this->hide();
//	//this->setWindowOpacity(0);
//}

//void MyWebView::onLoadProgress(int progress)
//{
//	//if (progress == 100)
//	//{
//	//	this->setWindowOpacity(1);
//	//	this->show();
//	//}
//}

//void MyWebView::onLoadFinished(bool result)
//{
//	//printf("页面加载完成: %s\n", result ? "成功" : "失败");
//}
//
//
void DlgWebView::enterEvent(QEvent *)
{
	//static QPropertyAnimation* anima = new QPropertyAnimation(this, "geometry");
	//anima->setDuration(1000);
	//anima->setStartValue(QRect(m_x, m_y, m_width, 43));
	//anima->setEndValue(QRect(m_x, m_y, m_width, m_height));
	//anima->start();
	//::SwitchToThisWindow(m_hWinHS, true);
}

void DlgWebView::leaveEvent(QEvent *)
{
	//static QPropertyAnimation* anima = new QPropertyAnimation(this, "geometry");
	//anima->setDuration(1000);
	//anima->setStartValue(QRect(m_x, m_y, m_width, m_height));
	//anima->setEndValue(QRect(m_x, m_y, m_width, 43));
	//anima->start();
	//
	if (m_hWinHS)
	{
		SwitchToThisWindow(m_hWinHS, true);
	}
}

bool DlgWebView::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	if (eventType == "windows_generic_MSG")
	{
		MSG* pMsg = reinterpret_cast<MSG*>(message);
		switch (pMsg->message)
		{
		case WM_LBUTTONDOWN:
		{//存在触摸板沉到当前窗口下面去了，点击不到，这边将点击的区域是触摸板的区域时候正好替对方发送收缩消息
							   if (nullptr != m_hWinTouch)
							   {
								   QWidget* p = QWidget::find((WId)m_hWinTouch);
								   if (nullptr != p)
								   {
									   int il = p->pos().x();
									   int ir = p->pos().x() + p->rect().right();
									   int it = p->pos().y();
									   int ib = p->pos().y() + p->rect().bottom();

									   QPoint point(pMsg->pt.x, pMsg->pt.y);
									   if (il <= point.x()
										   && ir > point.x()
										   && it <= point.y()
										   && ib > point.y())
									   {
										   ::PostMessage(m_hWinTouch, WM_LBUTTONDOWN, pMsg->wParam, pMsg->lParam);
									   }
								   }
							   }
		}break;
		default:
			break;
		}
	}
	return false;
}

//
////void MyWebView::setWindowSize(int _x, int _y, int _w, int _h)
////{
////	m_x = _x;
////	m_y = _y;
////	m_width = _w;
////	m_height = _h;
////
////	RECT rt;
////	::GetWindowRect(m_hWinHS, &rt);
////
////	//move(rt.left + _x, rt.top + _y);
////	move(_x, _y);
////	resize(_w, _h);
////}
