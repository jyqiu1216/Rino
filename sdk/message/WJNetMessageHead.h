#ifndef _WJ_MessageHead_H_
#define _WJ_MessageHead_H_

//#include <QDataStream>
#include "common/WJBaseType.h"
#include "common/WJCommonMarco.h"
namespace WJ
{
#define		MAX_PEOPLE				20				///桌子上最大游戏人数
#define		MAX_SEND_SIZE			4096 			//最大消息包

#define		MAX_LOGION_PLAYERMUN	5000			//最多登陆人数
#define		MAX_GAME_PLAYERMUN		200				//最多一个房间人数

#define		SECRECTKEY				20160316		//通信密钥

	///端口定义
#define PORT_LS				13050					//登陆服务器端口号
#define PORT_LS_COUNT		20000					//登陆服务器接入数量

#define PORT_GS				13051					//游戏服务器端口号
#define PORT_GS_COUNT		200						//游戏服务器接入数量

#define PORT_LS_TO_GS		13052					//登陆服务器与游戏服务器通信端口号
#define PORT_LS_TO_GS_COUNT	500						//登陆服务器与游戏服务器通信接入数量

	enum emSocketStatus
	{
		SocketStatus_UNKNOW = 0,	//未知状态
		SocketStatus_CONNECT,		//发起连接状态
		SocketStatus_OUTTIME,		//连接超时状态
		SocketStatus_SUCCESS,		//连接成功状态
		SocketStatus_RECV,			//接收消息状态
		SocketStatus_DISCONNECT,	//连接失败状态
	};

#pragma  pack(1)
	// network packet header
	typedef struct tagNetMessageHead
	{
		UINT						uMessageSize;			// 数据包大小
		UINT						bMainID;				// 处理主类型
		UINT						bAssistantID;			// 辅助处理类型 ID
		UINT						bHandleCode;			// 第三级id
		UINT						bReserve;				// 保留字段

		//tagNetMessageHead()
		//{
		//	std::memset(this, 0, sizeof(tagNetMessageHead));
		//}
	}  NetMessageHead;

	// 连接成功消息 
	typedef struct tagNet_ConnectSuccess
	{
		BYTE							bMaxVer;							//最新版本号码
		BYTE							bLessVer;							//最低版本号码
		BYTE							bReserve[2];						//保留字段
		UINT							i64CheckCode;						//加密后的校验码，由客户端解密在包头中返回

		tagNet_ConnectSuccess()
		{
			std::memset(this, 0, sizeof(tagNet_ConnectSuccess));
		}
	} Net_ConnectSuccess;
#pragma pack()

	//////////////////////////////////////////////////////////////////////////
	// 客户端到服务端心跳指令
#define		NET_M_CONNECT			1
#define		ASS_NET_CONNECT_1		1//相互间询问
#define		ASS_NET_CONNECT_2		2//
}

#endif // !_WJ_MessageHead_H_
