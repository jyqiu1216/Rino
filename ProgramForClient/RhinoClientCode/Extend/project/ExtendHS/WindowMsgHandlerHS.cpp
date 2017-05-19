#include "WindowMsgHandlerHS.h"


#include <stdlib.h>
#include <stdio.h>

#include "MainWindow.h"
#include "hook/HookEventParam.h"

char g_buf[1024] = { 0 };
bool g_testFlag = false;



extern MainWindow* g_mainWindow;
extern HANDLE g_hookThreadHandle;


LRESULT CALLBACK WndProcHS(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt;
	::GetWindowRect(hWnd, &rt);

	int w = rt.right - rt.left;
	int h = rt.bottom - rt.top;
	int x = 0;
	int y = 0;

	if (w < GetSystemMetrics(SM_CXSCREEN))
	{
		x = 8;
		y = 30;
	}

	if (255 != message && 132!=message && 512 != message && 32 != message)
	{
		// sprintf(g_buf, "MESSAGE ID：%d", message);
		// OutputDebugStringA(g_buf);
	}
	
	/*LRESULT res = */
	switch (message)
	{
	case WM_USER + 0x0400:
		::SendMessage((HWND)g_mainWindow->winId(), WM_USER + 0x0400, wParam, lParam);
		break;

	case WM_MOVE:
		//g_mainWindow->moveMyWindow();
		::SendMessage((HWND)g_mainWindow->winId(), message+WM_USER, wParam, lParam);
		break;



	case WM_MOUSEMOVE:
		x += LOWORD(lParam);
		y += HIWORD(lParam);
		break;

	

	case WM_LBUTTONDOWN:
		break;

	case WM_KEYDOWN:
	{
		EventParamHS param;
		switch (wParam)
		{
		case '1':
			param.id = E_EVENT_ID_HS_HOOK_AM_ReqAddFriend;
			g_mainWindow->SendEvent(&param, sizeof(EventParamHS));
			break;
		case '2':
			param.id = E_EVENT_ID_HS_HOOK_AM_AcceptAddFriend;
			g_mainWindow->SendEvent(&param, sizeof(EventParamHS));
			break;
		case '3':
			param.id = E_EVENT_ID_HS_HOOK_AM_ReqChallenge;
			g_mainWindow->SendEvent(&param, sizeof(EventParamHS));
			break;
		case '4':
			param.id = E_EVENT_ID_HS_HOOK_AM_AcceptChallenge;
			g_mainWindow->SendEvent(&param, sizeof(EventParamHS));
			break;

		case '8':
			::SendMessage((HWND)g_mainWindow->winId(), WM_USER + 0x0100, 0, 0);
			break;
		case '9':
			::SendMessage((HWND)g_mainWindow->winId(), WM_USER + 0x0101, 0, 0);
			break;

		case '0':
			//::TerminateThread(g_hookThreadHandle, 0);
			//OutputDebugStringA("HelloWorld:  我要走了");
		default:
			break;
		}
	}

	//sprintf(g_buf, "炉石传说: 键盘按键：%d", wParam);
	//OutputDebugStringA(g_buf);
	break;


	case WM_LBUTTONUP:
		//sprintf(buf, "炉石传说：鼠标左键释放：(%d, %d)", LOWORD(lParam), HIWORD(lParam));
		//OutputDebugStringA(buf);
		break;

	case WM_RBUTTONDOWN:
		x += LOWORD(lParam);
		y += HIWORD(lParam);

		sprintf(g_buf, "Extend HS：鼠标右键按下：x: (%d:%d-%f), y: (%d:%d-%f)", x, w, float(x) / float(w), y, h, float(y) / float(h));
		MessageBoxA(NULL, g_buf, "XXX", MB_OK);
		break;

		//	//case   WM_DESTROY:
		//	//	PostQuitMessage(0);
		//	//	return 0;


	case WM_DESTROY:
		//delete g_mainWindow;
		//g_mainWindow = nullptr;
		//g_pApp->exit(0);

		break;


	case WM_MOUSEHWHEEL:
		sprintf(g_buf, "WM_MOUSEHWHEEL：%d", LOWORD(wParam));
		OutputDebugStringA(g_buf);
		break;

	case WM_MOUSEWHEEL:
		sprintf(g_buf, "WM_MOUSEWHEEL：%d", wParam);
		OutputDebugStringA(g_buf);
		break;

	default:
		break;
	}
	return g_wndProcDefault(hWnd, message, wParam, lParam);
}
