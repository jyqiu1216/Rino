// AutoRun.cpp : Defines the exported functions for the DLL application.
//

#include "common/LuaEngine.h"

#include "AMUtil.h"
#include "WJAutomation.h"
#include "LuaInterface.h"
#include "LuaInterfaceHS.h"
#include "GameMonitorHS.h"
GameMonitorHS g_gameMonitorHS;


#include "identification/hs_identification.h"
#include "hook/WSMessageId.h"


#include <atlimage.h>
#include <math.h>
#include "XLog.h"

const char* WINDOW_TITLE_LOL_LOGIN = "英雄联盟登录程序";
const char* WINDOW_TITLE_LOL_CLIENT = "PVP.net 客户端";
const char* WINDOW_TITLE_LOL_GAME = "League of Legends (TM) Client";
const char* WINDOW_TITLE_HB_LOGIN = "战网登录";
const char* WINDOW_TITLE_HB_CLIENT = "PVP.net 客户端";
const char* WINDOW_TITLE_HB_BATTLENET = "战网";
const char* WINDOW_TITLE_HB_FRIENDS = "好友";

HWND g_hWnd = NULL;

CLuaEngine* g_luaEngine = NULL;

int g_gameId = -1;


float windowScaleX = 1.0f;
float windowScaleY = 1.0f;

XLog g_log;

int checkLuaCallRes(int _ret, lua_State* L, const char* _funcName)
{
	if (0 != _ret)
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "执行脚本函数[%s]失败", _funcName);
		OutputDebugStringA(lua_tostring(L, -1));
		return WJAM_RES_LUA_EXCE_ERROR;
	}

	int res = lua_tonumber(L, -1);
	g_log.writeLog(XLog::E_LOG_DEBUG, "执行脚本函数[%s]完成，返回结果：%d", _funcName, res);
	return res;
}

bool isLuaVaild()
{
	/* 判断LUA是否初始化 */
	if (NULL == g_luaEngine)
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "[ERROR] LuaEngine未初始化");
		return false;
	}
	else
	{
		return true;
	}
}

#define AM_SHARE_MEM_HS_SERVER		TEXT("AM_SHARE_MEM_HS_SERVER")	
#define AM_SHARE_MEM_HS_CLIENT		TEXT("AM_SHARE_MEM_HS_CLIENT")	

AMCallback* g_pAmCallback = nullptr;

DLL_EXPORT bool WJAM::initLua(const char* _luaFile, AMCallback* _callback)
{
	g_pAmCallback;// = _callback;
	::CoInitialize(NULL);
	

	/* 初始化LuaEngine */
	if (NULL != g_luaEngine)
	{
		delete g_luaEngine;
		g_luaEngine = NULL;
	}
	g_luaEngine = new CLuaEngine();

	lua_register(g_luaEngine->GetLuaState(), "abs", Inf_Abs);
	lua_register(g_luaEngine->GetLuaState(), "Inf_MoveWindow", Inf_MoveWindow);
	lua_register(g_luaEngine->GetLuaState(), "Inf_MouseClickLButton", Inf_MouseClickLButton);
	lua_register(g_luaEngine->GetLuaState(), "Inf_MouseClickLButton1", Inf_MouseClickLButton1);
	lua_register(g_luaEngine->GetLuaState(), "Inf_Input", Inf_Input);
	lua_register(g_luaEngine->GetLuaState(), "Inf_Backspace", Inf_Backspace);
	lua_register(g_luaEngine->GetLuaState(), "Sleep", Inf_Sleep);
	lua_register(g_luaEngine->GetLuaState(), "INF_IsWindowExist", Inf_IsWindowExist);
	lua_register(g_luaEngine->GetLuaState(), "Inf_ActiveWindow", Inf_ActiveWindow);
	lua_register(g_luaEngine->GetLuaState(), "Inf_GetWindowHwnd", Inf_GetWindowHwnd);
	lua_register(g_luaEngine->GetLuaState(), "Inf_SetWindowVisible", Inf_SetWindowVisible);
	lua_register(g_luaEngine->GetLuaState(), "Lua_GetWindowPixelRGB", Inf_GetWindowPixelRGB);
	lua_register(g_luaEngine->GetLuaState(), "Lua_SetWindowVisibleByClassName", Lua_SetWindowVisibleByClassName);
	lua_register(g_luaEngine->GetLuaState(), "Lua_SetWindowVisibleByTitle", Inf_SetWindowVisibleByTitle);
	lua_register(g_luaEngine->GetLuaState(), "Lua_InjectDll", Lua_InjectDll);
	lua_register(g_luaEngine->GetLuaState(), "Lua_SetWindowAlpha", Lua_SetWindowAlpha);
	lua_register(g_luaEngine->GetLuaState(), "Lua_ComparePixel", Lua_ComparePixel);
	lua_register(g_luaEngine->GetLuaState(), "Inf_PressKey", Inf_PressKey);
	//lua_register(g_luaEngine->GetLuaState(), "Inf_ImageRecognition", Inf_ImageRecognition);
	//lua_register(g_luaEngine->GetLuaState(), "Inf_ImageContrastRect", Inf_ImageContrastRect);
	lua_register(g_luaEngine->GetLuaState(), "Inf_GetHS_FriendWindowRect", Inf_GetHS_FriendWindowRect);
	lua_register(g_luaEngine->GetLuaState(), "Inf_GetHS_WindowRect", Inf_GetHS_WindowRect);
	lua_register(g_luaEngine->GetLuaState(), "lua_TestDebug", lua_TestDebug);
	
	if (NULL == _luaFile)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "lua文件指针为空");
		return false;
	}
	//先切换路径至lua脚本所在路径
	TCHAR path[1024] = { 0 };
	HMODULE  handle = GetModuleHandle(NULL);
	GetModuleFileName(handle, path, ARRAYSIZE(path));
	wchar_t* pwchExeName = wcsrchr(path, '\\');
	::ZeroMemory(pwchExeName, ::wcslen(pwchExeName));
	TCHAR CurPath[1024] = { 0 };
	::GetCurrentDirectory(ARRAYSIZE(CurPath), CurPath);
	::SetCurrentDirectory(path);

	//加载lua
	if (!g_luaEngine->loadLuaFile(_luaFile))
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "加载lua文件[%s]失败", _luaFile);
		return false;
	}
	else
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "加载lua文件[%s]成功", _luaFile);
	}
	//切换回来
	::SetCurrentDirectory(CurPath);

	/* 初始化共享内存处理线程 */
	//g_gameMonitorHS.initEventSignal(AM_SHARE_MEM_HS_SERVER, AM_SHARE_MEM_HS_CLIENT, EVENT_EXIT_DEFAULT_NAME, &g_gameMonitorHS);
	//g_gameMonitorHS.StartThread();
	return true;
}
extern int g_gameWndOffsetX;
extern int g_gameWndOffsetY;

DLL_EXPORT int WJAM::login(LOGININFO* _loginInfo)
{
	/* 判断LUA是否初始化 */
	if (NULL == g_luaEngine)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "LuaEngine未初始化");
		return WJAM_RES_LUA_NOT_INIT;
	}

	g_gameId = _loginInfo->gameid;
	std::string windowTitle;
	switch (_loginInfo->gameid)
	{
	case GAME_ID_LOL: /* 英雄联盟 */
		windowTitle = WINDOW_TITLE_LOL_LOGIN;

		g_gameWndOffsetX = 0;
		g_gameWndOffsetY = 0;

		/* 判断游戏是否已经启动 */
		if (g_hWnd = ::FindWindowA(NULL, WINDOW_TITLE_LOL_GAME))
		{
			g_log.writeLog(XLog::E_LOG_WARNING, "LOL游戏客户端已经运行,玩家正在游戏中,需要关闭客户端！");
			return WJAM_RES_LOL_EXIST_IN_GAME;
		}
		else if (g_hWnd = ::FindWindowA(NULL, WINDOW_TITLE_LOL_CLIENT))
		{
			g_log.writeLog(XLog::E_LOG_WARNING, "LOL游戏客户端已经运行,玩家已经选择服务器,需要关闭客户端！");
			return WJAM_RES_LOL_EXIST_IN_SERVER;
		}
		else if (g_hWnd = ::FindWindowA(NULL, WINDOW_TITLE_LOL_LOGIN))
		{
		}
		break;

	case GAME_ID_HB: /* 炉石传说 */
		windowTitle = WINDOW_TITLE_HB_LOGIN;
		break;

	default:
		g_log.writeLog(XLog::E_LOG_ERROR, "启动游戏失败[id=%d]：未定义的游戏", _loginInfo->gameid);
		return WJAM_RES_UNKNOWN_GAME;
		break;
	}

	if (::FindWindowA(NULL, windowTitle.c_str()) == NULL)
	{
		WinExec(_loginInfo->path, SW_SHOW);
		//ShellExecute(NULL, A2T("open"), A2T(pLoginInfo->path), NULL, NULL, SW_SHOWNORMAL);
	}
	g_log.writeLog(XLog::E_LOG_DEBUG, "启动游戏[id=%d]：%s", _loginInfo->gameid, WINDOW_TITLE_LOL_LOGIN);

	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "login");
	lua_pushnumber(L, _loginInfo->gameid);
	lua_pushnumber(L, _loginInfo->serverid);
	lua_pushstring(L, _loginInfo->path);


	int ret = lua_pcall(L, 3, 1, 0);// 调用函数，调用完成以后，会将返回值压入栈中，3表示参数个数，1表示返回结果个数。
	return checkLuaCallRes(ret, L, "login");
}


DLL_EXPORT int WJAM::createRoom(ROOMINFO* pRoomInfo)
{
	/* 判断LUA是否初始化 */
	if (NULL == g_luaEngine)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "LuaEngine未初始化");
		return WJAM_RES_LUA_NOT_INIT;
	}

	if (::FindWindowA(NULL, WINDOW_TITLE_LOL_CLIENT) == NULL)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "未找到游戏窗口: %s", WINDOW_TITLE_LOL_CLIENT);
		return WJAM_RES_WINDOW_NOT_EXIST;
	}

	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "createroom");			// 获取函数，压入栈中  
	lua_pushstring(L, pRoomInfo->roomname);          // 压入第一个参数  
	lua_pushstring(L, pRoomInfo->password);
	lua_pushstring(L, pRoomInfo->account);
	lua_pushnumber(L, pRoomInfo->mode);
	lua_pushnumber(L, pRoomInfo->map);
	lua_pushnumber(L, pRoomInfo->playernum);
	lua_pushnumber(L, pRoomInfo->observe);
	lua_pushnumber(L, pRoomInfo->gametype);

	// 调用函数，n表示参数个数，r表示返回结果个数。
	return checkLuaCallRes(lua_pcall(L, 8, 1, 0), L, "createroom");
}



DLL_EXPORT int WJAM::joinRoom(ROOMINFO* pRoomInfo)
{
	/* 判断LUA是否初始化 */
	if (NULL == g_luaEngine)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "LuaEngine未初始化");
		return WJAM_RES_LUA_NOT_INIT;
	}

	if (::FindWindowA(NULL, WINDOW_TITLE_LOL_CLIENT) == NULL)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "未找到游戏窗口: %s", WINDOW_TITLE_LOL_CLIENT);
		return WJAM_RES_WINDOW_NOT_EXIST;
	}


	g_log.writeLog(XLog::E_LOG_DEBUG, "加入游戏房间 [%s]", pRoomInfo->roomname);
	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "joinroom");        // 获取函数，压入栈中  
	lua_pushstring(L, pRoomInfo->roomname);          // 压入第一个参数  
	lua_pushstring(L, pRoomInfo->password);
	lua_pushstring(L, pRoomInfo->account);
	lua_pushnumber(L, pRoomInfo->side);

	// 调用函数，n表示参数个数，r表示返回结果个数。  
	return checkLuaCallRes(lua_pcall(L, 4, 1, 0), L, "joinroom");
}


DLL_EXPORT int WJAM::startGame(WJAM::GAME_ID _gameId, bool _isRoomOwner)
{
	/* 判断LUA是否初始化 */
	if (NULL == g_luaEngine)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "LuaEngine未初始化");
		return WJAM_RES_LUA_NOT_INIT;
	}

	//if (::FindWindowA(NULL, WINDOW_TITLE_LOL_CLIENT) == NULL)
	//{
	//	writeLog(LOG_FILE, "[ERROR] 未找到游戏窗口: %s", WINDOW_TITLE_LOL_CLIENT);
	//	return WJAM_RES_WINDOW_NOT_EXIST;
	//}

	g_log.writeLog(XLog::E_LOG_DEBUG, "开始游戏 [%s]");
	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "playgame");
	lua_pushnumber(L, int(_gameId));
	lua_pushnumber(L, int(_isRoomOwner));
	
	// 调用lua函数，n表示参数个数，r表示返回结果个数。
	return checkLuaCallRes(lua_pcall(L, 2, 1, 0), L, "playgame");
}



DLL_EXPORT int injectRomateDll(WJAM::GAME_ID _gameId)
{
	/* 判断LUA是否初始化 */
	if (NULL == g_luaEngine)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "LuaEngine未初始化");
		return WJAM_RES_LUA_NOT_INIT;
	}
	g_log.writeLog(XLog::E_LOG_DEBUG, "注入远程DLL，游戏ID[%d]", _gameId);

	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "injectDll");
	lua_pushnumber(L, int(_gameId));

	return checkLuaCallRes(lua_pcall(L, 2, 1, 0), L, "injectDll");
}


#include <psapi.h> 
#include   <tlhelp32.h>
DWORD GetProcessIdByName(LPCTSTR pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return 0;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL res;
	for (res = Process32First(hSnapshot, &pe); res; res = Process32Next(hSnapshot, &pe))
	{
		if (0 == wcscmp(pe.szExeFile, pName))
		{
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
	}
	return 0;
}


int CloseWindow(const char* _windowTitle)
{
	if (!_windowTitle)
		return 1;

	HWND hWnd = ::FindWindowA(NULL, _windowTitle);
	if (!hWnd)
		return 2;

	// 进程ID, 主线程ID 
	DWORD processId = 0;
	DWORD threadId = 0;

	/* 同时得到进程ID和主线程ID */
	threadId = GetWindowThreadProcessId(hWnd, &processId);

	/* 通过进程ID获取控制句柄 */
	HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_ALL_ACCESS, TRUE, processId);;
	if (!TerminateProcess(handle, 0))
		return 3;

	return 0;
}


DLL_EXPORT int WJAM::closeGame(WJAM::GAME_ID _gameId)
{
	HWND hWnd = NULL;
	switch (_gameId)
	{
	case WJAM::GAME_ID_LOL:
	{
		CloseWindow(WINDOW_TITLE_LOL_GAME);
		CloseWindow(WINDOW_TITLE_LOL_CLIENT);
		CloseWindow(WINDOW_TITLE_LOL_LOGIN);
	}
		break;
	case WJAM::GAME_ID_HB:
		// WINDOW_TITLE_HB_LOGIN = "战网登录";
		// WINDOW_TITLE_HB_CLIENT = "PVP.net 客户端";
		// WINDOW_TITLE_HB_BATTLENET = "战网";
		// WINDOW_TITLE_HB_FRIENDS = "好友";
		break;
	default:
		return WJAM_RES_LUA_PARAM_ERROR;	// LUA 参数错误
		break;
	}

	return 0;
}


HWND setWindowVisible(const char* _windowTitle, bool _visible)
{
	HWND hWnd = FindWindowA(NULL, _windowTitle);
	if (hWnd)
	{
		ShowWindow(hWnd, _visible);
		if (_visible)
		{
			BringWindowToTop(hWnd);
		}
	}
	return hWnd;
}

DLL_EXPORT void WJAM::setGameWindowVisible(WJAM::GAME_ID _gameId, bool _visible)
{
	HWND hWnd = NULL;

	switch (_gameId)
	{
	case WJAM::GAME_ID_LOL:
	{
		setWindowVisible(WINDOW_TITLE_LOL_GAME, _visible);
		setWindowVisible(WINDOW_TITLE_LOL_CLIENT, _visible);
		setWindowVisible(WINDOW_TITLE_LOL_LOGIN, _visible);
	}
		break;
	case WJAM::GAME_ID_HB:
		// WINDOW_TITLE_HB_LOGIN = "战网登录";
		// WINDOW_TITLE_HB_CLIENT = "PVP.net 客户端";
		// WINDOW_TITLE_HB_BATTLENET = "战网";
		// WINDOW_TITLE_HB_FRIENDS = "好友";
		break;
	default:
		break;
	}
}





DLL_EXPORT int WJAM::startupHS(int _wsPort, const char* _url, std::string& _playerName)
{
	g_log.writeLog(XLog::E_LOG_DEBUG, "执行自动化：startupHS");

	/* 重置炉石监视器的状态 */
	g_gameMonitorHS.setGameStatus(E_STATUS_HS_HOOKED);

	// 检查luaEngine的状态， luaEngine异常时无法执行脚本
	if (!isLuaVaild())
	{
		return WJAM_RES_LUA_NOT_INIT;
	}
	g_gameMonitorHS.initEmbedWinHS(_wsPort, _url, "");
	g_log.writeLog(XLog::E_LOG_DEBUG, "创建共享纹理成功.");
	return WJAM_RES_OK;


	/**
	*	调用执行lua脚本函数
	*/
	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "startup_hs");

	// 调用lua函数，n表示参数个数，r表示返回结果个数。
	int res = checkLuaCallRes(lua_pcall(L, 0, 1, 0), L, "startup_hs");
	if (WJAM_RES_OK != res)
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "lua 脚本执行异常");
		return res;
	}

	// 初始化共享内存处理线程
	g_gameMonitorHS.initEventSignal(AM_SHARE_MEM_HS_SERVER, AM_SHARE_MEM_HS_CLIENT, EVENT_EXIT_DEFAULT_NAME, &g_gameMonitorHS);
	g_gameMonitorHS.StartThread();


	/**
	*	从炉石渲染窗口中创建共享句柄用于图像识别，该过程需要等待炉石的启动及有一定的延时性
	*	所以在此处进行循环监控。
	*/
	double tick = GetTickCount();
	while (true)
	{
		double t = GetTickCount() - tick;
		EnGameStatusHS& status = g_gameMonitorHS.getGameStatus();
		if (E_STATUS_HS_NOTSTART != status && E_STATUS_HS_D3D_ERROR != status)
		{
			HWND hWnd = g_gameMonitorHS.getGameWindowHwnd();
			if (!hWnd)
			{
				res = WJAM_RES_WINDOW_NOT_EXIST;
			}
			else
			{
				//g_gameMonitorHS.getPlayerName(hWnd, _playerName);
				/* 初始化炉石的内嵌窗口 */
				g_gameMonitorHS.initEmbedWinHS(_wsPort, _url, "");
				g_log.writeLog(XLog::E_LOG_DEBUG, "创建共享纹理成功.");
				res = WJAM_RES_OK;
			}
			break;
		}
		//else if (E_STATUS_HS_D3D_ERROR == status)
		//{
		//	res = WJAM_RES_D3D_ERROE;
		//	break;
		//}
		else if (t > 25000)
		{
			res = WJAM_RES_HOOK_TIMEOUT;
			g_log.writeLog(XLog::E_LOG_DEBUG, "创建共享纹理失败：超时退出（%d秒）", int(t*0.001));
			break;
		}
		Sleep(1000);
	}

	if (WJAM_RES_OK == res)
	{
	}
	return res;
}


DLL_EXPORT int WJAM::reqAddFriend(const char *_name)
{
	g_log.writeLog(XLog::E_LOG_DEBUG, "调用执行自动化 $BEGIN$：请求添加好友[%s]", _name);

	HWND hWnd = g_gameMonitorHS.getGameWindowHwnd();
	if (!hWnd)
		return WJAM_RES_WINDOW_NOT_EXIST;

	if (!isLuaVaild())
		return WJAM_RES_LUA_NOT_INIT;

	

	/* 窗口前端显示 */
	//BringWindowToTop(hWnd, true);
	BringWindowToTop(hWnd);
	Sleep(200);

	g_gameMonitorHS.clickWindow(hWnd, 0.5, 0.2, 1000);

	/* 通知内嵌窗口 */
	g_gameMonitorHS.setEvent(GAME_EVENT_AM_ADD_FRIEND);

	//if (g_gameMonitorHS.imageContrast("game_task") >= 120)
	//{
	//	g_gameMonitorHS.clickWindow(hWnd, 0.5, 0.2);
	//	//RECT rt;
	//	//::GetWindowRect(hWnd, &rt);
	//	//AMUtil::mouseClickLButton1(hWnd, rt.left + (rt.right - rt.left) * 0.5, rt.top + (rt.bottom - rt.top) * 0.2);
	//}

	/* 在屏幕上点击一次，关闭开始任务页（优化处理方案： 提高图像识别的准确率，通过图片识别判断状态后进行点击操作） */
	//RECT rt;
	//::GetWindowRect(hWnd, &rt);
	//AMUtil::mouseClickLButton1(hWnd, rt.left + (rt.right - rt.left) * 0.5, rt.top + (rt.bottom - rt.top) * 0.2);

	/* 显示好友列表窗口 */
	g_gameMonitorHS.showFriendWindow(hWnd);
	std::string playerName;
	//g_gameMonitorHS.getPlayerName(hWnd, playerName);

	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "reqAddFriend");
	lua_pushinteger(L, (int)hWnd);
	lua_pushstring(L, _name);

	//g_gameMonitorHS.setGameStatus(E_STATUS_HS_ADDFRIEND_REQ);

	// 调用lua函数，n表示参数个数，r表示返回结果个数。
	int res = checkLuaCallRes(lua_pcall(L, 2, 1, 0), L, "reqAddFriend");

	//g_gameMonitorHS.setGameStatus((WJAM_RES_OK == res) ? E_STATUS_HS_ADDFRIEND_REQ : E_STATUS_HS_AM_FAIL);
	return res;
	//return 0;
}

DLL_EXPORT int WJAM::acceptFriendReq(const char *_name)
{
	g_log.writeLog(XLog::E_LOG_DEBUG, "调用执行自动化 $BEGIN$：接受好友[%s]", _name);

	HWND hWnd = g_gameMonitorHS.getGameWindowHwnd();
	if (!hWnd)
		return WJAM_RES_WINDOW_NOT_EXIST;

	if (!isLuaVaild())
		return WJAM_RES_LUA_NOT_INIT;
	
	/* 窗口前端显示 */
	BringWindowToTop(hWnd);
	Sleep(200);

	g_gameMonitorHS.clickWindow(hWnd, 0.5, 0.2, 1000);
	/* 通知内嵌窗口 */
	g_gameMonitorHS.setEvent(GAME_EVENT_AM_ACCEPT_FRIEND);


	vector<Rect> vecRt;
	for (int i = 0; i < 3; ++i)
	{
		
		if (g_gameMonitorHS.getFriendRect(_name, vecRt))
		{
			break;
		}
		g_log.writeLog(XLog::E_LOG_DEBUG, "从好友列表中获取好友位置失败 [好友：%s]", _name);
	}

	g_log.writeLog(XLog::E_LOG_DEBUG, "==================================");
	int size = vecRt.size();
	if (size <= 0)
		return WJAM_RES_HS_OPERATE_ERROR;
	g_log.writeLog(XLog::E_LOG_DEBUG, "==================================");

	bool flag = false;
	int res = WJAM_RES_OK;

	Rect rtFriend;
	for (auto iter = vecRt.begin(); iter != vecRt.end(); ++iter)
	{
		if (iter->x < 38)
		{
			rtFriend = *iter;

			/* 在屏幕上点击一次，关闭开始任务页（优化处理方案： 提高图像识别的准确率，通过图片识别判断状态后进行点击操作） */
			RECT rt;
			::GetWindowRect(hWnd, &rt);
			AMUtil::mouseClickLButton1(hWnd, rt.left + (rt.right - rt.left) * 0.5, rt.top + (rt.bottom - rt.top) * 0.2);


			/* 显示好友列表窗口 */
			g_gameMonitorHS.showFriendWindow(hWnd);
			Sleep(200);
			g_log.writeLog(XLog::E_LOG_DEBUG, "==================================");
			lua_State *L = g_luaEngine->GetLuaState();
			lua_getglobal(L, "acceptFriendReq");
			lua_pushinteger(L, (int)hWnd);
			lua_pushinteger(L, rtFriend.y + rtFriend.height);
			res = checkLuaCallRes(lua_pcall(L, 2, 1, 0), L, "acceptFriendReq");
		}
		else
		{
			g_log.writeLog(XLog::E_LOG_DEBUG, "添加好友位置异常：x=%d", iter->x);
			flag = true;
		}
		flag = true;
	}

	if (!flag)
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "好友位置获取错误 [好友：%s]", _name);
		res = WJAM_RES_HS_OPERATE_ERROR;
	}

	/* 设置自动化执行状态 */
	//g_gameMonitorHS.setGameStatus((WJAM_RES_OK == res) ? E_STATUS_HS_ADDFRIEND_ACCEPT : E_STATUS_HS_AM_FAIL);

	g_log.writeLog(XLog::E_LOG_DEBUG, "自动化：END 接受好友, res=%d ", res);
	return res;
}

DLL_EXPORT int WJAM::reqChallenge(const char *_name)
{
	g_log.writeLog(XLog::E_LOG_DEBUG, "调用执行自动化 $BEGIN$：发起挑战请求");

	HWND hWnd = g_gameMonitorHS.getGameWindowHwnd();
	if (!hWnd)
		return WJAM_RES_WINDOW_NOT_EXIST;

	if (!isLuaVaild())
		return WJAM_RES_LUA_NOT_INIT;
	
	/* 通知内嵌窗口 */
	g_gameMonitorHS.setEvent(GAME_EVENT_AM_REQ_CHALLENGE);

	/* 窗口前端显示 */
	BringWindowToTop(hWnd);
	Sleep(200);

	vector<Rect> vecRt;
	for (int i = 0; i < 3; ++i)
	{
		if (g_gameMonitorHS.getFriendRect(_name, vecRt))
		{
			break;
		}
		g_log.writeLog(XLog::E_LOG_DEBUG, "从好友列表中获取好友位置失败 [好友：%s]", _name);
	}

	int size = vecRt.size();
	if (size <= 0)
		return WJAM_RES_HS_OPERATE_ERROR;

	bool flag = false;
	Rect rt;
	for (auto iter = vecRt.begin(); iter != vecRt.end(); ++iter)
	{
		rt = *iter;
		flag = true;
		break;
	}

	if (!flag)
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "好友位置获取错误 [好友：%s]", _name);
		return WJAM_RES_HS_OPERATE_ERROR;
	}

	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "reqChallenge");
	lua_pushinteger(L, (int)hWnd);	// 窗口句柄
	//lua_pushstring(L, _name);		// 
	lua_pushinteger(L, g_gameMonitorHS.m_rectFriendWindow.width*0.82);
	lua_pushinteger(L, rt.y + rt.height*0.5);

	int res = checkLuaCallRes(lua_pcall(L, 3, 1, 0), L, "reqChallenge");
	return res;
}


DLL_EXPORT int WJAM::acceptChallenge()
{
	HWND hWnd = g_gameMonitorHS.getGameWindowHwnd();
	if (!hWnd)
		return WJAM_RES_WINDOW_NOT_EXIST;

	if (!isLuaVaild())
		return WJAM_RES_LUA_NOT_INIT;

	g_log.writeLog(XLog::E_LOG_DEBUG, "调用执行自动化 $BEGIN$：接受挑战");

	/* 窗口前端显示 */
	BringWindowToTop(hWnd);
	Sleep(200);

	int w = g_gameMonitorHS.m_width;
	int h = g_gameMonitorHS.m_height;
	g_gameMonitorHS.clickWindow(hWnd, w*0.5 - h*0.54*0.168, h*0.66);
	
	//lua_State *L = g_luaEngine->GetLuaState();
	//lua_getglobal(L, "acceptChallenge");
	//lua_pushinteger(L, (int)hWnd);
	//
	//
	//int res = checkLuaCallRes(lua_pcall(L, 1, 1, 0), L, "acceptChallenge");
	//g_gameMonitorHS.setGameStatus((WJAM_RES_OK == res) ? E_STATUS_HS_CHALLENGE_ACCEPT : E_STATUS_HS_AM_FAIL);
	//if (WJAM_RES_OK == res)
	{
		/* 通知内嵌窗口, 设置进度条 */
		g_gameMonitorHS.setEvent(GAME_EVENT_AM_ACCEPT_CHALLENGE);
	}
	return WJAM_RES_OK;
}


DLL_EXPORT int WJAM::getGameStatusHS()
{
	return g_gameMonitorHS.getGameStatus();
}


DLL_EXPORT int WJAM::takeEvent()
{
	return g_gameMonitorHS.takeEvent();
}
DLL_EXPORT void WJAM::setGameStatusHS(const EnGameStatusHS& _status)
{
	g_gameMonitorHS.setGameStatus(_status);
}


//初始化炉石识别英雄
DLL_EXPORT int WJAM::InitKnowHeroLS(std::string &modelFile, std::string &deployFile, std::string &meanFile, std::string &labelFile)
{
	return g_gameMonitorHS.InitKnowHeroLS(modelFile, deployFile, meanFile, labelFile);
}

//炉石识别英雄
DLL_EXPORT void WJAM::SetKnowHeroLSCallBack(Callback_KnowHeroLS* pCallback)
{
	g_gameMonitorHS.SetKnowHeroLSCallBack(pCallback);
}