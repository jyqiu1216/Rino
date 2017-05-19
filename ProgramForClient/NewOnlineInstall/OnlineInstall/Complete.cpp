#include "stdafx.h"
#include "Complete.h"


CComplete::CComplete()
{

}

CComplete::~CComplete()
{

}

DuiLib::CDuiString CComplete::GetSkinFile()
{
	return TEXT("SetupComplete.xml");
}

LPCTSTR CComplete::GetWindowClassName() const
{
	return TEXT("Rhino.Install.Complete");
}

void CComplete::InitWindow()
{
	if (g_bNoWindow) {

		::ExitProcess(0);
	}
}

void CComplete::Notify(TNotifyUI& msg)
{
	try {

		CDuiString CDuiName = msg.pSender->GetName();

		if (msg.sType == TEXT("windowinit")) {

		}
		else if (msg.sType == TEXT("click")) {

			if (CDuiName == TEXT("Btn_Close")) {

				::ExitProcess(0);
			}
			else if (CDuiName == TEXT("Btn_Min")) {

				::ShowWindow(m_hWnd, SW_MINIMIZE);
			}
			else if (CDuiName == TEXT("BTN_OK")) {

				::ExitProcess(0);
			}
			else if (CDuiName == TEXT("ETN_START")) {

				StartClient();

				::ExitProcess(0);
			}
		}
	}
	catch (...) {

	}
}

void CComplete::StartClient()
{
	try {

		namespace fs = boost::filesystem;
		fs::path path(g_wchInstallPath);

		path.append(TEXT("RhinoClient.exe"));

		if (fs::exists(path)) {

			SHELLEXECUTEINFO ShExecInfo = { 0 };
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = NULL;
			ShExecInfo.lpFile = path.c_str();
			ShExecInfo.lpParameters = NULL;
			ShExecInfo.lpDirectory = g_wchInstallPath;
			ShExecInfo.nShow = SW_SHOW;
			ShExecInfo.hInstApp = NULL;

			if (::ShellExecuteEx(&ShExecInfo)) {

				if (NULL != ShExecInfo.hProcess) {

					::CloseHandle(ShExecInfo.hProcess);
				}
			}
		}
	}
	catch (...) {

	}
}
