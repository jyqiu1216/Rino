
#include "websocket/WSMsgHandler.h"
#include "betwindowlol.h"
#include "hook/WSMessageId.h"



class WSMsgHandleLOL : public WSMsgHandler
{
public:
	WSMsgHandleLOL(BetWindowLoL* _mainWin)
		: m_mainWin(_mainWin)
		, m_sockClient(NULL)
	{
	}

	virtual void onRecvMsg(const QString &_msg, Cweb_c_type _type)
	{
		QWebSocket *pSocket = qobject_cast<QWebSocket *>(sender());

		printf("LOL内嵌窗口收到 Web Socket 消息: %s", _msg.toStdString().c_str());

		QJsonParseError jsonError;
		QJsonDocument jsonDoc = QJsonDocument::fromJson(_msg.toLocal8Bit(), &jsonError);
		if (jsonError.error != QJsonParseError::NoError)
		{
			printf("Web Socket message JSON parser error.");
			return;
		}

		if (!jsonDoc.isObject())
		{
			printf("Web Socket message JSON data error: is not json object.");
			return;
		}

		QJsonObject obj = jsonDoc.object();
		if (!obj.contains("MSGID"))
		{
			printf("BetWindowLoL [WSMsgHandlerHS::onRecvMsg]: JSON data error: [msgid] is not exist.");
			return;
		}

		int msgId = obj["MSGID"].toInt();

		switch (msgId)
		{
			//case GAME_EVENT_AM_BEGIN:
			//	m_mainWin->beginLoading();
			//	break;
			//case GAME_EVENT_GAME_BEGIN:
			//	m_mainWin->startGame();
			//	break;
			//case WS_MSGID_2007:
			//	break;

		default:
			break;
		}
	}

	virtual void callbackConn(QWebSocket *_socket, bool _isSrv)
	{
		//m_mainWin->setWebSockClient(_socket);
		m_sockClient = _socket;
		printf("BetWindowLoL： H5 Websoekcet连接成功\n");
	}

	virtual void callbackDisConn(QWebSocket *_socket, bool _isSrv)
	{
		m_sockClient = nullptr;
	}

	/**
	*	消息格式：{"MSGID":"2000"}
	*/
	void sendMsg(QString& _msg, QWebSocket* _websocket = nullptr)
	{
		QJsonObject jsonRoot;
		jsonRoot.insert("MSGID", _msg);

		QJsonDocument jsonDoc;
		jsonDoc.setObject(jsonRoot);

		QString msg = jsonDoc.toJson(QJsonDocument::Compact);

		if (_websocket)
		{
			_websocket->sendTextMessage(msg);
		}
		else if (m_sockClient)
		{
			m_sockClient->sendTextMessage(msg);
		}

		printf("WebSocket : Server send msg: %s\n", msg.toStdString().c_str());
	}

public:
	BetWindowLoL *m_mainWin;
	QWebSocket *m_sockClient;
};