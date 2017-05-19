
#include "system_lua.h"

  
int lua_import(lua_State *L)
{
	/* 得到参数个数 */
	int n = lua_gettop(L);
	double sum = 0;
	int i;

	/* 循环求参数之和  二期加入lua table 进行文件判断，防止多次导入！！！！！！！！！！！！！ */
	
	for (i = 1; i <= n; i++)
	{
		/* 求和 */
		const char* luaFile = lua_tostring(L, i);
		luaL_dofile(L,luaFile);
		 
	}
	/* 返回返回值的个数 */
	return 0;
}
