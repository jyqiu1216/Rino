#include "webpopuppag.h"

webpopuppag::webpopuppag(QWidget *parent)
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
	engine->page()->setBackgroundColor(QColor(0.7, 0, 0, 255));
	bool u= connect((QObject*)bridge::instance(), SIGNAL(slgnal_webpagclose()), this, SLOT(slots_close())); 
	bool l = connect((QObject*)bridge::instance(), SIGNAL(slgnal_webejectpag(QString)), this, SLOT(webeject(QString)));
	bool b = connect((QObject*)bridge::instance(), SIGNAL(slgnal_setwebpagsize(int, int)), this, SLOT(websizepag(int, int)));
}
void webpopuppag::websizepag(int x,int y)
{
	this->setFixedSize(x, y);
	this->setModal(true);
}
void webpopuppag::setwebPopup(QString webstr)
{
	engine->page()->load(QUrl(webstr));
}
void webpopuppag::slots_close()
{
	this->hide();
}
void webpopuppag::stopweb()
{
	engine->hide();
	engine->page()->triggerAction(QWebEnginePage::Stop);
}

void webpopuppag::webeject(QString str)
{
	m_webPopup = new webPopup();
	m_webPopup->setwebPopup(str);
	m_webPopup->setModal(true);
	m_webPopup->show();
}

webpopuppag::~webpopuppag()
{

}
