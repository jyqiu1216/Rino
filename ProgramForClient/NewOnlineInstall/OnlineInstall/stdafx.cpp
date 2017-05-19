// stdafx.cpp : 只包括标准包含文件的源文件
// OnlineInstall.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用
//互斥
HANDLE																						g_hSetupMutex = NULL;
//安装路径
wchar_t																						g_wchInstallPath[1024] = { 0 };
//下载ZIP路径
wchar_t																						g_wchDownloadPath[1024] = { 0 };
//主窗口指针
CPaintManagerUI*																			g_pMainWndManager = NULL;
//静默安装
bool																						g_bNoWindow = false;