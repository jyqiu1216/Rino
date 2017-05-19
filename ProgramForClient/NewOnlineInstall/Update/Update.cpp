// Update.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Update.h"
#include "MainWnd.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	__try {

		UNREFERENCED_PARAMETER(hPrevInstance);
		UNREFERENCED_PARAMETER(lpCmdLine);
		UNREFERENCED_PARAMETER(nCmdShow);

		if (!CheckConfigFile()) {

			::MessageBox(NULL, TEXT("配置文件丢失!"), TEXT("提示"), 0);
			return 0;
		}

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

		g_hSetupMutex = ::CreateMutex(NULL, FALSE, TEXT("Update-AA5FEE91-ACC9-496C-B478-12A1E00A8FDA"));

		if (ERROR_ALREADY_EXISTS != GetLastError()) {

			OnlineUpdate(hInstance);
		}

		if (NULL != g_hSetupMutex) {

			::CloseHandle(g_hSetupMutex);
		}

		::OleUninitialize();
		::CoUninitialize();
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

	}

	return 0;
}

void OnlineUpdate(HINSTANCE hInstance)
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

		CMainWnd* pMainWnd = new CMainWnd();

		pMainWnd->Create(NULL, TEXT(""), UI_WNDSTYLE_DIALOG, 0L, 0, 0, 800, 182);
		pMainWnd->CenterWindow();

		::ShowWindow(pMainWnd->GetHWND(), SW_SHOWNORMAL);

		CPaintManagerUI::MessageLoop();
		delete pMainWnd;
		pMainWnd = NULL;

		CResourceManager::GetInstance()->Release();
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
		path.append(TEXT("update.ini"));

		if (fs::exists(path)) {

			bRet = true;
		}
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}