
#include "ICapture.h"
#include <d3d9.h>
#include <d3d11.h>



LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND CreateMyWnd()
{
	DWORD ecode = 0;
	HMODULE curMoudle = ::GetModuleHandle(NULL);
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_OWNDC, MsgProc, 0L, 0L,
		curMoudle, NULL, NULL, NULL, NULL,
		TEXT("ZSGameCapture"), NULL
	};
	if (!RegisterClassEx(&wc))
	{
		ecode = GetLastError();
		return NULL;
	}

	return CreateWindowEx(0, TEXT("ZSGameCapture"), TEXT("TestDemo"),
		WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, 1, 1, NULL, NULL, wc.hInstance, NULL);
}



EPIXFORMAT convert_format_dx9(DWORD format)
{
	switch ((_D3DFORMAT)format)
	{
	case D3DFMT_A8:					return EPIXFORMAT_A8;
	case D3DFMT_A8R8G8B8:			return EPIXFORMAT_A8R8G8B8;
	case D3DFMT_A8B8G8R8:			return EPIXFORMAT_A8B8G8R8;
	case D3DFMT_R8G8B8:				return EPIXFORMAT_R8G8B8;
	case D3DFMT_X8R8G8B8:			return EPIXFORMAT_X8R8G8B8;
	case D3DFMT_X8B8G8R8:			return EPIXFORMAT_X8B8G8R8;
	case D3DFMT_R32F:				return EPIXFORMAT_R32F;
	case D3DFMT_A32B32G32R32F:		return EPIXFORMAT_A32B32G32R32F;

	default:
		return EPIXFORMAT_B8G8R8A8;
	}
}

DWORD convert_dx9format_sys(EPIXFORMAT format)
{
	switch (format)
	{
	case EPIXFORMAT_A8:				return		(DWORD)D3DFMT_A8;
	case EPIXFORMAT_A8R8G8B8:		return		(DWORD)D3DFMT_A8R8G8B8;
	case EPIXFORMAT_A8B8G8R8:		return		(DWORD)D3DFMT_A8B8G8R8;
	case EPIXFORMAT_X8R8G8B8:		return		(DWORD)D3DFMT_X8R8G8B8;
	case EPIXFORMAT_X8B8G8R8:		return		(DWORD)D3DFMT_X8B8G8R8;
	case EPIXFORMAT_R8G8B8:			return		(DWORD)D3DFMT_R8G8B8;
	case EPIXFORMAT_R32F:			return		(DWORD)D3DFMT_R32F;
	case EPIXFORMAT_A32B32G32R32F:	return		(DWORD)D3DFMT_A32B32G32R32F;

	default:
		return (DWORD)D3DFMT_X8B8G8R8;
	}
}

EPIXFORMAT convert_format_dxgi(DWORD format)
{
	switch ((DXGI_FORMAT)format)
	{
	case	DXGI_FORMAT_R8G8B8A8_UNORM:
	case	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case	DXGI_FORMAT_R8G8B8A8_UINT:
		return EPIXFORMAT_R8G8B8A8;


	case	DXGI_FORMAT_B8G8R8A8_UNORM:
	case	DXGI_FORMAT_B8G8R8X8_UNORM:
	case	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case	DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case	DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
		return EPIXFORMAT_B8G8R8A8;

	case	DXGI_FORMAT_R8_TYPELESS:
	case	DXGI_FORMAT_R8_UNORM:
	case	DXGI_FORMAT_R8_UINT:
	case	DXGI_FORMAT_R8_SNORM:
	case	DXGI_FORMAT_R8_SINT:
		return EPIXFORMAT_R8;
	case	DXGI_FORMAT_A8_UNORM:
		return EPIXFORMAT_A8;

	case DXGI_FORMAT_R32_FLOAT:				return EPIXFORMAT_R32F;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:		return EPIXFORMAT_A32B32G32R32F;

	default:
		return EPIXFORMAT_R8G8B8A8;
	}
}
DWORD convert_dxgiformat_sys(EPIXFORMAT format)
{
	switch (format)
	{

	case EPIXFORMAT_R8G8B8A8:
		return DXGI_FORMAT_R8G8B8A8_UNORM;

	case EPIXFORMAT_B8G8R8A8:
		return DXGI_FORMAT_B8G8R8A8_UNORM;

	case EPIXFORMAT_R8:
		return DXGI_FORMAT_R8_UNORM;

	case EPIXFORMAT_A8:
		DXGI_FORMAT_A8_UNORM;


	case EPIXFORMAT_R32F:
		return DXGI_FORMAT_R32_FLOAT;

	case EPIXFORMAT_A32B32G32R32F:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;

	default:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	}
}

//void _stdcall HookExitProcessFun(UINT uExitCode)
//{
//	//	add_log("INFO:ExitProcessHook execute!");
//	g_HookManager.UninstallHook();
//	if (g_OrgExitProcessPtr)
//	{
//		g_OrgExitProcessPtr(uExitCode);
//	}
//}
//
//void HookExitProcessFunProc()
//{
//	//	add_log("INFO:Hook ExitProcess!");
//	DetourTransactionBegin();
//	DetourUpdateThread(GetCurrentThread());
//	DetourAttach(&(PVOID&)g_OrgExitProcessPtr, HookExitProcessFun);
//
//	DetourTransactionCommit();
//}
