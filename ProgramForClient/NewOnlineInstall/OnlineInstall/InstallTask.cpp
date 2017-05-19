#include "stdafx.h"
#include "InstallTask.h"


CInstallTask::CInstallTask()
{
	m_SetupFlags = TEXT("");
}

CInstallTask::~CInstallTask()
{

}

bool CInstallTask::InstallTask(LPCTSTR lptInstallDir, CPaintManagerUI* pManager, int inVersion)
{
	bool bRet = false;

	try {

		if ((NULL == lptInstallDir) || (NULL == pManager)) {

			return bRet;
		}

		wchar_t wchTemp[256] = { 0 };
		::swprintf_s(wchTemp, ARRAYSIZE(wchTemp), TEXT("正在执行初始化操作，请稍候..."));

		static CTextUI* pText = NULL;

		if (NULL == pText) {

			pText = static_cast<CTextUI*>(pManager->FindControl(TEXT("Text.Download")));
		}

		if (pText) {

			pManager->SendNotify(pText, TEXT("Msg.DwonLoad.Speed"), (WPARAM)pText, (LPARAM)wchTemp);
		}

		bRet = AddDesktopLink(lptInstallDir);

		if (false == bRet) {

			return bRet;
		}

		AddInstall(TEXT(""), g_wchInstallPath);

		WriteVersionInfo(inVersion, g_wchInstallPath);

		StartMonitor(g_wchInstallPath);

		ReportSetup();
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}

bool CInstallTask::AddDesktopLink(LPCTSTR lptLink)
{
	bool bRet = false;

	try {

		if (NULL == lptLink) {

			return bRet;
		}

		namespace fs = boost::filesystem;
		fs::path path(lptLink);

		path.append(TEXT("RhinoClient.exe"));

		if (!fs::exists(path)) {

			return bRet;
		}

		wchar_t wchLinkName[1024] = { 0 };

		if (!GetDesktopLinkName(wchLinkName, ARRAYSIZE(wchLinkName))) {

			return bRet;
		}

		::wcscat_s(wchLinkName, ARRAYSIZE(wchLinkName), TEXT("\\犀牛电竞.lnk"));
		HRESULT hResult = ::CoInitialize(NULL);

		if (S_OK != hResult) {

			return bRet;
		}

		IShellLink* pShellLink = NULL;
		hResult = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShellLink);

		if (SUCCEEDED(hResult)) {

			IPersistFile* pPerFile = NULL;
			pShellLink->SetPath(path.c_str());

			CDuiString strPath = path.c_str();
			pShellLink->SetWorkingDirectory(strPath.Left(strPath.ReverseFind('\\')));
			hResult = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPerFile);

			if (SUCCEEDED(hResult)) {

				pPerFile->Save(T2BSTR(wchLinkName), TRUE);
				pPerFile->Release();

				bRet = true;
			}

			pPerFile->Release();
		}

		::CoUninitialize();
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}

bool CInstallTask::GetDesktopLinkName(LPTSTR lptName, int inLen)
{
	bool bRet = false;

	try {

		if ((NULL == lptName) || (inLen <= 0)) {

			return bRet;
		}

		if (S_OK != ::CoInitialize(NULL)) {

			return bRet;
		}

		LPITEMIDLIST pidlBeginAt = NULL;

		if (S_OK == ::SHGetSpecialFolderLocation(HWND_DESKTOP, CSIDL_DESKTOPDIRECTORY, &pidlBeginAt)) {

			wchar_t wchPath[1024] = { 0 };

			BOOL bSHRet = ::SHGetPathFromIDList(pidlBeginAt, wchPath);

			if ((TRUE == bSHRet) && (::wcslen(wchPath) > 0)) {

				::wcscpy_s(lptName, inLen, wchPath);
				bRet = true;
			}
		}

		::CoUninitialize();
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}

void CInstallTask::AddInstall(LPCTSTR lptAppPath, LPCTSTR lptDirectory)
{
	try {

		if ((NULL == lptAppPath) || (NULL == lptDirectory)) {

			return;
		}

		namespace fs = boost::filesystem;
		HKEY hKey = NULL;

		LSTATUS lStatus = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), 0, KEY_ENUMERATE_SUB_KEYS, &hKey);

		if (ERROR_SUCCESS != lStatus) {

			return;
		}

		HKEY hKeyValue = NULL;
		lStatus = ::RegOpenKey(hKey, TEXT("Rhino81D6A461-B578-4615-90B1-51675927B00D"), &hKeyValue);

		if (ERROR_SUCCESS != lStatus) {

			lStatus = ::RegCreateKey(hKey, TEXT("Rhino81D6A461-B578-4615-90B1-51675927B00D"), &hKeyValue);

			if (ERROR_SUCCESS != lStatus) {

				return;
			}
		}

		fs::path IconPath(lptDirectory);
		IconPath.append(TEXT("RhinoClient.exe"));
		wstring wstrFilePath = IconPath.c_str();

		::RegSetValueEx(hKeyValue, TEXT("DisplayIcon"), 0, REG_SZ, (const BYTE*)wstrFilePath.c_str(), wstrFilePath.length() * sizeof(wchar_t));

		wstring wstrDisplay = TEXT("犀牛电竞平台");
		::RegSetValueEx(hKeyValue, TEXT("DisplayName"), 0, REG_SZ, (const BYTE*)wstrDisplay.c_str(), wstrDisplay.length() * sizeof(wchar_t));

		wstring wstrPublisher = TEXT("犀牛电竞");
		::RegSetValueEx(hKeyValue, TEXT("Publisher"), 0, REG_SZ, (const BYTE*)wstrPublisher.c_str(), wstrPublisher.length() * sizeof(wchar_t));

		fs::path UnInstallpath(lptDirectory);
		UnInstallpath.append(TEXT("RhinoUnInstall.exe"));
		wstring wstrUnInstallPath = UnInstallpath.c_str();

		::RegSetValueEx(hKeyValue, TEXT("UninstallString"), 0, REG_SZ, (const BYTE*)wstrUnInstallPath.c_str(), wstrUnInstallPath.length() * sizeof(wchar_t));

		::RegCloseKey(hKeyValue);
		::RegCloseKey(hKey);
	}
	catch (...) {

	}
}

void CInstallTask::WriteVersionInfo(int inVersion, LPCTSTR lptDirectory)
{
	try {

		if ((inVersion < 0) || (NULL == lptDirectory)) {

			return;
		}

		namespace fs = boost::filesystem;
		fs::path path(lptDirectory);

		path.append(TEXT("update.ini"));
		wchar_t wchTemp[1024] = { 0 };
		::swprintf_s(wchTemp, ARRAYSIZE(wchTemp), TEXT("%d"), inVersion);

		//配置文件只读处理
		DWORD dwAttributes = ::GetFileAttributes(path.c_str());

		if (dwAttributes & FILE_ATTRIBUTE_READONLY) {

			DWORD dwTempAttribute = dwAttributes & (~FILE_ATTRIBUTE_READONLY);

			::SetFileAttributes(path.c_str(), dwTempAttribute);
		}

		//主版本
		::WritePrivateProfileString(TEXT("config"), TEXT("version"), wchTemp, path.c_str());
		//子版本
		::WritePrivateProfileString(TEXT("config"), TEXT("smallversion"), TEXT("0"), path.c_str());

		//安装标记
		wchar_t wchFlags[1024] = { 0 };
		::GetModuleFileName(::GetModuleHandle(NULL), wchFlags, ARRAYSIZE(wchFlags));

		fs::path pathFlags(wchFlags);
		pathFlags.remove_filename();
		pathFlags.append(TEXT("set.ini"));

		if (fs::exists(pathFlags)) {

			wchar_t wchSource[128] = { 0 };
			::GetPrivateProfileString(TEXT("config"), TEXT("Source"), TEXT("错误"), wchSource, ARRAYSIZE(wchSource), pathFlags.c_str());

			if ((::wcscmp(wchSource, TEXT("错误")) != 0) && (::wcslen(wchSource) > 0)) {

				if (TRUE == ::WritePrivateProfileString(TEXT("config"), TEXT("source"), wchSource, path.c_str())) {

					m_SetupFlags = wchSource;
				}
			}
		}
		else {

			m_SetupFlags = TEXT("0");
		}
	}
	catch (...) {

	}
}

void CInstallTask::StartMonitor(LPCTSTR lptDirectory)
{
	try {

		if (NULL == lptDirectory) {

			return;
		}

		namespace fs = boost::filesystem;
		fs::path path(lptDirectory);

		path.append(TEXT("RhinoMonitor.exe"));

		if (fs::exists(path)) {

			SHELLEXECUTEINFO ShExecInfo = { 0 };
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = NULL;
			ShExecInfo.lpFile = path.c_str();
			ShExecInfo.lpParameters = TEXT("/uninstall");
			ShExecInfo.lpDirectory = NULL;
			ShExecInfo.nShow = SW_SHOW;
			ShExecInfo.hInstApp = NULL;

			if (::ShellExecuteEx(&ShExecInfo)) {

				if (NULL != ShExecInfo.hProcess) {

					::CloseHandle(ShExecInfo.hProcess);
				}
			}

			::Sleep(3000);

			::ZeroMemory(&ShExecInfo, sizeof(SHELLEXECUTEINFO));
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = NULL;
			ShExecInfo.lpFile = path.c_str();
			ShExecInfo.lpParameters = TEXT("/install");
			ShExecInfo.lpDirectory = NULL;
			ShExecInfo.nShow = SW_SHOW;
			ShExecInfo.hInstApp = NULL;

			if (::ShellExecuteEx(&ShExecInfo)) {

				::CloseHandle(ShExecInfo.hProcess);
			}
		}
	}
	catch (...) {

	}
}

void CInstallTask::ReportSetup()
{
	try {

		if (m_SetupFlags.length() <= 0) {

			return;
		}

		wchar_t wchKey[2048] = { 0 };

		if (!GetDiskKey(wchKey, ARRAYSIZE(wchKey))) {

			return;
		}

		int inFlags = ::wcstol(m_SetupFlags.c_str(), NULL, 10);
		string strDiskKey = CW2A(wchKey);

		char chReportUrl[4096] = { 0 };
		::sprintf_s(chReportUrl, ARRAYSIZE(chReportUrl), ID_SETUPCOMPLETEADDR, inFlags, strDiskKey.c_str());

		wstring wstrPost = CA2W(chReportUrl);
		m_HttpPost.RepointInstall(wstrPost.c_str());
	}
	catch (...) {

	}
}

bool CInstallTask::GetDiskKey(LPWSTR lpwstr, int inLen)
{
	bool bRet = false;

	try {

		if ((NULL == lpwstr) || (inLen <= 0)) {

			return bRet;
		}

		TCHAR tBuffer[1024] = { 0 };
		DWORD dwLen = ::GetLogicalDriveStrings(sizeof(tBuffer) / sizeof(TCHAR), tBuffer);
		wstring wstrKey = TEXT("");
		int inCount = 0;

		for (TCHAR* ptDisk = tBuffer; *ptDisk; ptDisk += ::_tcslen(ptDisk) + 1, inCount++) {

			wchar_t wchDiskName[128] = { 0 };
			DWORD namesize = ARRAYSIZE(wchDiskName);

			DWORD serialnumber = 0;
			DWORD maxlen = 0;
			DWORD fileflag = 0;

			wchar_t systemnamebuf[128] = { 0 };
			DWORD systemnamesize = ARRAYSIZE(systemnamebuf);

			if (::GetVolumeInformation(ptDisk, wchDiskName, namesize, &serialnumber, &maxlen, &fileflag, systemnamebuf, systemnamesize)) {

				wchar_t wchKey[128] = { 0 };
				::swprintf_s(wchKey, ARRAYSIZE(wchKey), TEXT("%08x"), serialnumber);

				wstrKey += wchKey;
			}

			if (inCount >= 1) {

				break;
			}
		}

		::wcscpy_s(lpwstr, inLen, wstrKey.c_str());

		if (wstrKey.length() > 0) {

			bRet = true;
		}
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}