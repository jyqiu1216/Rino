#ifndef BRIDGE_H
#define BRIDGE_H
#include "common/Common.h"
#include <QObject>
class bridge : public QObject
{
	Q_OBJECT

public:
	bridge(QObject *parent);
	static bridge* instance();
	
	//~bridge();
	
signals:
	void slgnal_webclose();
	void slgnal_webpagclose();

	void slgnal_webejectpag(QString);
	void slgnal_webeject(QString);

	void slgnal_setwebsize(int,int);
	void slgnal_setwebpagsize(int,int);
signals:
	// @brief:	用来连接JS的函数，前台执行函数需要返回结果时，回调时使用
	void signal_sendResult(const QString &text);
public slots:
	void webclose();
	void webpagclose();

	void webejectpag(QString);
	void webeject(QString);

	void setwebsize(int,int);
	void setwebpagsize(int,int);


	void retcclose();//返回结果页


private:
	bridge();
	
};

#endif // BRIDGE_H
