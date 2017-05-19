// Update.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Update.h"
#include "UpdateWnd.h"
#include "BKWnd.h"
#define MAX_LOADSTRING 100

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	//if (0 == _tcslen(lpCmdLine))
	//{
	//	return 0;
	//}

	//char str[1000] = { "/nb:C:\Users\Administrator\Desktop\RhinoClient\RhinoClient.exe/ne /v1b:103/v1e /v2b:0/v2e /ub:http://cdn.xiniugame.com//xiniuclient/version/20161130204634//ue /pb:RhinoClient.zip/pe /sb:0/se"};
	
	HANDLE hMutex = ::CreateMutex(NULL, TRUE, "Update");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{//只运行开一个实例
		return 0;
	}
	
	char s[MAX_PATH] = {0};
	sprintf_s(s, "sdp_test update.exe %s", lpCmdLine);
	OutputDebugStringA(s);
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	int scrWidth, scrHeight;  
	scrWidth = GetSystemMetrics(SM_CXSCREEN);  
	scrHeight = GetSystemMetrics(SM_CYSCREEN);  
	RECT rect;  

	/*CBKWnd* m_BKWnd = new CBKWnd();
	if( m_BKWnd == NULL ) return 0;
	m_BKWnd->CreateDuiWindow(NULL, _T(""), UI_WNDSTYLE_FRAME);
	m_BKWnd->ShowWindow(true);
	GetWindowRect(m_BKWnd->GetHWND(), &rect);  
	SetWindowPos(m_BKWnd->GetHWND(), HWND_TOPMOST, (scrWidth - rect.right) / 2, (scrHeight - rect.bottom) / 2, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);  
*/
	CUpdateWnd* pFrame = new CUpdateWnd(/*str*/lpCmdLine);
	if( pFrame == NULL ) return 0;
	pFrame->CreateDuiWindow(nullptr/*m_BKWnd->GetHWND()*/, _T(""), UI_WNDSTYLE_FRAME);

	GetWindowRect(pFrame->GetHWND(), &rect);  
	SetWindowPos(pFrame->GetHWND(), HWND_TOPMOST, (scrWidth - rect.right) / 2, (scrHeight - rect.bottom) / 2+22, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);  
	pFrame->ShowModal();

	//delete m_BKWnd;
	delete pFrame;

	::CoUninitialize();
	return 0;
}