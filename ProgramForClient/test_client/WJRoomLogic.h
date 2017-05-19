#ifndef __WJ_WJRoomLogic_H__
#define __WJ_WJRoomLogic_H__

#include "clientsocket/WJSocketMessageDelegate.h"
#include "clientsocket/wjclientsocketlogic.h"
#include "common/WJUserInfoBaseStruct.h"
namespace WJ
{
	class WJPlatformLogic;
	/*********************************************************
	*
	*						类说明
	*
	*********************************************************/
	class WJRoomLogic : public ISocketMessageDelegate
	{
	public:
		// 构造函数是私有的
		WJRoomLogic(void);

		// 析构函数
		virtual ~WJRoomLogic(void);

		// 初始化
		bool init();

	public:
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

		//设置该房间的大厅类
		void SetWJPlatformLogic(WJPlatformLogic *pWJPlatformLogic);

	private:
		WJPlatformLogic* m_pWJPlatformLogic;
		// Socket逻辑
		WJClientSocketLogic*	m_socketLogic;

		// 平台是否登陆标志
		bool			m_logined;

		// 服务器地址（ip或者域名）
		std::string		m_serverAddress;

		// 服务器端口
		INT				m_serverPort;

		UserInfoInRoomStruct m_userinfo_r;//房间内玩家数据
		UINT							m_uGameStation;							///< 游戏状态
	private:
		//类拷贝是私有的
		WJRoomLogic(const WJRoomLogic &);
		WJRoomLogic & operator = (const WJRoomLogic &);
	};
}

#endif	//__WJ_WJRoomLogic_H__

