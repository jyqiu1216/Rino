#ifndef _WSMsgHandler_H_
#define _WSMsgHandler_H_

#include <QWebSocket>
#include <QtCore>
#include "WebSocketMessInfo.h"
enum Cweb_c_type
{
	web_c_unknow = 0,//未知
	web_c_hall = 1,//大厅
	web_c_wx = 2,//微信登录
	web_c_ls_inside = 3,//炉石内嵌
	web_c_lol_inside = 4,//lol内嵌 押注
	web_c_pt_login = 5,//普通登录
	web_c_lol_right_bottom = 6,//lol内嵌 屏幕右下角显示奖励情况
	web_c_ls_inside_yz = 7,//炉石内嵌 押注
	web_c_lol_task_result = 8,//lol任务赛结果页
	web_c_ls_inside_loading = 9,//炉石 游戏载入中界面

};

__interface WSMsgHandler_back
{
	virtual void GetSocketAndType(QWebSocket * pClient, Cweb_c_type _type) = 0;
};

// 继承该类处理WebSocket的消息
class WSMsgHandler : public QObject
{
public:
	WSMsgHandler(){ m_callback = nullptr; }

	~WSMsgHandler(){}

	void SetCallback(WSMsgHandler_back* pCallback){ m_callback = pCallback; }

	void sys_onRecvMsg(const QString &_msg)
	{
			QJsonParseError json_errorver;
			QJsonDocument parse_doucmentver = QJsonDocument::fromJson(_msg.toUtf8(), &json_errorver);
			if (json_errorver.error == QJsonParseError::NoError)
			{
				if (parse_doucmentver.isObject())
				{
					QJsonObject objectver = parse_doucmentver.object();
					QJsonValue value = objectver.value(QString("MSGID"));
					QJsonObject _TYPE;
					Cweb_c_type type;
					if (!objectver.contains("MSGID"))
					{
						return;
					}

					if (objectver.contains("TYPE"))
					{
						_TYPE = objectver.value(QString("TYPE")).toObject();
					}

					int iMSGID = value.toInt();
					if (WEB_SOCKET_1000 == iMSGID)
					{
						
						Cweb_c_type _type = (Cweb_c_type)_TYPE.value(QString("web_type")).toInt();
						type = _type;
						QWebSocket* webSocket = qobject_cast<QWebSocket *>(sender());
						qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("连接成功, web_type = ") << _type;

						if (nullptr != m_callback)
						{
							m_callback->GetSocketAndType(webSocket, _type);
						}

					}
					onRecvMsg(_msg, type);
					
				}
			}
	}

	virtual void onRecvMsg(const QString &_msg, Cweb_c_type _type) = 0;

	virtual void callbackConn(QWebSocket *_socket, bool _isSrv){}

	virtual void callbackDisConn(QWebSocket *_socket, bool _isSrv){}

private:
	WSMsgHandler_back* m_callback;
};

#endif // !_WSMsgHandler_H_
