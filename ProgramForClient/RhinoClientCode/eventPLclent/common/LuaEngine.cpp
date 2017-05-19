//#include "stdafx.h"

#include <iostream>
#include "LuaEngine.h"


using namespace std;
bool popTable(lua_State* L, int idx, LuaDataMap* datamap_ptr);
LuaData* getLuaData(lua_State* L, int _index);



CLuaEngine::CLuaEngine() :m_pLState(nullptr)
{
	m_pLState = luaL_newstate();
	luaL_openlibs(m_pLState);
	loadExtendLuaFile();
}

CLuaEngine::~CLuaEngine()
{
	if (m_pLState)
	{
		lua_close(m_pLState);
		m_pLState = nullptr;
	}
}

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
		luaL_dofile(L, luaFile);

	}
	/* 返回返回值的个数 */
	return 0;
}


void CLuaEngine::loadExtendLuaFile()
{
	//扩展lua函数，实现加载新 lua文件
	lua_register(m_pLState, "lua_import", lua_import);
}



bool CLuaEngine::loadLuaFile(std::string _luaFile)
{
	int iResult = 0;
	//iResult = luaL_dofile(m_pLState, _luaFile.c_str()); 

	int ia = luaL_loadfile(m_pLState, _luaFile.c_str());
	int ib = lua_pcall(m_pLState, 0, LUA_MULTRET, 0);
	iResult = ia || ib;

	if (iResult != 0)
		return false;
	return true;
}

std::string CLuaEngine::getString(std::string _paramName)
{
	lua_getglobal(m_pLState, _paramName.c_str());   //获得Lua中变量的值，将其放入栈中    

	if (!lua_isstring(m_pLState, -1))
		luaL_error(m_pLState, "%s should be a string\n", _paramName.c_str());
	return (std::string)lua_tostring(m_pLState, -1);    //将栈顶元素转化为数字    

}
std::string CLuaEngine::getString(std::string _tableName, std::string _paramName)
{

	lua_getglobal(m_pLState, _tableName.c_str());
	lua_pushstring(m_pLState, _paramName.c_str());//首先把元素的名字压入栈顶    

	lua_gettable(m_pLState, -2);//值会放在栈顶，同时刚才压入的元素名字被弹出 
	if (!lua_isstring(m_pLState, -1))
		luaL_error(m_pLState, "%s should be a string\n",_paramName.c_str());

	std::string reStr = (std::string)lua_tostring(m_pLState, -1);    //将栈顶元素转化为数字   
	lua_pop(m_pLState, 2);
	return reStr;
}


unsigned int	CLuaEngine::getInteger(std::string _paramName)
{
	lua_getglobal(m_pLState, _paramName.c_str());   //获得Lua中变量的值，将其放入栈中    

	if (!lua_isstring(m_pLState, -1))
		luaL_error(m_pLState, "%s should be a number\n", _paramName.c_str());
	return lua_tointeger(m_pLState, -1);    //将栈顶元素转化为数字    

}
unsigned int	CLuaEngine::getInteger(std::string _tableName, std::string _paramName)
{
	lua_getglobal(m_pLState, _tableName.c_str());
	lua_pushstring(m_pLState, _paramName.c_str());//首先把元素的名字压入栈顶    

	lua_gettable(m_pLState, -2);//值会放在栈顶，同时刚才压入的元素名字被弹出 
	if (!lua_isinteger(m_pLState, -1))
		luaL_error(m_pLState, "%s should be a integer\n", _paramName.c_str());

	int iResult = (int)lua_tointeger(m_pLState, -1);    //将栈顶元素转化为数字   
	lua_pop(m_pLState, 2);
	return iResult;
}

double	CLuaEngine::getDouble(std::string _paramName)
{

	lua_getglobal(m_pLState, _paramName.c_str());   //获得Lua中变量的值，将其放入栈中    

	if (!lua_isstring(m_pLState, -1))
		luaL_error(m_pLState, "%s should be a double\n", _paramName.c_str());
	return lua_tonumber(m_pLState, -1);    //将栈顶元素转化为数字    

}
double	CLuaEngine::getDouble(std::string _tableName, std::string _paramName)
{
	lua_getglobal(m_pLState, _tableName.c_str());
	lua_pushstring(m_pLState, _paramName.c_str());//首先把元素的名字压入栈顶    

	lua_gettable(m_pLState, -2);//值会放在栈顶，同时刚才压入的元素名字被弹出 
	if (!lua_isnumber(m_pLState, -1))
		luaL_error(m_pLState, "%s should be a integer\n", _paramName.c_str());

	double iResult = (double)lua_tonumber(m_pLState, -1);    //将栈顶元素转化为数字   
	lua_pop(m_pLState, 2);

	return iResult;
}

LuaDataMap* CLuaEngine::getTable(std::string _paramName)
{
	LuaDataMap* datamap_ptr = new LuaDataMap;

	lua_getglobal(m_pLState, _paramName.c_str());
	int type = lua_type(m_pLState, -1);
	if (type == LUA_TTABLE){
		int index = lua_gettop(m_pLState);
		if (popTable(m_pLState, index, datamap_ptr)){

		}
		else{
			//cout << "Error" << endl;

		}
	}
	return datamap_ptr; 
}

LuaDataMap* CLuaEngine::getTable(lua_State* L, int idx)
{
	LuaDataMap* datamap_ptr = new LuaDataMap;

	int type = lua_type(L, idx);
	if (type == LUA_TTABLE) {
		if (popTable(L, idx, datamap_ptr)) {

		}
		else {
			//cout << "Error" << endl;

		}
	}
	return datamap_ptr;
}

LuaData* getLuaData(lua_State* L, int _index)
{
	LuaData* pLuaData = new LuaData;

	if (lua_isinteger(L, _index))
	{
		pLuaData->type = Lua_int;
		pLuaData->value.value_int = (int)lua_tonumber(L, _index);		 
		return pLuaData;
	}
	else if (lua_isnumber(L, _index))
	{
		pLuaData->type = Lua_number;
		pLuaData->value.value_double = (double)lua_tonumber(L, _index);
		return pLuaData;
	}
	else if (lua_isstring(L, _index))
	{
		pLuaData->type = Lua_string;
		pLuaData->value.value_str = new std::string(lua_tostring(L, _index));
		return pLuaData;
	}
	else if (lua_istable(L, _index))
	{
		pLuaData->type = Lua_table;
		pLuaData->value.value_map = new LuaDataMap();

		return pLuaData;
	}
	return pLuaData;
}
void luadataprint(LuaData* data)
{
	switch (data->type) {
	case Lua_int:
		printf("value is %d \n", data->value.value_int);
		break;

	case Lua_number:
		printf("value is %f \n", data->value.value_double);
		break;

	case Lua_string:
		printf("value is %s \n", ((std::string*)data->value.value_data)->c_str());
		break;

	default:
		break;
	}
}

bool popTable(lua_State* L, int idx, LuaDataMap* datamap_ptr)
{
	try{
		lua_pushnil(L);
		while (lua_next(L, idx) != 0){
			int keyType = lua_type(L, -2);
			std::string keyName;
			if (keyType == LUA_TNUMBER){
				int value = lua_tonumber(L, -2);
				char buf[32] = {};
				_itoa_s(lua_tointeger(L, -2), buf, 10);
				keyName = buf;
				//cout << "Key:" << value << endl;
			}
			else if (keyType == LUA_TSTRING){
				const char*  value = lua_tostring(L, -2);
				keyName = value;
				//cout << "Key:" << value << endl;
			}
			else{
				//cout << "Invalid key type: " << keyType << endl;
				continue;
			}


			LuaData* pLuaData = getLuaData(L, -1);
			pLuaData->name = keyName;
			if (pLuaData->type == Lua_table)
			{
				datamap_ptr->insert(LuaDataMap::value_type(keyName, pLuaData));
				//cout << "====sub table===" << endl;
				int index = lua_gettop(L);
				if (!popTable(L, index, (LuaDataMap*)pLuaData->value.value_map)) {
					cout << "popTable error in  popTable,error occured" << endl;
					return false;
				}
			}
			else if(pLuaData->type == Lua_string)
			{
				 
				datamap_ptr->insert(LuaDataMap::value_type(keyName, pLuaData));
			}
			else if (pLuaData->type == Lua_int)
			{
				datamap_ptr->insert(LuaDataMap::value_type(keyName, pLuaData));
			}

			lua_pop(L, 1);
		}
	}
	catch (const char* s){
		string errMsg = s;
		lua_pop(L, 1);
		cout << errMsg << endl;
		return false;
	}
	catch (std::exception& e){
		const char* errMsg = e.what();
		lua_pop(L, 1);
		cout << errMsg << endl;
		return false;
	}
	catch (...){
		const char* errMsg = lua_tostring(L, -1);
		lua_pop(L, 1);
		cout << errMsg << endl;
		return false;
	}
	return true;
}
