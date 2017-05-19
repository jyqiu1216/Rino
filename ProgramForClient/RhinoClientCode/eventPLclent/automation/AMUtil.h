#ifndef _AMUtil_H_
#define _AMUtil_H_

#include <Windows.h>

class AMUtil
{
public:
	//Êó±êµã»÷×ó¼ü
	static void mouseClickLButton(HWND _hWnd, int _x, int _y)
	{
		Sleep(10);
		::SendMessage(_hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(_x, _y));
		Sleep(10);
		::SendMessage(_hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(_x, _y));
		Sleep(10);
		::SendMessage(_hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(_x, _y));
		Sleep(10);
	}
	//Êó±êµã»÷°´Å¥1
	static void mouseClickLButton1(HWND _hWnd, int _x, int _y)
	{
		SwitchToThisWindow(_hWnd, true);
		//BringWindowToTop(_hWnd);
		Sleep(50);
		::SetCursorPos(_x, _y);
		Sleep(1);
		//::mouse_event(MOUSEEVENTF_LEFTDOWN, _x, _y, 0, 0);
		//::mouse_event(MOUSEEVENTF_LEFTUP, _x, _y, 0, 0);
		::SendMessage(_hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(_x, _y));
		Sleep(10);
		::SendMessage(_hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(_x, _y));
		Sleep(10);
	}
	//Õ³Ìù¼ôÇÐ°å
	static void pasteClipboard(HWND _hWnd)
	{
		SwitchToThisWindow(_hWnd, true);
		keybd_event(VK_CONTROL, 0, 0, 0);
		Sleep(10);
		keybd_event('V', 0, 0, 0);
		//::SendMessage(_hWnd, WM_SYSKEYDOWN, 'V', 0);
		Sleep(10);
		keybd_event('V', 0, 2, 0);
		//::SendMessage(_hWnd, WM_SYSKEYUP, 'V', 0);
		Sleep(10);
		keybd_event(VK_CONTROL, 0, 2, 0); // release ctrl
		Sleep(10);
	}
	//ÉèÖÃ¼ôÇÐ°åÄÚÈÝ
	static void setClipboard(const char* _text)
	{
		if (_text)
		{
			OpenClipboard(NULL);
			EmptyClipboard();

			HGLOBAL hgl = GlobalAlloc(GMEM_MOVEABLE, 100 * sizeof(WCHAR));
			LPWSTR lpstrcpy = (LPWSTR)GlobalLock(hgl);
			memcpy(lpstrcpy, _text, 100 * sizeof(WCHAR));
			GlobalUnlock(hgl);
			SetClipboardData(CF_TEXT, lpstrcpy);
			CloseClipboard();
		}
	}
	//°´ÏÂ¼ü
	static void pressKey(HWND _hWnd, UINT _keyCode)
	{
		Sleep(10);
		::PostMessage(_hWnd, WM_SYSKEYDOWN, _keyCode, NULL);
		Sleep(10);
		::PostMessage(_hWnd, WM_SYSKEYUP, _keyCode, NULL);
		Sleep(10);
	}

	static void pressKeyDelete(HWND _hWnd)
	{
		Sleep(10);
		::PostMessage(_hWnd, WM_SYSKEYDOWN, VK_BACK, NULL);
		Sleep(10);
		::PostMessage(_hWnd, WM_SYSKEYUP, VK_BACK, NULL);
		Sleep(10);
	}

	static char* getSysTimeBuffer()
	{
		static char buffer[64] = { 0 };

		SYSTEMTIME sys;
		GetLocalTime(&sys);

		sprintf_s(buffer, "%4d-%02d-%02d %02d:%02d:%02d.%03d",
			sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);

		return buffer;
	};

	static void outputDebugStr(char* _format, ...)
	{
		char s_buf[512] = { 0 };
		memset(s_buf, 0, 512);

		va_list va;
		va_start(va, _format);
		vsprintf_s(s_buf, 512 - 1, _format, va);

		OutputDebugStringA(s_buf);
	}
};

#endif // !_AMUtil_H_
