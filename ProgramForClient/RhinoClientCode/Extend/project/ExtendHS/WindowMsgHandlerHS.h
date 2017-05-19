#ifndef _WindowMsgHandlerHS_H_
#define _WindowMsgHandlerHS_H_

#include <Windows.h>

extern WNDPROC g_wndProcDefault;

/*
* \brief:	炉石注入窗口消息处理回调函数
*/

LRESULT CALLBACK WndProcHS(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



#endif	// $END$ _WindowMsgHandlerHS_H_