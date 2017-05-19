#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include "WSMsgHandler.h"

#include <QObject>
#include <QtWebSockets/QWebSocket>


class WebSocketClient : public QObject
{
	Q_OBJECT

public:
	WebSocketClient(QString _name="", QObject *parent=NULL);
	~WebSocketClient();
	
public:
	/**
	* \brief:	连接 webSocket 服务
	* \param:	[_url]: 服务url （QUrl(QStringLiteral("ws://localhost:1234"))）
	* \param:	[_msgHandler]: 消息处理对象指针
	* \return:	true: 连接成功;	false: 连接失败;
	*/
	bool connToServer(const QUrl &_url, WSMsgHandler *_msgHandler);


	/* 发送消息 */
	bool sendMsg(const QString& _msg);

	public slots:
	void onConnected();
	void onCloseClient();

	bool isValid() { return m_webSocket.isValid(); }

private:
	QWebSocket m_webSocket;

	WSMsgHandler *m_msgHandler;

	QUrl m_url;
};

#endif // WEBSOCKETCLIENT_H
