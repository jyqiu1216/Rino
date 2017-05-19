#ifndef _LuaInterface_H_
#define _LuaInterface_H_


#include "common/LuaEngine.h"
#include <Windows.h>

HWND GetWindowHwnd(const char* _title);
int Inf_MoveWindow(lua_State *L);
int Inf_MouseClickLButton(lua_State *L);
int Inf_MouseClickLButton1(lua_State *L);

int Inf_Input(lua_State *L);
int Inf_Backspace(lua_State *L);
int Inf_Sleep(lua_State *L);
int Inf_IsWindowExist(lua_State *L);
int Inf_ActiveWindow(lua_State* L);
int Inf_GetWindowHwnd(lua_State *L);
int Inf_GetWindowPixelRGB(lua_State *L);
int Inf_Abs(lua_State *L);
int Inf_IsWindowExist(lua_State *L);
int Lua_SetWindowVisibleByClassName(lua_State* L);
int Inf_SetWindowVisibleByTitle(lua_State* L);
int Lua_InjectDll(lua_State *L);
int Lua_SetWindowAlpha(lua_State *L);
int Lua_ComparePixel(lua_State *L);
int Inf_SetWindowVisible(lua_State *L);
int Inf_PressKey(lua_State *L);


#endif // !_LuaInterface_H_
