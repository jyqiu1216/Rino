#include "RoomLogic.h"
#include <thread>
#include "clientsocket/WJSocketMessage.h"
#include "web/WebChannelExt.h"
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QtWebChannel/QtWebChannel>
#include "common/Common.h"
#include <QTextCodec>
#include <qobjectdefs.h>
#include "clientsocket/commonfunction_c.h"
#include "web/WJMainWindow.h"
#include "common/WJGameInfo.h"
#include "WebSocketMessInfo.h"
#include "GameMonitorHS.h"
#include "common/LoadWindowsMess.h"
#include "websocket\WebSocketServer.h"
#include "web/WJMainWindow.h"
#include "websocket/DataDrocessing.h"
#include "common/LoadWindowsMess.h"
#include "websocket/WebSocketMessInfo.h"
#include "GameMonitorHS.h"
#include "message/WJNetRoomMessage.h"
#include "WJPlatformLogic.h"
void DEBUG_WriteLog(const wchar_t* filename, const void* p, int l)
{
#ifdef _DEBUG
	HANDLE	hFile;
	DWORD	NumberOfBytesWritten;
	hFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	WriteFile(hFile, p, l, &NumberOfBytesWritten, NULL);
	CloseHandle(hFile);
#endif
}

CRoomLogic* g_pCRoomLogic = nullptr;

CRoomLogic::CRoomLogic()
{
	bInGame = false;
	m_WJClientSocketLogic = new WJ::WJClientSocketLogic(this, "Room");

	Sys_ReSetData();
	char szAppPath[MAX_PATH];
	GetModuleFileNameA(NULL, szAppPath, MAX_PATH);
	std::string str;
	str = CommonData()->currentpath.toLocal8Bit();
	str += "/luaScript/main.out";
	AutomateManager::init(str.c_str());
}


CRoomLogic *CRoomLogic::getInstance()
{
	if (nullptr == g_pCRoomLogic)
		g_pCRoomLogic = new CRoomLogic();
	return g_pCRoomLogic;
}

bool CRoomLogic::Sys_OnStart()
{
	int itage = CommonData()->m_NowContest - WJ::g_eventid_first;
	bool bEvent = false;//是否为联赛
	if (itage > 0)
	{//联赛
		bEvent = true;
	}

	m_struct_StartContest.bTrue = false;
	//比赛按钮暂时锁住
	DataDrocessing::inobgect()->OnContestButLock();
	//////////////////////////////////////////////////////
	if (!bEvent)
	{//这是solo赛考虑的事情
		E_GameID igameid = e_gameid_10000001;
		QString award = "0";
		QString centestname;
		QString matchpoint;
		QJsonObject data = m_struct_StartContest._QJsonObject_StartContest;
		int i = 0;
		int money = 0; // 积分或娱币
		if (data.contains("CENTEST_PT"))
		{
			std::string pt = data.value("CENTEST_PT").toString().toStdString();
			i = atoi(pt.c_str());
		}
		if (data.contains("CENTEST_PS"))
		{
			std::string ps = data.value("CENTEST_PS").toString().toStdString();
			money = atoi(ps.c_str());
		}

		if (data.contains("GAMENAME"))
		{//报名成功
			std::string _str = data.value("GAMENAME").toString().toStdString();
			igameid = (E_GameID)atoi(_str.c_str());
		}

		if (data.contains("I64AWARD"))
		{//奖励
			award = data.value("I64AWARD").toString();
		}

		if (data.contains("CENTEST_NAME"))
		{//比赛名称
			centestname = data.value("CENTEST_NAME").toString();
		}

		if (data.contains("CENTEST_MATCHPOINT"))
		{//赛事详情
			matchpoint = data.value("CENTEST_MATCHPOINT").toString();
		}
		//i = 1 积分  || i = 2 娱币

		if (i == 1)
		{
			//积分够不够
			if (CommonData()->userinfo.dwPoint < money)
				return false;
		}
		else if (i == 2)
		{
			if (CommonData()->userinfo.i64Money < money)
				return false;

		}
	}

	//WJ_SAFE_QBoxMess(WJMainWindow(), QStringLiteral("正在启动游戏，您可以在游戏内参与匹配．"));
	bool bCanStart = false;//是否可以开始
	do
	{
		if (!CommonData()->bExecutionAutomationDoing)
		{
			WJAM::closeGame(WJAM::GAME_ID_LOL);
			CommonData()->bExecutionAutomationDoing = true;
			if (CommonData()->m_GameRoomList.size() <= 0)
			{
				WJ_SAFE_QBoxMess(WJMainWindow(), QStringLiteral("暂无房间信息无法报名,请稍后重试"));
				CommonData()->bExecutionAutomationDoing = false;
				break;
			}

			//触发
			bCanStart = true;
		}
	} while (0);


	if (!bCanStart)
	{//不能开始
		DataDrocessing::inobgect()->OnContestButUnlock();
		return false;
	}

	CommonData()->bExecutionAutomationDoing = false;
	Sys_ReSetData();
	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		return pgr->OnStart();
	}

	return true;
}

void CRoomLogic::CreateMonitorThread()
{
	WJ_gameid_10000001::CMyGameRoom* p = (WJ_gameid_10000001::CMyGameRoom*)CommonData()->GetMyGameRoom(e_gameid_10000001);
	if (p != nullptr)
	{
		p->CreateMonitorThread();
	}
}

void CRoomLogic::setipport()
{
	if (m_game_connect_iter_tag < CommonData()->m_GameRoomList.size())
	{
		connect(CommonData()->m_GameRoomList.at(m_game_connect_iter_tag).strIP, CommonData()->m_GameRoomList.at(m_game_connect_iter_tag).uport);
	}
	else if (m_game_connect_iter_tag == CommonData()->m_GameRoomList.size())
	{
		WJ_SAFE_QBoxMess(WJMainWindow(), QStringLiteral("游戏服务器登陆失败,请稍后再战!"));
		Sys_OnDisposeErr(CommonData()->m_NowGame);
	}
	else
	{
		//剩下的不处理
	}
}
void CRoomLogic::Sys_OnJoinRoom(bool bFirst)
{
	SystemLog()->LogToFile("sdp_test c 连接游戏服务器---");
	if (bFirst)
	{
		m_game_connect_iter_tag = 0;
	}
	else
	{
		++m_game_connect_iter_tag;
	}

	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->OnJoinRoom(bFirst);
	}

	if (isConnect())
	{//已经建立了连接
		WJ::RoomMessDate_user_action_login login;
		(UINT)login.uuserid = CommonData()->userinfo.dwUserID;
		(UINT)login.contestid = CommonData()->m_NowContest;
		sendData((UINT)WJ::e_gs_mess_user_action, (UINT)WJ::e_gs_ass_user_action_login, &login, sizeof(login));
		SystemLog()->LogToFile("sdp_test c 游戏服务器---发送登陆消息");
	}
	else
	{
		setipport();
	}
}

//初始化数据
void CRoomLogic::Sys_ReSetData()
{
	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->ReSetData();
	}

	m_udeskid = 0;//桌子id
	m_udeskstation = 0;//座位
	m_bOwerUserid = false;//是否为桌子主人
	m_UserDatalist.clear();
	WJ::Room_ManageInfoStruct cmd;
	m_RoomInfo = cmd;
	m_struct_StartContest.bTrue = false;
}

//游戏过程中异常错误，需要清理不正确的数据，保证可以再次点击一键开赛
void CRoomLogic::Sys_OnDisposeErr(E_GameID igameid)
{
	SystemLog()->LogToFile("sdp_test 游戏过程中异常错误，需要清理不正确的数据，保证可以再次点击一键开赛(%d)", igameid);
	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->OnDisposeErr();
	}
	RoomLogic()->Sys_ReSetData();
	//取消登录游戏服务器
	WJ::RoomMessDate_user_action_login_cancel login_cancel;
	login_cancel.uuserid = CommonData()->userinfo.dwUserID;
	if (RoomLogic()->isConnect())
	{
		RoomLogic()->sendData(WJ::e_gs_mess_user_action, WJ::e_gs_ass_user_action_login_cancel, &login_cancel, sizeof(login_cancel));
	}
	else
	{
		SystemLog()->LogToFile("sdp_test c 不在房间，开始申请退费");
		//退费
		WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_apply, (UINT)WJ::e_hs_ass_apply_restitution, NULL, 0);
		DataDrocessing::inobgect()->OnContestButUnlock();
	}
}

//离开当前比赛详情页
void CRoomLogic::Sys_OnLeave()
{
	//WJMainWindow()->DoMsgBoxSignal();
	bInGame = false;
	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->OnLeave();
	}
	SocketClose();
	WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_apply, (UINT)WJ::e_hs_ass_apply_restitution, NULL, 0);
	Sys_ReSetData();
	SystemLog()->LogToFile("sdp_test c OnLeave()---CommonData()->m_NowContest=%d", CommonData()->m_NowContest);
	DataDrocessing::inobgect()->OnContestButUnlock();
	CommonData()->m_NowRoom = 0;
	CommonData()->m_NowContest = 0;
	CommonData()->m_NowErea = 0;
	WJMainWindow()->m_LolSoloId = 0;
	WJMainWindow()->onShowGameFinishwind(false);
}

//游戏结束
void CRoomLogic::Sys_OnEnd()
{
	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->OnEnd();
	}

	WJ::PlatformLogic().m_bApplySuc = false;
	m_udeskid = 0;//桌子id
	m_udeskstation = 0;//座位
	m_bOwerUserid = false;//是否为桌子主人
	m_UserDatalist.clear();
	std::memset(&m_RoomInfo, 0, sizeof(m_RoomInfo));
	m_struct_StartContest.bTrue = false;
	SystemLog()->LogToFile("sdp_test c OnEnd()---CommonData()->m_NowContest=%d", CommonData()->m_NowContest);
}

CRoomLogic::~CRoomLogic()
{
	Sys_ReSetData();
}

//收到与自己相关的本地消息
bool CRoomLogic::Sys_OnMess(const QByteArray &eventType, void *message, long *result)
{
	if (eventType != "windows_generic_MSG")
	{
		return false;
	}

	try
	{
		IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
		if (nullptr != pgr)
		{
			pgr->OnMess(eventType, message, result);
		}
		PMSG msg = (PMSG)message;
		switch (msg->message)
		{
		case WM_GAME_MESS_2022:
		{
			char str[MAX_PATH] = { 0 };
			sprintf_s(str, "报名失败，请稍后再试！(%d)", (UINT)msg->lParam);
			QString s = QString::fromLocal8Bit(str);
			WJ_SAFE_QBoxMess(WJMainWindow(), s);
		}break;
		case WM_GAME_MESS_2009://游戏服务器登陆成功
		{
			qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("游戏服务器登陆成功");
		}break;
		case WM_GAME_MESS_2007://游戏服务器---连接超时
		{
			SystemLog()->LogToFile("sdp_test c 游戏服务器---连接超时,继续查找房间并连接---");
			Sys_OnJoinRoom(false);
		}break;
		case WM_GAME_MESS_2008://游戏服务器---连接失败
		{
			SystemLog()->LogToFile("sdp_test c 游戏服务器---连接失败,继续查找房间并连接---");
		}break;
		case WM_GAME_MESS_2021:
		{
			bool OnStart = false;
			int itage = CommonData()->m_NowContest - WJ::g_eventid_first;
			if (itage > 0)
			{//说明是联赛
				OnStart = true;
			}
			else
			{
				if (m_struct_StartContest.bTrue)
				{
					OnStart = true;
				}
			}

			if (OnStart)
			{
				Sys_OnStart();
			}

		}break;
		default:
			break;
		}
	}
	catch (...)
	{
		///
	}

	return true;
}

bool CRoomLogic::connect(QString cip, int iport)
{
	if (nullptr == cip.toLatin1().data())
	{
		_serverAddress = clientconfig().GetData()->hall_server_address;
	}
	else
	{
		_serverAddress = cip.toLatin1().data();
	}

	if (0 >= iport)
	{
		_serverPort = 0;
	}
	else
	{
		_serverPort = iport;
	}
	m_WJClientSocketLogic->close();
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("游戏服务器----发送连接")
		<< _serverAddress.c_str() << _serverPort;
	return m_WJClientSocketLogic->openWithIp(_serverAddress.c_str(), _serverPort);
}
bool CRoomLogic::SocketClose()
{	
	_connected = false;
	return m_WJClientSocketLogic->close();

}

bool CRoomLogic::isConnect() const
{
	return _connected;
}

bool CRoomLogic::isLogin() const
{

	return _logined;
}

void CRoomLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
{
	if (MainID == 104 && AssistantID == 3)
	{
		OutputDebugStringA("sdp_test client send mISocketMessageDelegate::sendData");
	}

	m_WJClientSocketLogic->send(MainID, AssistantID, object, objectSize);
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("游戏服务器---发送消息：") << MainID << AssistantID << objectSize;
}

//void mISocketMessageDelegate::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, WJ::SEL_SocketMessage selector)
//{
//	m_WJClientSocketLogic->send(MainID, AssistantID, object, objectSize);
//	m_WJClientSocketLogic->addEventSelector(MainID, AssistantID, selector);
//}

void CRoomLogic::addEventSelector(UINT MainID, UINT AssistantID, WJ::SEL_SocketMessage selector)
{
	m_WJClientSocketLogic->addEventSelector(MainID, AssistantID, selector);
}
void CRoomLogic::removeEventSelector(UINT MainID, UINT AssistantID)
{
	m_WJClientSocketLogic->removeEventSelector(MainID, AssistantID);
}

void CRoomLogic::onConnected(bool connect, WJ::emSocketStatus status)
{
	if (WJ::SocketStatus_OUTTIME == status)
	{
		//连接超时的消息
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("游戏服务器---连接超时");
		_logined = false;
		_connected = false;

		WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2007, 0, 0);
	}
	else if (WJ::SocketStatus_SUCCESS == status)
	{
		//连接成功的消息
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("游戏服务器---连接成功");
	}

	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->onConnected(connect, status);
	}
}
void CRoomLogic::onDisConnect()
{
	//连接失败的消息
	bInGame = false;
	//SystemLog::getInstance()->writeLog(SystemLog::E_LOG_DEBUG, "WJTestLOG.log", "1", 1, "游戏服务器---连接失败");
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("游戏服务器---连接失败");
	_logined = false;
	_connected = false;
	WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2008, 0, 0);
	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->onDisConnect();
	}
}
void CRoomLogic::onSocketMessage(WJ::WJSocketMessage* socketMessage)
{
	SystemLog()->LogToFile("sdp_test c onSocketMessage游戏服务器返回：%d,%d,%d"
		, socketMessage->messageHead.bMainID, socketMessage->messageHead.bAssistantID, socketMessage->messageHead.bHandleCode);
	//所有收到的消息处理函数在这里写
	switch (socketMessage->messageHead.bMainID)
	{
	case NET_M_CONNECT:
	{
		if (ASS_NET_CONNECT_2 == socketMessage->messageHead.bAssistantID)
		{
			_connected = true;

			WJ::RoomMessDate_user_action_login login;
			(UINT)login.uuserid = CommonData()->userinfo.dwUserID;
			(UINT)login.contestid = CommonData()->m_NowContest;
			sendData((UINT)WJ::e_gs_mess_user_action, (UINT)WJ::e_gs_ass_user_action_login, &login, sizeof(login));
			SystemLog()->LogToFile("sdp_test c 游戏服务器---发送登陆消息");
		}
	}break;
	case WJ::e_gs_mess_user_action:
	{
		if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_user_action_login)
		{
			if (socketMessage->messageHead.bHandleCode == WJ::e_gs_hcid_login_suc_)
			{
				_logined = true;
				SystemLog()->LogToFile("sdp_test c 游戏服务器登陆成功");
				WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2009, 0, 0);
			}
			else if (socketMessage->messageHead.bHandleCode == WJ::e_gs_hcid_login_suc_recome)
			{
				SystemLog()->LogToFile("sdp_test c 游戏服务器登陆成功,断线重回---");
				WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2010, 0, 0);
			}
			else if (socketMessage->messageHead.bHandleCode == WJ::e_gs_hcid_login_err_)
			{
				SystemLog()->LogToFile("sdp_test c 游戏服务器登陆失败，继续连接其他游戏服务器---");
				SocketClose();
				Sys_OnJoinRoom(false);
			}
		}
		else if (WJ::e_gs_ass_user_action_login_cancel == socketMessage->messageHead.bAssistantID)
		{
			if (WJ::e_gs_hcid_login_cancel_suc_ == socketMessage->messageHead.bHandleCode)
			{
				SystemLog()->LogToFile("sdp_test c 游戏服务器取消登陆成功，开始申请退费");
				//退费
				WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_apply, (UINT)WJ::e_hs_ass_apply_restitution, NULL, 0);
				DataDrocessing::inobgect()->OnContestButUnlock();
			} 
			else if (WJ::e_gs_hcid_login_cancel_err_ == socketMessage->messageHead.bHandleCode)
			{
				//暂不处理
			}
		}
	}break;
	case WJ::e_gs_mess_queue:
	{
		if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_queue_allot)
		{
			WJ::RoomMessDate_queue_allot* p = (WJ::RoomMessDate_queue_allot*)socketMessage->object;
			m_udeskid = p->udeskid;//桌子id
			m_udeskstation = p->udeskstation;//座位
			m_bOwerUserid = p->bOwerUserid;//是否为桌子主人
			SystemLog()->LogToFile("sdp_test c 游戏服务器---匹配成功,立马请求桌子数据");
			sendData((UINT)WJ::e_gs_mess_game_frame, (UINT)WJ::e_gs_ass_game_frame_info, NULL, 0);

		}
	}break;
	case WJ::e_gs_mess_user_list:
	{
		if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_user_list_get_desk_userinfo)
		{//在游戏开始之前，在获取桌子基础数据 和 状态数据的同时，服务端紧接着也发送了桌子内所有用户数据，所有这里可以作为
			//游戏开始前的所有服务端准备工作已经完成，接下来客户端可以着手做好准备开始游戏前的所有动作了,最后记得发起举手同意开始游戏的消息
			SystemLog()->LogToFile("sdp_test c 游戏服务器---获取桌子里面所有玩家的数据");
			int size = sizeof(WJ::UserInfoInRoomStruct);
			for (int i = 0; i < socketMessage->objectSize / size; i++)
			{
				WJ::UserInfoInRoomStruct* pCmd = (WJ::UserInfoInRoomStruct*)(socketMessage->object + i*(sizeof(WJ::UserInfoInRoomStruct)));
				WJ::UserInfoInRoomStruct cmd;
				std::memcpy(&cmd, pCmd, sizeof(WJ::UserInfoInRoomStruct));
				SystemLog()->LogToFile("sdp_test c 用户数据(uDeskStation=%d,dwUserID=%d,nickName=%s)", cmd.uDeskStation, cmd.basedata.dwUserID, cmd.basedata.nickName);
				m_UserDatalist.push_back(cmd);
				
				///lol/////////////////////////////////
				if (e_gameid_10000001 == CommonData()->m_NowGame)
				{
					//请求solo赛匹配到的对手详细信息
					QEventLoop loop;
					QNetworkAccessManager Manager;
					//创建一个请求
					QNetworkRequest requestq;
					QString userId = QString::number(cmd.basedata.dwUserID, 10);
					QString str;
					str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/userInfo?id=") + userId;
					requestq.setUrl(QUrl(str));
					//发送GET请求
					QNetworkReply *p = Manager.get(requestq);
					QObject::connect(p, SIGNAL(finished()), &loop, SLOT(quit()));//关键之处1
					loop.exec();//关键之处2
					QByteArray data = p->readAll();

					//通知H5匹配到的对手信息
					QJsonObject sendjsonpart;
					sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_3007);
					sendjsonpart.insert(QStringLiteral("TYPE"), QString::fromUtf8(data).toStdString().c_str());
					QJsonDocument documentpack;
					documentpack.setObject(sendjsonpart);
					QString json_json(documentpack.toJson(QJsonDocument::Compact));
					WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
				}
				//////////////////////////////////////
			}
			WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2011, 0, 0);
		}
	}break;
	case WJ::e_gs_mess_room:
	{
		if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_room_info)
		{//获取房间数据
			SystemLog()->LogToFile("sdp_test c 游戏服务器---房间数据");
			WJ::Room_ManageInfoStruct* pCmd = (WJ::Room_ManageInfoStruct*)socketMessage->object;
			std::memcpy(&m_RoomInfo, pCmd, sizeof(WJ::Room_ManageInfoStruct));

			SystemLog()->LogToFile("sdp_test c 游戏服务器---房间数据(award=%d, %d)(_i64cost=%I64d, %I64d)"
				, m_RoomInfo.award, pCmd->award
				, m_RoomInfo._i64cost, pCmd->_i64cost);
		}
	}break;
	case WJ::e_gs_mess_game_frame:
	{
		if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_info)
		{//获取到游戏桌子数据消息
			SystemLog()->LogToFile("sdp_test c 游戏服务器---获取游戏桌子数据消息");
			WJ::RoomMessDate_deskinfo* pCmd = (WJ::RoomMessDate_deskinfo*)socketMessage->object;
			CommonData()->GameInformation.GameState = pCmd->bGameStation;
		}
		else if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_station)
		{//获取到游戏当前状态数据消息

		}
		else if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_agree)
		{//有人发起了同意开始游戏
			SystemLog()->LogToFile("sdp_test c 有人发起了同意开始游戏");
			WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2013, 0, 0);
		}
		else if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_begin)
		{//游戏正式开始
			SystemLog()->LogToFile("sdp_test c 游戏正式开始");
			WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2012, 0, 0);
		}
		else if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_end_c)
		{//某玩家主动告知自己结束了
			SystemLog()->LogToFile("sdp_test c 游戏服务器---有玩家达到条件断开关闭游戏");	
		}
		else if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_end_s)
		{//游戏正式结束
		}
		else if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_user_leave)
		{//玩家在非游戏进行中离开桌子
			m_IsLOLPVPExist = false;
			Sys_OnDisposeErr(e_gameid_10000001);

			QJsonObject sendjsonpart;
			sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_3013);
			QJsonDocument documentpack;
			documentpack.setObject(sendjsonpart);
			QString json_json(documentpack.toJson(QJsonDocument::Compact));
			WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

			char s[MAX_PATH] = { 0 };
			sprintf_s(s, "对手掉线，请重新开始匹配。");
			WJ_SAFE_QBoxMess(WJMainWindow(), QString(QString::fromLocal8Bit(s)));
		}

	}break;
	case WJ::e_gs_mess_game_notify:
	{

	}break;
	default:
		break;
	}

	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->onSocketMessage(socketMessage);
	}
}