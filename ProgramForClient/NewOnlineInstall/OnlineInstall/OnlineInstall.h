#pragma once

#include "resource.h"

//正常安装
void													SetupRhino(HINSTANCE hInstance, bool bShow = true);

//静默安装
VOID CALLBACK											InstallTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

//校验配置文件
bool													CheckConfigFile();