#ifndef _LUAENGINE_luaengine_H_
#define _LUAENGINE_luaengine_H_

#include "lua.hpp" 
#include <vector>
#include <string>
#include <map>
enum ELuaType
{
	Lua_nil,
	Lua_bool,
	Lua_int,            //int
	Lua_number,         //double
	Lua_string,         //string
	Lua_array,          //array  LuaData*
	Lua_table           //          LuaDataMap
};

typedef struct _SLuaInfo LuaData;
typedef std::map<std::string, _SLuaInfo*> LuaDataMap;

struct _SLuaInfo{
	std::string name;
	ELuaType type;
	union  {
		int value_int;
		double value_double;
		std::string* value_str;
		LuaDataMap* value_map;
		void* value_data;
	}value;

};

//typedef std::map<std::string, _SLuaInfo*> LuaDataMap;

class CLuaEngine 
{
public:
	CLuaEngine();
	~CLuaEngine();

	//extend luaEngine api
	void loadExtendLuaFile();

	
	bool loadLuaFile(std::string _luaFile);

	std::string getString(std::string _paramName);
	std::string getString(std::string _tableName, std::string _paramName);


	unsigned int	getInteger(std::string _paramName);
	unsigned int	getInteger(std::string _tableName, std::string _paramName);

	double	getDouble(std::string _paramName);
	double	getDouble(std::string _tableName, std::string _paramName);

	LuaDataMap* getTable(std::string _paramName);
	LuaDataMap* getTable(lua_State* L, int idx);
	
	lua_State*	GetLuaState(){ return m_pLState; };
protected:
	lua_State * m_pLState;

};



#endif /*_SYSTEMLUA_systemlua_H_*/