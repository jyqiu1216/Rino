// OnlineInstall.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "OnlineInstall.h"
#include "MainWindow.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	__try {

		UNREFERENCED_PARAMETER(hPrevInstance);
		UNREFERENCED_PARAMETER(nCmdShow);

		/*if (!CheckConfigFile()) {

			::MessageBox(NULL, TEXT("配置文件丢失!"), TEXT("提示"), 0);
			return 0;
		}*/

		HRESULT hResult = ::CoInitialize(NULL);

		if (FAILED(hResult)) {

			return 0;
		}

		hResult = ::OleInitialize(NULL);

		if (FAILED(hResult)) {

			::CoUninitialize();
			return 0;
		}

		CPaintManagerUI::SetInstance(hInstance);

		g_hSetupMutex = ::CreateMutex(NULL, FALSE, TEXT("Install-25453D3A-2952-405C-9E97-809B5169ED44"));

		if (ERROR_ALREADY_EXISTS != GetLastError()) {

#ifdef STAUP_NOWINDOW
			//静默安装
			SetupRhino(hInstance, false);
#else
			//正常安装
			SetupRhino(hInstance);
#endif
		}

		if (NULL != g_hSetupMutex) {

			::CloseHandle(g_hSetupMutex);
		}

		::OleUninitialize();
		::CoUninitialize();
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

	}

	::ExitProcess(0);
	return 0;
}

void SetupRhino(HINSTANCE hInstance, bool bShow)
{
	try {

		if (NULL == hInstance) {

			return;
		}

		HRSRC hResource = ::FindResource(hInstance, MAKEINTRESOURCE(IDR_SKIN1), TEXT("skin"));

		if (NULL != hResource) {

			HGLOBAL hGlobal = ::LoadResource(hInstance, hResource);

			if (NULL != hGlobal) {

				DWORD dwSize = ::SizeofResource(hInstance, hResource);

				if (dwSize > 0) {

					CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
				}
			}

			::FreeResource(hResource);
		}

		CMainWindow* pMainWindow = new CMainWindow();

		pMainWindow->Create(NULL, TEXT(""), UI_WNDSTYLE_DIALOG, 0L, 0, 0, 600, 400);
		pMainWindow->CenterWindow();

		if (bShow) {

			::ShowWindow(pMainWindow->GetHWND(), SW_SHOWNORMAL);
		}
		else {

			::ShowWindow(pMainWindow->GetHWND(), SW_HIDE);

			::SetTimer(pMainWindow->GetHWND(), 0, 100, InstallTimerProc);
		}

		CPaintManagerUI::MessageLoop();
		delete pMainWindow;
		pMainWindow = NULL;

		CResourceManager::GetInstance()->Release();
	}
	catch (...) {

	}
}

VOID CALLBACK InstallTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	try {

		::KillTimer(hwnd, idEvent);

		CButtonUI* pButton = static_cast<CButtonUI*>(g_pMainWndManager->FindControl(TEXT("Btn_Install")));

		if (NULL != pButton) {

			g_bNoWindow = true;

			g_pMainWndManager->SendNotify(pButton, DUI_MSGTYPE_CLICK);
		}
	}
	catch (...) {

	}
}

bool CheckConfigFile()
{
	bool bRet = false;

	try {

		wchar_t wchAppPath[1024] = { 0 };
		::GetModuleFileName(::GetModuleHandle(NULL), wchAppPath, ARRAYSIZE(wchAppPath));

		namespace fs = boost::filesystem;
		fs::path path(wchAppPath);

		path.remove_filename();
		path.append(TEXT("set.ini"));

		if (fs::exists(path)) {

			bRet = true;
		}
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}
