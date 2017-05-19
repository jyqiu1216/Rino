#ifndef WEBSOCKETSERVICE_H
#define WEBSOCKETSERVICE_H


#include "WSMsgHandler.h"
#include <QThread>
#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>

#include <map>
#include <vector>

class WebSocketServer : public QThread, public WSMsgHandler_back
{
	Q_OBJECT
public:
	WebSocketServer(int _port, WSMsgHandler *_msgHandler);
	~WebSocketServer();
	
	virtual void GetSocketAndType(QWebSocket * pClient, Cweb_c_type _type);

	static WebSocketServer *inobgect();
	void sendMessage(QString massage, Cweb_c_type _type);
	void sendMessage(QByteArray massage, Cweb_c_type _type);
	
	void setEstablishwebsoocket(int post, WSMsgHandler *_msgHandler);//创建websocket
public slots:
	// 客户端建立连接事件回调
	void onClientConnect();

	// 客户端断开连接事件回调
	void onClientDisconnect();



private:
	bool initService(int port, WSMsgHandler *_msgHandler);


private:
	QWebSocketServer *m_webSocketSrv;

	/* 接收消息的处理类，使用者继承WebSocketMsgHandler进行消息处理操作 */
	WSMsgHandler *m_msgHandler;

	std::map<Cweb_c_type, QWebSocket*> m_Client_map;
	int webpost;
};

#endif // WEBSOCKETSERVICE_H
