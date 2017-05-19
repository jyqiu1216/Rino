//
//  macrodefinition.h
//  MusicalNotation
//
//  Created by mac on 16-1-26.
//  Copyright (c) 2016年 zhengbensuyuan. All rights reserved.
//

#ifndef _GAMEINFO_H_
#define _GAMEINFO_H_
#include <windows.h>

enum EInstructionsType {
	EInstruction_HA,		//心跳指令	-------
	EInstruction_Exit,		//发送退出指令-----
	EInstruction_ReadMemory	//读取内存指令-----
};

enum EDataType {
	BYTE_TYPE = 0,
	UBYTE_TYPE = 1,
	SHORT_TYPE = 2,
	USHORT_TYPE = 3,
	INT_TYPE = 4,
	UINT_TYPE = 5,
	FLOAT_TYPE = 6,
	DOUBLE_TYPE = 7,
	INT64_TYPE = 8,
	UINT64_TYPE = 9,
	STRING_TYPE = 10,			//string类型，大小不得超过256个字符
	ADDRESS_TYPE = 11			//返回地址类型
};

struct SInstructionData {
	EDataType dataType;		//数据类型
	void*	dataAddress;	//数据的地址
	union _data
	{
		char	byteData;
		short	shortData;
		int		intData;
		float	floatData;
		long long	int64Data;
		double	doubleData;
		char	stringData[64];
		void*	ptrData;
	}data;					//数据内容

};

/************************************************************************/
/* 本次测试10条地址信息，该记录                                          */
/************************************************************************/
//指令结构体
#define MAX_INSTRUCTIONS_NUMBERS	256
struct SInstructions
{
	EInstructionsType instructionsType;
	int	instructionsNumbers = 10;
	SInstructionData  dataArray[MAX_INSTRUCTIONS_NUMBERS];
};




/************************************************************************/
/* 查询系统函数                                                                     */
/************************************************************************/
typedef struct _SGameMemoryValue {

}GameMemoryValue;

/************************************************************************/
/* LOL 玩家天生技能                             */
/************************************************************************/


#define MAX_STR_LEN		64

struct LOLFUWEN
{
	char fuwen1[MAX_STR_LEN];
	char fuwen2[MAX_STR_LEN];
};
struct BackpackLOL
{
	char item1[MAX_STR_LEN];
	char item2[MAX_STR_LEN];
	char item3[MAX_STR_LEN];
	char item4[MAX_STR_LEN];
	char item5[MAX_STR_LEN];
	char item6[MAX_STR_LEN];
};

struct TeamData
{
	int teamKills;
	int teamDeads;
	int teamAsstants;
	int desTa;
	int killBigDragon;
	int killLittleDragon;
	int killXiaoB;
	int momeny;
};

struct SummonerSpells
{
	char spell1[MAX_STR_LEN];
	char spell2[MAX_STR_LEN];
};



struct GameInfoLOL 
{

	DWORD gameCode;					// 游戏编号
	char userName[MAX_STR_LEN];		// 用户名 ---
	char roleName[MAX_STR_LEN];		// 英雄名 ---	
	int	 gold;						// 用户当前金钱 
	int	 gameResult;				// 游戏结果   0: 初始化, 1: 胜利, 2: 失败, 3: 放弃
	BackpackLOL backPack;			// 背包装备信息
	SummonerSpells summonerSpell;	// 召唤师技能
	int  level;
	int  killNum;			// 击杀	---条件	
	int  deathNum;			// 死亡 ---条件
	int  assistsNum;		// 助攻
	int  killForceNum;		// 补兵数
	DWORD  dwKillTower1;		// 房主_推塔;
	DWORD  dwKillTower2;		// 非房主_推塔;
	int  bigDragon;			// 大龙;
	int  littleDragon;		// 小龙;
	float timeStart;			// 游戏时长（LOL游戏中右上角的时间）
	GameInfoLOL()
	{
		memset(this, 0, sizeof(GameInfoLOL));
	}

};


#endif /*_GAMEINFO_H_*/
