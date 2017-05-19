#ifndef GAMEQTHREAD_H
#define GAMEQTHREAD_H
#include <QThread>
#include <QObject>
#include <QStringList>
#include <QMutex>
#include <string>

#include "automation/AutomateManager.h"
#include "../hook/HookManager.h"
#include "../hook/ExtendGameManager.h"
#include "clientsocket/WJSocketMessageDelegate.h"
#include "clientsocket/WJSocketMessage.h"
#include "common/WJUserInfoBaseStruct.h"
#include "server/comstruct.h"
#include "clientsocket/wjclientsocketlogic.h"
class ExtendGameManager;
class AutomateManager;
enum E_GameID;

struct struct_StartContest
{
	bool bTrue;//是否存在数据
	QJsonObject _QJsonObject_StartContest;

	struct_StartContest()
	{
		bTrue = false;
	}
};

class CRoomLogic : public QObject, public WJ::ISocketMessageDelegate
{
	Q_OBJECT
		//函数
public:
	////////////////////////////////////
	//需要设计成模板供游戏逻辑套用的方法
	//开始
	bool Sys_OnStart();
	//初始化数据
	void Sys_ReSetData();
	//游戏过程中异常错误，需要清理不正确的数据，保证可以再次点击一键开赛
	void Sys_OnDisposeErr(E_GameID igameid);
	//离开当前比赛详情页
	void Sys_OnLeave();
	//游戏结束
	void Sys_OnEnd();
	//加入房间
	void Sys_OnJoinRoom(bool bFirst);
	//收到与自己相关的本地消息
	bool Sys_OnMess(const QByteArray &eventType, void *message, long *result);
	// 连接消息
	virtual void onConnected(bool connect, WJ::emSocketStatus status) override;
	// 断线消息
	virtual void onDisConnect() override;
	// TCP消息转发
	virtual void onSocketMessage(WJ::WJSocketMessage* socketMessage) override;
	////////////////////////////////////

public:
	CRoomLogic();
	~CRoomLogic();
	static CRoomLogic* getInstance();


	void setipport();

	//创建监控Dll注入线程
	void CreateMonitorThread();

protected:

public:

	// 连接平台
	bool connect(QString cip, int iport);
	// 发送数据
	void sendData(UINT MainID, UINT AssistantID, void* object = nullptr, INT objectSize = 0);

	// 关闭平台
	bool SocketClose();
	// 是否连接
	bool isConnect() const;
	// 是否登陆
	bool isLogin() const;

	// 发送数据（带回调）
	//SEL_SocketMessage 使用举例
	//PlatformLogic()->sendData(1, 1, nullptr, 0,WJ_SOCKET_CALLBACK(BindPhoneLayer::modifyUserInfoSelector, this));
	//void sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, WJ::SEL_SocketMessage selector);
	// 添加指定消息监听
	//SEL_SocketMessage 使用举例
	//PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_ACCEPT, WJ_SOCKET_CALLBACK(BindPhoneLayer::modifyUserInfoSelector, this));
	void addEventSelector(UINT MainID, UINT AssistantID, WJ::SEL_SocketMessage selector);
	// 移除指定消息监听
	void removeEventSelector(UINT MainID, UINT AssistantID);

public:

	//成员变量
public:
	WJ::Room_ManageInfoStruct m_RoomInfo;//房间信息
	struct_StartContest m_struct_StartContest;//点击一键比赛,一场比赛的相关信息
	UINT m_udeskid;//桌子id
	UINT m_udeskstation;//座位
	bool m_bOwerUserid;//是否为桌子主人
	std::vector<WJ::UserInfoInRoomStruct> m_UserDatalist;//桌子内用户列表
	int m_game_connect_iter_tag;//当前连接的游戏服务器节点
private:
	WJ::WJClientSocketLogic* m_WJClientSocketLogic;
	// 平台是否连接标志
	bool			_connected;
	// 平台是否登陆标志
	bool			_logined;
	// 服务器地址（ip或者域名）
	std::string		_serverAddress;
	// 服务器端口
	INT				_serverPort;
public:
	//玩家处于游戏状态
	bool bInGame;	
	//游戏开始后关闭检测LOL是否存在线程
	bool m_IsLOLPVPExist = false;
};

#define  RoomLogic()	CRoomLogic::getInstance()
#endif // GAMEQTHREAD_H
