#include "stdafx.h"
#include "MainWnd.h"


CMainWnd::CMainWnd()
{
	m_pTilText = NULL;
	m_pTilSpeed = NULL;
	m_pProgress = NULL;
	m_pCancelReDownLoad = NULL;
}

CMainWnd::~CMainWnd()
{

}

DuiLib::CDuiString CMainWnd::GetSkinFile()
{
	return TEXT("MainWnd.xml");
}

LPCTSTR CMainWnd::GetWindowClassName() const
{
	return TEXT("Update.MainWnd");
}

void CMainWnd::InitWindow()
{
	try {

		EnablePrivilege(SE_BACKUP_NAME);
		EnablePrivilege(SE_RESTORE_NAME);

		m_pTilText = static_cast<CTextUI*>(m_pm.FindControl(TEXT("Update.Title.Version")));
		ASSERT(NULL != m_pTilText);

		m_pTilSpeed = static_cast<CTextUI*>(m_pm.FindControl(TEXT("Update.Title.Speed")));
		ASSERT(NULL != m_pTilSpeed);

		m_pProgress = static_cast<CProgressUI*>(m_pm.FindControl(TEXT("Update.Progress")));
		ASSERT(NULL != m_pProgress);

		m_pCancelReDownLoad = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("BTN_CONTROL")));
		ASSERT(NULL != m_pCancelReDownLoad);

		HANDLE hThread = ::CreateThread(NULL, 0, TaskThreadProc, this, 0, NULL);

		if (NULL != hThread) {

			::CloseHandle(hThread);
		}
	}
	catch (...) {

	}
}

void CMainWnd::Notify(TNotifyUI& msg)
{
	try {

		if (msg.sType == TEXT("windowinit")) {

		}
		else if (msg.sType == TEXT("click")) {

			CDuiString CstrName = msg.pSender->GetName();

			if (CstrName == TEXT("BTN_WEBDOWNLOAD")) {

				::ShellExecute(m_hWnd, TEXT("open"), TEXT("http://www.xiniugame.com/download"), NULL, NULL, SW_SHOWMAXIMIZED);

				if (m_pProgress->GetValue() <= 0) {

					::PostQuitMessage(0);
				}
			}
			else if (CstrName == TEXT("BTN_MIN")) {

				::ShowWindow(m_hWnd, SW_MINIMIZE);
			}
			else if (CstrName == TEXT("BTN_CONTROL")) {

				wstring wstrTemp = m_pCancelReDownLoad->GetText();

				if (::wcscmp(wstrTemp.c_str(), TEXT("重试")) == 0) {

					m_pCancelReDownLoad->SetText(TEXT("取消更新"));

					m_pTilText->SetText(TEXT("(版本:0.0) 平台正在更新..."));
					m_pTilText->SetTextColor(0xFFFFFFFF);

					m_pTilSpeed->SetText(TEXT("(0.000KB/S)"));

					HANDLE hThread = ::CreateThread(NULL, 0, TaskThreadProc, this, 0, NULL);

					if (NULL != hThread) {

						::CloseHandle(hThread);
					}
				}
				else if (::wcscmp(wstrTemp.c_str(), TEXT("取消更新")) == 0) {

					CCancel* pCancel = new CCancel;

					pCancel->Create(m_hWnd, TEXT(""), UI_WNDSTYLE_DIALOG, 0L, 0, 0, 435, 136);
					pCancel->CenterWindow();
					pCancel->ShowModal();

					delete pCancel;
				}
			}
		}
		else if (msg.sType == TEXT("msg.Title")) {

			wstring wstrData = (wchar_t*)msg.wParam;
			DWORD dwColor = (DWORD)msg.lParam;
			int inType = dwColor;

			m_pTilText->SetText(wstrData.c_str());
			m_pTilText->SetTextColor(dwColor);

			if (-1 != inType) {
				//出错了

				wstring wstrTemp = m_pCancelReDownLoad->GetText();

				if (::wcscmp(wstrTemp.c_str(), TEXT("取消更新")) == 0) {

					m_pCancelReDownLoad->SetText(TEXT("重试"));
				}
			}
		}
		else if (msg.sType == TEXT("msg.Title.Text")) {

			wstring wstrText = (wchar_t*)msg.wParam;

			CTextUI* pText = static_cast<CTextUI*>(msg.pSender);

			if (pText) {

				pText->SetText(wstrText.c_str());
			}
		}
		else if (msg.sType == TEXT("msg.DwonLoad.Progress")) {

			int inPos = (int)msg.wParam;
			CProgressUI* pProgress = static_cast<CProgressUI*>(msg.pSender);

			if (NULL != pProgress) {

				pProgress->SetValue(inPos);

				wchar_t wchTemp[128] = { 0 };
				::swprintf_s(wchTemp, ARRAYSIZE(wchTemp), TEXT("%d%%"), inPos);

				pProgress->SetText(wchTemp);
			}
		}
	}
	catch (...) {

	}
}

DWORD WINAPI CMainWnd::TaskThreadProc(LPVOID lpParameter)
{
	try {

		CMainWnd* pThis = static_cast<CMainWnd*>(lpParameter);
		CGetUpdate tGetUpdate;

		if (!tGetUpdate.GetUpdate(&pThis->m_pm)) {

			pThis->m_pm.SendNotify(pThis->m_pTilText, TEXT("msg.Title"), (WPARAM)TEXT("error：获取更新版本数据失败了！"), (LPARAM)0xFFFF5252);

			throw TEXT("error：TaskThreadProc()->GetUpdate!");
		}

		if (!tGetUpdate.CheckVersion(&pThis->m_pm)) {

			pThis->m_pm.SendNotify(pThis->m_pTilText, TEXT("msg.Title"), (WPARAM)TEXT("error：检验版本信息失败了！"), (LPARAM)0xFFFF5252);

			throw TEXT("error：TaskThreadProc()->CheckVersion!");
		}

		pThis->RunMainroutine();
	}
	catch (...) {

	}

	return 0;
}

void CMainWnd::RunMainroutine()
{
	try {

		wchar_t wchAppPath[1024] = { 0 };
		::GetModuleFileName(::GetModuleHandle(NULL), wchAppPath, ARRAYSIZE(wchAppPath));

		namespace fs = boost::filesystem;
		fs::path path(wchAppPath);

		path.remove_filename();
		fs::path dir(path);
		path.append(TEXT("RhinoClient.exe"));

		if (fs::exists(path)) {

			SHELLEXECUTEINFO ShExecInfo = { 0 };
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = NULL;
			ShExecInfo.lpFile = path.c_str();
			ShExecInfo.lpParameters = NULL;
			ShExecInfo.lpDirectory = dir.c_str();
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

	::ExitProcess(0);
}

BOOL CMainWnd::EnablePrivilege(LPCTSTR lptName)
{
	BOOL bRet = FALSE;

	if (NULL == lptName) {

		return bRet;
	}

	__try {

		HANDLE hObject = NULL;
		LUID Luid = { 0 };
		TOKEN_PRIVILEGES NewStatus = { 0 };

		if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hObject)) {

			return bRet;
		}

		if (::LookupPrivilegeValue(NULL, lptName, &Luid)) {

			NewStatus.Privileges[0].Luid = Luid;
			NewStatus.PrivilegeCount = 1;
			NewStatus.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			::AdjustTokenPrivileges(hObject, FALSE, &NewStatus, sizeof(TOKEN_PRIVILEGES), 0, 0);
			::CloseHandle(hObject);

			bRet = TRUE;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

		bRet = FALSE;
	}

	return bRet;
}