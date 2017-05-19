#ifndef _HOOKHS_H_
#define _HOOKHS_H_

#include "hook/HookD3D9.h"
#include "hook/HookEventParam.h"

//#define WH_SHARE_MEMORY_HS		TEXT("WH_SHARE_MEMORY_HS")

#define AM_SHARE_MEM_HS_SERVER		TEXT("AM_SHARE_MEM_HS_SERVER")	
#define AM_SHARE_MEM_HS_CLIENT		TEXT("AM_SHARE_MEM_HS_CLIENT")	


class HookHS : public IDataRecive, public EventSignal
{
public:
	HookHS() : m_isHooked(false) {}

public:
	bool initialize()
	{
		this->initEventSignal(AM_SHARE_MEM_HS_SERVER, AM_SHARE_MEM_HS_CLIENT, EVENT_EXIT_DEFAULT_NAME, this, false);
		this->StartThread();
		OutputDebugStringA("ExtendHS [HookHS::initialize]: init EventSignal successful.");

		if (!CHookD3D9::PrepareRealApiEntry())
		{
			OutputDebugStringA("ExtendHS [HookHS::initialize]: PrepareRealApiEntry error.");
			return false;
		}

		if (!CHookD3D9::ms_pHookD3D9->InitD3D9())
		{
			OutputDebugStringA("ExtendHS [HookHS::initialize]: InitD3D9 error.");
			return false;
		}

		return CHookD3D9::DoHook();
	}


	void update()
	{
		static double tick = GetTickCount();

		CaptureInfo& capInfo = CHookD3D9::getInstance()->m_captureInfo;
		// 通知程序端进行接收数据，
		EventParamHS param;
		
		param.data.captInfo.captureId = capInfo.mCaptureId;
		param.data.captInfo.format = capInfo.mFormat;
		param.data.captInfo.tick = capInfo.mHeartBeat;
		param.data.captInfo.height = capInfo.mHeight;
		param.data.captInfo.HUDLocation = capInfo.mHUDLocation;
		param.data.captInfo.isHooked = capInfo.mIsHOOKed;
		param.data.captInfo.offsetX = capInfo.mOffsetX;
		param.data.captInfo.offsetY = capInfo.mOffsetY;
		param.data.captInfo.sharedHandle = capInfo.mSharedHandle;
		param.data.captInfo.width = capInfo.mWidth;
		strcpy_s(param.data.captInfo.data, capInfo.pData);
		param.data.captInfo.processId = capInfo.processId;


		/* 等待HOOK成功结束的系统事件 */
		if (0 == ::WaitForSingleObject(CHookD3D9::getInstance()->m_eventId_HookEnd, 10))
		{
			char buf[128] = { 0 };
			sprintf_s(buf, "ExtendHS [HookHS::update()]: HOOK SUCCESSFUL:  CaptureInfo: W:%d, H:%d, HANDLE:%d", capInfo.mWidth, capInfo.mHeight, capInfo.mSharedHandle);
			OutputDebugStringA(buf);

			param.id = E_EVENT_ID_HS_HOOK_SUCCESS;
			this->SendEvent(&param, sizeof(EventParamHS));
			OutputDebugStringA("ExtendHS : SendEvent E_EVENT_ID_HS_HOOK_SUCCESS");

			/* 保存HOOK状态 */
			m_isHooked = true;
		}

		double curTick = GetTickCount();
		//char buf[128] = { 0 };
		//sprintf_s(buf, "=============: T:%lf, CUR:%lf, ==>%lf", tick, curTick, curTick-tick);
		//OutputDebugStringA(buf);


		if (curTick-tick > 1000)
		{
			param.id = E_EVENT_ID_HS_HOOK_HEARTBEAT;
			this->SendEvent(&param, sizeof(EventParamHS));

			char buf[128] = { 0 };
			sprintf_s(buf, "ExtendHS [HookHS::update()]: HOOK SUCCESSFUL:  CaptureInfo: W:%d, H:%d, HANDLE:%d", capInfo.mWidth, capInfo.mHeight, capInfo.mSharedHandle);
			OutputDebugStringA(buf);
			tick = curTick;
		}
	}


	virtual void DoData(void* pData, size_t size)
	{}


	virtual void SendData(void* pData, size_t maxSize)
	{}


private:
	bool m_isHooked;
};


#endif //_HOOKHS_H_
