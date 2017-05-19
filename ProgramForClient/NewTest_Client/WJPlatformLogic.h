#ifndef __WJ_WJPlatformLogic_H__
#define __WJ_WJPlatformLogic_H__

#include "clientsocket/WJSocketMessageDelegate.h"
#include "clientsocket/wjclientsocketlogic.h"
#include <list>
#include <mutex>
#include "message/WJNetPlatformMessage.h"
#include "WJRoomLogic.h"
namespace WJ
{
	/*********************************************************
	*
	*						类说明
	*
	*********************************************************/
	class WJPlatformLogic : public ISocketMessageDelegate
	{
	public:
		// 构造函数是私有的
		WJPlatformLogic(UINT index);

		// 析构函数
		virtual ~WJPlatformLogic(void);

		// 初始化
		bool init();
		//执行
		void run();
	public:
		BYTE GetSocketState();

		// 连接平台
		bool connect(char *cip, int iport);

		// 关闭平台
		bool close();
		
		// 是否连接
		bool isConnect() const;

		// 是否登陆
		bool isLogin() const;
		
		// 发送数据
		void sendData(UINT MainID, UINT AssistantID, void* object = nullptr, INT objectSize = 0);

		// 发送数据（带回调）
		//SEL_SocketMessage 使用举例
		//PlatformLogic()->sendData(1, 1, nullptr, 0,WJ_SOCKET_CALLBACK(BindPhoneLayer::modifyUserInfoSelector, this));
		void sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector);
		
		// 添加指定消息监听
		//SEL_SocketMessage 使用举例
		//PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_ACCEPT, WJ_SOCKET_CALLBACK(BindPhoneLayer::modifyUserInfoSelector, this));
		void addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector);

		// 移除指定消息监听
		void removeEventSelector(UINT MainID, UINT AssistantID);

		// 登陆
		void login(const std::string& name, const std::string& password, UINT uNameID);
	public:
		// 连接消息
		virtual void onConnected(bool connect, emSocketStatus status) override;
		
		// 断线消息
		virtual void onDisConnect() override;
		
		// TCP消息转发
		virtual void onSocketMessage(WJSocketMessage* socketMessage) override;

		//获得玩家数据
		UserInfoInHallStruct* getuserinfo();
		//获得房间节点数据
		ComRoomInfo* getComRoomInfo();

	private:
		UINT m_index;
		//username
		char m_username[NAME_LENGTH];
		//房间
		WJRoomLogic m_WJRoomLogic;

		// Socket逻辑
		WJClientSocketLogic*	m_socketLogic;

		// 平台是否登陆标志
		bool			m_logined;

		// 服务器地址（ip或者域名）
		std::string		m_serverAddress;

		// 服务器端口
		INT				m_serverPort;

		struct_data_hs_hcid_login_suc m_userinfo;

		std::vector<ComGameinfo> m_game_node;				//游戏节点
		std::vector<ComEreainfo> m_erea_node;				//区服节点
		std::vector<ComContestInfo> m_contest_node;			//比赛节点
		ComRoomInfo m_room_node;				//房间节点
	private:
		//类拷贝是私有的
		WJPlatformLogic(const WJPlatformLogic &);
		WJPlatformLogic & operator = (const WJPlatformLogic &);
	};
}

#endif	//__WJ_WJPlatformLogic_H__

