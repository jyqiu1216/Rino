/**
* D3D9 hookde的管理类
* 版权所有 (C) 2014-2015南京正溯网络科技有限公司
* 创建日期 2015/10/25
* 修订记录 2015/10/25 新创建，添加对于D3D9的指定API绑定，本文绑定present 和 resset函数.
* 修订记录 2015/10/25 。
* @author：Steven Zhang
* @version V1.0.0.2
* @usage
*/


#ifndef _HOOK_D3D9_H_
#define _HOOK_D3D9_H_

 
#include "ntstatus.h"
 
#include "EventSignal.h"
#include "macrodefinition.h"
//#pragma comment(lib,"EasyHook32.lib")
#include "ntstatus.h"
#include <d3d9.h>

#include "DirectX_Jun2010/d3dx9tex.h"
#include "macrodefinition.h"
// Windows 头文件: 
#include <windows.h>
#include <Objbase.h>

#include <process.h>

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <Shlobj.h>
#pragma comment(lib, "Shell32.lib")

//#define Z_CAPTURE_LIB_


#include <tchar.h>

#include "detver.h"
#include "detours.h"
#pragma comment(lib, "detours.lib")

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib, "dxguid.lib")

#include "Dx9GPUPatch.h"

#include "ICapture.h"

#define WINDOW_MSG_USER_HANDLE  WM_APP + 1024




union MyUnion
{
	struct{
		short mWidth;
		short mHeight;
	}mSParam;
	int mParam;
};

typedef VOID(_stdcall *ExitProcessFunPtr)(_In_ UINT uExitCode);

typedef HRESULT(_stdcall *PresentFunPtr)(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);

typedef HRESULT(_stdcall *ResetFunPtr)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

/**
* @date: 2015/10/25
* @author: Steven
* @version: V1.0.0.1
* @return: 0：成功  1：失败
* @usage: HookPresent
* 主要用来保存该函数的指定位置用来还原地址,新的HookPresent的执行地址，参数和标准参数一致
**/
HRESULT _stdcall HookPresent(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);


/**
* @date: 2015/10/25
* @author: Steven
* @version: V1.0.0.1
* @return: 0：成功  1：失败
* @usage: HookPresent
* 主要用来保存该函数的指定位置用来还原地址,新的HookReset的执行地址，当设备丢失之后进行重置数据参数和标准参数一致
**/
HRESULT _stdcall HookReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

void _stdcall HookExitProcessFun(UINT uExitCode);

class CHookD3D9 :public PatchCallback,public IDataRecive
{
protected:
	CHookD3D9();
	~CHookD3D9();
	public:
		static CHookD3D9* getInstance() { return ms_pHookD3D9; }
public:
	/**
	* @date: 2015/10/25
	* @author: Steven
	* @version: V1.0.0.1
	* @return: 0：成功  1：失败
	* @usage: 主要用来保存该函数的指定位置用来还原地址
	**/

	static BOOL PrepareRealApiEntry();

 
	/**
	* @date: 2015/10/25
	* @author: Steven
	* @version: V1.0.0.1
	* @return: 0：成功  1：失败
	* @usage: HookPresent
	* 主要用来保存该函数的指定位置用来还原地址,新的HookReset的执行地址，当设备丢失之后进行重置数据参数和标准参数一致
	**/
	static BOOL DoHook();


	/**
	* @date: 2015/10/25
	* @author: Steven
	* @version: V1.0.0.1
	* @return:  void
	* @usage: HookPresent
	*	//dll 退出之后，调用该函数进行资源释放
	**/

	static void DoneHook();

	static void OnTimer(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime);

	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   DoData  回调处理的接口，共享内存中读取数据进行处理和回调的接口函数
	* @version: V1.0.0.1
	* @param:
	* @return:  bool
	**/
	virtual void DoData(void* pData, size_t size);
	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   SendData   默认执行函数的函数约定
	* @version: V1.0.0.1
	* @param:
	* @return:  bool
	**/
	virtual void SendData(void* pData, size_t maxSize);

	

	BOOL InitD3D9(unsigned int deviceID = 0);


	BOOL CreateShareTexture();

	void Clear(); 


	CaptureInfo& getD3D9Param(){ return m_captureInfo; } 

	IDirect3DDevice9* getD3D9Device(){ return m_pD3D9Device; }
	IDirect3DSurface9* getD3D9Surface9(){ return m_pD3D9Surface9Src; }
	IDirect3DTexture9* getD3D9Tex9(){ return pD3DTex9; }
	
	IDirect3DSurface9* getD3D9OffSurface9(){ return m_pD3DOffScreenSurface9; }



public:

	//存储原先的地址
	static PresentFunPtr ms_pD3D9OldPresentFun;
	static ResetFunPtr ms_pD3D9OldResetFun;
	static ExitProcessFunPtr ms_exitProcessFun;

	static CHookD3D9*  ms_pHookD3D9;


protected:
	 
	//当前地址
	//static NTSTATUS				ms_statue;
	//
	static ULONG                   ms_HookPresent_ACLEntries[1];
	static ULONG                   ms_HookReset_ACLEntries[1];

	//主程序过程，如果资源不存在，则创建，否则进行数据拷贝
	void MainHook(LPDIRECT3DDEVICE9 pDevice);
	//测试函数
	void Test();


	virtual void CallbackPatch(void* _data);

public:
	HANDLE m_eventId_HookEnd;


	//用来接受backsurface的surface，
	//该数据可能会被重置后需要从新初始化
	IDirect3DSurface9 *		m_pSharedSurface9;
	IDirect3DTexture9*		m_pD3D9ShareTex;

	D3DSURFACE_DESC			m_d3dsurface_desc; 

	IDirect3DSurface9*	m_pD3D9Surface9Src = NULL;
	IDirect3DSurface9*	m_pD3DOffScreenSurface9 = NULL;
	IDirect3DTexture9*  pD3DTex9 = NULL;

	IDirect3D9Ex           *m_pD3DEx;
	IDirect3DDevice9       *m_pD3D9Device;

	//系统通知事件,客户端
	//EventSignal m_client_signal;

	CaptureInfo m_captureInfo;


	ECLIENTSTATUS m_clientStatus;

	bool m_captureInit = false;

	friend HRESULT _stdcall HookPresent(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
	friend HRESULT _stdcall HookReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
};

#endif /*_HOOK_D3D9_H_*/