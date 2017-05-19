#include "stdafx.h"
#include "DownloadZip.h"
#include "MainWnd.h"

//下载
CDownloadZip g_DownloadZip;

CDownloadZip::CDownloadZip()
{
	m_pCurl = NULL;
	m_lFileLen = 0;
	m_bEndDownLoad = false;
	m_pMainWnd = NULL;
	m_dDownLen = 0;
}

CDownloadZip::~CDownloadZip()
{

}

bool CDownloadZip::DownLoadZip(LPCTSTR lptInstallPath, LPCTSTR lptLink, int inVersion, LPCTSTR lptConfig, void* pThis)
{
	bool bRet = false;

	try {

		if ((NULL == lptInstallPath) || (NULL == lptLink) || (inVersion < 0) || (NULL == lptConfig) || (NULL == pThis)) {

			return bRet;
		}

		m_pMainWnd = pThis;
		m_lFileLen = 0;
		m_bEndDownLoad = false;
		m_dDownLen = 0;

		namespace fs = boost::filesystem;
		fs::path path(lptLink);

		wstring wstrDownLoadPath = lptInstallPath;
		wstring wstrFileName = path.filename().c_str();

		fs::path filepath(wstrDownLoadPath);
		filepath.append(wstrFileName);
		m_InstallPath = filepath.c_str();

		m_pCurl = curl_easy_init();

		::DeleteFile(filepath.c_str());

		FILE* pFile = NULL;
		::fopen_s(&pFile, filepath.string().c_str(), "ab+");

		if (NULL == pFile) {

			curl_easy_cleanup(m_pCurl);
			return bRet;
		}

		curl_easy_setopt(m_pCurl, CURLOPT_URL, path.string().c_str());
		curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 0L);
		curl_easy_setopt(m_pCurl, CURLOPT_NOBODY, 0L);
		curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(m_pCurl, CURLOPT_RESUME_FROM, m_lFileLen);

		curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteFunc);
		curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, pFile);

		curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSFUNCTION, ProgressFunc);
		curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSDATA, this);

		CURLcode RetCode = curl_easy_perform(m_pCurl);

		long retcode = 0;
		RetCode = curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &retcode);

		if ((RetCode == CURLE_OK) && (retcode == 200)) {

			bRet = CheckFileComplete();
		}
		else {

			bRet = false;
		}

		curl_easy_cleanup(m_pCurl);

		::fclose(pFile);
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}

void CDownloadZip::CancelDownLoad()
{
	m_bEndDownLoad = true;
}

size_t CDownloadZip::WriteFunc(char* pstr, size_t size, size_t nmemb, void* pstream)
{
	return ::fwrite(pstr, size, nmemb, (FILE*)pstream);
}

size_t CDownloadZip::ProgressFunc(void* pData, double dDwonLoadCount, double dCurDwonLoadCount, double uUpCount, double uUpCurCount)
{
	__try {

		if (dDwonLoadCount > -0.1 && dDwonLoadCount < 0.1) {

			return 0;
		}

		CDownloadZip* pThis = static_cast<CDownloadZip*>(pData);

		if (NULL == pThis) {

			return -1;
		}

		CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(pThis->m_pMainWnd);

		if (NULL == pManager) {

			return -1;
		}

		if (pThis->m_bEndDownLoad) {

			return -1;
		}

		double dSpeed = 0;
		curl_easy_getinfo(pThis->m_pCurl, CURLINFO_SPEED_DOWNLOAD, &dSpeed);

		if (0 != dSpeed) {

			wchar_t wchTemp[256] = { 0 };
			double dKb = dSpeed / 1024;

			if (dKb <= 1024) {

				::swprintf_s(wchTemp, ARRAYSIZE(wchTemp), TEXT("(%0.3fKB/S)"), dKb);
			}
			else {

				::swprintf_s(wchTemp, ARRAYSIZE(wchTemp), TEXT("(%0.3fMB/S)"), dKb / 1024);
			}

			static CTextUI* pTileText = NULL;

			if (NULL == pTileText) {

				pTileText = static_cast<CTextUI*>(pManager->FindControl(TEXT("Update.Title.Speed")));
			}

			if (pTileText) {

				pManager->SendNotify(pTileText, TEXT("msg.Title.Text"), (WPARAM)wchTemp);
			}
		}

		pThis->m_dDownLen = (long)dCurDwonLoadCount;
		pThis->m_lFileLen = (long)dDwonLoadCount;
		int nPos = (int)((dCurDwonLoadCount / dDwonLoadCount) * 100);

		static CProgressUI* pProgress = NULL;

		if (NULL == pProgress) {

			pProgress = static_cast<CProgressUI*>(pManager->FindControl(TEXT("Update.Progress")));

			pManager->SendNotify(pProgress, TEXT("msg.DwonLoad.Progress"), (WPARAM)0);
		}

		if (pProgress) {

			pManager->SendNotify(pProgress, TEXT("msg.DwonLoad.Progress"), (WPARAM)nPos);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

	}

	return 0;
}

bool CDownloadZip::CheckFileComplete()
{
	bool bRet = false;

	try {

		//下载长度一致算是成功了(此处应该再加上校验MD5)
		if (m_dDownLen == m_lFileLen) {

			bRet = true;
		}
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}

std::wstring CDownloadZip::RepPath(LPCTSTR lptPath)
{
	wstring wstrRet = TEXT("");

	try {

		while (::wcschr(lptPath, '/') != NULL) {

			wchar_t* pwchPath = (wchar_t*)::wcschr(lptPath, '/');

			if (NULL != pwchPath) {

				*pwchPath = '\\';
			}
		}

		wstrRet = lptPath;
	}
	catch (...) {

		wstrRet = TEXT("");
	}

	return  wstrRet;
}

void CDownloadZip::ExitProcessList()
{
	try {

		HANDLE hToolhelp = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (INVALID_HANDLE_VALUE == hToolhelp) {

			return;
		}

		//结束进程列表
		wstring wstrProcessTable[] = {

		TEXT("RhinoClient.exe"),
		TEXT("RhinoInternetcafes.exe"),
		TEXT("RhinoMonitor.exe"),
		TEXT("Client.exe"),
		TEXT("QtWebEngineProcess.exe"),
		TEXT("Hearthstone.exe"),
		TEXT("LolClient.exe"),
		TEXT("CrossProxy.exe"),
		TEXT("League of Legends.exe") };

		PROCESSENTRY32 process = { 0 };
		process.dwSize = sizeof(PROCESSENTRY32);

		BOOL bNext = ::Process32First(hToolhelp, &process);

		while (bNext) {

			for (int i = 0; i < sizeof(wstrProcessTable) / sizeof(wstring); i++) {

				if (::wcscmp(process.szExeFile, wstrProcessTable[i].c_str()) == 0) {

					HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, process.th32ProcessID);

					if (NULL != hProcess) {

						::TerminateProcess(hProcess, 0);
						::CloseHandle(hProcess);
					}

					break;
				}
			}

			bNext = ::Process32Next(hToolhelp, &process);
		}

		::CloseHandle(hToolhelp);
	}
	catch (...) {

	}
}

bool CDownloadZip::UnpackZip(LPCTSTR lptInstallPath, int inVersion, LPCTSTR lptConfig, LPCTSTR lptTilName)
{
	bool bRet = false;

	try {

		if ((NULL == lptInstallPath) || (inVersion < 0) || (NULL == lptConfig) || (NULL == lptTilName)) {

			return bRet;
		}

		//结束进程列表再解压
		ExitProcessList();

		namespace fs = boost::filesystem;
		fs::path path(m_InstallPath);

		if (!fs::exists((path))) {

			return bRet;
		}

		fs::path dir(lptInstallPath);
		wstring wstrParentPath = TEXT("");

		if (::wcsstr(lptInstallPath, TEXT("\\RhinoClient"))) {

			wstrParentPath = dir.parent_path().c_str();
		}
		else {

			wstrParentPath = lptInstallPath;
		}

		wchar_t wchCurDirectory[1024] = { 0 };
		::GetCurrentDirectory(ARRAYSIZE(wchCurDirectory), wchCurDirectory);

		if (::wcslen(wchCurDirectory) <= 0) {

			return bRet;
		}

		::SetCurrentDirectory(wstrParentPath.c_str());
		HZIP hZip = OpenZip((void*)path.c_str(), 0, ZIP_FILENAME);

		if (NULL != hZip) {

			wchar_t wchFilePath[1024] = { 0 };
			ZIPENTRY ZipEntry = { 0 };
			GetZipItem(hZip, -1, &ZipEntry);

			for (int i = 0; i < ZipEntry.index; i++) {

				ZIPENTRY tZipEntry = { 0 };

				if (ZIP_OK == GetZipItem(hZip, i, &tZipEntry)) {

					::ZeroMemory(wchFilePath, ARRAYSIZE(wchFilePath));
					::swprintf_s(wchFilePath, ARRAYSIZE(wchFilePath), TEXT("%ws\\%ws"), wstrParentPath.c_str(), tZipEntry.name);
					wstring wstrPath = RepPath(wchFilePath);

					DWORD dwAttributes = ::GetFileAttributes(wstrPath.c_str());

					if (dwAttributes & FILE_ATTRIBUTE_READONLY) {

						DWORD dwTempAttribute = dwAttributes & (~FILE_ATTRIBUTE_READONLY);

						::SetFileAttributes(wstrPath.c_str(), dwTempAttribute);
					}

					if (FILE_ATTRIBUTE_DIRECTORY & tZipEntry.attr) {

						if (!fs::is_directory(wstrPath.c_str())) {

							fs::create_directories(wstrPath.c_str());
						}
					}
					else {

						::DeleteFile(wstrPath.c_str());
					}

					UnzipItem(hZip, i, (void*)wstrPath.c_str(), 0, ZIP_FILENAME);
				}
			}
		}

		CloseZip(hZip);
		::SetCurrentDirectory(wchCurDirectory);

		::DeleteFile(path.c_str());

		//配置文件只读处理
		DWORD dwAttributes = ::GetFileAttributes(lptConfig);

		if (dwAttributes & FILE_ATTRIBUTE_READONLY) {

			DWORD dwTempAttribute = dwAttributes & (~FILE_ATTRIBUTE_READONLY);

			::SetFileAttributes(lptConfig, dwTempAttribute);
		}

		wchar_t wchTemp[128] = { 0 };
		::swprintf_s(wchTemp, ARRAYSIZE(wchTemp), TEXT("%d"), inVersion);

		if (::WritePrivateProfileString(TEXT("config"), lptTilName, wchTemp, lptConfig)) {

			bRet = true;
		}
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}