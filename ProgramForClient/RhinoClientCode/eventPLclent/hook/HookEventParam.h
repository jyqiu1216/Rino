#ifndef _HookEventParam_H_
#define _HookEventParam_H_

#include <Windows.h>

enum EnEventID
{
	E_EVENT_ID_HS_HOOK_SUCCESS = 1,
	E_EVENT_ID_HS_HOOK_HEARTBEAT,
	
	E_EVENT_ID_HS_HOOK_AM_ReqAddFriend,
	E_EVENT_ID_HS_HOOK_AM_AcceptAddFriend,
	E_EVENT_ID_HS_HOOK_AM_ReqChallenge,
	E_EVENT_ID_HS_HOOK_AM_AcceptChallenge
};


#include "ICapture.h"

struct ParamHSCapture
{
	long long tick;				// 心跳时间戳
	bool isHooked;				// 是否被HOOK
	HANDLE sharedHandle;		// 共享句柄
	UINT captureId;				// 系统唯一ID，
	UINT offsetX;				// x offset
	UINT offsetY;				// y offset
	UINT width;					// 纹理宽度
	UINT height;				// 纹理高度
	EPIXFORMAT format;			// 纹理像素格式,系统需要根据类型进行一次处理
	int HUDLocation;			// HUD 显示的位置
	DWORD processId;			// 进程ID
	char data[256];
};


struct EventParamHS
{
	EnEventID id;				// 消息ID
	
	union 
	{
		ParamHSCapture captInfo;
	}data;
};

struct ShareParamHS
{
	int wsPort_Srv;
	int wsPort_Hook;

	char urlMatch[256];
	char urlCover[256];

	ShareParamHS()
	{
		wsPort_Srv = 0;
		wsPort_Hook = 0;

		memset(urlMatch, 0, 256);
		memset(urlCover, 0, 256);
	}
};


#endif // !_HOOKEVENTPARAM_H_
