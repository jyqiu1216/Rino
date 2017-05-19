// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <windows.h>
#include <shellapi.h>
#include <string>
#include "LOLCore.h"
#include <thread>
using namespace std;



TGameInfo info;
void GetGameInfo(TGameInfo& info);
wstring GetAreaName(DWORD id);
void NotifyMainWnd(HWND hWnd, UINT32 msg, void* buf, int len);

DWORD WINAPI ThreadSendProc(LPVOID lpParameter);
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {

		OutputDebugString(L"DllMain::DLL_PROCESS_ATTACH\n");

		HANDLE hThread = CreateThread(NULL, 0, ThreadSendProc, 0, 0, NULL);

		if (NULL != hThread) {

			::CloseHandle(hThread);
		}
	}
		break;
	case DLL_THREAD_ATTACH:
		OutputDebugString(L"DllMain::DLL_THREAD_ATTACH\n");

		break;
	case DLL_THREAD_DETACH:
		OutputDebugString(L"DllMain::DLL_THREAD_DETACH\n");

		break;
	case DLL_PROCESS_DETACH:
		OutputDebugString(L"DllMain::DLL_PROCESS_DETACH\n");

		break;
	}
	return TRUE;
}
DWORD WINAPI ThreadSendProc(LPVOID lpParameter)
{
	HWND hWnd;
	memset(&info, 0, sizeof(TGameInfo));
	GetGameInfo(info);


	while (true)
	{
		//OutputDebugString(L"111");
		hWnd = ::FindWindow(NULL, L"犀牛电竞");
		if (hWnd)
		{
			//OutputDebugString(L"犀牛电竞窗口存在");
			NotifyMainWnd(hWnd, MSG_LOLINFO_HALL, &info, sizeof(TGameInfo));
			//MessageBox(NULL, info.area, TEXT(""), NULL);
		}
		Sleep(3000);
	}
	return 0;
}

void GetGameInfo(TGameInfo& info)
{
	LPWSTR *szArgList;
	int argCount;
	LPWSTR  cmdline = GetCommandLine();
	if (cmdline == NULL)
	{
		return;
	}

	OutputDebugString(cmdline);
	szArgList = CommandLineToArgvW(cmdline, &argCount);

	//QQ号
	if (NULL != szArgList[2]) {

		info.dwUin = wcstoul(szArgList[2], 0, 10);
	}
	//区服
	if (NULL != szArgList[6]) {

		DWORD dwArea = wcstoul(szArgList[6], 0, 10);
		wstring strArea = GetAreaName(dwArea);
		wmemcpy(info.area, strArea.c_str(), strArea.length());
	}

	LocalFree(szArgList);

}
void NotifyMainWnd(HWND hWnd, UINT32 msg, void* buf, int len)
{
	COPYDATASTRUCT cpd;
	cpd.dwData = msg;
	cpd.lpData = buf;
	cpd.cbData = len;

	SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&cpd);
}
wstring GetAreaName(DWORD id)
{
	wstring a;
	switch (id)
	{
		//电信
	case  AiOuNiYa:
		a = L"艾欧尼亚";
		break;
	case ZhuAng:
		a = L"祖安";
		break;
	case NuoKeShaShi:
		a = L"诺克萨斯";
		break;
	case BanDeErCheng:
		a = L"班德尔城";
		break;
	case PiErTeWoFu:
		a = L"皮尔特沃夫";
		break;
	case ZhangZhengXueYuan:
		a = L"战争学院";
		break;
	case JuShengFeng:
		a = L"巨神峰";
		break;
	case LeiSheShouBei:
		a = L"雷瑟守备";
		break;
	case GangTieLieYang:
		a = L"钢铁烈阳";
		break;
	case ChaiJueZhiDi:
		a = L"裁决之地";
		break;
	case HeiSheMeiZhui:
		a = L"黑色玫瑰";
		break;
	case AngYingDao:
		a = L"暗影岛";
		break;
	case JunHengJiaoPai:
		a = L"均衡教派";
		break;
	case ShuiJinZhiHeng:
		a = L"水晶之痕";
		break;
	case YinLiu:
		a = L"影流";
		break;
	case ShouWanZhiHai:
		a = L"守望之海";
		break;
	case ZhengFuZhiHai:
		a = L"征服之海";
		break;
	case KaLaMangDa:
		a = L"卡拉曼达";
		break;
	case PiChengJinBei:
		a = L"皮城警备";
		break;
	case Nanjuelingyu:
		a = L"男爵领域";
		break;

		//网通
	case BiErJiWoTe:
		a = L"比尔吉沃特";
		break;
	case DeMaXiYa:
		a = L"德玛西亚";
		break;
	case FeiLeiErZhuoDe:
		a = L"弗雷尔卓德";
		break;
	case WuWeiXianFeng:
		a = L"无畏先锋";
		break;
	case ShuRuiMa:
		a = L"恕瑞玛";
		break;
	case NiuQuChongLin:
		a = L"扭曲丛林";
		break;
	case JuLongZhiChao:
		a = L"巨龙之巢";
		break;

		//其他
	case JiaoYuWangZhuanQu:
		a = L"教育网专区";
		break;

	default:
		break;
	}
	return  a;
}