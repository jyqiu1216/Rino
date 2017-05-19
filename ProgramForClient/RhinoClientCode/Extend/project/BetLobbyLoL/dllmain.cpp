// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "GameLobby.h"

CGameLobby g_GameLobby;

void __cdecl HookThreadProc(LPVOID lpParameter)
{
	HWND hGame = NULL;

	while (true) {

		hGame = ::FindWindow(NULL, TEXT("PVP.net 客户端"));

		if (NULL != hGame) {

			break;
		}

		::Sleep(1000);
	}

	g_GameLobby.HookGameLobby();

	::_endthread();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) {

	case DLL_PROCESS_ATTACH:
		::_beginthread(HookThreadProc, 0, NULL);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		g_GameLobby.UnHookGameLobby();
		break;
	}

	return TRUE;
}

