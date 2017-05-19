#include "WJWebView.h"
#include "WebChannelExt.h"
#include "WJMainWindow.h"

WJWebView::WJWebView(QWidget *parent) :QWebEngineView(parent)
{
	m_pChannel = new QWebChannel(this);
	m_webChannelExt = new WebChannelExt(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	setAttribute(Qt::WA_DeleteOnClose);
	setAttribute(Qt::WA_TranslucentBackground);
	m_pChannel->registerObject(QStringLiteral(QT_H5_EXTEND_CLAZZ_NAME), m_webChannelExt);
	page()->setWebChannel(m_pChannel);

	QObject::connect(this, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()));
	QObject::connect(this, SIGNAL(loadProgress(int)), this, SLOT(onLoadProgress(int)));
	QObject::connect(this, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));

}

WJWebView::~WJWebView()
{
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("~WJWebView()");
}


void WJWebView::paintEvent(QPaintEvent *)
{
	//QPainter painter(this);
	//painter.drawPixmap(0, 0, m_backgroundPix);//绘制图像
	//painter.dr
}
void WJWebView::timerEventFunction()
{
	qDebug("timer event");
}

#include <QDebug>

void WJWebView::onLoadStarted()
{
	//this->hide();
	this->setWindowOpacity(0);
}

void WJWebView::onLoadProgress(int progress)
{
	if (progress == 100)
	{
		this->setWindowOpacity(1);
		this->show();
	}
}
void WJWebView::onLoadFinished(bool result)
{
	qDebug() << "finished: " << result << endl;
}
void WJWebView::setbageweb()
{
	//m_webChannelExt->setbageweb();
}



