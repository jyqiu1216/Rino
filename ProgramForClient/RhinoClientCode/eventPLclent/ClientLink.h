#ifndef CLIENTLINK_H
#define CLIENTLINK_H

#include <QDialog>
#include <QTimer>
#include "common\Common.h"
#include "ui_ClientLink.h"
class CClientLink : public QDialog
{
	Q_OBJECT

public:
	CClientLink(QWidget *parent = 0);
	~CClientLink();
	static CClientLink* getInstance();
	bool isllink;
	void onStart();	//连接启动客户端

public slots:
	void slot_landlink(bool);
private:
	Ui::ClientLink ui;

};

#define  ClientLink()	CClientLink::getInstance()

#endif // CLIENTLINK_H
