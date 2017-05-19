#pragma once

#include "automation/WJAutomation.h"


#ifdef _DEBUG
#pragma comment(lib, "WJAutomation_d.lib")
#else
#pragma comment(lib, "WJAutomation.lib")
#endif

#include "hook/ExtendGameManager.h"
#include "hook/HookManager.h"





#include "AMTest.h"

#include <Windows.h>

#include <QMessageBox>

#define CMD_TEST				99999	
#define CMD_AUTO				88888
#define CMD_RELOAD_LUA			1

#define CMD_LOL_STARTUP			101
#define CMD_LOL_CREATEROOM		102
#define CMD_LOL_JOINROOM		103
#define CMD_LOL_PLAY			104

#define CMD_HS_STARTUP			201
#define CMD_HS_REQ_FRIEND		202
#define CMD_HS_ACCEPT_FRIEND	203
#define CMD_HS_REQ_CHALLENGE	204
#define CMD_HS_ACCEPT_CHALLENGE	205

#define CMD_HS_ACCEPT_PIPEI		206




//WNDPROC g_wndProcDefault;
//LRESULT CALLBACK WndProcMy(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)//消息的处理程序
//{
//	char buf[32] = { 0 };
//	LRESULT res = g_wndProcDefault(hwnd, message, wParam, lParam);
//	switch (message)
//	{
//	case   WM_PAINT:
//		return 0;
//
//	case WM_LBUTTONDOWN:
//		sprintf(buf, "炉石传说：鼠标左键按下：(%d, %d)", LOWORD(lParam), HIWORD(lParam));
//		OutputDebugStringA(buf);
//		break;
//
//		//case   WM_DESTROY:
//		//	PostQuitMessage(0);
//		//	return 0;
//	}
//
//	return res;
//}

HWND GetWindowHwnd(AMTest* _main, const char* _title)
{
	HWND hWnd = ::FindWindowA(NULL, _title);
	if (!hWnd)
	{
		char buf[128] = { 0 };
		sprintf(buf, "窗口%s不存在", _title);
		MessageBoxA((HWND)_main->winId(), buf, NULL, NULL);
	}
	return hWnd;
}


#define WINDOW_TITLE_HS_GAME	"炉石传说"	


void ThreadProc(void *p)
{
	AMTest	*pAmTest = (AMTest*)p;
	if (!pAmTest)
	{
		return;
	}

	HWND hWnd = NULL;

	//g_wndProcDefault = (WNDPROC)::GetWindowLong(hWndHS, GWL_WNDPROC);
	//::SetWindowLong(hWndHS, GWL_WNDPROC, (LONG)WndProcMy);


	

	do
	{
		//hWnd = ::FindWindowA(NULL, "炉石传说");

		int status = WJAM::getGameStatusHS();

		int cmd = pAmTest->getCmd();
		switch (cmd)
		{
		case CMD_TEST:
		{
			
			//QString title = pAmTest->getParam1();
			//HWND hWnd = ::FindWindowA(NULL, title.toLocal8Bit().data());
			//
			//if (hWnd)
			//{
			//	QString path = QCoreApplication::applicationDirPath();
			//	HookManager::ganmetaskinjectDll(hWnd, path.toStdString().c_str(), pAmTest->getParam2().toStdString().c_str());
			//}
		}
		pAmTest->setCmd(-1);
		break;

		case CMD_RELOAD_LUA:
			if (!WJAM::initLua("./luaScript/main.out"))
			{
				printf("=====> 加载 LUA 脚本失败!\n");
			}
			pAmTest->setCmd(-1);
			break;

		case CMD_LOL_STARTUP:
		{
			/*登录*/
			WJAM::LOGININFO loginInfo = { 0 };
			loginInfo.gameid = WJAM::GAME_ID_LOL;
			strcpy_s(loginInfo.account, "546630550");
			strcpy_s(loginInfo.password, "bugaosuni");
			strcpy_s(loginInfo.path, "D:/Games/英雄联盟/TCLS/Client.exe");
			loginInfo.serverid = 10;
			int res = WJAM::login(&loginInfo);
			printf("=====> 结束: %d\n", res);
		}
		pAmTest->setCmd(-1);
		break;

		case CMD_LOL_CREATEROOM:
		{
			/*创建房间*/
			WJAM::ROOMINFO roomInfo = { 0 };
			strcpy_s(roomInfo.roomname, "2016-08-26");
			strcpy_s(roomInfo.password, "123456");
			strcpy_s(roomInfo.account, "2187239535");
			roomInfo.mode = WJAM::GAME_MODE_CLASSIC;
			roomInfo.map = WJAM::GAME_MAP_ABYSS;
			roomInfo.playernum = 1;
			roomInfo.observe = WJAM::GAME_OBSERVE_ALL;
			roomInfo.gametype = WJAM::GAME_TYPE_SELFCHOOSE;
			int res = WJAM::createRoom(&roomInfo);
			printf("=====> 结束: %d\n", res);
		}
		pAmTest->setCmd(-1);
		break;

		case CMD_LOL_JOINROOM:
		{
			/*加入房间*/
			WJAM::ROOMINFO roomInfo = { 0 };
			strcpy_s(roomInfo.roomname, "20180000");
			strcpy_s(roomInfo.password, "12345");
			strcpy_s(roomInfo.account, "2187239535");
			roomInfo.side = WJAM::GAME_SIDE_RED;
			int res = WJAM::joinRoom(&roomInfo);
			printf("=====> 结束: %d\n", res);
		}
		pAmTest->setCmd(-1);
		break;

		case CMD_LOL_PLAY:
		{
			int res = WJAM::startGame(WJAM::GAME_ID_LOL, true);
			if (WJAM_RES_OK == res)
			{
				ExtendGameManager* pEventMgr = new ExtendGameManager;
				pEventMgr->initEventSignal(EVENT_SERVER_DEFAULT_NAME, EVENT_CLIENT_DEFAULT_NAME, EVENT_EXIT_DEFAULT_NAME, pEventMgr);
				pEventMgr->StartThread();
			}
		}
		pAmTest->setCmd(-1);
		break;

		case CMD_HS_STARTUP:
		{
			//pAmTest->initWebSocketSrv(12341);
			//HWND hWndHS = NULL;
			//do 
			//{
			//	hWndHS = ::FindWindowA(NULL, "炉石传说");
			//} while (true);

			std::string playerName;
			if (WJAM_RES_OK == WJAM::startupHS(12351, "/Resource/H5/HSMatchesAtOnce.html", playerName))
			{
				AMTest::addEvent(1);
			}
		}

		pAmTest->setCmd(-1);
		break;

		case CMD_HS_REQ_FRIEND:
			WJAM::reqAddFriend(pAmTest->getParam1().toLocal8Bit());
			pAmTest->setCmd(-1);
			break;

		case CMD_HS_ACCEPT_FRIEND:
			WJAM::acceptFriendReq(pAmTest->getParam1().toLocal8Bit());
			pAmTest->setCmd(-1);
			break;

		case CMD_HS_REQ_CHALLENGE:
			WJAM::reqChallenge(pAmTest->getParam1().toLocal8Bit());
			pAmTest->setCmd(-1);
			break;

		case CMD_HS_ACCEPT_CHALLENGE:
		{
			int x = WJAM::acceptChallenge();
			pAmTest->setCmd(-1);
			break;
		}
			

		default:
			Sleep(500);
			break;
		}


	} while (true);
}
