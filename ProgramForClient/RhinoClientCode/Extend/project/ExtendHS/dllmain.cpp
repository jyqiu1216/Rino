// dllmain.cpp : 定义 DLL 应用程序的入口点。

#include <SDKDDKVer.h>
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <process.h>

#include "HookHS.h"


WNDPROC g_defaultWndProcHS = NULL;
bool g_isUserLock = false;
double g_tick = 0;

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!g_defaultWndProcHS)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	
	//::ShowCursor(false);
	//RECT rt;
	//::GetWindowRect(hWnd, &rt);
	//::SetCursorPos(rt.left, rt.top);

	switch (message)
	{
	case WM_USER + 260:
		g_isUserLock = true;
		g_tick = GetTickCount();
		//MessageBoxA(NULL, "==================================================", "", NULL);
		break;

	case WM_USER + 261:
		g_isUserLock = false;
		break;

	//case 32:
	case 132:
	//case 127:
	//case 255:
	//case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
		if (g_isUserLock)
		{
			//if (GetTickCount()-g_tick >= 10000)
			//{
			//	g_isUserLock = false;
			//}
			return 0;
		}
		break;

	default:
		break;
	}

	if (g_isUserLock)
	{
		if (GetTickCount() - g_tick >= 8000)
		{
			g_isUserLock = false;
			//MessageBoxA(NULL, "-----------------------------------------------", "", NULL);
		}
	}

	//if (message!=32 && message!=132)
	{
		//char buf[128] = { 0 };
		//DWORD a = HIWORD(lParam);
		//DWORD b = LOWORD(lParam);
		//sprintf_s(buf, "MESSAGE=%d, wParam=%d, lParam=%d, (%d,%d), (%d,%d)", message, wParam, lParam, a, b, HIWORD(a), LOWORD(b));
		//OutputDebugStringA(buf);
	}
	

	return g_defaultWndProcHS(hWnd, message, wParam, lParam);
}

void HookThreadProc(void* _p)
{
	HWND hWnd = ::FindWindow(NULL, TEXT("炉石传说"));
	if (hWnd)
	{
		g_defaultWndProcHS = (WNDPROC)::GetWindowLong(hWnd, GWL_WNDPROC);
		::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)MyWndProc);
	}
	
	HookHS hookHS;
	hookHS.initialize();
	do
	{
		hookHS.update();
		Sleep(10);
	} while (true);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		HANDLE hThread = (HANDLE)_beginthread(HookThreadProc, 0, NULL);
	}
	break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

