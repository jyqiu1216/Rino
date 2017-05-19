#include "stdafx.h"
#include "MainWindow.h"


CButtonUI* CMainWindow::m_pBtnShow = NULL;
CVerticalLayoutUI* CMainWindow::m_pVerlayout = NULL;

CMainWindow::CMainWindow()
{
	m_pPath = NULL;
	m_dwMSize = 0;
	m_pDownload = NULL;
}

CMainWindow::~CMainWindow()
{
	if (NULL != m_pDownload) {

		delete m_pDownload;
	}
}

DuiLib::CDuiString CMainWindow::GetSkinFile()
{
	return TEXT("MainWnd.xml");
}

LPCTSTR CMainWindow::GetWindowClassName() const
{
	return TEXT("Rhino.Install");
}

void CMainWindow::InitWindow()
{
	try {

		g_pMainWndManager = &m_pm;

		m_pBtnShow = static_cast<CButtonUI*>(m_pm.FindControl(TEXT("Btn_Show")));

		if (NULL != m_pBtnShow) {

			m_pBtnShow->SetTag(0);
		}

		m_pVerlayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(TEXT("Layout_Info")));
		ASSERT(NULL != m_pVerlayout);

		m_pPath = static_cast<CEditUI*>(m_pm.FindControl(TEXT("Edit_Path")));
		ASSERT(NULL != m_pPath);
		SetInstallPath();

		GetDiskInfo();
	}
	catch (...) {

	}
}

void CMainWindow::Notify(TNotifyUI& msg)
{
	try {

		CDuiString CDuiName = msg.pSender->GetName();

		if (msg.sType == TEXT("windowinit")) {

		}
		else if (msg.sType == TEXT("click")) {

			if (CDuiName == TEXT("Btn_Close")) {

				::PostQuitMessage(0);
			}
			else if (CDuiName == TEXT("Btn_Min")) {

				::ShowWindow(m_hWnd, SW_MINIMIZE);
			}
			else if (CDuiName == TEXT("Btn_Show")) {

				ShowHidePath(static_cast<CButtonUI*>(msg.pSender));
			}
			else if (CDuiName == TEXT("Btn_path")) {

				SwitchDirectory();
			}
			else if (CDuiName == TEXT("Btn_Install")) {

				if (m_dwMSize <= 300) {

					::MessageBox(m_hWnd, TEXT("无法安装，磁盘空间不足够！"), TEXT("提示"), 0);
				}
				else {

					::ShowWindow(m_hWnd, SW_HIDE);

					if (NULL == m_pDownload) {

						m_pDownload = new CDownload();
					}

					m_pDownload->Create(NULL, TEXT(""), UI_WNDSTYLE_DIALOG, 0L, 0, 0, 600, 400);
					m_pDownload->CenterWindow();

#ifdef STAUP_NOWINDOW
					//静默安装
					::ShowWindow(m_pDownload->GetHWND(), SW_HIDE);
#else
					//正常安装
					::ShowWindow(m_pDownload->GetHWND(), SW_SHOWNORMAL);
#endif
					
				}
			}
		}
	}
	catch (...) {

	}
}

void CMainWindow::ShowHidePath(CButtonUI* pBtnShow)
{
	try {

		if (NULL == pBtnShow) {

			return;
		}

		int inValue = pBtnShow->GetTag();

		if (0 == inValue) {
			//显示

			pBtnShow->SetTag(1);
			pBtnShow->SetMouseEnabled(false);

			::SetTimer(m_hWnd, 0, 5, ShowTimerProc);
		}
		else if (1 == inValue) {
			//隐藏

			pBtnShow->SetTag(0);
			pBtnShow->SetMouseEnabled(false);

			::SetTimer(m_hWnd, 0, 5, HideTimerProc);
		}
	}
	catch (...) {

	}
}

VOID CALLBACK CMainWindow::ShowTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	__try {

		RECT tRect = m_pVerlayout->GetPadding();

		if (tRect.top > 0) {

			tRect.top -= 5;

			if (tRect.top < 0) {

				tRect.top = 0;
			}
		}
		else {

			m_pBtnShow->SetAttribute(TEXT("normalimage"), TEXT("file='Custom_Down.png' source='0,0,130,30'"));
			m_pBtnShow->SetAttribute(TEXT("hotimage"), TEXT("file='Custom_Down.png' source='0,30,130,60'"));
			m_pBtnShow->SetAttribute(TEXT("pushedimage"), TEXT("file='Custom_Down.png' source='0,60,130,90'"));

			m_pBtnShow->Invalidate();
			m_pBtnShow->SetMouseEnabled(true);
			::KillTimer(hwnd, idEvent);
		}

		m_pVerlayout->SetPadding(tRect);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

	}
}

VOID CALLBACK CMainWindow::HideTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	__try {

		RECT tRect = m_pVerlayout->GetPadding();

		if (tRect.top < 73) {

			tRect.top += 5;

			if (tRect.top > 73) {

				tRect.top = 73;
			}
		}
		else {

			m_pBtnShow->SetAttribute(TEXT("normalimage"), TEXT("file='Custom_Up.png' source='0,0,130,30'"));
			m_pBtnShow->SetAttribute(TEXT("hotimage"), TEXT("file='Custom_Up.png' source='0,30,130,60'"));
			m_pBtnShow->SetAttribute(TEXT("pushedimage"), TEXT("file='Custom_Up.png' source='0,60,130,90'"));

			m_pBtnShow->Invalidate();
			m_pBtnShow->SetMouseEnabled(true);
			::KillTimer(hwnd, idEvent);
		}

		m_pVerlayout->SetPadding(tRect);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

	}
}

void CMainWindow::SetInstallPath()
{
	try {

		wchar_t wchPath[1024] = { 0 };

		if (::ExpandEnvironmentStrings(TEXT("%ProgramFiles%"), (LPTSTR)wchPath, ARRAYSIZE(wchPath)) == 0) {

			return;
		}

		if (NULL != m_pPath) {

			::wcscpy_s(g_wchInstallPath, ARRAYSIZE(g_wchInstallPath), wchPath);
			::wcscat_s(g_wchInstallPath, ARRAYSIZE(g_wchInstallPath), TEXT("\\"));
			::wcscat_s(g_wchInstallPath, ARRAYSIZE(g_wchInstallPath), ID_INSTALLDIR);

			::wcscat_s(wchPath, ARRAYSIZE(wchPath), TEXT("\\"));
			::wcscat_s(wchPath, ARRAYSIZE(wchPath), ID_INSTALLDIR);
			m_pPath->SetText(wchPath);
		}
	}
	catch (...) {

	}
}

void CMainWindow::SwitchDirectory()
{
	__try {

		wchar_t pwchPath[MAX_PATH] = { 0 };
		BROWSEINFO bi = { 0 };

		bi.hwndOwner = m_hWnd;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = NULL;
		bi.lpszTitle = TEXT("请选择安装目录");
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
		bi.lpfn = NULL;
		bi.lParam = 0;
		bi.iImage = 0;

		LPITEMIDLIST pidl = ::SHBrowseForFolder(&bi);

		if (NULL == pidl) {

			return;
		}

		if (::SHGetPathFromIDList(pidl, pwchPath)) {

			if (::PathIsDirectory(pwchPath)) {

				if (NULL != m_pPath) {

					::wcscpy_s(g_wchInstallPath, ARRAYSIZE(g_wchInstallPath), pwchPath);
					::wcscat_s(g_wchInstallPath, ARRAYSIZE(g_wchInstallPath), TEXT("\\"));
					::wcscat_s(g_wchInstallPath, ARRAYSIZE(g_wchInstallPath), ID_INSTALLDIR);

					::wcscat_s(pwchPath, ARRAYSIZE(pwchPath), TEXT("\\"));
					::wcscat_s(pwchPath, ARRAYSIZE(pwchPath), ID_INSTALLDIR);
					m_pPath->SetText(pwchPath);

					GetDiskInfo();
				}
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

	}
}

void CMainWindow::GetDiskInfo()
{
	try {

		if (NULL == m_pPath) {

			return;
		}

		wchar_t wchPath[1024] = { 0 };
		::wcscpy_s(wchPath, ARRAYSIZE(wchPath), m_pPath->GetText());

		namespace fs = boost::filesystem;
		fs::path path(wchPath);

		wstring wstrRoot = path.root_name().wstring();

		DWORD64 qwFreeBytes = 0, qwFreeBytesToCaller = 0, qwTotalBytes = 0;

		if (::GetDiskFreeSpaceEx(wstrRoot.c_str(), (PULARGE_INTEGER)&qwFreeBytesToCaller, (PULARGE_INTEGER)&qwTotalBytes, (PULARGE_INTEGER)&qwFreeBytes)) {

			DWORD64 dwMSize = qwFreeBytes / 1024 / 1024;
			m_dwMSize = dwMSize;
			double dGSize = (double)dwMSize / 1024;

			wchar_t wchTemp[1024] = { 0 };
			::swprintf_s(wchTemp, ARRAYSIZE(wchTemp), TEXT("所需空间：150M，可用空间：%0.2fGB"), dGSize);

			CTextUI* pText = static_cast<CTextUI*>(m_pm.FindControl(TEXT("Text_InstallInfo")));

			if (pText) {

				pText->SetText(wchTemp);
			}
		}
	}
	catch (...) {

	}
}
