#include "CaptureFromShareTexDx9.h"
#include <atlstr.h>

CaptureFromShareTexDx9::CaptureFromShareTexDx9()
{

}
CaptureFromShareTexDx9::~CaptureFromShareTexDx9()
{

}


bool CaptureFromShareTexDx9::init(CaptureInfo& info)
{
	//判断句柄等信息;
	if (info.mWidth < 0 || info.mWidth >MAX_SHORT
		|| info.mHeight<0 || info.mHeight > MAX_SHORT
		|| NULL == info.mSharedHandle)
	{
		return false;
	}


	HRESULT err;
	if (!m_pD3D9Device)
	{
		HWND hWnd = CreateMyWnd();
		if (FAILED(err = DX9Init(info, hWnd)))
		{
			return false;
		}

	}
		
	if (m_pD3D9Device)
	{ 
	 
		//测试 
		//m_d3dParam.mWidth = 640;
		//m_d3dParam.mHeight = 400;
		//m_d3dParam.mFormat = D3DFMT_A8R8G8B8;
		//m_d3dParam.mSharedHandle = NULL;

		HRESULT hr = m_pD3D9Device->CreateTexture(info.mWidth, info.mHeight, 1, D3DUSAGE_RENDERTARGET, (D3DFORMAT)convert_dx9format_sys(info.mFormat), D3DPOOL_DEFAULT, &m_pD3D9ShareTex, &info.mSharedHandle);

		if (SUCCEEDED(hr))
		{
			m_pD3D9ShareTex->GetSurfaceLevel(0, &m_pSharedSurface9);

			m_pD3D9ShareTex->Release();
			//		OutputDebugStringA("INFO:Leave CreateShareTexture,success.\n");
			
			D3DSURFACE_DESC desc;
			m_pSharedSurface9->GetDesc(&desc);
			hr = m_pD3D9Device->CreateOffscreenPlainSurface(desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM,
				&m_pD3DOffScreenSurface9, NULL);

			if (SUCCEEDED(hr))
			{

				return TRUE;
			}
		}
	}

	return false;
}

void CaptureFromShareTexDx9::clear()
{

}

IDirect3DSurface9* CaptureFromShareTexDx9::getRenderData()
{
	if (!m_pSharedSurface9 || !m_pD3DOffScreenSurface9)
		return NULL;

	//D3DXSaveTextureToFile(TEXT("D:\\zhang.jpg"), D3DXIFF_JPG, m_pD3D9ShareTex, NULL);

	HRESULT hr;
	if (FAILED(hr = m_pD3D9Device->GetRenderTargetData(m_pSharedSurface9, m_pD3DOffScreenSurface9)))
	{
		return NULL;
	}
	else
	{
		return m_pD3DOffScreenSurface9;
	}

	return NULL;
}

bool CaptureFromShareTexDx9::lock(CaptureCallback* _capture_callback)
{
	//显卡内处理方式
	//m_pD3D9Device->StretchRect(m_pSharedSurface9, 0, m_pD3DOffScreenSurface9, 0, D3DTEXF_NONE);
	//显卡到内存处理方式
	if (!m_pSharedSurface9 || !m_pD3DOffScreenSurface9)
		return false;

	//D3DXSaveTextureToFile(TEXT("D:\\zhang.jpg"), D3DXIFF_JPG, m_pD3D9ShareTex, NULL);

	HRESULT hr;
	if (FAILED(hr = m_pD3D9Device->GetRenderTargetData(m_pSharedSurface9, m_pD3DOffScreenSurface9)))
	{
		return false;
	}

	if (_capture_callback)
		_capture_callback->_callback(m_pD3DOffScreenSurface9);


	return true;
}

void CaptureFromShareTexDx9::unlock()
{
}


void* CaptureFromShareTexDx9::_callback(void* _data)
{
	return nullptr;
}


HRESULT CaptureFromShareTexDx9::DX9Init(CaptureInfo& info,HWND hWnd)
{
	
	//创建窗口
	if (!hWnd)
	{
		hWnd = CreateMyWnd();
	}

	SafeReleaseObject(m_pD3DEx);
	SafeReleaseObject(m_pD3D9Device);


	HRESULT hr = S_OK;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DADAPTER_IDENTIFIER9 adapterId;
	unsigned int iAdapter = NULL;
	OutputDebugStringA("INFO:Entry InitD3D9...\n");
 
	hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &m_pD3DEx);

	if (FAILED(hr))
	{
		OutputDebugStringA("ERROR:InitD3D9 CreateDevice failed.\n");
		return S_FALSE;
	}

	//if (deviceID >= m_pD3DEx->GetAdapterCount())
	//{
	//	OutputDebugStringA("ERROR:InitD3D9 not a valid GPU device1111.\n");
	//	return FALSE;
	//}
	int deviceID = 0;
	hr = m_pD3DEx->GetAdapterIdentifier(deviceID, 0, &adapterId);
	if (hr != S_OK)
	{
		OutputDebugStringA("ERROR:InitD3D9 not a valid GPU device2222.\n");
		SafeReleaseObject(m_pD3DEx);
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


	return S_OK;
}

void CaptureFromShareTexDx9::save_tex()
{
	//if (m_pD3D9ShareTex)
	//{
	//	HRESULT hr = D3DXSaveTextureToFile(TEXT("D:\\zhang1.jpg"), D3DXIFF_JPG, m_pD3D9ShareTex, NULL);
	//	if (S_OK != hr)
	//	{
	//		OutputDebugStringA("save images  error!"); 
	//	}
	//}
}
void CaptureFromShareTexDx9::onHeartStick(CaptureInfo* info)
{
	if (!info)
		return;
	if (!m_pD3DOffScreenSurface9)
	{
		init(*info);
	}
}