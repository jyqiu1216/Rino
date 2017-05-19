#ifndef __WJ_WJPlatformLogic_H__
#define __WJ_WJPlatformLogic_H__
#include "clientsocket/WJSocketMessageDelegate.h"
#include "clientsocket/wjclientsocketlogic.h"
#include "message/WJNetPlatformMessage.h"
namespace WJ
{
	/*********************************************************
	*
	*						类说明
	*
	*********************************************************/
	class WJPlatformLogic : public ISocketMessageDelegate
	{
		//Q_OBJECT
	public:
		// 获取单例
		static WJPlatformLogic& getInstance();

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

		//发送数据
		void sendDataL(UINT MainID, UINT AssistantID, UINT uHandleCode,void* object = nullptr, INT objectSize = 0);
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

		// 注册
		void regist(BYTE byFastRegister, UINT uNameID, const std::string& usn, const std::string& name = "", const std::string& password = "", const std::string& tgname="");

		//报名比赛前先请求获得该比赛节点信息和比赛下的房间列表
		void getInfoBeforeApply();

		//获取押注数据
		void GetHttpBetData(int inType);
	public:
		// 连接消息
		virtual void onConnected(bool connect, emSocketStatus status) override;
		
		// 断线消息
		virtual void onDisConnect() override;
		
		// TCP消息转发
		virtual void onSocketMessage(WJSocketMessage* socketMessage) override;
	// 消息处理
	protected:

	public:
		struct_data_hs_mess_login m_logininfo;
		struct_data_hs_mess_login_WX m_logininfo_WX;
		bool m_bWX_login;//当前是微信登陆的
		bool m_bApplySuc;//solo赛报名成功
	private:
		// Socket逻辑
		WJClientSocketLogic*	_socketLogic;

		// 平台是否连接标志
		bool			_connected;

		// 平台是否登陆标志
		bool			_logined;

		// 服务器地址（ip或者域名）
		std::string		_serverAddress;

		// 服务器端口
		INT				_serverPort;

		int				_requestUserID;
		//登录失败原因  1：用户名错误 2：密码错误 3：用户已登录
		int				_logFailId;

	private:
		// 构造函数是私有的
		WJPlatformLogic(void);

		// 析构函数
		virtual ~WJPlatformLogic(void);

	private:
		//类拷贝是私有的
		WJPlatformLogic(const WJPlatformLogic &);
		WJPlatformLogic & operator = (const WJPlatformLogic &);
		
	};

#define  PlatformLogic()	WJPlatformLogic::getInstance()
}

#endif	//__WJ_WJPlatformLogic_H__

