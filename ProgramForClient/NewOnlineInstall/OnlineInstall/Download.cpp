#include "stdafx.h"
#include "Download.h"

CLabelUI* CDownload::m_pBkImage;
int CDownload::m_CurIndex;
int CDownload::m_NextIndex;
int	CDownload::m_Opacity;

CDownload::CDownload()
{
	m_pBkImage = NULL;
	m_CurIndex = 0;
	m_NextIndex = 0;
	m_Opacity = 255;
	m_pProgrText = NULL;
	m_pProgress = NULL;
	m_pSpeedText = NULL;
	m_pComplete = NULL;
	m_bClose = false;
}

CDownload::~CDownload()
{
	m_bClose = true;

	::KillTimer(m_hWnd, ID_TIMERIMAGEHIDE);
	::KillTimer(m_hWnd, ID_TIMERIMAGESHOW);
	::KillTimer(m_hWnd, ID_TIMERIMAGESWITCH);

	if (NULL != m_pComplete) {

		delete m_pComplete;
	}
}

DuiLib::CDuiString CDownload::GetSkinFile()
{
	return TEXT("Download.xml");
}

LPCTSTR CDownload::GetWindowClassName() const
{
	return TEXT("Rhino.Download");
}

void CDownload::InitWindow()
{
	try {

		m_pBkImage = static_cast<CLabelUI*>(m_pm.FindControl(TEXT("Label.Image")));
		ASSERT(NULL != m_pBkImage);

		m_pProgrText = static_cast<CTextUI*>(m_pm.FindControl(TEXT("Text.Progress")));
		ASSERT(NULL != m_pProgrText);

		::SetTimer(m_hWnd, ID_TIMERIMAGESWITCH, 3000, SwitchTimerProc);

		HANDLE hThread = ::CreateThread(NULL, 0, DownLoadThreadProc, this, 0, NULL);

		if (NULL != hThread) {

			::CloseHandle(hThread);
		}
	}
	catch (...) {

	}
}

void CDownload::Notify(TNotifyUI& msg)
{
	try {

		if (msg.sType == TEXT("windowinit")) {

		}
		else if (msg.sType == TEXT("click")) {

			CDuiString CDuiName = msg.pSender->GetName();

			if (CDuiName == TEXT("Btn_Close")) {

				m_GetHttpUrl.CloseDownload();

				::PostQuitMessage(0);
			}
			else if (CDuiName == TEXT("Btn_Min")) {

				::ShowWindow(m_hWnd, SW_MINIMIZE);
			}
		}
		else if (msg.sType == TEXT("msg.errortip")) {

			wstring wstrTemp = (wchar_t*)(msg.wParam);

			if (wstrTemp.length() > 0) {

				m_pProgrText->SetText(wstrTemp.c_str());
			}
		}
		else if (msg.sType == TEXT("Msg.DwonLoad.Show")) {

			CTextUI* pText = (CTextUI*)(msg.wParam);

			if (NULL != pText) {

				pText->SetVisible(TRUE);
			}
		}
		else if (msg.sType == TEXT("Msg.DwonLoad.Speed")) {

			CTextUI* pText = (CTextUI*)(msg.wParam);
			wchar_t* pwchData = reinterpret_cast<wchar_t*>(msg.lParam);

			if ((NULL != pText) && (NULL != pwchData)) {

				pText->SetText(pwchData);
			}
		}
		else if (msg.sType == TEXT("Msg.DwonLoad.TilText")) {

			CTextUI* pText = (CTextUI*)(msg.wParam);
			wchar_t* pwchData = reinterpret_cast<wchar_t*>(msg.lParam);

			if ((NULL != pText) && (NULL != pwchData)) {

				pText->SetText(pwchData);
			}
		}
		else if (msg.sType == TEXT("Msg.DwonLoad.Progress")) {

			CProgressUI* pProgress = (CProgressUI*)(msg.wParam);
			int intPos = int(msg.lParam);

			if (NULL != pProgress) {

				pProgress->SetValue(intPos);
			}
		}
		else if (msg.sType == TEXT("msg.Install.Complete")) {

			::ShowWindow(m_hWnd, SW_HIDE);

			if (NULL == m_pComplete) {

				m_pComplete = new CComplete();
			}

			m_pComplete->Create(NULL, TEXT(""), UI_WNDSTYLE_DIALOG, 0L, 0, 0, 600, 400);
			m_pComplete->CenterWindow();
			m_pComplete->ShowModal();
		}
	}
	catch (...) {

	}
}

VOID CALLBACK CDownload::SwitchTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (NULL != m_pBkImage) {

		switch (m_CurIndex) {

		case 0:
			m_CurIndex = 0;
			::KillTimer(hwnd, idEvent);
			::SetTimer(hwnd, ID_TIMERIMAGEHIDE, 20, HideTimerProc);
			m_NextIndex = 1;
			break;

		case 1:
			m_CurIndex = 1;
			::KillTimer(hwnd, idEvent);
			::SetTimer(hwnd, ID_TIMERIMAGEHIDE, 20, HideTimerProc);
			m_NextIndex = 2;
			break;

		case 2:
			m_CurIndex = 2;
			::KillTimer(hwnd, idEvent);
			::SetTimer(hwnd, ID_TIMERIMAGEHIDE, 20, HideTimerProc);
			m_NextIndex = 0;
			break;

		default:
			break;
		}

	}
}

VOID CALLBACK CDownload::HideTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (NULL != m_pBkImage) {

		wchar_t wchAlpha[128] = { 0 };
		::swprintf_s(wchAlpha, ARRAYSIZE(wchAlpha), TEXT("file=\'%d.png\' fade=\'%d\'"), m_CurIndex, m_Opacity);

		m_pBkImage->SetBkImage(wchAlpha);

		m_Opacity -= 5;

		if (m_Opacity <= 20) {

			m_Opacity = 50;
			::ZeroMemory(wchAlpha, ARRAYSIZE(wchAlpha));
			::swprintf_s(wchAlpha, ARRAYSIZE(wchAlpha), TEXT("file=\'%d.png\' fade=\'%d\'"), m_NextIndex, m_Opacity);

			m_pBkImage->SetBkImage(wchAlpha);

			::KillTimer(hwnd, idEvent);
			::SetTimer(hwnd, ID_TIMERIMAGESHOW, 50, ShowTimerProc);
		}
	}
}

VOID CALLBACK CDownload::ShowTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (NULL != m_pBkImage) {

		wchar_t wchAlpha[128] = { 0 };
		::swprintf_s(wchAlpha, ARRAYSIZE(wchAlpha), TEXT("file=\'%d.png\' fade=\'%d\'"), m_NextIndex, m_Opacity);

		m_pBkImage->SetBkImage(wchAlpha);

		m_Opacity += 6;

		if (m_Opacity >= 255) {

			m_Opacity = 255;

			::KillTimer(hwnd, idEvent);
			::SetTimer(hwnd, ID_TIMERIMAGESWITCH, 3000, SwitchTimerProc);

			m_CurIndex = m_NextIndex;
		}
	}
}

DWORD WINAPI CDownload::DownLoadThreadProc(LPVOID lpParameter)
{
	try {

		CDownload* pDownLoad = static_cast<CDownload*>(lpParameter);

		int inVersion = 0;
		wstring wstrUrl = TEXT("");
		DWORD dwFileSize = 0;

		if (!pDownLoad->m_GetHttpUrl.GetHttpUrl(&wstrUrl, inVersion)) {

			if (false == pDownLoad->m_bClose) {

				pDownLoad->m_pm.SendNotify(pDownLoad->m_pProgrText, TEXT("msg.errortip"), (WPARAM)TEXT("error：获取下载地址出错了!"));
			}

			throw TEXT("error:DownLoadThreadProc()->GetHttpUrl");
		}

		if (!pDownLoad->m_GetHttpUrl.GetInstallZip(g_wchInstallPath, wstrUrl.c_str(), &pDownLoad->m_pm, dwFileSize)) {

			if (false == pDownLoad->m_bClose) {

				pDownLoad->m_pm.SendNotify(pDownLoad->m_pProgrText, TEXT("msg.errortip"), (WPARAM)TEXT("error：下载安装包出错了!"));
			}

			throw TEXT("error:DownLoadThreadProc()->GetInstallZip");
		}

		if (!pDownLoad->m_UnZipFile.UnZipFile(&pDownLoad->m_pm, g_wchDownloadPath, dwFileSize, g_wchInstallPath)) {

			if (false == pDownLoad->m_bClose) {

				pDownLoad->m_pm.SendNotify(pDownLoad->m_pProgrText, TEXT("msg.errortip"), (WPARAM)TEXT("error：解压安装包出错了!"));
			}

			throw TEXT("error:DownLoadThreadProc()->UnZipFile");
		}

		if (!pDownLoad->m_InstallTask.InstallTask(g_wchInstallPath, &pDownLoad->m_pm, inVersion)) {

			if (false == pDownLoad->m_bClose) {

				pDownLoad->m_pm.SendNotify(pDownLoad->m_pProgrText, TEXT("msg.errortip"), (WPARAM)TEXT("error：执行安装过程出错了!"));
			}

			throw TEXT("error:DownLoadThreadProc()->InstallTask");
		}

		if (false == pDownLoad->m_bClose) {

			pDownLoad->m_pm.SendNotify(NULL, TEXT("msg.Install.Complete"));
		}
	}
	catch (...) {

	}

	return 0;
}
