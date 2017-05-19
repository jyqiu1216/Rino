#ifndef HOOKTHREAD_H
#define HOOKTHREAD_H

#include <QThread>

class hookthread : public QThread
{
	Q_OBJECT

public:
	hookthread(QObject *parent);
	~hookthread();

private:
	
};

#endif // HOOKTHREAD_H
