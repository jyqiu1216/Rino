#include "WJRoomLogic.h"
#include "clientconfig.h"
#include "message/WJNetRoomMessage.h"
#include "WJPlatformLogic.h"
#include "../../Games/10000001_lol/MyGameDeskData_10000001.h"
#include <windows.h>
namespace WJ
{
	WJRoomLogic::WJRoomLogic(void)
		: m_logined(false)
		, m_serverPort(INVALID_VALUE)
	{
		m_pWJPlatformLogic = nullptr;
		m_socketLogic = new WJClientSocketLogic(this, "room");
	}

	WJRoomLogic::~WJRoomLogic(void)
	{
		WJ_SAFE_DELETE(m_socketLogic);
	}

	//设置该房间的大厅类
	void WJRoomLogic::SetWJPlatformLogic(WJPlatformLogic *pWJPlatformLogic)
	{
		m_pWJPlatformLogic = pWJPlatformLogic;
	}

	bool WJRoomLogic::init()
	{
		//断开套接字
		close();
		//清理数据
		m_serverAddress = "";
		m_serverPort = 0;
		m_logined = false;
		return true;
	}

	bool WJRoomLogic::connect(char *cip, int iport)
	{
		OutputDebugStringA("sdp_test client connect的时候先 初始化房间");
		init();//初始化房间

		m_serverAddress = cip;
		m_serverPort = iport;
		return m_socketLogic->openWithIp(m_serverAddress.c_str(), m_serverPort);
	}

	bool WJRoomLogic::close()
	{
		return m_socketLogic->close();
	}

	bool WJRoomLogic::isConnect() const
	{ 
		return m_socketLogic->connected(); 
	} 

	bool WJRoomLogic::isLogin() const
	{
		return m_logined;
	}

	void WJRoomLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
	{	
		m_socketLogic->send(MainID, AssistantID, object, objectSize);
	}

	void WJRoomLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector)
	{
		m_socketLogic->send(MainID, AssistantID, object, objectSize);
		m_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}

	void WJRoomLogic::addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector)
	{
		m_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}

	void WJRoomLogic::removeEventSelector(UINT MainID, UINT AssistantID)
	{
		m_socketLogic->removeEventSelector(MainID, AssistantID);
	}

	void WJRoomLogic::login(const std::string& name, const std::string& password, UINT uNameID)
	{

	} 
	
	void WJRoomLogic::onConnected(bool connect, emSocketStatus status)
	{
		if (SocketStatus_OUTTIME == status)
		{
			OutputDebugStringA("sdp_test client 连接超时的消息 WJRoomLogic");
		} 
		else if (SocketStatus_SUCCESS == status)
		{
			OutputDebugStringA("sdp_test client 连接成功的消息 WJRoomLogic");
		}
	}

	void WJRoomLogic::onDisConnect()
	{
		OutputDebugStringA("sdp_test client 连接失败的消息 WJRoomLogic");
	}

	void WJRoomLogic::onSocketMessage(WJSocketMessage* socketMessage)
	{
		//所有收到的消息处理函数在这里写
		switch (socketMessage->messageHead.bMainID)
		{
		case NET_M_CONNECT:
		{//连接成功
							  if (ASS_NET_CONNECT_2 == socketMessage->messageHead.bAssistantID)
							  {//连接成功才能发起登陆
								  RoomMessDate_user_action_login cmd;
								  cmd.uuserid = m_pWJPlatformLogic->getuserinfo()->basedata.dwUserID;
								  sendData(e_gs_mess_user_action, e_gs_ass_user_action_login, &cmd, sizeof(cmd));
								  break;
							  }
		}break;
		case e_gs_mess_user_list:
		{

		}break;
		case e_gs_mess_user_action:
		{
									  switch (socketMessage->messageHead.bAssistantID)
									  {
									  case e_gs_ass_user_action_login:
									  {//登陆
																		 if (e_gs_hcid_login_suc_ == socketMessage->messageHead.bHandleCode)
																		 {
																			 std::memcpy(&m_userinfo_r, &socketMessage->object, sizeof(UserInfoInRoomStruct));
																			 //等待搓桌，什么都不做
																		 }
																		 else  if (e_gs_hcid_login_suc_recome == socketMessage->messageHead.bHandleCode)
																		 {
																			 //断线重回
																		 }
																		 else
																		 {//登陆房间失败，则断开大厅和房间
																			 m_pWJPlatformLogic->close();
																		 }
									  }break;
									  case e_gs_ass_user_action_force_out:
									  {//强退
																			 //不用处理	 
									  }break;
									  default:
										  break;
									  }
		}break;
		case e_gs_mess_room:
		{

		}break;
		case e_gs_mess_queue:
		{
								if (e_gs_ass_queue_allot == socketMessage->messageHead.bAssistantID)
								{//收到搓桌成功
									RoomMessDate_queue_allot cmd;
									std::memcpy(&cmd, &socketMessage->object, sizeof(RoomMessDate_queue_allot));
									m_userinfo_r.uDeskid = cmd.udeskid;
									m_userinfo_r.uDeskStation = cmd.udeskstation;

									sendData(e_gs_mess_game_frame, e_gs_ass_game_frame_info, nullptr, 0);
								}
		}break;
		case e_gs_mess_game_frame:
		{
									 switch (socketMessage->messageHead.bAssistantID)
									 {
									 case e_gs_ass_game_frame_info:
									 {//获取桌子数据
																	  RoomMessDate_deskinfo cmd;
																	  std::memcpy(&cmd, &socketMessage->object, sizeof(RoomMessDate_deskinfo));
																	  m_uGameStation = cmd.bGameStation;
									 }break;
									 case e_gs_ass_game_frame_station:
									 {//服务端游戏内部发送断线重回消息
																		 switch (m_uGameStation)
																		 {
																		 case WJ_gameid_10000001::game_station_wait:
																		 {
																													   WJ_gameid_10000001::GAME_STATION_WAIT cmd;
																													   std::memcpy(&cmd, &socketMessage->object, sizeof(WJ_gameid_10000001::GAME_STATION_WAIT));

																													   OutputDebugStringA("sdp_test client 客户端游戏收到断线重回消息。");
																													   char sdp[MAX_PATH] = { 0 };
																													   sprintf_s(sdp, "sdp_test client (roomname=%s,password=%x%x%x%x%x%x,mode=%d,map=%d,playernum=%d,observe=%d,gametype=%d,side=%d,bCreate=%d,dwUserID=%d)。"
																														   , cmd.roomname
																														   , cmd.password[0], cmd.password[1], cmd.password[2], cmd.password[3], cmd.password[4], cmd.password[5]
																														   , cmd.mode
																														   , cmd.map
																														   , cmd.playernum
																														   , cmd.observe
																														   , cmd.gametype
																														   , cmd.side
																														   , cmd.bCreate
																														   , m_userinfo_r.basedata.dwUserID);
																													   OutputDebugStringA(sdp);

																													   //发起举手
																													   sendData(e_gs_mess_game_frame, e_gs_ass_game_frame_agree, nullptr, 0);
																		 }break;
																		 case WJ_gameid_10000001::game_station_playing:
																		 {
																														  WJ_gameid_10000001::GAME_STATION_PLAYING cmd;
																														  std::memcpy(&cmd, &socketMessage->object, sizeof(WJ_gameid_10000001::GAME_STATION_PLAYING));
																														  WJ_gameid_10000001::GAME_SIDE side = cmd.side;
																		 }break;
																		 default:
																			 break;
																		 }

									 }break;
									 case e_gs_ass_game_frame_agree:
									 {//玩家同意消息

									 }break;
									 case e_gs_ass_game_frame_begin:
									 {//游戏消息
																	   sendData(e_gs_mess_game_frame, e_gs_ass_game_frame_end_c, nullptr, 0);
									 }break;
									 case e_gs_ass_game_frame_end_s:
									 {//游戏正式结束																		   
																	   //离开房间，大厅也断开
																	   m_pWJPlatformLogic->close();
									 }break;
									 default:
										 break;
									 }
		}break;
		case e_gs_mess_game_notify:
		{

		}break;
		default:
		{
				   break;
		}
		}
	}
}
