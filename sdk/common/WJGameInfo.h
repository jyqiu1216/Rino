#pragma once
#include "common/WJBaseType.h"


enum E_GameID
{
	e_gameid_unknow = 0,	//未知游戏
	e_gameid_10000001 = 1,	//英雄联盟
	e_gameid_10000002 = 2,	//炉石传说
};

enum class MatchType
{
	LOLMission,
	LOLSolo,
	LOLTeam,
	HSMission,
	HSSolo,
};

static char g_Exe_Name_BattleNet[MAX_PATH] = { "Battle.net.exe" };//战网进程名字
static char g_Exe_Name_Hearthstone[MAX_PATH] = { "Hearthstone.exe" };//炉石进程名字

static char g_Wind_Name_BattleNet[MAX_PATH] = { "暴雪游戏平台" };//战网窗口名字
static char g_Wind_Name_Hearthstone[MAX_PATH] = { "炉石传说" };//炉石窗口名字




#ifdef WJ_CLIENT
///////////////////////////////////////////////////////////////
#include "clientsocket/WJCInterface.h"
//添加游戏

/*英雄联盟*/
#include "10000001_lol/MyGameDeskData_10000001.h"
#include "10000001_lol/client/MyDataDrocessing_10000001.h"
#include "10000001_lol/client/MyGameHall_10000001.h"
#include "10000001_lol/client/MyGameRoom_10000001.h"

/*炉石传说*/
#include "10000002_hearth_stone/MyGameDeskData_10000002.h"
#include "10000002_hearth_stone/client/MyDataDrocessing_10000002.h"
#include "10000002_hearth_stone/client/MyGameHall_10000002.h"
#include "10000002_hearth_stone/client/MyGameRoom_10000002.h"
//////////////////////////////////////////////////////////////
#endif // WJ_CLIENT