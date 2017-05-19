#include "WJPlatformLogic.h"
#include "clientconfig.h"
#include "message/WJNetPlatformMessage.h"
#include <windows.h>
#include "clientsocket/commonfunction_c.h"
namespace WJ
{
	WJPlatformLogic::WJPlatformLogic(UINT index)
		: m_logined(false)
		, m_serverPort(INVALID_VALUE)
	{
		m_index = index;
		m_socketLogic = new WJClientSocketLogic(this, "platform");
		m_WJRoomLogic.SetWJPlatformLogic(this);

		sprintf_s(m_username, "%s%d", AI_NAME_TAG, (clientconfig().GetData()->AIFirstID + m_index));
	}

	WJPlatformLogic::~WJPlatformLogic(void)
	{
		WJ_SAFE_DELETE(m_socketLogic);
	}

	bool WJPlatformLogic::init()
	{
		return true;
	}

	//获得玩家数据
	UserInfoInHallStruct* WJPlatformLogic::getuserinfo()
	{
		return &m_userinfo;
	}

	//获得房间节点数据
	ComRoomInfo* WJPlatformLogic::getComRoomInfo()
	{
		return &m_room_node;
	}

	bool WJPlatformLogic::connect(char *cip, int iport)
	{
		if (nullptr == cip)
		{
			m_serverAddress = clientconfig().GetData()->hall_server_address;
		}
		else
		{
			m_serverAddress = cip;
		}

		if (0 >= iport)
		{
			m_serverPort = PORT_LS;
		}
		else
		{
			m_serverPort = iport;
		}

		return m_socketLogic->openWithIp(m_serverAddress.c_str(), m_serverPort);
	}

	bool WJPlatformLogic::close()
	{
		MyDebugString("sdp_test", "client 初始化房间");
		m_WJRoomLogic.init();//初始化房间
		return m_socketLogic->close();
	}

	BYTE WJPlatformLogic::GetSocketState()
	{
		return m_socketLogic->GetSocketState();
	}

	bool WJPlatformLogic::isConnect() const
	{
		return m_socketLogic->connected();
	}

	bool WJPlatformLogic::isLogin() const
	{
		return m_logined;
	}

	void WJPlatformLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
	{
		m_socketLogic->send(MainID, AssistantID, object, objectSize);
	}

	void WJPlatformLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector)
	{
		m_socketLogic->send(MainID, AssistantID, object, objectSize);
		m_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}

	void WJPlatformLogic::addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector)
	{
		m_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}

	void WJPlatformLogic::removeEventSelector(UINT MainID, UINT AssistantID)
	{
		m_socketLogic->removeEventSelector(MainID, AssistantID);
	}

	void WJPlatformLogic::login(const std::string& name, const std::string& password, UINT uNameID)
	{

	}

	void WJPlatformLogic::onConnected(bool connect, emSocketStatus status)
	{
		if (SocketStatus_OUTTIME == status)
		{
			MyDebugString("sdp_test", "client 连接超时的消息 WJPlatformLogic(%d)", m_index);
		}
		else if (SocketStatus_SUCCESS == status)
		{
			//char sdp[MAX_PATH] = { 0 };
			//sprintf_s(sdp, "sdp_test client 连接成功的消息 WJPlatformLogic(%d)", m_index);
			//OutputDebugStringA(sdp);
		}
	}

	void WJPlatformLogic::onDisConnect()
	{
		MyDebugString("sdp_test", "client 连接失败的消息 WJPlatformLogic(%d)", m_index);
	}

	void WJPlatformLogic::onSocketMessage(WJSocketMessage* socketMessage)
	{
		//所有收到的消息处理函数在这里写
		switch (socketMessage->messageHead.bMainID)
		{
		case NET_M_CONNECT:
		{//连接成功
			if (ASS_NET_CONNECT_2 == socketMessage->messageHead.bAssistantID)
			{//连接成功才能发起登陆
				struct_data_hs_mess_login cmd;
				sprintf_s(cmd.username, "%s", m_username);
				sprintf_s(cmd.password, "%s", AI_PASS_TAG);
				sendData(e_hs_mess_login, e_hs_ass_login, &cmd, sizeof(cmd));

				MyDebugString("sdp_test", "client 连接成功才能发起登陆 run_message(%d)", m_index);
				break;
			}
		}break;
		case e_hs_mess_login:
		{//登陆
			switch (socketMessage->messageHead.bAssistantID)
			{
			case e_hs_ass_login:
			{
				if (e_hs_hcid_login_suc == socketMessage->messageHead.bHandleCode)
				{//登陆成功，保存玩家数据
					struct_data_hs_hcid_login_suc *p = (struct_data_hs_hcid_login_suc*)socketMessage->object;
					if (nullptr != p && socketMessage->objectSize == sizeof(struct_data_hs_hcid_login_suc))
					{
						m_logined = true;
						std::memcpy(&m_userinfo, p, sizeof(struct_data_hs_hcid_login_suc));
						MyDebugString("sdp_test", "client 登陆成功,发送请求游戏列表(%d)", m_index);
						//发送请求游戏列表
						sendData(e_hs_mess_gamelist, e_hs_ass_gamelist_game, nullptr, 0);
						break;
					}
				}
				else
				{//登陆失败就断开连接
					MyDebugString("sdp_test", "client 登陆失败就断开连接(%d)", m_index);
					close();
					break;
				}
			}break;
			default:
				break;
			}
		}break;
		case e_hs_mess_gamelist:
		{//游戏列表
			switch (socketMessage->messageHead.bAssistantID)
			{
			case e_hs_ass_gamelist_game:
			{//游戏
				UINT ucount = socketMessage->objectSize / sizeof(ComGameinfo);
				for (UINT i = 0; i < ucount; ++i)
				{
					ComGameinfo cmd;
					std::memcpy(&cmd, socketMessage->object + i*sizeof(ComGameinfo), sizeof(ComGameinfo));
					bool bexist = false;
					auto iter = m_game_node.begin();
					while (iter != m_game_node.end())
					{
						if ((*iter).gameid == cmd.gameid)
						{//存在就覆盖
							std::memcpy(&(*iter), &cmd, sizeof(ComGameinfo));
							bexist = true;
						}
						++iter;
					}

					if (!bexist)
					{
						m_game_node.push_back(cmd);
					}

					struct_data_hs_ass_gamelist_erea cmd2;
					cmd2.gameid = cmd.gameid;
					sendData(e_hs_mess_gamelist, e_hs_ass_gamelist_erea, &cmd2, sizeof(struct_data_hs_ass_gamelist_erea));
				}
			}break;
			case e_hs_ass_gamelist_erea:
			{//区服
				UINT ucount = socketMessage->objectSize / sizeof(ComEreainfo);
				for (UINT i = 0; i < ucount; ++i)
				{
					ComEreainfo cmd;
					std::memcpy(&cmd, socketMessage->object + i*sizeof(ComEreainfo), sizeof(ComEreainfo));
					bool bexist = false;
					auto iter = m_erea_node.begin();
					while (iter != m_erea_node.end())
					{
						if ((*iter).ereaid == cmd.ereaid)
						{//存在就覆盖
							std::memcpy(&(*iter), &cmd, sizeof(ComEreainfo));
							bexist = true;
						}
						++iter;
					}

					if (!bexist)
					{
						m_erea_node.push_back(cmd);
					}

					struct_data_hs_ass_gamelist_contest cmd2;
					cmd2.ereaid = cmd.ereaid;
					sendData(e_hs_mess_gamelist, e_hs_ass_gamelist_contest, &cmd2, sizeof(struct_data_hs_ass_gamelist_contest));
				}
			}break;
			case e_hs_ass_gamelist_contest:
			{//比赛
				UINT ucount = socketMessage->objectSize / sizeof(ComContestInfo);
				for (UINT i = 0; i < ucount; ++i)
				{
					ComContestInfo cmd;
					std::memcpy(&cmd, socketMessage->object + i*sizeof(ComContestInfo), sizeof(ComContestInfo));
					bool bexist = false;
					auto iter = m_contest_node.begin();
					while (iter != m_contest_node.end())
					{
						if ((*iter).contestid == cmd.contestid)
						{//存在就覆盖
							std::memcpy(&(*iter), &cmd, sizeof(ComContestInfo));
							bexist = true;
						}
						++iter;
					}

					if (!bexist)
					{
						m_contest_node.push_back(cmd);
					}

					if (clientconfig().GetData()->contestid == cmd.contestid)
					{
						struct_data_hs_ass_gamelist_room cmd2;
						cmd2.contestid = cmd.contestid;
						sendData(e_hs_mess_gamelist, e_hs_ass_gamelist_room, &cmd2, sizeof(struct_data_hs_ass_gamelist_room));
					}
				}
			}break;
			case e_hs_ass_gamelist_room:
			{
				UINT ucount = socketMessage->objectSize / sizeof(ComRoomInfo);
				for (UINT i = 0; i < ucount; ++i)
				{
					ComRoomInfo cmd;
					std::memcpy(&cmd, socketMessage->object + i*sizeof(ComRoomInfo), sizeof(ComRoomInfo));
					if (clientconfig().GetData()->roomid == cmd.roomid)
					{//覆盖
						std::memcpy(&m_room_node, &cmd, sizeof(ComRoomInfo));

						MyDebugString("sdp_test", "client 发送登陆完成(%d)", m_index);
						//发送登陆完成
						sendData(e_hs_mess_login, e_hs_ass_login_c_finish, nullptr, 0);
						break;
					}
				}
			}break;
			default:
				break;
			}
		}break;
		case e_hs_mess_apply:
		{//报名
			switch (socketMessage->messageHead.bAssistantID)
			{
			case e_hs_ass_apply:
			{//请求报名结果
				if (e_hs_hcid_apply_suc == socketMessage->messageHead.bHandleCode)
				{
					MyDebugString("sdp_test", "client 报名成功,连接游戏服务器(%d)", m_index);
					//连接游戏服务器
					m_WJRoomLogic.connect(m_room_node.strIP, m_room_node.uport);
				}
				else
				{
					MyDebugString("sdp_test", "client 报名失败,断开连接(%d)", m_index);
					//一旦错误则离开大厅
					close();
				}
			}break;
			case e_hs_ass_apply_login_recome_apply:
			{//玩家断线后回来发现还未加入房间，发送该消息给客户端
				MyDebugString("sdp_test", "client 玩家断线后回来发现还未加入房间,连接游戏服务器(%d)", m_index);
				//连接游戏服务器
				m_WJRoomLogic.connect(m_room_node.strIP, m_room_node.uport);
			}break;
			case e_hs_ass_apply_login_recome_playing:
			{//玩家断线后回来发现已经加入房间，发送该消息给客户端
				MyDebugString("sdp_test", "client 玩家断线后回来发现已经加入房间,连接游戏服务器(%d)", m_index);
				//连接游戏服务器
				m_WJRoomLogic.connect(m_room_node.strIP, m_room_node.uport);
			}break;
			case e_hs_ass_apply_restitution:
			{//退费结果
				//暂不处理
				MyDebugString("sdp_test", "client 退费结果,暂不处理(%d)", m_index);
			}break;
			case e_hs_ass_apply_unplay:
			{//玩家没有在房间里面玩，比赛继续有效
				MyDebugString("sdp_test", "client 玩家没有在房间里面玩,比赛继续有效,连接游戏服务器(%d)", m_index);
				//连接游戏服务器
				m_WJRoomLogic.connect(m_room_node.strIP, m_room_node.uport);
			}break;
			case e_hs_ass_unapply:
			{//玩家并没有报名参加任何比赛
				MyDebugString("sdp_test", "client 玩家并没有报名参加任何比赛,发起报名(%d)", m_index);
				//发起报名
				struct_data_hs_mess_apply cmd;
				cmd.ucontestid = clientconfig().GetData()->contestid;
				sendData(e_hs_mess_apply, e_hs_ass_apply, &cmd, sizeof(struct_data_hs_mess_apply));
			}break;
			default:
				break;
			}
		}break;
		default:
		{
			break;
		}
		}
	}

	//执行
	void WJPlatformLogic::run()
	{
		if (NO_CONNECT == GetSocketState())
		{//未连接先连接
			char*  ch;
			ch = (char*)(clientconfig().GetData()->hall_server_address.c_str());
			MyDebugString("sdp_test", "client 执行Connect(%d,%x)", m_index, GetSocketState());
			connect(ch, 0);
			return;
		}
	}
}
