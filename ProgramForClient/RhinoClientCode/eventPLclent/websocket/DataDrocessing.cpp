#include "DataDrocessing.h"
#include <QtCore>
#include "common/Common.h"
#include "common/Log.h"
#include "WebSocketMessInfo.h"
#include "common/WJGameInfo.h"
#include "clientsocket/commonfunction_c.h"
#include "websocket\WebSocketServer.h"
#include "WJPlatformLogic.h"
#include "common/Common.h"
#include "ClientLink.h"
#include "Qthread/RoomLogic.h"
#include "web/WJMainWindow.h"
#include "common/LoadWindowsMess.h"

DataDrocessing::DataDrocessing()
//: QObject(parent)
{

}

DataDrocessing::~DataDrocessing()
{

}

DataDrocessing *DataDrocessing::inobgect()
{
	static DataDrocessing s_obj;
	return &s_obj;
}

void DataDrocessing::callbackConn(QWebSocket *_socket, bool _isSrv)
{
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1000);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	_socket->sendTextMessage(json_json);
}

void DataDrocessing::Processing(const QString &_msg, Cweb_c_type _type)//websocket消息处理
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
			if (!objectver.contains("MSGID"))
			{
				return;
			}

			if (objectver.contains("TYPE"))
			{
				_TYPE = objectver.value(QString("TYPE")).toObject();
			}

			int iMSGID = value.toInt();
			switch (iMSGID)
			{
			case WEB_SOCKET_1027:
			{//告知当前是什么游戏id
				web_Processing_1027(_TYPE);
			}break;
			case WEB_SOCKET_1001://登陆
			{
				web_Processing_1001(_TYPE);//接受到web登陆消息
			}break;
			case WEB_SOCKET_1004://任务赛大厅数据
			{
				web_Processing_1004(_TYPE);//接受到web请求大厅消息
			}break;
			case WEB_SOCKET_1005://任务赛报名
			{
				web_Processing_1005(_TYPE);//接受到web请求任务赛报名消息
			}break;
			case WEB_SOCKET_1006://任务赛详情数据
			{
				web_Processing_1006(_TYPE);//接受到web请求任务赛详情数据
			}break;
			case WEB_SOCKET_1031:
			{
				web_Processing_1031(_TYPE);
			}break;
			case WEB_SOCKET_9001:
			{//微信登陆二维码扫描窗口打开交付
				web_Processing_9001();
			}break;
			case WEB_SOCKET_9002:
			{//微信登陆二维码扫描窗口关闭交付
				web_Processing_9002(_TYPE);
			}break;
			case WEB_SOCKET_9003:
			{//微信登陆二维码扫描拿到用户信息进行转发交付
				web_Processing_9003(_TYPE);
			}break;
			case WEB_SOCKET_9004:
			{//微信登陆二维码扫描通知大厅使用账户登陆交付
				web_Processing_9004();
			}break;
			case WEB_SOCKET_9005:
			{//微信登陆二维码扫描通知大厅使用key+userid登陆业务服务器交付
				web_Processing_9005(_TYPE);
			}break;
			case WEB_SOCKET_9006:
			{//设置中修改密码
				web_Processing_9006(_TYPE);
			}break;
			case WEB_SOCKET_1013:
			{//点击比赛节点获取房间列表交付
				web_Processing_1013(_TYPE);
			}break;
			case WEB_SOCKET_1014:
			{//离开比赛详情页面交付
				web_Processing_1014(_TYPE);
			}break;
			case WEB_SOCKET_7000:
			{//H5索要注册需要的推广id
				web_Processing_7000();
			}break;
			case WEB_SOCKET_1028:
			{//告知当前客户端版本
				web_Processing_1028();
			}break;
			case WEB_SOCKET_9013:
			{//注册窗口打开交付
				WJMainWindow()->showRegistrationPage(true);
			}break;
			case WEB_SOCKET_1029:
			{//关闭注册窗口
				WJMainWindow()->showRegistrationPage(false);
			}break;
			case WEB_SOCKET_3012:
			{//断线重回自动加入房间后锁住开赛按钮并启动英雄联盟

				web_Processing_3012(_TYPE);
			}break;
			case WEB_SOCKET_6000:
			{
				OnGetWebAddr();
			}break;
			case WEB_SOCKET_9011:
			{//普通登录转微信登录
				web_Processing_9011();
			}break;
			case WEB_SOCKET_9012:
			{//普通登录窗口关闭交付
				web_Processing_9012();
			}break;
			case WEB_SOCKET_9010:
			{//玩家是否查看对阵图
				web_Processing_9010(_TYPE);
			}break;
			case WEB_SOCKET_9016:
			{//炉石玩家pick英雄
				web_Processing_9016(_TYPE);
			}break;
			case WEB_SOCKET_9021:
			{//炉石玩家ban英雄
				web_Processing_9021(_TYPE);
			}
			case WEB_SOCKET_3014:
			{//LOL任务赛结果页关闭
				WJMainWindow()->onShowGameTaskLOL(false);
			}break;
			case WEB_SOCKET_1000:
			{//与LOL任务赛结果页成功建立连接
				if (_type == web_c_lol_task_result)
				{
					WJ_SAFE_QPostMessage(WJMainWindow(), WM_HALL_MESS_1015, 0, 0);
				}
			}break;
			case WEB_SOCKET_1037:
			{//联赛结束后点击确认按钮关闭炉石内嵌窗口
				if (nullptr != WJMainWindow()->m_pEmbedWindowHS)
				{
					WJMainWindow()->m_pEmbedWindowHS->close();
					delete WJMainWindow()->m_pEmbedWindowHS;
					WJMainWindow()->m_pEmbedWindowHS = nullptr;
				}
			}break;
			case WEB_SOCKET_9017:
			{//联赛房间内玩家发送聊天消息
				web_Processing_9017(_TYPE);
			}break;
			case WEB_SOCKET_9019:
			{//重新加载大厅页面
				std::string str_url = clientconfig().GetData()->h5_server_address + "index.html";
				WJMainWindow()->openUrl(QUrl(QString(str_url.c_str())));
			}
			default:
			{
				if (WEB_SOCKET_1034 == iMSGID)
				{
					int a = 0;
					a = 1;
				}
				IBaseMyDataDrocessing*p = (IBaseMyDataDrocessing*)CommonData()->GetMyDataDrocessing(CommonData()->m_NowGame);
				if (nullptr != p)
				{
					p->Processing(iMSGID, _TYPE);
				}
				break;
			}
			}
		}
	}
}

//点击比赛节点获取房间列表交付
void DataDrocessing::web_Processing_1013(QJsonObject TYPE)
{
	int icontestid = TYPE.value(QString("CONTESTID")).toString().toInt();

	if (TYPE.contains("MODE"))
	{
		int inSolo = TYPE.value("MODE").toInt();
		WJMainWindow()->m_LolSoloId = inSolo;
	}

	CommonData()->m_NowContest = icontestid;
	CommonData()->GetNowContestInfo();

	SystemLog()->LogToFile("点击比赛节点获取房间列表交付,比赛id = %d", CommonData()->m_NowContest);
	SystemLog()->LogToFile("sdp_test c 点击比赛节点获取房间列表交付---CommonData()->m_NowContest=%d", CommonData()->m_NowContest);
}

//离开比赛详情页面交付
void DataDrocessing::web_Processing_1014(QJsonObject TYPE)
{
	QJsonDocument documentpack;
	documentpack.setObject(TYPE);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));

	int modeId = (TYPE.value(QString("modeId")).toInt());
	SystemLog()->LogToFile("离开比赛详情页面交付");
	SystemLog()->LogToFile(json_json);

	if (modeId == 2)
	{
		return;
	}


	RoomLogic()->Sys_OnLeave();
}

void DataDrocessing::web_Processing_1031(QJsonObject TYPE)
{
	//炉石游戏中判断
	if (nullptr != WJMainWindow()->m_pEmbedWindowHS)
	{
		RoomLogic()->bInGame = true;
	}
	//LOL游戏中判断
	DWORD pid = GetProcessIdByName(TEXT("LolClient.exe"));
	if (pid != 0)
		RoomLogic()->bInGame = true;

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1031);
	sendjsonpart.insert(QStringLiteral("GAMESTATE"), RoomLogic()->bInGame);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

//微信登陆二维码扫描窗口打开交付
void DataDrocessing::web_Processing_9001()
{
	SystemLog()->LogToFile("微信登陆二维码扫描窗口打开交付");
	WJMainWindow()->onShowWXwind(true);
}

//微信登陆二维码扫描窗口关闭交付
void DataDrocessing::web_Processing_9002(QJsonObject TYPE)
{
	SystemLog()->LogToFile("微信登陆二维码扫描窗口关闭交付");
	WJMainWindow()->onShowWXwind(false);
	QJsonObject sendjsonpart;
	QJsonObject json;
	json.insert(QStringLiteral("status"), TYPE.value("status").toString());
	sendjsonpart.insert(QStringLiteral("TYPE"), json);
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_9002);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

//微信登陆二维码扫描拿到用户信息进行转发交付
void DataDrocessing::web_Processing_9003(QJsonObject TYPE)
{
	SystemLog()->LogToFile("微信登陆二维码扫描拿到用户信息进行转发交付");
	QJsonObject sendjsonpart;
	QJsonObject json;
	json.insert(QStringLiteral("KEY"), TYPE.value("KEY").toString());
	sendjsonpart.insert(QStringLiteral("TYPE"), json);
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_9003);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

	if (TYPE.value("KEY").isObject()) {

		std::memcpy(WJ::PlatformLogic().m_logininfo_WX.wx_key, TYPE.value("KEY").toString().toStdString().c_str(), sizeof(WJ::PlatformLogic().m_logininfo_WX.wx_key));
	}
}

//通知大厅使用普通登陆交付
void DataDrocessing::web_Processing_9004()
{
	SystemLog()->LogToFile("微信登陆二维码扫描通知大厅使用账户登陆交付");
	WJMainWindow()->onShowWXwind(false);
	SystemLog()->LogToFile("普通登录窗口打开交付");
	WJMainWindow()->onShowLogin(true);

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_9004);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

//微信登陆二维码扫描通知大厅使用key+userid登陆业务服务器交付
void DataDrocessing::web_Processing_9005(QJsonObject TYPE)
{
	SystemLog()->LogToFile("微信登陆二维码扫描通知大厅使用key+userid登陆业务服务器交付");
	WJ::PlatformLogic().m_logininfo_WX.userid = TYPE.value(QString("USERID")).toInt();
	std::memcpy(WJ::PlatformLogic().m_logininfo_WX.username, TYPE.value("USERNAME").toString().toStdString().c_str(), sizeof(WJ::PlatformLogic().m_logininfo_WX.username));

	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("点击登陆按钮");
	WJ::PlatformLogic().m_bWX_login = true;//微信登陆标记
	WJ::PlatformLogic().close();
	//{
		ClientLink()->onStart();
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("链接不成功重新登陆");
	//}
	//else
	//{
	//	WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_login, (UINT)WJ::e_hs_ass_login_WX, &WJ::PlatformLogic().m_logininfo_WX, sizeof(WJ::PlatformLogic().m_logininfo_WX));
	//	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("链接成功发送【微信】登录");
	//}
}

void DataDrocessing::web_Processing_9006(QJsonObject TYPE)
{

	QString tUserId;
	QString tOlePassWord;
	QString tNewPassWord;

	if (TYPE.contains("USERID"))
	{
		//tUserId = QString("%1").arg(TYPE.value("USERID").toInt());
		tUserId = TYPE.value("USERID").toString();
	}

	if (TYPE.contains("oldPSW"))
	{
		tOlePassWord = TYPE.value("oldPSW").toString();
	}

	if (TYPE.contains("newPSW"))
	{
		tNewPassWord = TYPE.value("newPSW").toString();
	}

	//创建一个管理器
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	//连接请求结束信号
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	//SystemLog::getInstance()->writeLog(SystemLog::E_LOG_DEBUG, "WJTestLOG.log", "1", 1, "发送修改密码");
	//创建一个请求
	QNetworkRequest request;
	QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/changePassword?userId=") + tUserId + QStringLiteral("&originalPassword=") + tOlePassWord + QStringLiteral("&newpassword=") + tNewPassWord;
	request.setUrl(QUrl(str));

	SystemLog()->LogToFile("修改密码请求:");
	SystemLog()->LogToFile(str.toStdString().c_str());
	//发送GET请求
	QNetworkReply *reply = manager->get(request);
	//连接响应时返回数据信号
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));//关键之处1
	loop.exec();//关键之处2
	/////////////////////////////////////////////
	QByteArray data = reply->readAll();//网页内容输出

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_9006);
	QJsonParseError json_errorver;
	QJsonDocument JsionDecument = QJsonDocument::fromJson(data, &json_errorver);
	sendjsonpart.insert(QStringLiteral("TYPE"), JsionDecument.object());
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

//告知当前是什么游戏id
void DataDrocessing::web_Processing_1027(QJsonObject TYPE)
{
	CommonData()->m_NowGame = (E_GameID)TYPE.value(QString("GAME")).toInt();
	SystemLog()->LogToFile("当前选中的游戏ID=%d", CommonData()->m_NowGame);
	SystemLog()->LogToFile("sdp_test 告知当前游戏id=%d", CommonData()->m_NowGame);
}

//H5索要注册需要的推广id
void DataDrocessing::web_Processing_7000()
{
	std::string str_file_update = GetAppPath().c_str();
	str_file_update += "\\update.ini";
	int isource = GetPrivateProfileIntA("config", "source", 0, str_file_update.c_str());//推广id
	QString sSource = QString::number(isource, 10);
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_7001);
	sendjsonpart.insert(QStringLiteral("TYPE"), sSource);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_pt_login);
}

//告知当前客户端版本
void DataDrocessing::web_Processing_1028()
{
	std::string str_file_update = GetAppPath().c_str();
	str_file_update += "\\update.ini";
	int iversion = GetPrivateProfileIntA("config", "version", 0, str_file_update.c_str());//主版本号
	int ismallversion = GetPrivateProfileIntA("config", "smallversion", 0, str_file_update.c_str());//子版本号
	QString sVersion = QString::number(iversion, 10);
	sVersion += QStringLiteral("-");
	sVersion += QString::number(ismallversion, 10);

	int isource = GetPrivateProfileIntA("config", "source", 0, str_file_update.c_str());//推广id
	QString sSource = QString::number(isource, 10);

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1028);
	sendjsonpart.insert(QStringLiteral("TYPE"), sVersion);
	sendjsonpart.insert(QStringLiteral("ID"), sSource);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

void DataDrocessing::web_Processing_1006(QJsonObject TYPE)//接受到web请求任务赛详情数据
{
	QString TASKID;
	QString USERID;
	if (TYPE.contains("TASKID"))
	{
		TASKID = TYPE.value("TASKID").toString();
	}
	if (TYPE.contains("USERID"))
	{
		USERID = QString("%1").arg(TYPE.value("USERID").toInt());
	}

	QEventLoop loop;
	QNetworkAccessManager m_networkAccessManger;
	QNetworkRequest request;
	QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/task/loldetail?userId=") + USERID + QStringLiteral("&taskId=") + TASKID;
	request.setUrl(QUrl(str));

	SystemLog()->LogToFile("请求任务赛详情数据:");
	SystemLog()->LogToFile(str.toStdString().c_str());
	//发送GET请求
	QNetworkReply *reply = m_networkAccessManger.get(request);

	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));//关键之处1
	loop.exec();//关键之处2
	/////////////////////////////////////////////
	QByteArray data = reply->readAll();//网页内容输出

	SystemLog()->LogToFile("返回任务赛详情数据:");
	SystemLog()->LogToFile(data.toStdString().c_str());

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1006);
	sendjsonpart.insert(QStringLiteral("TYPE"), data.toStdString().c_str());
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

	WJ_SAFE_QPostMessage(WJMainWindow(), WM_HALL_MESS_1009, 0, 0);
	reply->deleteLater();
}
void DataDrocessing::web_Processing_1005(QJsonObject TYPE)//接受到web请求任务赛报名消息
{
	QString TASKID;
	QString USERID;
	if (TYPE.contains("TASKID"))
	{
		TASKID = QString("%1").arg(TYPE.value("TASKID").toInt());
	}
	if (TYPE.contains("USERID"))
	{
		USERID = TYPE.value("USERID").toString();
	}
	if (TYPE.contains("SERVER"))
	{
		CommonData()->m_NowErea = TYPE.value("SERVER").toInt();
	}

	QEventLoop loop;
	QNetworkAccessManager m_networkAccessManger;
	QNetworkRequest request;
	QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/task/apply?userId=") + USERID + QStringLiteral("&taskId=") + TASKID;
	request.setUrl(QUrl(str));

	SystemLog()->LogToFile("请求任务赛报名数据:");
	SystemLog()->LogToFile(str);
	//发送GET请求
	QNetworkReply *reply = m_networkAccessManger.get(request);
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));//关键之处1
	loop.exec();//关键之处2
	/////////////////////////////////////////////
	QByteArray data = reply->readAll();//网页内容输出
	SystemLog()->LogToFile("返回任务赛报名数据:");
	SystemLog()->LogToFile(data.toStdString());

	QJsonParseError json_errorver;
	QJsonDocument parse_doucmentver = QJsonDocument::fromJson(data, &json_errorver);
	if (json_errorver.error == QJsonParseError::NoError)
	{
		if (parse_doucmentver.isObject())
		{
			QJsonObject objectver = parse_doucmentver.object();
			if (objectver.contains("code"))
			{
				if (objectver.value("code").toInt() == 1)//报名成功
				{
					CommonData()->match = MatchType::LOLMission;
				}
			}

		}
	}

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1005);
	sendjsonpart.insert(QStringLiteral("TYPE"), QString::fromUtf8(data).toStdString().c_str());
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

	SystemLog()->LogToFile("发送任务赛报名消息结果到前端,数据:");
	SystemLog()->LogToFile(json_json.toStdString().c_str());
	reply->deleteLater();
}
void DataDrocessing::web_Processing_1004(QJsonObject TYPE)//接受到web请求任务赛大厅消息
{
	QString SERVER;
	QString GAME;
	QString USERID;

	if (TYPE.contains("GAME"))
	{
		GAME = QString("%1").arg(TYPE.value("GAME").toInt());
	}
	if (TYPE.contains("USERID"))
	{
		USERID = QString("%1").arg(TYPE.value("USERID").toInt());
	}

	QEventLoop loop;
	QNetworkAccessManager m_networkAccessManger;
	QNetworkRequest request;
	QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/task/list?userId=") + USERID + QStringLiteral("&game=") + GAME;
	request.setUrl(QUrl(str));
	//qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("请求任务赛大厅数据:") << str;
	//发送GET请求
	QNetworkReply *reply = m_networkAccessManger.get(request);

	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));//关键之处1
	loop.exec();//关键之处2
	/////////////////////////////////////////////
	QByteArray data = reply->readAll();//网页内容输出
	//qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("返回任务赛大厅数据:") << QString::fromUtf8(data).toStdString().c_str();

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1004);
	sendjsonpart.insert(QStringLiteral("TYPE"), data.data());
	//sendjsonpart.insert(QStringLiteral("TYPE"), data.toStdString().c_str());
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	SystemLog()->LogToFile("web请求任务赛大厅消息,大厅数据:");
	SystemLog()->LogToFile(json_json.toStdString().c_str());
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

	reply->deleteLater();
}

void DataDrocessing::web_Processing_1001(QJsonObject TYPE)//接受到web登陆消息
{
	SystemLog()->LogToFile("处理web登陆消息");
	QString NAME;
	QString PASSWORD;
	if (TYPE.contains("NAME"))
	{
		NAME = TYPE.value("NAME").toString();
	}
	if (TYPE.contains("PASSWORD"))
	{
		PASSWORD = TYPE.value("PASSWORD").toString();
	}

	std::memcpy(WJ::PlatformLogic().m_logininfo.username, NAME.toLatin1().data(), sizeof(WJ::PlatformLogic().m_logininfo.username));
	std::memcpy(WJ::PlatformLogic().m_logininfo.password, PASSWORD.toLatin1().data(), sizeof(WJ::PlatformLogic().m_logininfo.password));
	WJ::PlatformLogic().close();
	//if (!WJ::PlatformLogic().isConnect())
	//{
		ClientLink()->onStart();
	//}
	//else
	//{
	//	WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_login, (UINT)WJ::e_hs_ass_login, &WJ::PlatformLogic().m_logininfo, sizeof(WJ::PlatformLogic().m_logininfo));
	//}
}

void DataDrocessing::SetRoute(QString &str_result, QString &str_name)
{
	QString str_route;//文件路径
	while (1)
	{
		str_route = QFileDialog::getOpenFileName();
		if (str_route == QStringLiteral(""))
		{//没设置，离开
			break;
		}
		else
		{
			if (!CheckRoute(str_route, str_name))
			{//提示设置错误
				WJ_SAFE_QBoxMess(WJMainWindow(), QStringLiteral("游戏路径设置错误,请重新设置"));
			}
			else
			{//成功则离开
				break;
			}
		}
	};



	str_result = str_route;
}
void DataDrocessing::OnGetWebAddr()
{
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_6000);
	sendjsonpart.insert(QStringLiteral("TYPE"), Q_WEB_ROOT_ADDRESS);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}
bool DataDrocessing::CheckRoute(QString &str_route, QString &str_name)
{
	QFile file(str_route);//打开路径下指定文件
	QString str = str_route;
	str = str.mid(str.lastIndexOf("/") + 1, str.length());
	str = str.mid(0, str.indexOf("."));
	if (str != str_name || !file.exists())
	{//提示设置错误
		return false;
	}

	return true;
}

//锁住比赛详情页按钮
void DataDrocessing::OnContestButLock()
{
	//比赛按钮解锁
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1015);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}
//解锁比赛详情页按钮
void DataDrocessing::OnContestButUnlock()
{
	//比赛按钮解锁
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1016);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

//通知h5，路径不对，请求再次设置游戏路径
void DataDrocessing::HTTPMessageProcessing_1012()
{
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1012);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

//获得游戏路径交付
void DataDrocessing::HTTPMessageProcessing_1010(QString &data)
{
	QJsonObject json;
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1010);
	{
		json.insert("GAMENAME", data.toStdString().c_str());
	}
	sendjsonpart.insert(QStringLiteral("TYPE"), json);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}
void  DataDrocessing::web_Processing_3012(QJsonObject TYPE)
{
	OnContestButLock();
	int serverId;
	if (TYPE.contains("SEVERID"))
		serverId = TYPE.value("SEVERID").toInt();

	bool bExist_route = false;//路径是否存在
	bExist_route = DataDrocessing::inobgect()->CheckRoute(CommonData()->lolroute, QStringLiteral("Client"));
	WJAM::LOGININFO stLogin = { 0 };
	stLogin.gameid = WJAM::GAME_ID_LOL;    /*游戏*/
	strcpy_s(stLogin.account, "");
	strcpy_s(stLogin.password, "");
	stLogin.serverid = serverId;    /*服务器ID*/
	strcpy_s(stLogin.path, CommonData()->lolroute.toLocal8Bit());   /*可执行文件路径*/
	//  int retlol = AutomateManager::executeLogin(stLogin, nullptr);
	if (bExist_route)
	{
		AutomateManager::executeLogin(stLogin, nullptr);
	}
	else
		DataDrocessing::inobgect()->HTTPMessageProcessing_1012();

}
//普通登录转微信登录
void DataDrocessing::web_Processing_9011()
{
	SystemLog()->LogToFile("普通登录转微信登录");
	WJMainWindow()->onShowLogin(false);
	WJMainWindow()->onShowWXwind(true);
}
//普通登录窗口关闭交付
void DataDrocessing::web_Processing_9012()
{
	SystemLog()->LogToFile("普通登录窗口关闭交付");
	WJMainWindow()->onShowLogin(false);
}
//玩家是否查看对阵图
void DataDrocessing::web_Processing_9010(QJsonObject TYPE)
{
	int ViewDraw;
	int EventId;
	if (TYPE.contains("VIEWDRAW"))
	{
		ViewDraw = TYPE.value("VIEWDRAW").toInt();
	}
	if (TYPE.contains("EVENTID"))
	{
		EventId = TYPE.value("EVENTID").toInt();
	}
	if (1 == ViewDraw)
	{
		WJ::struct_data_hs_mess_event_clash _ID;
		_ID.eventId = EventId;
		WJ::PlatformLogic().sendDataL(WJ::e_hs_mess_event, WJ::e_hs_ass_event_clash, WJ::e_hs_hcid_event_clash_looking, &_ID, sizeof(_ID));
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("玩家正在观察对阵图");
	}
	else if (0 == ViewDraw)
	{
		WJ::PlatformLogic().sendDataL(WJ::e_hs_mess_event, WJ::e_hs_ass_event_clash, WJ::e_hs_hcid_event_unlooking);
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("玩家离开对阵图界面");
	}
}
//炉石联赛玩家pick英雄
void DataDrocessing::web_Processing_9016(QJsonObject TYPE)
{
	WJ::_struct_data_hs_mess_pick tPick;

	if (TYPE.contains("PICK_HEROS"))
	{
		QJsonArray TempArray = TYPE.value("PICK_HEROS").toArray();
		QJsonDocument document;
		document.setArray(TempArray);
		QByteArray byte_array = document.toJson(QJsonDocument::Compact);
		::memcpy(tPick.chPickData, byte_array, byte_array.length());
	} 
	if (TYPE.contains("SELF_ID"))
	{
		tPick.uFromId = TYPE.value("SELF_ID").toInt();
	}
	if (TYPE.contains("OPPONENT_ID"))
	{
		tPick.uToId= TYPE.value("OPPONENT_ID").toInt();
	}

	WJ::PlatformLogic().sendData(WJ::e_hs_mess_pick, WJ::e_hs_ass_event_pick, &tPick, sizeof(tPick));
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("联赛房间PICK英雄.");
}

//炉石联赛玩家ban英雄
void DataDrocessing::web_Processing_9021(QJsonObject TYPE)
{
	
}

//联赛房间内玩家发送聊天消息
void DataDrocessing::web_Processing_9017(QJsonObject TYPE)
{
	WJ::struct_data_hs_mess_chat m_data;
	if (TYPE.contains("CHAT_DATA"))
	{
		sprintf_s(m_data.chat_data, "%s", TYPE.value("CHAT_DATA").toString().toStdString().c_str());
	}
	if (TYPE.contains("FROMID"))
	{
		m_data.FromId = TYPE.value("FROMID").toInt();
	}
	if (TYPE.contains("TOID"))
	{
		m_data.ToId = TYPE.value("TOID").toInt();
	}

	WJ::PlatformLogic().sendData(WJ::e_hs_mess_chat, WJ::e_hs_ass_event_chat, &m_data, sizeof(m_data));
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("联赛房间内玩家发送聊天消息");
}
//通知玩家开始联赛小组游戏是否成功
//UINT resid
//0 成功
//1 路径错误
//2 其他错误
void DataDrocessing::tellEventGroupStartRes(int resid)
{
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1034);
	QJsonObject res_js;
	res_js.insert("res", resid);
	sendjsonpart.insert(QStringLiteral("TYPE"), res_js);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}