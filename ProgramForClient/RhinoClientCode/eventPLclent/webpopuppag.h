#ifndef WEBPOPUPPAG_H
#define WEBPOPUPPAG_H

#include <QDialog>
#include "ui_webpopuppag.h"
#include "common\Common.h"
class QWebChannel;
class QWebEngineView;
class webPopup;
class webpopuppag : public QDialog
{
	Q_OBJECT

public:
	webpopuppag(QWidget *parent = 0);
	~webpopuppag();
	void setwebPopup(QString webstr);
	void stopweb();
	

private slots:
	void slots_close();
	void websizepag(int,int);
	void webeject(QString);
signals:
private:
	Ui::webpopuppag ui;
	QWebEngineView* engine;//Ç¶Ì×ÍøÕ¾
	QWebChannel *channel;
	webPopup *m_webPopup;
};

#endif // WEBPOPUPPAG_H
