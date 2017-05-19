// LOLExtend.cpp : 定义 DLL 应用程序的导出函数。
//
#include "LOLExtend.h"
#include <Windows.h>
#include <exception>
#include <thread>  
#include "lua.hpp"

#define WM_LOL_MESSAGE	WM_USER+2023					//LOL退出游戏消息
//#define 		WM_GAME_MESS_2023					//LOL退出游戏消息转发
#define OFFSET(a,b) IsBadReadPtr((void*)a,4)? 0 : *((int*)a) + b

extern GameInfoLOL g_info;
CLOLExtend extend;
bool isRead = true;	//solo完成结束条件后不再获取逆向数据
template <typename T>

T GetValue(T* a)
{
	if (IsBadReadPtr(a, 4) != 0)
	{
		OutputDebugStringA("此地址不可读");
		return 0;
	}
	return *a;
}

//利用lua脚步获取地址
DWORD GetValue_lua(lua_State* plua_State, char* s)
{
	DWORD res = 0;
	if (nullptr == s || nullptr == plua_State)
	{
		return 0;
	}

	// 游戏编号	
	lua_getglobal(plua_State, s);    // 获取lua函数f
	if (lua_pcall(plua_State, 0, -1, 0) != 0)
	{
		char str[MAX_PATH] = { 0 };
		sprintf_s(str, "sdp_test error running function '%s': %s", s, lua_tostring(plua_State, -1));
		OutputDebugStringA(str);
	}

	if (!lua_isinteger(plua_State, -1))
	{
		char str[MAX_PATH] = { 0 };
		sprintf_s(str, "function '%s' must return a number", s);
		OutputDebugStringA(str);
	}

	res = lua_tonumber(plua_State, -1);
	lua_pop(plua_State, -1);
	return res;
}

// 这是导出变量的一个示例
LOLEXTEND_API int nLOLExtend = 0;

// 这是导出函数的一个示例。
LOLEXTEND_API int fnLOLExtend(void)
{
	return 42;
}


std::string GetAppPath()
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(GetModuleHandleA("ExtendLOL.dll"), szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[1] = 0; //删除文件名，只获得路径
	std::string ss = szFilePath;
	return ss;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 LOLExtend.h
CLOLExtend::CLOLExtend()
{
	m_hWindow = nullptr;
	return;
}

// 各个基址
const DWORD GameCodeBase = 0x1A04258;	//游戏编号
const DWORD KillTowerBase1 = 0x1A05E7C;	//房主推塔地址
const DWORD KillTowerBase2 = 0x1A01D70;	//非房主推塔地址
const DWORD GameResultBase = 0x1A029CC;	//游戏结果
const DWORD TimeStartBase = 0x36859E8;	//时间对象
const DWORD PlayBase = 0x1A02284;	//玩家角色
const DWORD CenterHudBase = 0x1A00A38;	//信息面板
DWORD g_ExitGameCall = 0x6F0BB0;  //退出游戏

lua_State* L = nullptr;
DWORD g_addr[10][2] = { 0 };

//待Lua调用的C注册函数。
static int fun_offset(lua_State* L)
{
	//检查栈中的参数是否合法，1表示Lua调用时的第一个参数(从左到右)，依此类推。
	//如果Lua代码在调用时传递的参数不为number，该函数将报错并终止程序的执行。
	lua_Integer op1 = luaL_checkinteger(L, 1);
	lua_Integer op2 = luaL_checkinteger(L, 2);

	DWORD addr = OFFSET(op1, op2);

	//将函数的结果压入栈中。如果有多个返回值，可以在这里多次压入栈中。
	lua_pushinteger(L, addr);
	//返回值用于提示该C函数的返回值数量，即压入栈中的返回值数量。
	return 1;
}

int lua_GetGameKey(lua_State* L)
{
	if (lua_gettop(L) == 2) {

		const char* pchAddresss = lua_tostring(L, 1);
		int inAddress = ::strtol(pchAddresss, NULL, 16);
		int inOffset = lua_tointeger(L, 2);

		DWORD dwKey = extend.GetGameKey(inAddress, inOffset);

		lua_pushinteger(L, dwKey);
	}

	return 1;
}

int lua_TestDebug(lua_State* L)
{
	if (1 == lua_gettop(L)) {

		std::string strData = lua_tostring(L, 1);

		if (strData.length() > 0) {

			//::OutputDebugStringA(strData.c_str());
			::MessageBoxA(NULL, strData.c_str(), NULL, 0);
		}
	}

	return 0;
}

void initCoreAddress()
{
	char str[MAX_PATH] = { 0 };
	static bool bInit = false;//读一次lua脚本获取相关参数
	static bool bSuc = false;

	if (!bInit)
	{
		bInit = true;
		////////////////////////////////////////////////////
		//默认数据
		//[1]游戏编号
		g_addr[0][0] = OFFSET(GameCodeBase, 0x70);
		//[2]推塔数1
		g_addr[1][0] = KillTowerBase1;
		//[3]推塔数2
		g_addr[2][0] = KillTowerBase2;
		//[4]游戏结果
		g_addr[3][0] = GameResultBase;
		//[5]开始时间
		g_addr[4][0] = OFFSET(TimeStartBase, 0x18);
		//[6]玩家角色名称
		g_addr[5][0] = PlayBase;
		//[7]击杀数(5c.4.0.0.24)
		g_addr[6][0] = OFFSET(CenterHudBase, 0x5c);
		g_addr[6][0] = OFFSET(g_addr[6][0], 4);
		g_addr[6][0] = OFFSET(g_addr[6][0], 0);
		g_addr[6][0] = OFFSET(g_addr[6][0], 0);
		g_addr[6][0] = OFFSET(g_addr[6][0], 0x24);
		//[8]死亡数(5c.4.0.0.28)
		g_addr[7][0] = OFFSET(CenterHudBase, 0x5c);
		g_addr[7][0] = OFFSET(g_addr[7][0], 4);
		g_addr[7][0] = OFFSET(g_addr[7][0], 0);
		g_addr[7][0] = OFFSET(g_addr[7][0], 0);
		g_addr[7][0] = OFFSET(g_addr[7][0], 0x28);
		//[9]助攻数(5c.4.0.0.2C)
		g_addr[8][0] = OFFSET(CenterHudBase, 0x5c);
		g_addr[8][0] = OFFSET(g_addr[8][0], 4);
		g_addr[8][0] = OFFSET(g_addr[8][0], 0);
		g_addr[8][0] = OFFSET(g_addr[8][0], 0);
		g_addr[8][0] = OFFSET(g_addr[8][0], 0x2c);
		//[10]补刀数(5c.4.0.4.24)
		g_addr[9][0] = OFFSET(CenterHudBase, 0x5c);
		g_addr[9][0] = OFFSET(g_addr[9][0], 4);
		g_addr[9][0] = OFFSET(g_addr[9][0], 0);
		g_addr[9][0] = OFFSET(g_addr[9][0], 4);
		g_addr[9][0] = OFFSET(g_addr[9][0], 0x24);
		/////////////////////////////////////////////////////
		L = luaL_newstate();
		luaL_openlibs(L);

		lua_register(L, "fun_offset", fun_offset);//注册函数
		lua_register(L, "lua_TestDebug", lua_TestDebug);
		lua_register(L, "GetGameKey", lua_GetGameKey);

		std::string s = GetAppPath();
		s += "luaScript\\LOL_Extend.out";
		int iResult = 0;
		iResult = luaL_dofile(L, s.c_str());

		if (iResult != 0)
		{//lua操作失败
			sprintf_s(str, "sdp_test 读取lua失败Path=%s, iResult=%d", s.c_str(), iResult);
			OutputDebugStringA(str);
			return;
		}

		lua_getglobal(L, "FunGetValue_11");

		if (-1 != lua_pcall(L, 0, 1, NULL)) {

			const char* pchExitGame = lua_tostring(L, -1);
			g_ExitGameCall = ::strtol(pchExitGame, NULL, 16);
			lua_pop(L, 1);
		}

		bSuc = true;
	}

	int index = bSuc ? 1 : 0;

	if (bSuc)
	{
		//读取lua后获得最新数据
		//[1]游戏编号
		g_addr[0][1] = GetValue_lua(L, "FunGetValue_1");
		//[2]推塔数1
		g_addr[1][1] = GetValue_lua(L, "FunGetValue_2");
		//[3]推塔数2
		g_addr[2][1] = GetValue_lua(L, "FunGetValue_3");
		//[4]游戏结果
		g_addr[3][1] = GetValue_lua(L, "FunGetValue_4");
		//[5]开始时间
		g_addr[4][1] = GetValue_lua(L, "FunGetValue_5");
		//[6]玩家角色名称
		g_addr[5][1] = GetValue_lua(L, "FunGetValue_6");
		//[7]击杀数(5c.4.0.0.24)
		g_addr[6][1] = GetValue_lua(L, "FunGetValue_7");
		//[8]死亡数(5c.4.0.0.28)
		g_addr[7][1] = GetValue_lua(L, "FunGetValue_8");
		//[9]助攻数(5c.4.0.0.2C)
		g_addr[8][1] = GetValue_lua(L, "FunGetValue_9");
		//[10]补刀数(5c.4.0.4.24)
		g_addr[9][1] = GetValue_lua(L, "FunGetValue_10");
	}

	//[1]游戏编号
	g_info.gameCode = GetValue_lua(L, "FunGetValue_1");// GetValue((DWORD*)g_addr[0][index]);
	//[2]推塔数1
	g_info.dwKillTower1 = GetValue((DWORD*)g_addr[1][index]);//房主
	//[3]推塔数2
	g_info.dwKillTower2 = GetValue((DWORD*)g_addr[2][index]);//非房主
	//[4]游戏结果
	g_info.gameResult = GetValue((int*)g_addr[3][index]);
	//[5]开始时间
	g_info.timeStart = GetValue((float*)g_addr[4][index]);
	//[6]玩家角色名称
	int len = *(int*)(*(DWORD*)g_addr[5][index] + 0x30);
	char* name;
	if (len < 16)
	{
		name = (char*)(*(DWORD*)g_addr[5][index] + 0x20);
	}
	else
	{
		name = (char*)(*(DWORD*)(*(DWORD*)g_addr[5][index] + 0x20));
	}
	memcpy(g_info.userName, name, strlen(name));
	//[7]击杀数(5c.4.0.0.24)
	g_info.killNum = GetValue((int*)g_addr[6][index]);
	//[8]死亡数(5c.4.0.0.28)
	g_info.deathNum = GetValue((int*)g_addr[7][index]);
	//[9]助攻数(5c.4.0.0.2C)
	g_info.assistsNum = GetValue((int*)g_addr[8][index]);
	//[10]补刀数(5c.4.0.4.24)
	g_info.killForceNum = GetValue((int*)g_addr[9][index]);
}

GameInfoLOL& CLOLExtend::getUserInfo()
{

	__try
	{
		initCoreAddress();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{

	}

	return g_info;
}

void CLOLExtend::ExitGame()
{
	__try {

		DWORD dwCall = g_ExitGameCall;

		_asm {

			pushad
				pushfd

				call dword ptr dwCall

				popfd
				popad

				ret
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

	}
}
void CLOLExtend::IsLOLExist()
{

	HWND hWnd = ::FindWindow(NULL, TEXT("League of Legends (TM) Client"));
	if (!hWnd)
	{
		CloseWindow(hWnd);
	}
}
void CLOLExtend::CloseLol()
{
	std::thread _threadLoL(std::bind(&CLOLExtend::IsLOLExist, this));
	_threadLoL.detach();
}

DWORD CLOLExtend::GetGameKey(DWORD dwCall, int inOffset)
{
	DWORD dwRet = 0;

	__try {

		if ((NULL == dwCall) || (NULL == inOffset)) {

			return dwRet;
		}

		__asm {

			pushad
				pushfd

				call dword ptr dwCall
				mov ebx, dword ptr inOffset
				mov eax, dword ptr[eax + ebx]
				mov dword ptr dwRet, eax

				popfd
				popad
		}

	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

		dwRet = 0;
	}

	return dwRet;
}

#define GWL_USERDATA        (-21)
//窗口回调函数
LRESULT CALLBACK WindowProcFunc_CTCPClientSocket_load(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:		//窗口建立消息
	{
		//DWORD iIndex = TlsAlloc();
		//CLOLExtend * pClientSocket = (CLOLExtend *)(((CREATESTRUCT *)lParam)->lpCreateParams);
		//TlsSetValue(iIndex, pClientSocket);
		//::SetWindowLong((HWND)hWnd, GWL_USERDATA, iIndex);
		break;
	}
	case WM_LOL_MESSAGE:		//LOL退出游戏消息转发
	{							
		isRead = false;
		extend.ExitGame();
		Sleep(6000);
		extend.CloseLol();
		break;
	}
	case WM_CLOSE:		//窗口关闭消息
	{
		DestroyWindow((HWND)hWnd);
		break;
	}
	case WM_DESTROY:	//窗口关闭消息
	{
		//DWORD iIndex = ::GetWindowLong((HWND)hWnd, GWL_USERDATA);
		//CLOLExtend * pClientSocket = (CLOLExtend *)::TlsGetValue(iIndex);
		//::TlsFree(iIndex);
		PostQuitMessage(0);
		//ExitProcess(0);
		break;
	}
	}
	return ::DefWindowProc((HWND)hWnd, uMsg, wParam, lParam);
}
