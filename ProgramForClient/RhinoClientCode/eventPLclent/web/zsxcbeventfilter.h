#ifndef ZSXCBEVENTFILTER_H
#define ZSXCBEVENTFILTER_H

#include <QApplication>
#include <QAbstractNativeEventFilter>



class ZsXcbEventFilter: public QAbstractNativeEventFilter
{

public:
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *) Q_DECL_OVERRIDE;
};

#endif // ZSXCBEVENTFILTER_H
