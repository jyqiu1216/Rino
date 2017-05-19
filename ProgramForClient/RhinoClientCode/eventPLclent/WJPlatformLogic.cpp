#include "WJPlatformLogic.h"
#include "common/clientconfig.h"
#include "message\WJNetPlatformMessage.h"
#include "common\WJUserInfoBaseStruct.h"
#include "message\WJNetPM_gamelist.h"
#include "web\WJMainWindow.h"
#include "websocket\WebSocketServer.h"
#include "clientsocket/commonfunction_c.h"
#include "DataDrocessing.h"
#include "WebSocketMessInfo.h"
#include "Qthread/RoomLogic.h"
#include "common/LoadWindowsMess.h"
#include "web/WJMainWindow.h"
#include "clientsocket/WJCInterface.h"
namespace WJ
{
	WJPlatformLogic& WJPlatformLogic::getInstance()
	{
		static WJPlatformLogic sHNPlatformLogic;
		return sHNPlatformLogic;
	}

	WJPlatformLogic::WJPlatformLogic(void)
		: _logined(false)
		, _connected(false)
		, _serverPort(INVALID_VALUE)
	{
		m_bWX_login = false;
		_socketLogic = new WJClientSocketLogic(this, "platform");
		_logFailId = 0;
	}

	WJPlatformLogic::~WJPlatformLogic(void)
	{
		_socketLogic->close();
		WJ_SAFE_DELETE(_socketLogic);
	}

	bool WJPlatformLogic::init()
	{
		return true;
	}

	bool WJPlatformLogic::connect(char *cip, int iport)
	{
		if (nullptr == cip)
		{
			_serverAddress = clientconfig().GetData()->hall_server_address;
		}
		else
		{
			_serverAddress = cip;
		}

		if (0 >= iport)
		{
			_serverPort = PORT_LS;
		}
		else
		{
			_serverPort = iport;
		}
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("发起连接.........IP:") << _serverAddress.c_str() << QStringLiteral("端口:") << _serverPort;
		_socketLogic->close();
		//LOG_DEBUG("发起连接");
		return _socketLogic->openWithIp(_serverAddress.c_str(), _serverPort);
	}

	bool WJPlatformLogic::close()
	{
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("客户端关闭连接.........");
		//LOG_DEBUG("客户端关闭连接");
		_connected = false;
		return _socketLogic->close();
	}

	bool WJPlatformLogic::isConnect() const
	{
		return _connected;
	}

	bool WJPlatformLogic::isLogin() const
	{

		return _logined;
	}

	void WJPlatformLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
	{
		//qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("发送数据包") << MainID << AssistantID << objectSize;
		_socketLogic->send(MainID, AssistantID, object, objectSize);
	}
	void WJPlatformLogic::sendDataL(UINT MainID, UINT AssistantID, UINT uHandleCode,void* object, INT objectSize)
	{
		_socketLogic->send(MainID, AssistantID, uHandleCode, object, objectSize);
	}
	void WJPlatformLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector)
	{
		//qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("发送数据包") << MainID << AssistantID << objectSize;
		_socketLogic->send(MainID, AssistantID, object, objectSize);
		_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}
	void WJPlatformLogic::addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector)
	{
		_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}
	void WJPlatformLogic::removeEventSelector(UINT MainID, UINT AssistantID)
	{
		_socketLogic->removeEventSelector(MainID, AssistantID);
	}

	void WJPlatformLogic::login(const std::string& name, const std::string& password, UINT uNameID)
	{

	}

	void WJPlatformLogic::regist(BYTE byFastRegister, UINT uNameID, const std::string& usn, const std::string& name, const std::string& password, const std::string& tgname)
	{

	}

	void WJPlatformLogic::onConnected(bool connect, emSocketStatus status)
	{
		if (SocketStatus_OUTTIME == status)
		{
			//连接超时的消息
			qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("连接超时");
			_logined = false;
			_connected = false;
			//LOG_DEBUG("连接超时");
			WJ_SAFE_QSendMessage(WJMainWindow(), WM_HALL_MESS_1002, 0, 0);
		}
		else if (SocketStatus_SUCCESS == status)
		{
			//
		}
	}

	void WJPlatformLogic::onDisConnect()
	{
		//连接失败的消息
		//LOG_DEBUG("连接失败--服务器主动断开");
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("连接失败--服务器主动断开");
		_logined = false;
		_connected = false;
		WJ_SAFE_QSendMessage(WJMainWindow(), WM_HALL_MESS_1003, 0, 0);
		CommonData()->userinfo.landmassage = QStringLiteral("");

		WJMainWindow()->m_ptrayIcon->setToolTip(QString::fromUtf8("RhinoClient"));
	}

	void WJPlatformLogic::onSocketMessage(WJSocketMessage* socketMessage)
	{
		if (e_hs_mess_gamelist == socketMessage->messageHead.bMainID
			&& e_hs_ass_gamelist_onlien_count == socketMessage->messageHead.bAssistantID)
		{
			//
		}
		else
		{
			qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("SocketMessage数据返回包:")
				<< socketMessage->messageHead.bMainID << socketMessage->messageHead.bAssistantID
				<< socketMessage->messageHead.bHandleCode << socketMessage->messageHead.bReserve
				<< socketMessage->messageHead.uMessageSize;
		}
		//所有收到的消息处理函数在这里写
		switch (socketMessage->messageHead.bMainID)
		{
		case NET_M_CONNECT:
		{
			if (ASS_NET_CONNECT_2 == socketMessage->messageHead.bAssistantID)
			{
				_connected = true;
				//连接成功的消息
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("连接成功");
				//LOG_DEBUG("连接成功");
				WJ_SAFE_QSendMessage(WJMainWindow(), WM_HALL_MESS_1004, 0, 0);
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("大厅服务器连接成功消息----sdp");
				//LOG_DEBUG("大厅服务器连接成功消息----sdp");

				if (!m_bWX_login)
				{
					if (0 != strcmp(m_logininfo.username, ""))
					{//如果存在以前登陆账号的记忆则连接成功后直接使用该账号
						sendData((UINT)WJ::e_hs_mess_login, (UINT)WJ::e_hs_ass_login
							, &m_logininfo, sizeof(m_logininfo));
					}
				}
				else
				{
					if (0 != strcmp(m_logininfo_WX.username, ""))
					{//如果存在以前登陆账号的记忆则连接成功后直接使用该账号
						sendData((UINT)WJ::e_hs_mess_login, (UINT)WJ::e_hs_ass_login_WX
							, &m_logininfo_WX, sizeof(m_logininfo_WX));
					}
				}
			}
		}break;
		case WJ::e_hs_mess_login:
			if (socketMessage->messageHead.bAssistantID == WJ::e_hs_ass_login)
			{
				if (socketMessage->messageHead.bHandleCode == WJ::e_hs_hcid_login_suc)
				{

					//登陆成功就开启LOL监测线程
					RoomLogic()->CreateMonitorThread();
					WJMainWindow()->onShowLogin(false);
					qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("用户登陆成功返回基础信息");
					WJ::UserInfoBaseStruct userinfo;
					WJ::UserInfoBaseStruct* pCmd = (WJ::UserInfoBaseStruct*)socketMessage->object;
					userinfo.dwPoint = pCmd->dwPoint;
					userinfo.i64Money = pCmd->i64Money;

					CommonData()->userinfo.szName = QString::fromLocal8Bit(pCmd->szName);
					CommonData()->userinfo.nickName = QString::fromLocal8Bit(pCmd->nickName);
					CommonData()->userinfo.dwUserID = pCmd->dwUserID;	//服务器端传来的userid
					CommonData()->userinfo.dwPoint = pCmd->dwPoint;
					CommonData()->userinfo.i64Money = pCmd->i64Money;
					CommonData()->userinfo.icon = QString::fromLocal8Bit(pCmd->icon);
					CommonData()->userinfo.rewardTotal = QStringLiteral("0");
					_logined = true;
					///发送日活越
					//////////////////////////////////////////////////////////////////////////
					QEventLoop loop;
					QNetworkAccessManager m_networkAccessManger;
					QNetworkRequest request;
					QString userid;
					userid.setNum(pCmd->dwUserID, 10);
					QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("count/useractive?userId=") + userid;
					request.setUrl(QUrl(str));

					SystemLog()->LogToFile("日活越标志记录");
					SystemLog()->LogToFile(str.toStdString().c_str());

					//发送GET请求
					QNetworkReply *reply = m_networkAccessManger.get(request);
					QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));//关键之处1
					loop.exec();//关键之处2
					/////////////////////////////////////////////
					QByteArray data = reply->readAll();//网页内容输出
					QString ret = QString::fromUtf8(data);

					reply->deleteLater();

					//////////////////////////////////////////////////////////////////////////


					QJsonObject json;
					json.insert(QStringLiteral("LOGINED"), _logined);
					json.insert(QStringLiteral("SZNAME"), CommonData()->userinfo.szName);
					json.insert(QStringLiteral("NICKNAME"), CommonData()->userinfo.nickName);
					json.insert(QStringLiteral("PASSWORD"), QString::fromLocal8Bit(pCmd->password));
					json.insert(QStringLiteral("DWUSERID"), CommonData()->userinfo.dwUserID);
					json.insert(QStringLiteral("DWPOINT"), CommonData()->userinfo.dwPoint);
					json.insert(QStringLiteral("I64MONEY"), CommonData()->userinfo.i64Money);
					json.insert(QStringLiteral("ICON"), CommonData()->userinfo.icon);
					QJsonObject sendjsonpart;
					sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1001);
					sendjsonpart.insert(QStringLiteral("TYPE"), json);
					QJsonDocument documentpack;
					documentpack.setObject(sendjsonpart);
					QString json_json(documentpack.toJson(QJsonDocument::Compact));
					qDebug() << json_json;
					WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_login, (UINT)WJ::e_hs_ass_login_c_finish, NULL, 0);
					CommonData()->userinfo.landmassage = QStringLiteral("登陆成功");

					WJMainWindow()->m_ptrayIcon->setToolTip((QString::fromLocal8Bit("昵称：") + CommonData()->userinfo.nickName));
					WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

					//LOL
					GetHttpBetData(1);
					//炉石
					GetHttpBetData(2);
				}
				else 	if (socketMessage->messageHead.bHandleCode == WJ::e_hs_hcid_login_err_unexist)
				{
					qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("登陆失败,用户不存在");
					_logFailId = 1;
					QJsonObject jsonpart;
					jsonpart.insert(QStringLiteral("NUMBER"), _logFailId);
					jsonpart.insert(QStringLiteral("NAME"), QStringLiteral("登陆失败,用户不存在"));
					QJsonObject sendjsonpart;
					sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1001);
					sendjsonpart.insert(QStringLiteral("TYPE"), jsonpart);
					QJsonDocument documentpack;
					documentpack.setObject(sendjsonpart);
					QString json_json(documentpack.toJson(QJsonDocument::Compact));
					WebSocketServer::inobgect()->sendMessage(json_json, web_c_pt_login);
				}
				else 	if (socketMessage->messageHead.bHandleCode == WJ::e_hs_hcid_login_err_password)
				{
					qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("登陆失败,密码错误");
					_logFailId = 2;
					QJsonObject jsonpart;
					jsonpart.insert(QStringLiteral("NUMBER"), _logFailId);
					jsonpart.insert(QStringLiteral("NAME"), QStringLiteral("登陆失败,密码错误"));
					QJsonObject sendjsonpart;
					sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1001);
					sendjsonpart.insert(QStringLiteral("TYPE"), jsonpart);
					QJsonDocument documentpack;
					documentpack.setObject(sendjsonpart);
					QString json_json(documentpack.toJson(QJsonDocument::Compact));
					WebSocketServer::inobgect()->sendMessage(json_json, web_c_pt_login);
				}
				else 	if (socketMessage->messageHead.bHandleCode == WJ::e_hs_hcid_login_err_online)
				{
					qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("登陆失败,该账号已在线");
					_logFailId = 3;
					QJsonObject jsonpart;
					jsonpart.insert(QStringLiteral("NUMBER"), _logFailId);
					jsonpart.insert(QStringLiteral("NAME"), QStringLiteral("登陆失败,该账号已在线"));
					QJsonObject sendjsonpart;
					sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1001);
					sendjsonpart.insert(QStringLiteral("TYPE"), jsonpart);
					QJsonDocument documentpack;
					documentpack.setObject(sendjsonpart);
					QString json_json(documentpack.toJson(QJsonDocument::Compact));
					WebSocketServer::inobgect()->sendMessage(json_json, web_c_pt_login);
				}
			}
			break;
		case e_hs_mess_gamelist:
		{
			if (socketMessage->messageHead.bAssistantID == e_hs_ass_gamelist_game)
			{
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("游戏信息返回完成");

				//ComGameinfo* pCmd = (ComGameinfo*)socketMessage->object;
				for (int i = 0; i < socketMessage->objectSize / sizeof(ComGameinfo); i++)
				{
					ComGameinfo* pCmd = (ComGameinfo*)(socketMessage->object + i*(sizeof(ComGameinfo)));
					ComGameinfo _ComGameinfo;
					_ComGameinfo.gameid = pCmd->gameid;
					std::memcpy(_ComGameinfo.gamename, pCmd->gamename, sizeof(_ComGameinfo.gamename));
					CommonData()->m_GameNameList.push_back(_ComGameinfo);
				}
				if (socketMessage->messageHead.bHandleCode == e_hs_hcid_gamelist_finish)
				{
					WJ_SAFE_QSendMessage(WJMainWindow(), WM_HALL_MESS_1005, 0, 0);
				}
			}
			else if (socketMessage->messageHead.bAssistantID == e_hs_ass_gamelist_erea)
			{
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("区服信息返回");

				UINT ucount = socketMessage->objectSize / sizeof(ComEreainfo);
				for (int i = 0; i < ucount; i++)
				{
					ComEreainfo* pCmd = (ComEreainfo*)(socketMessage->object + i*(sizeof(ComEreainfo)));
					ComEreainfo severarea;
					std::memcpy(&severarea, pCmd, sizeof(ComEreainfo));
					CommonData()->m_AreaList.push_back(severarea);
				}
			}
			else if (socketMessage->messageHead.bAssistantID == e_hs_ass_gamelist_contest)
			{//比赛列表
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("比赛节点返回");

				UINT ucount = socketMessage->objectSize / sizeof(ComContestInfo);
				for (int i = 0; i < ucount; i++)
				{
					ComContestInfo* pCmd = (ComContestInfo*)(socketMessage->object + i*(sizeof(ComContestInfo)));
					ComContestInfo gamecontmap;
					std::memcpy(&gamecontmap, pCmd, sizeof(ComContestInfo));
					CommonData()->m_GameContestList.push_back(gamecontmap);
				}

				SystemLog()->LogToFile("sdp_test c 获取到比赛列表");
			}
			else if (socketMessage->messageHead.bAssistantID == e_hs_ass_gamelist_room)
			{//获取房间列表
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("房间信息返回");
				UINT ucount = socketMessage->objectSize / sizeof(ComRoomInfo);
				if (ucount <= 0)
				{
					qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("服务器返回房间信息为0");
				}
				for (int i = 0; i < ucount; i++)
				{
					ComRoomInfo* pCmd = (ComRoomInfo*)(socketMessage->object + i*(sizeof(ComRoomInfo)));
					ComRoomInfo cmd;
					std::memcpy(&cmd, pCmd, sizeof(ComRoomInfo));
					CommonData()->m_GameRoomList.push_back(cmd);
				}
				SystemLog()->LogToFile("sdp_test c 获取房间列表");
				WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2021, 0, 0);
			}
			else if (socketMessage->messageHead.bAssistantID == e_hs_ass_gamelist_onlien_count)
			{
				return;
				//SystemLog()->LogToFile("sdp_test c 在线人数刷新=%d", socketMessage->messageHead.bHandleCode);
				QJsonObject sendjsonpart;
				QJsonObject json;

				json.insert(QStringLiteral("online"), (int)socketMessage->messageHead.bHandleCode);
				sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_6001);
				sendjsonpart.insert(QStringLiteral("TYPE"), json);
				QJsonDocument documentpack;
				documentpack.setObject(sendjsonpart);
				QString json_json(documentpack.toJson(QJsonDocument::Compact));
				WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
			}
		}break;
		case e_hs_mess_apply:
		{
								if (socketMessage->messageHead.bAssistantID == WJ::e_hs_ass_apply)
								{
									if (socketMessage->messageHead.bHandleCode == WJ::e_hs_hcid_apply_suc)
									{//报名成功
										SystemLog()->LogToFile("sdp_test c 报名成功,准备登陆房间服务器---");
										m_bApplySuc = true;

										RoomLogic()->Sys_OnJoinRoom(true);
									}
									else
									{//报名失败
										WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2017, CommonData()->m_NowGame, 0);
										WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2022, CommonData()->m_NowGame, socketMessage->messageHead.bHandleCode);
									}
								}
								else if (socketMessage->messageHead.bAssistantID == WJ::e_hs_ass_apply_login_recome_apply)
								{//玩家断线后回来发现还未加入房间，发送该消息给客户端
									CommonData()->m_GameRoomList.clear();
									UINT ucount = socketMessage->objectSize / sizeof(WJ::ComRoomInfo);
									for (int i = 0; i < ucount; i++)
									{
										WJ::ComRoomInfo* pCmd = (WJ::ComRoomInfo*)(socketMessage->object + i*(sizeof(WJ::ComRoomInfo)));
										ComRoomInfo cmd;
										std::memcpy(&cmd, pCmd, sizeof(ComRoomInfo));
										CommonData()->m_GameRoomList.push_back(cmd);
										CommonData()->m_NowContest = pCmd->contestid;
										SystemLog()->LogToFile("sdp_test c 玩家断线后回来发现还未加入房间，发送该消息给客户端---CommonData()->m_NowContest=%d", CommonData()->m_NowContest);
									}

									qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("报名失败_未加入房间");
									WJ_SAFE_QSendMessage(WJMainWindow(), WM_HALL_MESS_1006, 0, 0);
								}
								else if (socketMessage->messageHead.bAssistantID == WJ::e_hs_ass_apply_login_recome_playing)
								{//玩家断线后回来发现已经加入房间，发送该消息给客户端
									CommonData()->m_GameRoomList.clear();
									qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("游戏短线重连房间信息返回");
									WJ::struct_data_hs_mess_apply_login_recome_playing* pCmd = (WJ::struct_data_hs_mess_apply_login_recome_playing*)(socketMessage->object);
									ComRoomInfo cmd;
									cmd.roomid = pCmd->roomid;//房间id
									cmd.gameid = pCmd->gameid;//游戏id
									cmd.ereaid = pCmd->ereaid;//游戏区服id
									cmd.contestid = pCmd->ucontestid;//比赛id
									std::memcpy(cmd.strIP, pCmd->strIP, sizeof(cmd.strIP));//房间ip地址
									cmd.uport = pCmd->uport;//房间端口号
									SystemLog()->LogToFile("sdp_test c 断线重连房间IP地址strIP = %s,房间端口号uport = %d", cmd.strIP, pCmd->uport);

									CommonData()->m_NowGame = (E_GameID)cmd.gameid;
									CommonData()->m_NowErea = cmd.ereaid;
									CommonData()->m_NowRoom = cmd.roomid;
									CommonData()->m_GameRoomList.push_back(cmd);
									CommonData()->m_NowContest = (int)pCmd->ucontestid;
									SystemLog()->LogToFile("sdp_test c 玩家断线后回来发现已经加入房间，发送该消息给客户端---CommonData()->m_NowContest=%d", CommonData()->m_NowContest);
									SystemLog()->LogToFile("sdp_test c,游戏id = %d, 比赛类型 id = %d游戏区服id  = %d,房间id = %d", cmd.gameid, pCmd->uContestKind, cmd.ereaid, cmd.contestid);
									QJsonObject sendjsonpart;
									QJsonObject json;

									json.insert(QStringLiteral("gameId"), int(cmd.gameid));
									json.insert(QStringLiteral("modeId"), pCmd->uContestKind);
									json.insert(QStringLiteral("sid"), int(cmd.ereaid));
									json.insert(QStringLiteral("rid"), int(cmd.contestid));
									sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_3011);
									sendjsonpart.insert(QStringLiteral("TYPE"), json);
									QJsonDocument documentpack;
									documentpack.setObject(sendjsonpart);
									QString json_json(documentpack.toJson(QJsonDocument::Compact));
									WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
									m_bApplySuc = true;
									WJ_SAFE_QSendMessage(WJMainWindow(), WM_HALL_MESS_1007, 0, 0);
								}
								else if (socketMessage->messageHead.bAssistantID == WJ::e_hs_ass_unapply)
								{//玩家并没有报名参加任何比赛
									getInfoBeforeApply();
								}
								else if (socketMessage->messageHead.bAssistantID == e_hs_ass_apply_restitution)
								{//退费
									if (socketMessage->messageHead.bHandleCode == WJ::e_hs_hcid_apply_restitution_suc)
									{//成功
										SystemLog()->LogToFile("sdp_test c 退费成功。");
										m_bApplySuc = false;

									}
									else if (socketMessage->messageHead.bHandleCode == WJ::e_hs_hcid_apply_restitution_err)
									{
										SystemLog()->LogToFile("sdp_test c 退费失败！！");
									}
								}
		}break;
		case e_hs_mess_event:
		{//联赛消息
			if (socketMessage->messageHead.bAssistantID == e_hs_ass_event_clash_update)
			{
				QJsonObject sendjsonpart;
				sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_9015);
				QJsonDocument documentpack;
				documentpack.setObject(sendjsonpart);
				QString json_json(documentpack.toJson(QJsonDocument::Compact));
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("通知H5更新对阵图");
				WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
			}
			else if (socketMessage->messageHead.bAssistantID == e_hs_ass_event_seat_update)
			{

				WJ::RRLine_s_Update_event_seat *pCmd = (WJ::RRLine_s_Update_event_seat*)socketMessage->object;
				QJsonObject sendjsonpart;
				QJsonObject json;

				json.insert(QStringLiteral("GROUPID"), int(pCmd->groupId));
				json.insert(QStringLiteral("SEATID"), int(pCmd->seatId));
				json.insert(QStringLiteral("USERID"), int(pCmd->userId));
				json.insert(QStringLiteral("READYTIME"), LLONG(pCmd->readyTime));
				json.insert(QStringLiteral("STATE"), int(pCmd->state));
				sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_9014);
				sendjsonpart.insert(QStringLiteral("TYPE"), json);
				QJsonDocument documentpack;
				documentpack.setObject(sendjsonpart);
				QString json_json(documentpack.toJson(QJsonDocument::Compact));
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("联赛房间内玩家准备消息");
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << json_json;
				WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
		
			}
			else if (e_hs_ass_event_begin == socketMessage->messageHead.bAssistantID)
			{
				switch (socketMessage->messageHead.bHandleCode)
				{
				case e_hs_hcid_event_suc:
				{
					struct_data_hs_mess_event_begin_suc *pCmd = (struct_data_hs_mess_event_begin_suc*)socketMessage->object;
					if (nullptr == pCmd)
					{
						return;
					}
					DataDrocessing::inobgect()->tellEventGroupStartRes(0);
					ComRoomInfo cmd;
					cmd.roomid = pCmd->roomid;
					memcpy(cmd.strIP, pCmd->strIP, sizeof(cmd.strIP));
					cmd.uport = pCmd->uport;
					CommonData()->m_GameRoomList.clear();
					CommonData()->m_GameRoomList.push_back(cmd);

					SystemLog()->LogToFile("sdp_test c 成功加入联赛");

					CommonData()->m_NowContest = pCmd->contestid;
					WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2021, 0, 0);
				}break;
				case e_hs_hcid_event_err:
				case e_hs_hcid_event_err_unknown:
				{
					DataDrocessing::inobgect()->tellEventGroupStartRes(2);
				}break;
				default:
					break;
				}
			}
			else if (socketMessage->messageHead.bAssistantID == e_hs_ass_event_unagree)
			{//只有一人未准备，发送未准备玩家的userID给H5
				UINT userid = 0;
				userid = socketMessage->messageHead.bHandleCode;
				QJsonObject sendjsonpart;
				QJsonObject json;

				json.insert(QStringLiteral("USERID"), int(userid));
				sendjsonpart.insert(QStringLiteral("TYPE"), json);
				sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1039);
				QJsonDocument documentpack;
				documentpack.setObject(sendjsonpart);
				QString json_json(documentpack.toJson(QJsonDocument::Compact));
				WebSocketServer::inobgect()->sendMessage(json_json, web_c_ls_inside);

			}
			else if (socketMessage->messageHead.bAssistantID == e_hs_ass_event_unagreeall)
			{//两个人都未准备，直接告知H5两人退赛
				QJsonObject sendjsonpart;
				sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1040);
				QJsonDocument documentpack;
				documentpack.setObject(sendjsonpart);
				QString json_json(documentpack.toJson(QJsonDocument::Compact));
				WebSocketServer::inobgect()->sendMessage(json_json, web_c_ls_inside);

			}
		}break;
		case e_hs_mess_chat:
		{//联赛房间内聊天消息
			if (e_hs_ass_event_chat == socketMessage->messageHead.bAssistantID)
			{
				struct_data_hs_mess_chat *data = (struct_data_hs_mess_chat*)socketMessage->object;
				QJsonObject sendjsonpart;
				QJsonObject json;
				json.insert(QStringLiteral("FROMID"), int(data->FromId));
				json.insert(QStringLiteral("TOID"), int(data->ToId));
				json.insert(QStringLiteral("TIME_YEAR"), data->time.iYear);
				json.insert(QStringLiteral("TIME_HOUR"), data->time.iHour);
				json.insert(QStringLiteral("TIME_DAY"), data->time.iDay);
				json.insert(QStringLiteral("TIME_MILLISECONDS"), data->time.iMilliseconds);
				json.insert(QStringLiteral("TIME_MINUTE"), data->time.iMinute);
				json.insert(QStringLiteral("TIME_SECOND"), data->time.iSecond);
				json.insert(QStringLiteral("TIME_MONTH"), data->time.iMonth);
				json.insert(QStringLiteral("CHAT_DATA"), data->chat_data);
				sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_9018);
				sendjsonpart.insert(QStringLiteral("TYPE"), json);
				QJsonDocument documentpack;
				documentpack.setObject(sendjsonpart);
				QString json_json(documentpack.toJson(QJsonDocument::Compact));
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("联赛房间内玩家收到聊天消息");
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << json_json;
				WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

			}
		}break;
		case e_hs_mess_pick:
		{//联赛房间PICK消息
			if (e_hs_ass_event_pick == socketMessage->messageHead.bAssistantID)
			{
				_struct_data_hs_mess_pick *data = (_struct_data_hs_mess_pick*)socketMessage->object;
				QJsonObject sendjsonpart;
				QJsonObject json;
				json.insert(QStringLiteral("FROMID"), int(data->uToId));
				json.insert(QStringLiteral("TOID"), int(data->uFromId));
				json.insert(QStringLiteral("PICK_DATA"), data->chPickData);
				sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_9020);
				sendjsonpart.insert(QStringLiteral("TYPE"), json);
				QJsonDocument documentpack;
				documentpack.setObject(sendjsonpart);
				QString json_json(documentpack.toJson(QJsonDocument::Compact));
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("联赛房间内玩家收到PICK消息");
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << json_json;
				WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

			}
		}break;
		default:
			break;
		}

		IBaseMyHallLogic* pgr = CommonData()->GetMyHallLogic(CommonData()->m_NowGame);
		if (nullptr != pgr)
		{
			pgr->onSocketMessage(socketMessage);
		}
	}

	//报名比赛前先请求获得该比赛节点信息和比赛下的房间列表
	void WJPlatformLogic::getInfoBeforeApply()
	{
		if (0 != CommonData()->m_NowContest)
		{//如果当前选中了比赛节点，则请求当前比赛节点数据，以及报名
			struct_data_hs_ass_gamelist_contest_online cmd;
			cmd.contestid = CommonData()->m_NowContest;//比赛id
			//获取比赛节点(单节点数据)
			sendData((UINT)e_hs_mess_gamelist, (UINT)e_hs_ass_gamelist_contest_online, &cmd, sizeof(cmd));

			//请求游戏列表
			struct_data_hs_ass_gamelist_room WJgameroot;
			WJgameroot.contestid = CommonData()->m_NowContest;//房间id
			sendData((UINT)e_hs_mess_gamelist, (UINT)e_hs_ass_gamelist_room, &WJgameroot, sizeof(WJgameroot));
			//请求区服
			struct_data_hs_ass_gamelist_contest erea;
			erea.ereaid = CommonData()->m_NowContest;//区服id
			sendData((UINT)e_hs_mess_gamelist, (UINT)e_hs_ass_gamelist_erea, &erea, sizeof(erea));
		}
	}

	void WJPlatformLogic::GetHttpBetData(int inType)
	{
		try {

			if (inType < 0) {

				return;
			}

			QEventLoop loop;
			QNetworkAccessManager manager;
			QNetworkRequest request;

			QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/stake/list?type=") + QString::number(inType);
			request.setUrl(QUrl(str));
			qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("发送获取押注信息:") << str;

			//发送GET请求
			QNetworkReply *reply = manager.get(request);
			QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));//关键之处1
			loop.exec();//关键之处2
			/////////////////////////////////////////////
			QByteArray data = reply->readAll();
			reply->deleteLater();

			QJsonParseError json_errorver;
			QJsonDocument parse_doucmentver = QJsonDocument::fromJson(data, &json_errorver);

			if (json_errorver.error == QJsonParseError::NoError) {

				if (parse_doucmentver.isArray()) {

					QJsonArray ObjectArray = parse_doucmentver.array();

					for (int i = 0; i < ObjectArray.size(); i++) {

						QJsonObject objectver = ObjectArray.at(i).toObject();

						if (objectver.contains("id") && objectver.contains("showNo") && objectver.contains("amount") && objectver.contains("rate")) {

							BetInfo tBetInfo;

							QJsonValue jsId = objectver.value(QString("id"));
							tBetInfo._sinGameId = jsId.toInt();

							QJsonValue jsPos = objectver.value(QString("showNo"));
							tBetInfo._sinIndex = jsPos.toInt();

							QJsonValue jsMoney = objectver.value(QString("amount"));
							tBetInfo._sinMoney = jsMoney.toInt();

							QJsonValue jsRate = objectver.value(QString("rate"));
							tBetInfo._sfRate = jsRate.toDouble();

							if (1 == inType) {

								CommonData()->m_BetInfo.insert(tBetInfo);
							}
							else if (2 == inType) {

								CommonData()->m_BetHsInfo.insert(tBetInfo);
							}
						}
					}
				}
			}
		}
		catch (...) {

		}
	}

}
