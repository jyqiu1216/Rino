
#include "GameMonitorHS.h"

#include "LuaInterfaceHS.h"

extern GameMonitorHS g_gameMonitorHS;


int Inf_GetHS_FriendWindowRect(lua_State* L)
{
	Rect& rt = g_gameMonitorHS.getFriendWindowRect();

	lua_pushinteger(L, rt.x);
	lua_pushinteger(L, rt.y);
	lua_pushinteger(L, rt.width);
	lua_pushinteger(L, rt.height);

	return 4;	/* 返回4个参数 */
}


int Inf_GetHS_WindowRect(lua_State* L)
{
	RECT rt;
	if (g_gameMonitorHS.getWindowRect(rt))
	{
		lua_pushinteger(L, rt.left);
		lua_pushinteger(L, rt.right);
		lua_pushinteger(L, rt.right - rt.left);
		lua_pushinteger(L, rt.bottom - rt.top);
	}
	else
	{
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
		lua_pushinteger(L, 0);
	}

	return 4;	/* 返回4个参数 */
}




int lua_TestDebug(lua_State* L)
{
	if (1 == lua_gettop(L)) {

		string strData = lua_tostring(L, 1);

		if (strData.length() > 0) {

			//::OutputDebugStringA(strData.c_str());
			::MessageBoxA(NULL, strData.c_str(), NULL, 0);
		}
	}

	return 0;
}

///**
//* \brief:	图像对比
//*/
//int Inf_ImageContrastRect(lua_State* L)
//{
//	const char* image = lua_tostring(L, 1);
//	float x = lua_tonumber(L, 2);
//	float y = lua_tonumber(L, 3);
//	float w = lua_tonumber(L, 4);
//	float h = lua_tonumber(L, 5);
//
//	bool res = g_gameMonitorHS.imageContrastRect(image, x, y, w, h);
//
//	lua_pushnumber(L, (int)res);
//	return 1;
//}

//
///**
//* \brief:	图像识别
//*/
//int Inf_ImageRecognition(lua_State* L)
//{
//	//const char* image = lua_tostring(L, 1);
//	char* res = g_gameMonitorHS.imageRecognition();
//	lua_pushnumber(L, (int)res);
//	return 1;
//}
