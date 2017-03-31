
#ifndef WJ_CINTERFACE_FILE
#define WJ_CINTERFACE_FILE
#include "common/LoadWindowsMess.h"
#include <QtCore>
#include <QObject> 
#include <QtNetwork>
#include <QWebSocket>
#include <qbytearray.h>
#include <QString>
#include "common/Common.h"
#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"
#include "websocket/WebSocketMessInfo.h"
#include "clientsocket/WJSocketMessage.h"

#include "websocket/WebSocketServer.h"
#include "websocket/DataDrocessing.h"
#include "Qthread/RoomLogic.h"
#include "WJPlatformLogic.h"

/********************************************************************************************/

///服务接口定义

/********************************************************************************************/

class IBaseMyGameRoom;
class IBaseMyHallLogic;

///为游戏封装的处理H5通信接口 
class IBaseMyDataDrocessing : public QObject
{
	Q_OBJECT
	///接口函数
public:
	IBaseMyDataDrocessing();
	~IBaseMyDataDrocessing();

	virtual void Processing(int iMSGID, QJsonObject &_TYPE) = 0;//数据处理
	virtual void InitData(IBaseMyHallLogic* phall, IBaseMyGameRoom* proom) = 0;//将其他该游戏相关的类保存
};

///为游戏封装的处理大厅逻辑接口
class IBaseMyHallLogic : public QObject
{
	Q_OBJECT

	///接口函数
public:
	IBaseMyHallLogic();
	~IBaseMyHallLogic();
	//将其他该游戏相关的类保存
	virtual void InitData(IBaseMyDataDrocessing* pDataDrocessing, IBaseMyGameRoom* proom) = 0;
	//收到与自己相关的本地消息
	virtual bool OnMess(const QByteArray &eventType, void *message, long *result) = 0;
	// TCP消息转发
	virtual void onSocketMessage(WJ::WJSocketMessage* socketMessage) = 0;
};

///为游戏封装的处理房间逻辑接口 
class IBaseMyGameRoom : public QObject
{
	Q_OBJECT
	///接口函数
public:
	IBaseMyGameRoom();
	~IBaseMyGameRoom();

	virtual void InitData(IBaseMyDataDrocessing* pDataDrocessing, IBaseMyHallLogic* phall) = 0;//将其他该游戏相关的类保存
	//加入房间
	virtual void OnJoinRoom(bool bFirst) = 0;
	//开始
	virtual bool OnStart() = 0;
	//初始化数据
	virtual void ReSetData() = 0;
	//游戏过程中异常错误，需要清理不正确的数据，保证可以再次点击一键开赛
	virtual void OnDisposeErr() = 0;
	//离开当前比赛详情页
	virtual void OnLeave() = 0;
	//游戏结束
	virtual void OnEnd() = 0;
	//收到与自己相关的本地消息
	virtual bool OnMess(const QByteArray &eventType, void *message, long *result) = 0;
	// 连接消息
	virtual void onConnected(bool connect, WJ::emSocketStatus status) = 0;
	// 断线消息
	virtual void onDisConnect() = 0;
	// TCP消息转发
	virtual void onSocketMessage(WJ::WJSocketMessage* socketMessage) = 0;
};
#endif//WJ_CINTERFACE_FILE