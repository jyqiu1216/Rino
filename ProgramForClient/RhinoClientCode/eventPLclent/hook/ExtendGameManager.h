#ifndef _WJ_WJExtendManager_H_
#define _WJ_WJExtendManager_H_

#include "EventSignal.h"
#include "GameInfo.h"
#include <QtCore>



static std::string utf8ToStr(const std::string& _utf8)
{
	static char sBuffer[1024];
	static wchar_t sBufferTemp[1024];

	ZeroMemory(sBufferTemp, ARRAYSIZE(sBufferTemp));
	ZeroMemory(sBuffer, 1024);

	int len = MultiByteToWideChar(CP_UTF8, 0, _utf8.c_str(), -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, _utf8.c_str(), _utf8.length(), sBufferTemp, len);

	int length = WideCharToMultiByte(CP_ACP, 0, sBufferTemp, -1, NULL, NULL, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, sBufferTemp, len, sBuffer, length, NULL, NULL);

	return std::string(sBuffer);
}


extern HWND g_gameWindowHwnd;


// 状态
#define GAME_STATUS_START		0	// 游戏开始
#define GAME_STATUS_FIRSTBLOOD	1	// 一血

#define MSG_USER_GAMEOVER		(WM_USER+1000)		// 游戏结束
#define MSG_USER_FIRSTBLOOD		(WM_USER+1002)		// 一血
#define MSG_USER_USER_QUIT		(WM_USER+1005)		// 玩家退出游戏

#include <fstream>


class ExtendGameManager : public EventSignal, public IDataRecive
{
public:
	ExtendGameManager() : m_isInjected(false)
	{
		//g_fsLog.open("WJHookTest.log", std::ios::out);
		memset(&m_playerInfoLOL, 0, sizeof(m_playerInfoLOL));
		m_playerInfoLOL.gameCode = -1;
	}

public:
	virtual void DoData(void* pData, size_t size)
	{
		if (pData)
		{
			memcpy(&m_playerInfoLOL, pData, sizeof(m_playerInfoLOL));
			//printf(m_playerInfoLOL.userName);

			if (!m_isInjected)
			{
				printf("注入成功");
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("注入成功.");
				//SystemLog::getInstance()->writeLog(SystemLog::E_LOG_DEBUG, "WJTestLOG.log", "1", 1, "注入成功.");
				m_isInjected = true;
			}

			if (::strcmp(m_playerInfoLOL.userName, "") != 0)
			{
				printf("注入成功");
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("注入成功.");
				std::string name = utf8ToStr(m_playerInfoLOL.userName);
			}

			//if (m_playerInfoLOL.killNum > 0 || m_playerInfoLOL.deathNum> 0)
			//{
			//	static int flag = 0;
			//	if (0 == flag)
			//	{
			//		printf("FIRST BLOOD");
			//		flag++;
			//	}
			//}
			if (m_playerInfoLOL.killNum > 0)
			{
				printf("m_playerInfoLOL.killNum:%d", m_playerInfoLOL.killNum);
				static int flag = 0;
				if (0 == flag)
				{
					printf("击杀");
					flag++;
				}
				
			}
			if (m_playerInfoLOL.gameCode>0)
			{
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("游戏编号:") << m_playerInfoLOL.gameCode;
			}
			if (m_playerInfoLOL.gameResult>0)
			{
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("游戏结果:") << m_playerInfoLOL.gameResult;
			}
			if (::strcmp(m_playerInfoLOL.userName, "") != 0)
			{
				static int flag = 0;
				//if (0 == flag)
				//{
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("用户名:") << QString::fromUtf8(m_playerInfoLOL.userName);
				std::string name = utf8ToStr(m_playerInfoLOL.userName);
				//	flag++;
				//}
			}
			if (::strcmp(m_playerInfoLOL.roleName, "") != 0)
			{
				static int flag = 0;
				//if (0 == flag)
				//{
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("英雄名:") << QString::fromLocal8Bit(m_playerInfoLOL.roleName);
				//	flag++;
				//}
			}
			if(m_playerInfoLOL.killNum < 0)
			{
				printf("m_playerInfoLOL.killNum:%d", m_playerInfoLOL.killNum);
				static int flag = 0;
				if (0 == flag)
				{
					printf("-击杀");
					flag++;
				}
			}
			if (m_playerInfoLOL.deathNum > 0)
			{
				printf("m_playerInfoLOL.deathNum:%d", m_playerInfoLOL.deathNum);
				static int flag = 0;
				if (0 == flag)
				{
					printf("被杀死亡");
					flag++;
				}
			}
			if (m_playerInfoLOL.deathNum < 0)
			{
				printf("m_playerInfoLOL.deathNum:%d", m_playerInfoLOL.deathNum);
				static int flag = 0;
				if (0 == flag)
				{
					printf("-被杀死亡");
					flag++;
				}
			}
	/*		if (m_playerInfoLOL.dwKillTower > 0 )
			{
				printf("m_playerInfoLOL.dwKillTower:%d", m_playerInfoLOL.dwKillTower);
				static int flag = 0;
				if (0 == flag)
				{
					printf("推塔成功");
					flag++;
				}
			}
			if (m_playerInfoLOL.dwKillTower < 0)
			{
				printf("m_playerInfoLOL.dwKillTower:%d", m_playerInfoLOL.dwKillTower);
				static int flag = 0;
				if (0 == flag)
				{
					printf("-推塔成功");
					flag++;
				}
			}*/
			if (m_playerInfoLOL.bigDragon > 0)
			{
				printf("m_playerInfoLOL.bigDragon:%d", m_playerInfoLOL.bigDragon);
				static int flag = 0;
				if (0 == flag)
				{
					printf("杀大龙成功");
					flag++;
				}
			}
			if (m_playerInfoLOL.bigDragon < 0)
			{
				printf("m_playerInfoLOL.bigDragon:%d", m_playerInfoLOL.bigDragon);
				static int flag = 0;
				if (0 == flag)
				{
					printf("杀大龙成功");
					flag++;
				}
			}
			if (m_playerInfoLOL.littleDragon > 0)
			{
				printf("m_playerInfoLOL.littleDragon:%d", m_playerInfoLOL.littleDragon);
				static int flag = 0;
				if (0 == flag)
				{
					printf("杀小龙成功");
					flag++;
				}
			}
			if (m_playerInfoLOL.littleDragon < 0)
			{
				printf("m_playerInfoLOL.littleDragon:%d", m_playerInfoLOL.littleDragon);
				static int flag = 0;
				if (0 == flag)
				{
					printf("杀小龙成功");
					flag++;
				}
			}
			if (m_playerInfoLOL.assistsNum > 0)
			{
				printf("m_playerInfoLOL.assistsNum:%d", m_playerInfoLOL.assistsNum);
				static int flag = 0;
				if (0 == flag)
				{
					printf("助攻");
					flag++;
				}
			}
			if (m_playerInfoLOL.assistsNum < 0)
			{
				printf("m_playerInfoLOL.assistsNum:%d", m_playerInfoLOL.assistsNum);
				static int flag = 0;
				if (0 == flag)
				{
					printf("助攻");
					flag++;
				}
			}
			/*
			个人SOLO
			率先推掉敌方一塔的个人获胜
			补刀先达到100刀的个人获胜
			10分钟以内获得金钱最多的个人获胜

			组队
			率先获得第一条大龙的队伍获胜
			率先推掉对方基地的队伍获胜
			率先推掉敌方4座防御塔的队伍获胜
			队伍中击杀英雄总数率先达到15个的队伍获胜
			*/
		}
	}


	/**
	* @date:	2016/08/21
	* @brief:	向共享内存中写入数据
	* @param:	[pData]	共享内存地址
	* @param:	[maxSize]	数据区大小
	* @usage:
	*/
	virtual void SendData(void* pData, size_t maxSize)
	{
	}


public:
	GameInfoLOL& getGameInfo()
	{
		return m_playerInfoLOL;
	}

	bool getInjectStatus() { return m_isInjected; }

private:
	GameInfoLOL m_playerInfoLOL;

	bool m_isInjected;
};

#endif // !_WJ_WJExtendManager_H_
