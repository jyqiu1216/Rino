/**
* ICapture.h 抓取接口类，用于声音，屏幕，串口，网卡等数据的抓取接口类
* 版权所有 (C) 2014-2015南京正溯网络科技有限公司
* 创建日期 2015/10/25
* 修订记录 2015/10/25  
* 修订记录 2015/10/25 。
* @author：Steven Zhang
* @version V1.0.0.2
* @usage
*/


#ifndef _ICAPTURE_icapture_H_
#define _ICAPTURE_icapture_H_
#include <windows.h>

#define MAX_SHORT 1<<14

enum EPIXFORMAT
{
	EPIXFORMAT_R8,
	EPIXFORMAT_A8,
	EPIXFORMAT_A8R8G8B8,
	EPIXFORMAT_A8B8G8R8,
	EPIXFORMAT_X8R8G8B8,
	EPIXFORMAT_X8B8G8R8,
	EPIXFORMAT_R8G8B8,
	EPIXFORMAT_B8G8R8,
	EPIXFORMAT_R8G8B8A8,
	EPIXFORMAT_B8G8R8A8,
	EPIXFORMAT_RGBA16F,
	EPIXFORMAT_R32F,
	EPIXFORMAT_R64F,
	EPIXFORMAT_R32G32B32A32F,
	EPIXFORMAT_A32B32G32R32F,
	EPIXFORMAT_A32R32G32B32F, 
	EPIXFORMAT_RGBA64F
};


//HUD 的位置
enum EHUDLocation
{
	EHUDLocation_TOP_LEFT = 0
	, EHUDLocation_TOP_RIGHT = 1
	, EHUDLocation_BOTTOM_LEFT = 2
	, EHUDLocation_BOTTOM_RIGHT = 3

};
typedef long long TIMERTICKS;
//视频的相关数据区，每次发送之后，系统会根据当前系统的心跳包进行检测，如果心跳包的执行超过3秒未，则进行原始进程的判断
//抓取界面窗口大小变化时，将会产生一次设备设备重置，设备重置需要通知客户端进行句柄重置，重新进行处理
enum ECaptureType
{
	CAPTURE_HOOK_INVALID = -2,		//未初始化
	CAPTURE_HOOK_FAIL = -1,			//失败，放弃本次操作
	CAPTURE_HOOK_SUCCESS,			//成功，传递共享句柄窗口大小，像素格式等信息
	CAPTURE_RESET,					//重置命令，重置数据传递，同样包含本次的窗口大小和句柄信息---需要测试
	CAPTURE_HEARTBEAT,				//心跳包命令，如果三秒未到达，则判断目标程序已经死亡
	CAPTURE_START_RECORD,			//本地配置为手动录制的时候，目标程序只进行程序数据分发，客户端不进行视频录制，这时候向客户端发送开始录制，本地进行判断进行相关操作！！！！！！！！！！-- 
	CAPTURE_STOP_RECORD,			//停止命令
	CAPTURE_PROCESS_EXIT,			//目标程序退出通知，程序进行exit的拦截处理
	TYPE_Test = 100,
	TYPE_Unknown
};

enum ECLIENTSTATUS {
	CLIENT_INVALID = -1,
	CLIENT_HOOK_FALSE,
	CLIENT_HOOK_SUCCESS,
	CLIENT_HOOK_CAPTURE_ING,
	CLIENT_HOOK_CAPTURE_PAUSE,
	CLIENT_HOOK_CAPTURE_STOP
};

enum EVideoServerStatus {
	VIDEO_SERVER_INVALID = -1,	//未初始化状态
	VIDEO_READY,				//系统准备完毕状态
	VIDEO_Client_READY,				//客户端准备完毕状态
	VIDEO_RECORDING,			//系统录制状态
	VIDEO_PAUSE,				//系统暂停
	VIDEO_STOP					//系统停止状态
};


typedef struct _CaptureInfo
{
	ECaptureType type;
	TIMERTICKS mHeartBeat;			//当前心跳包的，类似于时间戳的存在
	BOOL mIsHOOKed;					//是否被HOOK
	HANDLE mSharedHandle;			//共享句柄
	UINT mCaptureId;				//系统唯一ID，
	UINT mOffsetX;						//x offset
	UINT mOffsetY;						//y offset
	UINT mWidth;					//纹理宽度
	UINT mHeight;					//纹理高度
	EPIXFORMAT mFormat;					//纹理像素格式,系统需要根据类型进行一次处理
	EHUDLocation mHUDLocation;		//HUD 显示的位置
	DWORD processId;				//进程ID
	char pData[256];
	_CaptureInfo()
	{
		type = CAPTURE_HOOK_INVALID;
		mIsHOOKed = FALSE;
		mSharedHandle = NULL;
		mCaptureId = 0;
		mOffsetX = 0;
		mOffsetY = 0;
		mWidth = 800;
		mHeight = 600;
		mFormat = EPIXFORMAT_B8G8R8A8;
		mHUDLocation = EHUDLocation_TOP_LEFT;
		processId = GetCurrentProcessId();
		memset(pData, 0, 256);
	}
}CaptureInfo;



/************************************************************************/
/*
拦截回调函数，实际实现类需要进行继承或者单独的功能类需要继承该函数作为回调实现
*/
/************************************************************************/
class CaptureCallback {
public:
	virtual void* _callback(void* _data) = 0;
};

/************************************************************************/
/* 
基础回调接口类，通过该类可以实现
*/
/************************************************************************/
class ICapture
{
public:
	virtual bool init(CaptureInfo& info) = 0;

	virtual void clear() = 0;

	virtual bool lock(CaptureCallback* _capture_callback = nullptr) = 0;

	virtual void unlock() = 0;

	virtual void save_tex() = 0;

	virtual void onHeartStick(CaptureInfo* info) {};
}; 

struct LOCALSETTING
{
	UINT frameRate = 40;
};



//创建win窗口句柄
HWND CreateMyWnd();

EPIXFORMAT convert_format_dx9(DWORD format);
DWORD convert_dx9format_sys(EPIXFORMAT format);


EPIXFORMAT convert_format_dxgi(DWORD format);
DWORD convert_dxgiformat_sys(EPIXFORMAT format);

#endif /*_ICAPTURE_icapture_H_*/
