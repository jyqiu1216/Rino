#ifndef WEBPOPUP_H
#define WEBPOPUP_H
#include <QDialog>
#include "ui_webpopup.h"
#include "common\Common.h"
class QWebChannel;
class QWebEngineView;
class webpopuppag;
class webPopup : public QDialog
{
	Q_OBJECT

public:
	webPopup(QWidget *parent = 0);
	~webPopup();
	void setwebPopup(QString webstr);
	void stopweb();

private slots:
	void slots_close();
	void webeject(QString);
	void websize(int,int);
signals:

private:
	Ui::webPopup ui;
	QWebEngineView* engine;//Ç¶Ì×ÍøÕ¾
	QWebChannel *channel;
	webpopuppag *m_webpopuppag;
protected:
	//@brief oldPos
	QPoint oldPPos;
	// @brief press
	bool press;
};

#endif // WEBPOPUP_H
