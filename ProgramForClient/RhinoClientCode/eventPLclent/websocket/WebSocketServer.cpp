#include "WebSocketServer.h"
//#include "DataDrocessing.h"
#include "common/WJSpinlock.h"
WJ::CSpinlock g_logLock_WebSocket;

WebSocketServer::WebSocketServer(int _port, WSMsgHandler *_msgHandler)
	: QThread()
	, m_webSocketSrv(NULL)
{

}

WebSocketServer *WebSocketServer::inobgect()
{
	static WebSocketServer* s_obj = new WebSocketServer(NULL, NULL);
	return s_obj; 
}

void WebSocketServer::setEstablishwebsoocket(int post, WSMsgHandler *_msgHandler)//创建websocket
{
	initService(post, _msgHandler);
	webpost = post;
}

WebSocketServer::~WebSocketServer()
{
	g_logLock_WebSocket.Lock();
	auto iter = m_Client_map.begin();
	while (iter != m_Client_map.end())
	{
		iter->second->deleteLater();
		++iter;
	}

	m_Client_map.clear();
	g_logLock_WebSocket.UnLock();
}

void WebSocketServer::GetSocketAndType(QWebSocket * pClient, Cweb_c_type _type)
{
	g_logLock_WebSocket.Lock();
	auto _iter = m_Client_map.find(_type);
	if (_iter != m_Client_map.end())
	{//存在
		if (_iter->second != pClient)
		{//不一致
			_iter->second->deleteLater();
			_iter->second = pClient;
		}
	}
	else
	{//不存在
		m_Client_map.insert(std::map<Cweb_c_type, QWebSocket *>::value_type(_type, pClient));
	}
	g_logLock_WebSocket.UnLock();
}

void WebSocketServer::sendMessage(QString massage, Cweb_c_type _type)
{
	g_logLock_WebSocket.Lock();
	auto _iter = m_Client_map.find(_type);
	if (_iter != m_Client_map.end())
	{
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << "_type=" << _type << "websocket发送消息:" << massage << endl;
		_iter->second->sendTextMessage(massage);
	}
	else
	{
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << "_type=" << _type << "websocket发送消息(错误):" << massage << endl;
	}
	g_logLock_WebSocket.UnLock();
}

void WebSocketServer::sendMessage(QByteArray massage, Cweb_c_type _type)
{
	g_logLock_WebSocket.Lock();
	auto _iter = m_Client_map.find(_type);
	if (_iter != m_Client_map.end())
	{
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << "_type=" << _type << "websocket发送消息:" << massage << endl;
		_iter->second->sendTextMessage(massage);
	}
	else
	{
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << "_type=" << _type << "websocket发送消息(错误):" << massage << endl;
	}
	g_logLock_WebSocket.UnLock();
}


bool WebSocketServer::initService(int port, WSMsgHandler *_msgHandler)
{
	m_msgHandler = _msgHandler;
	m_webSocketSrv = new QWebSocketServer(QStringLiteral("WebSocket Server"), QWebSocketServer::NonSecureMode);

	if (m_webSocketSrv->listen(QHostAddress::Any, port))
	{
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << "WebSocket listening on port: " << port;
		connect(m_webSocketSrv, &QWebSocketServer::newConnection, this, &WebSocketServer::onClientConnect);
		return true;
	}
	else
	{
		return true;
	}
}

void WebSocketServer::onClientConnect()
{
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << "websocket新连接" << endl;
	QWebSocket *pSocket = m_webSocketSrv->nextPendingConnection();
	if (nullptr != pSocket)
	{
		connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::onClientDisconnect);
		if (m_msgHandler)
		{
			connect(pSocket, &QWebSocket::textMessageReceived, m_msgHandler, &WSMsgHandler::sys_onRecvMsg);
			m_msgHandler->callbackConn(pSocket, true);
		}
	}
}


void WebSocketServer::onClientDisconnect()
{
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << "websocket断线" << endl;
	QWebSocket* webSocket = qobject_cast<QWebSocket *>(sender());
	if (webSocket)
	{
		if (m_msgHandler)
		{
			m_msgHandler->callbackDisConn(webSocket, false);
		}

		g_logLock_WebSocket.Lock();
		auto _iter = m_Client_map.begin();
		while (_iter != m_Client_map.end())
		{
			if (_iter->second == webSocket)
			{
				_iter = m_Client_map.erase(_iter);
				break;
			}

			++_iter;
		}		
		g_logLock_WebSocket.UnLock();
		webSocket->deleteLater();
	}
}
