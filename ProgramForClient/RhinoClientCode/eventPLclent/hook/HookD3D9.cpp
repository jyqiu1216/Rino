
#include "HookD3D9.h"
#include "stdio.h"
#include "stdlib.h"

//全局句柄
HMODULE g_hModule = NULL;
//子线程通知事件
//HANDLE g_subThreadEvent = NULL;
HWND g_hWndTmp = NULL;
#define CLIENT_TIME_ID 1024

typedef IDirect3D9* (WINAPI*D3D9CREATEPROC)(UINT);
typedef HRESULT(WINAPI*D3D9CREATEEXPROC)(UINT, IDirect3D9Ex**);


IDirect3D9Ex *d3d9ex;
IDirect3DDevice9Ex *deviceEx;//D3DDEVTYPE_HAL -- HAL causes tabbing issues, NULLREF seems to fix the issue




void __cdecl LogOutput(LPCSTR fmt, ...)
{

	if (!fmt)
	{
		return;
	}

	static char logbuf[1024] = { 0 };

	va_list va_alist;

	va_start(va_alist, fmt);

	// #if defined(UNICODE) || defined(_UNICODE)
	// 		_vsnwprintf_s(logbuf, 512,fmt, va_alist);
	// #else
	_vsnprintf_s(logbuf, 1024, fmt, va_alist);
	//#endif
	va_end(va_alist);
	OutputDebugStringA(logbuf);

}



PresentFunPtr CHookD3D9::ms_pD3D9OldPresentFun = NULL;
ResetFunPtr CHookD3D9::ms_pD3D9OldResetFun = NULL;
ExitProcessFunPtr CHookD3D9::ms_exitProcessFun = ExitProcess;
CHookD3D9* CHookD3D9::ms_pHookD3D9 = new CHookD3D9();
CHookD3D9::CHookD3D9()
{
	m_eventId_HookEnd = CreateEvent(NULL, FALSE, FALSE, NULL);

	//该数据可能会被重置后需要从新初始化
	m_pSharedSurface9 = NULL;
	m_pD3D9ShareTex = NULL;


	m_pD3D9Surface9Src = NULL;
	m_pD3DEx = NULL;
	m_pD3D9Device = NULL;

	//if (m_client_signal.initEventSignal(AM_SHARE_MEM_HS_SERVER, AM_SHARE_MEM_HS_CLIENT, EVENT_EXIT_DEFAULT_NAME, this, false))
	//{
	//	m_client_signal.StartThread();
	//	OutputDebugStringA("initEventSignal ok!");
	//	m_captureInit = true;
	//}
	//else
	//{
	//	OutputDebugStringA("initEventSignal false!");
	//}
}


CHookD3D9::~CHookD3D9()
{
	//释放资源


	Clear();
}
void CHookD3D9::Clear()
{
	OutputDebugStringA("CHookD3D9::Clear()\n");
	SafeReleaseObject(m_pSharedSurface9);
	SafeReleaseObject(m_pD3D9ShareTex);

	SafeReleaseObject(m_pD3DOffScreenSurface9);
	SafeReleaseObject(m_pD3D9Surface9Src);
	SafeReleaseObject(pD3DTex9);

	SafeReleaseObject(m_pD3D9Device);
	SafeReleaseObject(m_pD3DEx);

	SafeReleaseObject(d3d9ex);
	SafeReleaseObject(deviceEx);

	//释放其他资
//	SafeReleaseObj(CHookD3D9::ms_pHookD3D9->m_pD3D9ShareTex);
	//SafeReleaseHandle(m_sharedHandle);
	//句柄由后台由后关闭的对象进行关闭
	//	m_sharedHandle = NULL;
}


/************************************************************************/
/*
判断 D3D9Ex扩展信息，该地址创建句柄共享
*/
/************************************************************************/
bool InitD3D9Capture(HWND hWnd)
{
	OutputDebugStringA("InitD3D9Capture : BEGIN");
	bool bSuccess = false;

	TCHAR lpD3D9Path[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, lpD3D9Path);
	wcscat_s(lpD3D9Path, MAX_PATH, TEXT("\\d3d9.dll"));

	HMODULE	hD3D9Dll = GetModuleHandle(lpD3D9Path);
	if (hD3D9Dll)
	{
		OutputDebugStringA("GetProcAddress : Direct3DCreate9Ex");
		D3D9CREATEEXPROC d3d9CreateEx = (D3D9CREATEEXPROC)GetProcAddress(hD3D9Dll, "Direct3DCreate9Ex");
		if (d3d9CreateEx)
		{
			HRESULT hRes;

			if (SUCCEEDED(hRes = (*d3d9CreateEx)(D3D_SDK_VERSION, &d3d9ex)))
			{
				D3DPRESENT_PARAMETERS d3dpp;
				ZeroMemory(&d3dpp, sizeof(d3dpp));
				// Create the Direct3D9 device and the swap chain. In this example, the swap
				// chain is the same size as the current display mode. The format is RGB-32. 
				d3dpp.Windowed = true;
				d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
				d3dpp.BackBufferWidth = 640;
				d3dpp.BackBufferHeight = 480;
				d3dpp.BackBufferCount = 1;
				d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
				d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
				d3dpp.Flags = D3DPRESENTFLAG_VIDEO;
				DWORD dwBehaviorFlags = D3DCREATE_FPU_PRESERVE | D3DCREATE_MULTITHREADED | D3DCREATE_HARDWARE_VERTEXPROCESSING;

				OutputDebugStringA("d3d9ex->CreateDeviceEx ");
				if (SUCCEEDED(hRes = d3d9ex->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, hWnd, dwBehaviorFlags, &d3dpp, NULL, &deviceEx)))
				{
					bSuccess = true;

					UPARAM *vtable = *(UPARAM**)deviceEx;

					FindD3D9ExOffsets(d3d9ex, deviceEx);

					//deviceEx->Release();
					/*d3d9Reset.Rehook();
					d3d9ResetEx.Rehook();*/
					OutputDebugStringA("d3d9ex->CreateDeviceEx: ");
				}
				else
				{
					OutputDebugStringA("InitD3D9Capture: d3d9ex->CreateDeviceEx failed, result: ");
				}

				//d3d9ex->Release();
			}
			else
			{
				OutputDebugStringA("InitD3D9Capture: Direct3DCreate9Ex failed, result: ");
			}
		}
		else
		{
			OutputDebugStringA("InitD3D9Capture: could not load address of Direct3DCreate9Ex");
		}
	}
	OutputDebugStringA("InitD3D9Capture : END");
	return bSuccess;
}



BOOL CHookD3D9::PrepareRealApiEntry()
{
	OutputDebugStringA("PrepareRealApiEntry : BEGIN");
	BOOL bRet(FALSE);

	HWND hWndTmp = CreateMyWnd();
	g_hWndTmp = hWndTmp;
	if (!hWndTmp)
	{
		OutputDebugStringA("CreateMyWnd   error \n");
		return FALSE;
	}
	//新版本需要进行入下初始化
	//----------------------------------------------------------------
	InitD3D9Capture(hWndTmp);


	//查询DX9 present 和 reset地址，用于系统hook
	LPDIRECT3D9         tmp_pD3D = NULL;
	LPDIRECT3DDEVICE9   tmp_pd3dDevice = NULL;

	if (NULL == (tmp_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		OutputDebugStringA("Direct3DCreate9(D3D_SDK_VERSION)   error \n");
		goto Exit0;
	}

	//
	HRESULT hr;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.hDeviceWindow = (HWND)hWndTmp;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	OutputDebugStringA("CreateDevice");
	hr = tmp_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, hWndTmp,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_NOWINDOWCHANGES, &d3dpp, &tmp_pd3dDevice);

	if (FAILED(hr))
	{
		OutputDebugStringA("CreateDevice: FAILED");
		tmp_pD3D->Release();

		//OutputDebugStringA("CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF  error \n");
		goto Exit0;
	}

	OutputDebugStringA("CreateDevice: SUCCESSFUL");

	if (tmp_pd3dDevice)
	{
		PVOID* tmpAddr = (PVOID*)((*((ULONG*)(tmp_pd3dDevice))) + 17 * sizeof(PVOID));
		ms_pD3D9OldPresentFun = (PresentFunPtr)(*tmpAddr);

		tmpAddr = (PVOID*)((*((ULONG*)(tmp_pd3dDevice))) + 16 * sizeof(PVOID));
		ms_pD3D9OldResetFun = (ResetFunPtr)(*tmpAddr);

		tmp_pd3dDevice->Release();
	}

	tmp_pD3D->Release();

	OutputDebugStringA("CreateDevice: END");
	bRet = TRUE;

Exit0:


	//::PostMessage(hWndTmp, WM_DESTROY, 0, 0);
	//ShowWindow(hWndTmp, SW_HIDE);
	//UpdateWindow(hWndTmp);

	//MSG msg;
	//while (GetMessage(&msg, NULL, 0, 0))

	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}

	return bRet;
	//hHookDispatchMessageW
}

BOOL CHookD3D9::DoHook()
{
	OutputDebugStringA("D3D : CHookD3D9::DoHook() : BEGIN");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)ms_pD3D9OldPresentFun, HookPresent);
	DetourAttach(&(PVOID&)ms_pD3D9OldResetFun, HookReset);
	DetourAttach(&(PVOID&)ms_exitProcessFun, HookExitProcessFun);
	DetourTransactionCommit();

	//创建定时器,默认发送当前的心跳包信息
	//SetTimer(g_hWndTmp, CLIENT_TIME_ID, 1000, (TIMERPROC)CHookD3D9::OnTimer);

	OutputDebugStringA("D3D : CHookD3D9::DoHook() : END Successful.");
	return TRUE;
}


void CHookD3D9::DoneHook()
{
	if (ms_pD3D9OldPresentFun || ms_pD3D9OldResetFun)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		if (ms_pD3D9OldPresentFun)
		{
			DetourDetach(&(PVOID&)ms_pD3D9OldPresentFun, HookPresent);
		}
		if (ms_pD3D9OldResetFun)
		{
			DetourDetach(&(PVOID&)ms_pD3D9OldResetFun, HookReset);
		}
		if (ms_exitProcessFun)
		{
			DetourDetach(&(PVOID&)ms_exitProcessFun, HookExitProcessFun);
		}

		DetourTransactionCommit();

	}
	ms_pHookD3D9->Clear();
}

void CHookD3D9::MainHook(LPDIRECT3DDEVICE9 pDevice)
{
	//OutputDebugStringA("MainHook");

	if (m_pSharedSurface9)
	{
		//OutputDebugStringA("m_pSharedSurface9 NOT NULL");
		IDirect3DSurface9 *backBuffer(NULL);
		if (SUCCEEDED(pDevice->GetRenderTarget(0, &backBuffer)))
		{
			HRESULT res = pDevice->StretchRect(backBuffer, 0, m_pSharedSurface9, 0, D3DTEXF_NONE);
			if (SUCCEEDED(res))
			{
				//OutputDebugStringA("SUCCEEDED pDevice");
			}
			else
			{
				OutputDebugStringA("FAILED pDevice");
			}
			
			//保存DDS文件，DX11打开该纹理，进行相关值数据同步
			//D3DXSaveTextureToFile(TEXT("D:\\zhang.jpg"), D3DXIFF_JPG, m_pD3D9ShareTex, NULL);
			//Sleep(30);

			backBuffer->Release();
		}
	}
	else
	{
		OutputDebugStringA("D3D : CHookD3D9::MainHook() : m_pSharedSurface9 is NULL");

		//if (m_d3dParam.mSharedHandle)
		//{
		//	D3D9Patch d3d9patch;
		//	d3d9patch.Patch(pDevice, (PatchCallback*)this); 
		//	if (CreateShareTexture() == TRUE)
		//	{

		//	}
		//}
		//else
		//{}
		//LogOutput("HookPresent device %d", pDevice);
		D3D9Patch d3d9patch;
		d3d9patch.Patch(pDevice, (PatchCallback*)this);

		if (m_pSharedSurface9)
		{

			// 通知程序端进行接收数据，
			//EventParamHS param;

			//param.data.captInfo.captureId = m_captureInfo.mCaptureId;
			//param.data.captInfo.format = m_captureInfo.mFormat;
			//param.data.captInfo.tick = m_captureInfo.mHeartBeat;
			//param.data.captInfo.height = m_captureInfo.mHeight;
			//param.data.captInfo.HUDLocation = m_captureInfo.mHUDLocation;
			//param.data.captInfo.isHooked = m_captureInfo.mIsHOOKed;
			//param.data.captInfo.offsetX = m_captureInfo.mOffsetX;
			//param.data.captInfo.offsetY = m_captureInfo.mOffsetY;
			//param.data.captInfo.sharedHandle = (UINT)m_captureInfo.mSharedHandle;
			//param.data.captInfo.width = m_captureInfo.mWidth;
			//strcpy_s(param.data.captInfo.data, m_captureInfo.pData);
			//param.data.captInfo.processId = m_captureInfo.processId;

			OutputDebugStringA("D3D : CHookD3D9::MainHook() : m_pSharedSurface9 NOT NULL");
			::SetEvent(m_eventId_HookEnd);
			//m_captureInfo.type = CAPTURE_HOOK_SUCCESS;
			//通知程序端进行接收数据
			//m_client_signal.SendEvent(&m_captureInfo, sizeof(CaptureInfo));
		}

		//测试代码
		// IDirect3DSurface9 *backBuffer(NULL);
		// if (SUCCEEDED(pDevice->GetRenderTarget(0, &backBuffer)))
		// {
		// 	backBuffer->GetDesc(&m_d3dsurface_desc); 
		// 	m_d3dParam.mWidth = m_d3dsurface_desc.Width;
		// 	m_d3dParam.mHeight = m_d3dsurface_desc.Height;
		// 	m_d3dParam.mFormat = (EPIXFORMAT)m_d3dsurface_desc.Format;
		// 	m_d3dParam.mSharedHandle = NULL;
		// 	if (g_subThreadEvent)
		// 		::SetEvent(g_subThreadEvent);
		// }


	}
}

/**
* @date: 2015/10/25
* @author: Steven
* @version: V1.0.0.1
* @return: 0：成功  1：失败
* @usage: HookPresent
* 主要用来保存该函数的指定位置用来还原地址,新的HookPresent的执行地址，参数和标准参数一致
**/
HRESULT _stdcall HookPresent(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{

	if (CHookD3D9::ms_pHookD3D9)
	{
		CHookD3D9::ms_pHookD3D9->MainHook(pDevice);
	}
	//pDevice->Clear(0, NULL, D3DCLEAR_TARGET , D3DCOLOR_RGBA(0,0, 0, 0), 1.0f, 0);
	/************************************************************************/
	/* 清空后台buffer                                                                     */
	/************************************************************************/
	return CHookD3D9::ms_pD3D9OldPresentFun(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	return FALSE;
}



/**
* @date: 2015/10/25
* @author: Steven
* @version: V1.0.0.1
* @return: 0：成功  1：失败
* @usage: HookPresent
* 主要用来保存该函数的指定位置用来还原地址,新的HookReset的执行地址，当设备丢失之后进行重置数据参数和标准参数一致
**/
HRESULT _stdcall HookReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (CHookD3D9::ms_pHookD3D9)
	{
		//释放资源
		SafeReleaseObject(CHookD3D9::ms_pHookD3D9->m_pSharedSurface9);
		SafeReleaseObject(CHookD3D9::ms_pHookD3D9->m_pD3D9ShareTex);
	}
	LogOutput("HookReset device %d", pDevice);

	return CHookD3D9::ms_pD3D9OldResetFun(pDevice, pPresentationParameters);
}


void _stdcall HookExitProcessFun(UINT uExitCode)
{

	CHookD3D9::DoneHook();

	if (CHookD3D9::ms_exitProcessFun)
	{
		CHookD3D9::ms_exitProcessFun(uExitCode);
	}
}

void CHookD3D9::Test()
{

}
void CHookD3D9::CallbackPatch(void* _data)
{
	LPDIRECT3DDEVICE9 pDevice = (LPDIRECT3DDEVICE9)_data;
	//在游戏D3D设备上创建一个共享纹理，用来存储buffer的表面数据
	IDirect3DSurface9 *backBuffer(NULL);
	if (SUCCEEDED(pDevice->GetRenderTarget(0, &backBuffer)))
	{
		backBuffer->GetDesc(&m_d3dsurface_desc);

		m_captureInfo.mWidth = m_d3dsurface_desc.Width;
		m_captureInfo.mHeight = m_d3dsurface_desc.Height;
		m_captureInfo.mFormat = convert_format_dx9(m_d3dsurface_desc.Format);

		if (m_captureInfo.mSharedHandle)
		{
			CloseHandle(m_captureInfo.mSharedHandle);
			m_captureInfo.mSharedHandle = NULL;
		}

		HRESULT hr = pDevice->CreateTexture(m_d3dsurface_desc.Width, m_d3dsurface_desc.Height, 1,
			D3DUSAGE_RENDERTARGET, m_d3dsurface_desc.Format, D3DPOOL_DEFAULT, &m_pD3D9ShareTex, &m_captureInfo.mSharedHandle);
		if (FAILED(hr))
		{
			backBuffer->Release();
			return;
		}
		//获得0层次的表面信息 
		m_pD3D9ShareTex->GetSurfaceLevel(0, &m_pSharedSurface9);

		//m_pD3D9ShareTex->Release();
		//m_pD3D9ShareTex = NULL;

		backBuffer->Release();

		//else
		OutputDebugStringA("	CreateShareTexture success --------------\n");
		//创建共享纹理，用于其他线程
		//if (CreateShareTexture() == TRUE)
		//{
		//	//通知识别线程--
		//	/*if (g_subThreadEvent)
		//	::SetEvent(g_subThreadEvent);*/

		//	//通知客户端进行抓取数据


		//}


	}

}

BOOL CHookD3D9::InitD3D9(unsigned int deviceID)
{
	HRESULT hr = S_OK;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DADAPTER_IDENTIFIER9 adapterId;
	unsigned int iAdapter = NULL;
	OutputDebugStringA("INFO:Entry InitD3D9...\n");
	hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &m_pD3DEx);

	if (FAILED(hr))
	{
		OutputDebugStringA("ERROR:InitD3D9 CreateDevice failed.\n");
	}

	if (deviceID >= m_pD3DEx->GetAdapterCount())
	{
		OutputDebugStringA("ERROR:InitD3D9 not a valid GPU device1111.\n");
		return FALSE;
	}

	hr = m_pD3DEx->GetAdapterIdentifier(deviceID, 0, &adapterId);
	if (hr != S_OK)
	{
		OutputDebugStringA("ERROR:InitD3D9 not a valid GPU device2222.\n");
		return FALSE;
	}

	// Create the Direct3D9 device and the swap chain. In this example, the swap
	// chain is the same size as the current display mode. The format is RGB-32.
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = 640;
	d3dpp.BackBufferHeight = 480;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags = D3DPRESENTFLAG_VIDEO;
	DWORD dwBehaviorFlags = D3DCREATE_FPU_PRESERVE | D3DCREATE_MULTITHREADED | D3DCREATE_HARDWARE_VERTEXPROCESSING;
	hr = m_pD3DEx->CreateDevice(deviceID,
		D3DDEVTYPE_HAL,
		NULL,
		dwBehaviorFlags,
		&d3dpp,
		&m_pD3D9Device);
	if (FAILED(hr))
	{
		OutputDebugStringA("ERROR:InitD3D9 CreateDevice failed.\n");
		return FALSE;
	}

	OutputDebugStringA("INFO:InitD3D9 success.\n");


	return TRUE;
}

BOOL CHookD3D9::CreateShareTexture()
{
	//	OutputDebugStringA("INFO:Entry CreateShareTexture.\n");

	SafeReleaseObject(m_pD3DOffScreenSurface9);
	SafeReleaseObject(m_pD3D9Surface9Src);
	SafeReleaseObject(pD3DTex9);

	//打开共享纹理 
	if (!m_pD3D9Device)
		return FALSE;

	//测试 
	//m_d3dParam.mWidth = 640;
	//m_d3dParam.mHeight = 400;
	//m_d3dParam.mFormat = D3DFMT_A8R8G8B8;
	//m_d3dParam.mSharedHandle = NULL;

	HRESULT hr = m_pD3D9Device->CreateTexture(m_captureInfo.mWidth, m_captureInfo.mHeight, 1, D3DUSAGE_RENDERTARGET, (D3DFORMAT)convert_dx9format_sys(m_captureInfo.mFormat), D3DPOOL_DEFAULT, &pD3DTex9, &m_captureInfo.mSharedHandle);

	if (SUCCEEDED(hr))
	{
		pD3DTex9->GetSurfaceLevel(0, &m_pD3D9Surface9Src);
		//pD3DTex9->Release(); 
		//		OutputDebugStringA("INFO:Leave CreateShareTexture,success.\n");


		pD3DTex9->Release();


		D3DSURFACE_DESC desc;
		m_pD3D9Surface9Src->GetDesc(&desc);
		hr = m_pD3D9Device->CreateOffscreenPlainSurface(desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM,
			&m_pD3DOffScreenSurface9, NULL);

		if (SUCCEEDED(hr))
		{
			return TRUE;
		}
	}
	//	OutputDebugStringA("INFO:Leave CreateShareTexture,failed.\n");
	return FALSE;
}
/**
* @date: 2016/8/2
* @author: Steven
* @brief:   DoData  回调处理的接口，共享内存中读取数据进行处理和回调的接口函数
* @version: V1.0.0.1
* @param:
* @return:  bool
**/
void CHookD3D9::DoData(void* pData, size_t size)
{

}
/**
* @date: 2016/8/2
* @author: Steven
* @brief:   SendData   默认执行函数的函数约定
* @version: V1.0.0.1
* @param:
* @return:  bool
**/
#include "common/timemanager.h"
void CHookD3D9::SendData(void* pData, size_t maxSize)
{
	//写入心跳和相关处理，包括事件类型
	m_captureInfo.mHeartBeat = CTimeManager::getInstance()->getCurrentTime();
	m_captureInfo.type = CAPTURE_HEARTBEAT;
	memcpy(pData, &m_captureInfo, sizeof(CaptureInfo));
}


char bufInfo[1024] = { 0 };

void CHookD3D9::OnTimer(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	switch (nTimerid)
	{
	case CLIENT_TIME_ID:
	{
		//ms_pHookD3D9->m_captureInfo.type = CAPTURE_HEARTBEAT;
		//ms_pHookD3D9->m_captureInfo.mHeartBeat = CTimeManager::getInstance()->getCurrentTime();
		////通知程序端进行接收数据
		//ms_pHookD3D9->m_client_signal.SendEvent(&ms_pHookD3D9->m_captureInfo, sizeof(CaptureInfo));
		//OutputDebugStringA("ontimer client-------------------------------");


						   //ms_pHookD3D9->m_captureInfo;

						   //EventParamHS param;

						   //param.data.captInfo.captureId = ms_pHookD3D9->m_captureInfo.mCaptureId;
						   //param.data.captInfo.format = ms_pHookD3D9->m_captureInfo.mFormat;
						   //param.data.captInfo.tick = ms_pHookD3D9->m_captureInfo.mHeartBeat;
						   //param.data.captInfo.height = ms_pHookD3D9->m_captureInfo.mHeight;
						   //param.data.captInfo.HUDLocation = ms_pHookD3D9->m_captureInfo.mHUDLocation;
						   //param.data.captInfo.isHooked = ms_pHookD3D9->m_captureInfo.mIsHOOKed;
						   //param.data.captInfo.offsetX = ms_pHookD3D9->m_captureInfo.mOffsetX;
						   //param.data.captInfo.offsetY = ms_pHookD3D9->m_captureInfo.mOffsetY;
						   //param.data.captInfo.sharedHandle = (UINT)ms_pHookD3D9->m_captureInfo.mSharedHandle;
						   //param.data.captInfo.width = ms_pHookD3D9->m_captureInfo.mWidth;
						   //strcpy_s(param.data.captInfo.data, ms_pHookD3D9->m_captureInfo.pData);
						   //param.data.captInfo.processId = ms_pHookD3D9->m_captureInfo.processId;

						   //param.id = E_EVENT_ID_HS_HOOK_HEARTBEAT;
						   //ms_pHookD3D9->m_client_signal.SendEvent(&param, sizeof(EventParamHS));

						   //sprintf_s(bufInfo, "D3D: 心跳包: W:%d, H:%d, HANDLE=%d",
							  // param.data.captInfo.width,
							  // param.data.captInfo.height,
							  // param.data.captInfo.sharedHandle);
						   //OutputDebugStringA(bufInfo);


	}
	break;
	}
}