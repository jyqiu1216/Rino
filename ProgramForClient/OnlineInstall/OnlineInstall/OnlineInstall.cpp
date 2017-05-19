// OnlineInstall.cpp : 定义应用程序的入口点。
//

/*Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/#include "stdafx.h"
#include "OnlineInstall.h"
#include "MainWnd.h"

#define MAX_LOADSTRING 100

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	HANDLE hMutex = ::CreateMutex(NULL, TRUE, "OnlineInstall");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{//只运行开一个实例
		return 0;
	}

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	CMainWnd* pFrame = new CMainWnd();
	if (pFrame == NULL) return 0;
	pFrame->CreateDuiWindow(NULL, _T(""), UI_WNDSTYLE_DIALOG | WS_MINIMIZEBOX);
	pFrame->CenterWindow();

	//////////////////////////////////////////////////////////////////////////
#ifdef WJ_QUIET
	//静默安装
	pFrame->ShowWindow(SW_HIDE);
	pFrame->NoWindowInstall();

	CPaintManagerUI::MessageLoop();
#else
	pFrame->ShowModal();
#endif // WJ_QUIET

	::CoUninitialize();

	return 0;
}