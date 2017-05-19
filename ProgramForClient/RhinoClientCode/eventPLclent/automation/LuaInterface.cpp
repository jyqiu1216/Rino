
#include <iostream>
#include "common/LuaEngine.h"
#include "automation/WJAutomation.h"
#include "automation/AMUtil.h"
#include "hook/HookManager.h"

#include "GameMonitorHS.h"
extern GameMonitorHS g_gameMonitorHS;

#include "XLog.h"
extern XLog g_log;

#include <atlimage.h>
#include <math.h>

extern int printscr(HWND hWnd, lua_State *L, int posindex, int valueindex);

extern CLuaEngine* g_luaEngine;
extern HWND g_hWnd;

int g_gameWndOffsetX = 0;
int g_gameWndOffsetY = 0;

extern float windowScaleX;
extern float windowScaleY;

HWND GetWindowHwnd(const char* _title)
{
	HWND hWnd = NULL;

	int wndWidth = 1280;
	int wndHeight = 768;

	bool isLol = false;
	if (0 == strcmp(_title, WINDOW_TITLE_LOL_LOGIN))
	{
		wndWidth = 1280;
		wndHeight = 768;
		isLol = true;
	}
	else if (0 == strcmp(_title, WINDOW_TITLE_LOL_CLIENT))
	{
		wndWidth = 1280;
		wndHeight = 800;
		isLol = true;
	}
	else if (0 == strcmp(_title, WINDOW_TITLE_LOL_GAME))
	{
		//wndWidth = 1280;
		//wndHeight = 800;
	}
	else if (0 == strcmp(_title, WINDOW_TITLE_HB_LOGIN))
	{
	}
	else
	{
	}

	windowScaleX = 1;
	windowScaleY = 1;
	hWnd = ::FindWindowA(NULL, _title);
	if (hWnd)
	{
		if (isLol)
		{
			RECT rect;
			::GetWindowRect(hWnd, &rect);

			windowScaleX = float(rect.right - rect.left) / float(wndWidth);
			windowScaleY = float(rect.bottom - rect.top) / float(wndHeight);
		}
		//::SetLayeredWindowAttributes(hWnd, NULL, 200, LWA_ALPHA);
		//::SetLayeredWindowAttributes(hWnd, 0x00ff00ff, 255, LWA_ALPHA);
	}
	return hWnd;
}




bool IsLuaParamCount(lua_State* L, int _count)
{
	/* 得到参数个数 */
	int n = lua_gettop(L);
	if (3 != n)
		return 0;

	return 0;
}



/*********************************************************************************************
* @author:
* @date:	2016/08/24
* @brief:	LUA 接口
*
*/


int Inf_GetWindowHwnd(lua_State* L)
{
	const char* title = lua_tostring(L, 1);
	if (!title)
		return 0;


	HWND hWnd = GetWindowHwnd(title);
	lua_pushinteger(L, (int)hWnd);
	return 1;
}

int Inf_MoveWindow(lua_State* L)
{
	HWND hWnd = (HWND)lua_tointeger(L, 1);
	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);
	//int w = lua_tointeger(L, 4);
	//int h = lua_tointeger(L, 5);
	if (hWnd)
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;

		if (-1 == x && -1 == y)
		{
			x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
			y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
		}
		else
		{
			DWORD dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
			dwExStyle |= WS_EX_TOOLWINDOW;
			SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle);
		}

		MoveWindow(hWnd, x, y, w, h, true);
		::SendMessage(hWnd, WM_SIZE, 0, MAKELPARAM(w, h));
	}
	return 0;
}


int Inf_SetWindowVisibleByTitle(lua_State* L)
{
	const char* title = lua_tostring(L, 1);
	int visible = lua_tointeger(L, 2);

	HWND hWnd = ::FindWindowA(NULL, title);
	if (hWnd)
	{
		ShowWindow(hWnd, visible);
		if (visible)
		{
			SwitchToThisWindow(hWnd, TRUE);
		}
	}
	return 0;
}

BOOL CALLBACK ProcHideWindow(HWND hwnd, LPARAM lParam)
{
	if (hwnd)
	{
		char className[256] = { 0 };
		GetClassNameA(hwnd, className, 255);
		if (0 == strcmp(className, (char*)lParam))
		{
			ShowWindow(hwnd, false);
		}
	}
	return true;
}

int Lua_SetWindowVisibleByClassName(lua_State* L)
{
	HWND hWnd = NULL;
	const char* className = lua_tostring(L, 1);
	bool visible = (bool)lua_tointeger(L, 2);

	if (className)
	{
		EnumWindows(ProcHideWindow, (LPARAM)className);
	}
	//lua_pushinteger(L, (int)hWnd);
	return 0;
}



int Inf_MouseClickLButton(lua_State *L)
{
	/* 检查参数个数 */
	if (4 != lua_gettop(L))
		return 0;

	HWND hWnd = (HWND)lua_tointeger(L, 1);

	if (!hWnd)
		return 0;

	float x = lua_tonumber(L, 2);
	float y = lua_tonumber(L, 3);
	bool isAbs = lua_tointeger(L, 4);

	if (!isAbs)
	{
		x *= windowScaleX;
		y *= windowScaleY;
	}

	printf("sdp______Inf_MouseClickLButton：%d,%d \n", g_gameWndOffsetX, g_gameWndOffsetY);
	AMUtil::mouseClickLButton(hWnd, x + g_gameWndOffsetX, y + g_gameWndOffsetY);

	//RECT rc;
	//::GetWindowRect(g_hWnd, &rc);
	//SetCursorPos(rc.left + x, rc.top + y);
	//POINT pt;
	//GetCursorPos(&pt);
	//
	//
	////mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
	//mouse_event(MOUSEEVENTF_LEFTDOWN, rc.left + x, rc.top + y, 0, 0);
	//mouse_event(MOUSEEVENTF_LEFTUP, rc.left + x, rc.top + y, 0, 0);

	//printf("[MOUSE CLICK LEFT](%d, %d)", pt.x, pt.y);
	return 0;
}


int Inf_MouseClickLButton1(lua_State *L)
{
	/* 检查参数个数 */
	if (4 != lua_gettop(L))
		return 0;

	HWND hWnd = (HWND)lua_tointeger(L, 1);

	if (!hWnd)
		return 0;

	float x = lua_tonumber(L, 2);
	float y = lua_tonumber(L, 3);
	bool isAbs = lua_tointeger(L, 4);

	if (!isAbs)
	{
		x *= windowScaleX;
		y *= windowScaleY;
	}

	if (x < 1 || y < 1)
	{
		RECT rt;
		::GetWindowRect(hWnd, &rt);

		x = x*(rt.right - rt.left);
		y = y*(rt.bottom - rt.top);
	}

	RECT rt;
	::GetWindowRect(hWnd, &rt);

	//AMUtil::mouseClickLButton1(hWnd, x + g_gameWndOffsetX, y + g_gameWndOffsetY);
	AMUtil::mouseClickLButton1(hWnd, rt.left + x + g_gameWndOffsetX, rt.top + y + g_gameWndOffsetY);
	return 0;
}


int Inf_Input(lua_State *L)
{
	int n = lua_gettop(L);
	if (2 != n)
		return 0;

	HWND hWnd = (HWND)lua_tointeger(L, 1);
	if (!hWnd)
		return 0;

	const char* text = lua_tostring(L, 2);
	printf("输入：%s\n", text);

	SwitchToThisWindow(hWnd, true);
	AMUtil::setClipboard(text);
	Sleep(200);
	AMUtil::pasteClipboard(hWnd);

	return 0;
}


int Inf_Backspace(lua_State *L)
{
	HWND hWNd = (HWND)lua_tointeger(L, 1);
	::SendMessage(hWNd, WM_SYSKEYDOWN, VK_BACK, NULL);
	::SendMessage(hWNd, WM_SYSKEYUP, VK_BACK, NULL);
	Sleep(10);

	return 0;
}

int Inf_PressKey(lua_State *L)
{
	HWND hWNd = (HWND)lua_tointeger(L, 1);
	int keyCode = lua_tointeger(L, 2);

	//keybd_event(VK_MENU, 0, 2, 0);
	Sleep(50);
	::PostMessage(hWNd, WM_KEYDOWN, keyCode, NULL);
	::PostMessage(hWNd, WM_KEYUP, keyCode, NULL);
	Sleep(10);


	return 0;
}


int Inf_Sleep(lua_State *L)
{
	if (1 != lua_gettop(L))
		return 0;

	int x = lua_tointeger(L, 1);
	Sleep(x);
	return 0;
}


int Inf_IsWindowExist(lua_State *L)
{
	int n = lua_gettop(L);
	if (1 != n)
		return 0;

	const char* title = lua_tostring(L, 1);
	if (!title)
		return 0;

	HWND hWnd = GetWindowHwnd(title);
	lua_pushinteger(L, (int)hWnd);
	return 1;
}


int Inf_ActiveWindow(lua_State* L)
{
	int n = lua_gettop(L);
	if (1 != n)
		return 0;

	const char* title = lua_tostring(L, 1);
	if (!title)
		return 0;

	g_hWnd = ::FindWindowA(NULL, title);
	if (g_hWnd)
	{
		SwitchToThisWindow(g_hWnd, TRUE);
		//Sleep(1000);
	}
	return 0;
}


int Inf_Abs(lua_State *L)
{
	int n = lua_gettop(L);
	if (1 != n)
	{
		return 0;
	}
	int x = lua_tointeger(L, 1);
	x = abs(x);
	lua_pushnumber(L, x);
	return 1;
}

#include <ShObjIdl.h>


int Inf_SetWindowVisible(lua_State* L)
{
	HWND hWnd = (HWND)lua_tointeger(L, 1);
	int flag = lua_tointeger(L, 2);

	if (hWnd)
	{
		int x, y;

		RECT rect;
		GetWindowRect(hWnd, &rect);
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;

		if (flag > 0)
		{
			x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
			y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

			DWORD dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
			dwExStyle &= ~WS_EX_TOOLWINDOW;
			SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle);
		}
		else
		{
			x = GetSystemMetrics(SM_CXSCREEN) - 1;
			y = 0;

			DWORD dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
			dwExStyle |= WS_EX_TOOLWINDOW;
			SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle);
		}

		MoveWindow(hWnd, x, y, w, h, true);
		::SendMessage(hWnd, WM_SIZE, 0, MAKELPARAM(w, h));

		ITaskbarList* pTaskbarList;
		HRESULT hRes = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList, (void**)&pTaskbarList);

		if (SUCCEEDED(hRes))
		{
			pTaskbarList->HrInit();
			if (flag > 0)
			{
				pTaskbarList->AddTab(hWnd);
			}
			else
			{
				pTaskbarList->DeleteTab(hWnd);
			}
			pTaskbarList->Release();
		}
	}
	return 0;
}




struct ColorRGB
{
	int r;
	int g;
	int b;

	ColorRGB() : r(0), g(0), b(0)
	{}

	ColorRGB(int _r, int _g, int _b) : r(_r), g(_g), b(_b)
	{}
};


bool compareImagePixel(LuaDataMap* _data, CImage& image, int _offset)
{
	LuaDataMap::iterator iter = _data->begin();

	int x1 = iter->second->value.value_int;
	int x = windowScaleX * x1;

	iter++;
	int y1 = iter->second->value.value_int;
	int y = windowScaleY*y1;

	ColorRGB color1;
	iter++;
	int r1 = iter->second->value.value_int;

	iter++;
	int g1 = iter->second->value.value_int;

	iter++;
	int b1 = iter->second->value.value_int;


	if (image.GetWidth() < x || image.GetHeight() < y)
	{
		printf("像素点颜色值：位置异常：图像大小[%d, %d], 位置：(%d, %d)", image.GetWidth(), image.GetHeight(), x, y);
		return false;
	}

	COLORREF pixel = image.GetPixel(x, y);
	int r = GetRValue(pixel);
	int g = GetGValue(pixel);
	int b = GetBValue(pixel);

	printf("颜色值：[%d,%d]  (%d,%d,%d)  (%d,%d,%d) ", x, y, r1, g1, b1, r, g, b);
	if (abs(r1 - r) > _offset || abs(g1 - g) > _offset || abs(b1 - b) > _offset)
	{
		return false;
	}
	else
	{
		return true;
	}
}



int Inf_GetWindowPixelRGB(lua_State *L)
{
	int n = lua_gettop(L);
	if (2 != n)
		return 0;

	const char* title = lua_tostring(L, 1);
	if (!title)
		return 0;

	HWND hWnd = ::FindWindowA(NULL, title);
	if (!hWnd)
		return 0;

	int width = 800;    // 图片宽度
	int height = 480;   // 图片高度
	int xpos = 0;       // 起始x坐标
	int ypos = 0;       // 起始y坐标


	RECT rect;
	::GetWindowRect(hWnd, &rect);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	// 获取窗口的设备上下文（Device Contexts）// 要截图的窗口句柄，为空则全屏
	HDC hdcWindow = GetDC(NULL);
	// 获取设备相关信息的尺寸大小
	int nBitPerPixel = GetDeviceCaps(hdcWindow, BITSPIXEL);

	// 创建图像，设置宽高，像素// 创建图像，设置宽高，像素
	CImage image;
	image.Create(width, height, nBitPerPixel);
	// 对指定的源设备环境区域中的像素进行位块（bit_block）转换
	BitBlt(
		image.GetDC(),  // 保存到的目标 图片对象 上下文
		0, 0,     // 起始 x, y 坐标
		width, height,  // 截图宽高
		hdcWindow,      // 截取对象的 上下文句柄
		rect.left, rect.top,           // 指定源矩形区域左上角的 X, y 逻辑坐标
		SRCCOPY);

	// 释放 DC句柄
	ReleaseDC(NULL, hdcWindow);
	// 释放图片上下文
	image.ReleaseDC();

	LuaDataMap* pMapPos = g_luaEngine->getTable(L, 2);

	if (rect.left < 0 && rect.right < 0 && rect.top < 0 && rect.bottom < 0)
	{
		lua_pushnumber(L, 0);
		printf("=====> 窗口位置异常, RECT(%d,%d,%d,%d)\n", rect.left, rect.right, rect.top, rect.bottom);
		return 1;
	}

	LuaDataMap::iterator it;
	int count = 0;
	int matchNum = 0;
	for (it = pMapPos->begin(); it != pMapPos->end(); ++it, ++count)
	{
		LuaDataMap* value_map = it->second->value.value_map;
		if (!compareImagePixel(value_map, image, 8))
		{
			printf("    ----- 不匹配\n");
		}
		else
		{
			printf("    ----- 匹配\n");
			++matchNum;
		}
	}
	delete pMapPos;
	float res = float(matchNum) / float(count);
	printf("匹配结果: %0.2f%%\n", res * 100);
	lua_pushnumber(L, res >= 0.8 ? 1 : 0);
	return 1;

	//printscr(hWnd, L, 2, 2);
}


int printscr(HWND hWnd, lua_State *L, int posindex, int valueindex)
{
	int width = 800;    // 图片宽度
	int height = 480;   // 图片高度
	int xpos = 0;       // 起始x坐标
	int ypos = 0;       // 起始y坐标


	RECT rect;
	::GetWindowRect(hWnd, &rect);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	// 获取窗口的设备上下文（Device Contexts）// 要截图的窗口句柄，为空则全屏
	HDC hdcWindow = GetDC(NULL);
	// 获取设备相关信息的尺寸大小
	int nBitPerPixel = GetDeviceCaps(hdcWindow, BITSPIXEL);

	// 创建图像，设置宽高，像素// 创建图像，设置宽高，像素
	CImage image;
	image.Create(width, height, nBitPerPixel);
	// 对指定的源设备环境区域中的像素进行位块（bit_block）转换
	BitBlt(
		image.GetDC(),  // 保存到的目标 图片对象 上下文
		0, 0,     // 起始 x, y 坐标
		width, height,  // 截图宽高
		hdcWindow,      // 截取对象的 上下文句柄
		rect.left, rect.top,           // 指定源矩形区域左上角的 X, y 逻辑坐标
		SRCCOPY);

	// 释放 DC句柄
	ReleaseDC(NULL, hdcWindow);
	// 释放图片上下文
	image.ReleaseDC();

	LuaDataMap* pMapPos = g_luaEngine->getTable(L, posindex);

	if (rect.left < 0 && rect.right < 0 && rect.top < 0 && rect.bottom < 0)
	{
		lua_pushnumber(L, 0);
		printf("=====> 窗口位置异常, RECT(%d,%d,%d,%d)\n", rect.left, rect.right, rect.top, rect.bottom);
		return 1;
	}

	LuaDataMap::iterator it;
	int i = 1;
	int res = 1;
	for (it = pMapPos->begin(); it != pMapPos->end(); ++it, ++i)
	{
		LuaDataMap* value_map = it->second->value.value_map;
		if (!compareImagePixel(value_map, image, 8))
		{
			printf("=====> 不匹配\n");
			res = 1;
		}
	}
	delete pMapPos;
	printf("=====> 匹配\n");
	lua_pushnumber(L, res);
	return 1;
}


int Lua_SetWindowAlpha(lua_State *L)
{
	const char* title = lua_tostring(L, 1);
	int alpha = lua_tointeger(L, 2);
	HWND hWnd = FindWindowA(NULL, title);
	if (hWnd)
	{
		ShowCursor(false);
		DWORD style = ::GetWindowLong(hWnd, GWL_EXSTYLE);
		style = style | WS_EX_LAYERED | WS_EX_TOPMOST;
		style = style &~WS_EX_TOOLWINDOW;
		::SetWindowLong(hWnd, GWL_EXSTYLE, style);
		//::SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA /*| LWA_COLORKEY*/);



		RECT rect;
		::GetWindowRect(hWnd, &rect);
		::ClipCursor(&rect);
		::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_DRAWFRAME | SWP_NOSIZE);
		//::SetWindowPos(hWnd, HWND_TOPMOST, rect.left, rect.right, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_DRAWFRAME | SWP_NOSIZE);
	}
	return 0;
}



bool compareHdcPixel(LuaDataMap* _data, HDC _hdc, int _offset)
{
	LuaDataMap::iterator iter = _data->begin();

	int x1 = iter->second->value.value_int;
	int x = windowScaleX * x1;

	iter++;
	int y1 = iter->second->value.value_int;
	int y = windowScaleY*y1;

	iter++;
	int r1 = iter->second->value.value_int;


	iter++;
	int g1 = iter->second->value.value_int;

	iter++;
	int b1 = iter->second->value.value_int;

	COLORREF pixel = ::GetPixel(_hdc, x, y);
	byte r = GetRValue(pixel);
	byte g = GetGValue(pixel);
	byte b = GetBValue(pixel);

	printf("像素点颜色值：[%d,%d] (%d,%d,%d)  (%d,%d,%d)", x, y, r, g, b, r1, g1, b1);

	if (abs(r1 - r) > _offset || abs(g1 - g) > _offset || abs(b1 - b) > _offset)
	{
		return false;
	}
	else
	{
		return true;
	}
}


int Lua_ComparePixel(lua_State *L)
{
	int n = lua_gettop(L);
	const char* title = lua_tostring(L, 1);
	HWND hWnd = ::FindWindowA(NULL, title);
	if (!hWnd)
	{
		lua_pushnumber(L, true);
		return 1;
	}
	RECT rc;
	::GetWindowRect(hWnd, &rc);

	//AMUtil::getWindowPixel(hWnd, 150, 200);

	//创建 
	HDC hdcScreen = GetDC(NULL);
	HDC hdc = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, rc.right - rc.left, rc.bottom - rc.top);
	SelectObject(hdc, hbmp);
	PrintWindow(hWnd, hdc, PW_CLIENTONLY);

	LuaDataMap* pMapPos = g_luaEngine->getTable(L, 2);

	int matchNum = 0;
	int count = 0;
	for (LuaDataMap::iterator it = pMapPos->begin(); it != pMapPos->end(); ++it, ++count)
	{
		LuaDataMap* value_map = it->second->value.value_map;
		if (!compareHdcPixel(value_map, hdc, 8))
		{
			lua_pushnumber(L, false);
			printf("     ----- 不匹配\n");
		}
		else
		{
			++matchNum;
			printf("     ----- 匹配\n");
		}
	}
	delete pMapPos;
	float res = float(matchNum) / float(count);
	printf("匹配结果: %0.2f%%\n", res * 100);
	lua_pushnumber(L, res >= 0.8 ? 1 : 0);
	return 1;
}




int Lua_InjectDll(lua_State *L)
{
	/* 检查参数个数 */
	if (2 != lua_gettop(L))
		return 0;

	const char* windowTitle = lua_tostring(L, 1);
	const char* dll = lua_tostring(L, 2);
	if (!windowTitle || !dll)
		return 0;

	HWND hWnd = ::FindWindowA(NULL, windowTitle);
	if (!hWnd)
	{
		printf("窗口[%s]不存在", windowTitle);
		return 0;
	}


	DWORD dwProcId = 0;
	DWORD dwThreadId = 0;

	// 获取进程ID和主线程ID.
	dwThreadId = GetWindowThreadProcessId(hWnd, &dwProcId);

	TCHAR dllName[100];
#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, dll, -1, dllName, 100);
#else
	strcpy(Name, strUsr);
#endif
	TCHAR pBuf[MAX_PATH] = {0};		//存放路径的变量
	//GetCurrentDirectory(MAX_PATH, pBuf);	 //获取程序的当前目录
	::GetModuleFileName(::GetModuleHandle(NULL), pBuf, ARRAYSIZE(pBuf));

	TCHAR* pTchBuffer = ::wcsrchr(pBuf, '\\');
	::ZeroMemory(pTchBuffer, ::_tcslen(pTchBuffer));

	TCHAR hookDll[MAX_PATH] = { 0 };
	lstrcat(hookDll, pBuf);
	lstrcat(hookDll, TEXT("\\"));
	lstrcat(hookDll, dllName);

	EnHookStatus res = HookManager::injectDll(dwProcId, hookDll);
	if (res != E_HOOK_SUCCESS)
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "DLL[%s] 执行注入操作失败! res=%d", dll, res);
	}
	else
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "DLL[%s] 执行注入操作成功", dll);
	}
	return int(res);
}




