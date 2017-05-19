#include "bridge.h"
#include "web/WJMainWindow.h"
#include "common/Common.h"
bridge::bridge(QObject *parent)
	: QObject(parent)
{

}
//bridge::~bridge()
//{
//
//}

bridge* bridge::instance()
{
	static bridge s_obj;
	return &s_obj;
}

bridge::bridge()
{  
}
void bridge::setwebsize(int x,int y)
{
	emit slgnal_setwebsize(x,y);
}
void bridge::setwebpagsize(int x,int y)
{
	emit slgnal_setwebpagsize(x,y);
}

void bridge::webclose()
{
	emit slgnal_webclose();
}
void bridge::webpagclose()
{
	emit slgnal_webpagclose();
}
void bridge::webeject(QString str)
{
	webpopuppag *m_webpopuppag = new webpopuppag();
	m_webpopuppag->setwebPopup(str);
	m_webpopuppag->setModal(true);
	m_webpopuppag->setAttribute(Qt::WA_DeleteOnClose);
	m_webpopuppag->exec();
}
void bridge::webejectpag(QString str)
{
	webPopup *m_webPopup = new webPopup();
	m_webPopup->setwebPopup(str);
	m_webPopup->setModal(true);
	m_webPopup->setAttribute(Qt::WA_DeleteOnClose);
	m_webPopup->exec();
}
void bridge::retcclose()
{
	emit signal_sendResult(CommonData()->Resultspagecontent);
	return;
}