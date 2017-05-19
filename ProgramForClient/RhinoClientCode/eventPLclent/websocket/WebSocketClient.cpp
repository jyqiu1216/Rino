#include "WebSocketClient.h"

WebSocketClient::WebSocketClient(QString _name, QObject *parent)
	: QObject(parent)
	, m_msgHandler(NULL)
{
	m_webSocket.setObjectName(_name);
	
	/* 设置连接和断连的信号槽 */
	connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
	connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onCloseClient);
}

WebSocketClient::~WebSocketClient()
{

}

//#include <QEventLoop>

bool WebSocketClient::connToServer(const QUrl &_url, WSMsgHandler *_msgHandler)
{
	/* 保存消息处理对象指针 */
	m_msgHandler = _msgHandler;
	
	/* 保存服务url */
	m_url = _url;


	/* 连接到 webSocket */
	m_webSocket.open(_url);
	

	//if (m_webSocket.isValid())
	return true;
}

#include <Windows.h>
void WebSocketClient::onConnected()
{
	OutputDebugStringA("WebSocket: connect server successful!");
	if (m_msgHandler)
	{
		connect(&m_webSocket, &QWebSocket::textMessageReceived, m_msgHandler, &WSMsgHandler::onRecvMsg);
		m_msgHandler->callbackConn(&m_webSocket, false);
	}
}


bool WebSocketClient::sendMsg(const QString& _msg)
{
	if (!m_webSocket.isValid())
	{
		qDebug() << "WebSocket 不可用， (" << m_url.toString() << ")";
		return false;
	}

	m_webSocket.sendTextMessage(_msg);
	qDebug() << "Send MSG:" << _msg;
	return true;
}


void WebSocketClient::onCloseClient()
{
	m_webSocket.close();
}