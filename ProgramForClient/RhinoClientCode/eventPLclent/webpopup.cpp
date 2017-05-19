#include "webpopup.h"
#include "web/WJMainWindow.h"
webPopup::webPopup(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	Qt::WindowFlags flag = (Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint); 
	setWindowFlags(flag);
	setAttribute(Qt::WA_TranslucentBackground, true);
	//setWindowOpacity(0.7);

	//Ç¶Ì×ÍøÕ¾
	engine = new QWebEngineView(this);
	channel = new QWebChannel(this);
	channel->registerObject("bridge", (QObject*)bridge::instance());
	engine->page()->setWebChannel(channel);
	ui.gridLayout->addWidget(engine);
	engine->page()->setBackgroundColor(QColor(0, 0, 0, 255));
	bool u=connect((QObject*)bridge::instance(), SIGNAL(slgnal_webclose()), this, SLOT(slots_close()));
	bool p = connect((QObject*)bridge::instance(), SIGNAL(slgnal_webeject(QString)), this, SLOT(webeject(QString)));
	bool l = connect((QObject*)bridge::instance(), SIGNAL(slgnal_setwebsize(int, int)), this, SLOT(websize(int, int)));
	engine->setStyleSheet(QStringLiteral("QWebEngineView{background-image: url(:/eventq/png/background.png);border-image: url(:/eventq/png/background.png);}"));
	
}
void webPopup::websize(int x, int y)
{
	this->setFixedSize(x, y);
	this->setModal(true);
}
void webPopup::setwebPopup(QString webstr)
{
	engine->page()->load(QUrl(webstr));
	this->show();
}
void webPopup::slots_close()
{
	this->hide();
	//if (CommonData()->strweb == QStringLiteral("zhuce"))
	//{
	//	WJMainWindow()->resize(550, 320);
	//}
}
void webPopup::stopweb()
{
	engine->hide();
}
void webPopup::webeject(QString str)
{
	m_webpopuppag = new webpopuppag();
	m_webpopuppag->setwebPopup(str);
	m_webpopuppag->setModal(true);
	m_webpopuppag->show();
}

webPopup::~webPopup()
{

}