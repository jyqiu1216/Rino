#ifndef _WJ_AutomateManager_H_
#define _WJ_AutomateManager_H_



#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>

#include "WJAutomation.h"
//#include "common/Common.h"

#ifdef _DEBUG
#pragma comment(lib, "WJAutomation_d.lib")
#else
#pragma comment(lib, "WJAutomation.lib")
#endif


#define AUTOMATE_START_PROGRAM		0x0001	/* 启动游戏 */
#define AUTOMATE_CREATE_ROOM		0x0002	/* 创建房间 */
#define AUTOMATE_JOIN_ROOM			0x0003	/* 加入房间 */


typedef	std::map<int, std::string>	MAP_IntStr;

class AutomateManager
{
public:
	static bool init(const char* _luaFile);


	/**
	* @date:	2016/08/17
	* @brief:	执行自动化脚本：登陆
	* @param:	[_loginInfo]:	登陆信息
	* @param:	[_pFunc]:	回调函数
	* @return:	[int]	0: 脚本执行成功; >0: 脚本执行失败;
	* @usage:	
	*/
	static int executeLogin(WJAM::LOGININFO& _loginInfo, PFuncCallBack _pFunc);

	/**
	* @date:	2016/08/17
	* @brief:	执行自动化脚本：创建房间
	* @param:	[_roomInfo]:	房间信息
	* @param:	[_pFunc]:	回调函数
	* @return:	[int]	0: 脚本执行成功; >0: 脚本执行失败;
	* @usage:
	*/
	static int executeCreateRoom(WJAM::ROOMINFO& _roomInfo, PFuncCallBack _pFunc);

	/**
	* @date:	2016/08/17
	* @brief:	执行自动化脚本：加入房间
	* @param:	[_gameId]:	房间信息
	* @param:	[_pFunc]:	回调函数
	* @return:	[int]	0: 脚本执行成功; >0: 脚本执行失败;
	* @usage:
	*/
	static int executeJoinRoom(WJAM::ROOMINFO& _roomInfo, PFuncCallBack _pFunc);



	static int executeStartGame(WJAM::GAME_ID _gameId, bool _isRoomOwner);

	//初始化炉石识别英雄
	static int InitKnowHeroLS(std::string &modelFile, std::string &deployFile, std::string &meanFile, std::string &labelFile);

	//炉石识别英雄
	static void SetKnowHeroLSCallBack(Callback_KnowHeroLS* pCallback);
private:
	/**
	* @date:	2016/08/16
	* @brief:	执行自动化脚本
	* @param:	[_gameId]:	游戏ID
	* @param:	[_cmd]:		操作指令
	* @param:	[_pFunc]:	回调函数
	* @return:	[bool]	true: 调用成功;	false: 调用失败
	* @usage:
	*/
	static bool execute(int _gameId, int _cmd, PFuncCallBack _pFunc);


private:
	
};





#endif // !_WJ_AutomateManager_H_
