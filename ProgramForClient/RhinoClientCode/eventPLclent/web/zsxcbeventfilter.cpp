#include "zsxcbeventfilter.h"
#include <windows.h>
#include <QDebug>

#include "WJMainWindow.h"



bool ZsXcbEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *)
{
	if (eventType == "xcb_generic_event_t") {
		qDebug() << "test";
		// ...
	}

	else if (eventType == "windows_generic_MSG")
	{
		MSG* pMsg = (MSG*)message;

		switch (pMsg->message)
		{

		default:
			break;
		}

		//qDebug() <<QString("%1").arg((int)pMsg->message);
		int il = WJMainWindow()->pos().x();
		int ir = WJMainWindow()->pos().x() + WJMainWindow()->rect().right();
		int it = WJMainWindow()->pos().y();
		int ib = WJMainWindow()->pos().y() + WJMainWindow()->rect().bottom();

		if (pMsg->message == WM_LBUTTONDOWN)
		{
			QPoint point(pMsg->pt.x, pMsg->pt.y);
			if (WJMainWindow() != nullptr)
			{
				if (il <= point.x()
					&& ir > point.x()
					&& it <= point.y()
					&& ib > point.y())
				{
					WJMainWindow()->mousePressEvent1(point);
				}
			}
			
		}
		else  if (pMsg->message == WM_LBUTTONUP)
		{
			QPoint point(pMsg->pt.x, pMsg->pt.y);
			char s[MAX_PATH] = {0};
			if (WJMainWindow() != nullptr)
			{
				WJMainWindow()->mouseReleaseEvent1();
			}
		}
		else if (pMsg->message == WM_MOUSEMOVE)
		{
			int xPos = LOWORD(pMsg->lParam);
			int yPos = HIWORD(pMsg->lParam);
			if (yPos > 50)
				return false;

			QPoint point(pMsg->pt.x, pMsg->pt.y);
			if (WJMainWindow() != nullptr)
			{
				if (il <= point.x()
					&& ir > point.x()
					&& it <= point.y()
					&& ib > point.y())
					if (it < 0)
					{
						QPoint a(point.x(), point.y() + abs(it));
						return WJMainWindow()->mouseMoveEvent1(a);
					}
				{
					return WJMainWindow()->mouseMoveEvent1(point);
				}
			}	
		}
	}

	return false;
}
