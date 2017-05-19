#include "stdafx.h"
#include "GetUpdate.h"

//下载
extern CDownloadZip g_DownloadZip;

CGetUpdate::CGetUpdate()
{
	m_pCurl = NULL;
	m_VerLink.clear();
	m_pMainWnd = NULL;

	curl_global_init(CURL_GLOBAL_DEFAULT);
}

CGetUpdate::~CGetUpdate()
{
	curl_global_cleanup();
}

bool CGetUpdate::GetUpdate(void* pThis)
{
	bool bRet = false;

	try {

		if (NULL == pThis) {

			return bRet;
		}

		m_pMainWnd = pThis;

		string strBuffer = "";
		m_pCurl = curl_easy_init();
		char chErrorBuffer[CURL_ERROR_SIZE] = { 0 };

		curl_easy_setopt(m_pCurl, CURLOPT_ERRORBUFFER, chErrorBuffer);
		curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(m_pCurl, CURLOPT_URL, ID_GETHTTPADDR);
		curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteHttpData);
		curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &strBuffer);

		CURLcode RetCode = curl_easy_perform(m_pCurl);

		long retcode = 0;
		RetCode = curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &retcode);

		if ((RetCode == CURLE_OK) && (retcode == 200)) {

			bRet = ParseVersion(strBuffer);
		}
		else {

			bRet = false;
		}

		curl_easy_cleanup(m_pCurl);
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}

int CGetUpdate::WriteHttpData(char* pchData, size_t size, size_t nmemb, string* pWriterData)
{
	unsigned long sizes = size * nmemb;

	if (NULL == pWriterData) {

		return 0;
	}

	pWriterData->append(pchData, sizes);

	return sizes;
}

bool CGetUpdate::ParseVersion(string& strData)
{
	bool bRet = false;

	try {

		wstring wstrData = CA2W(strData.c_str());

		if (wstrData.length() <= 0) {

			return bRet;
		}

		m_pVerLink = ::make_pair(0, TEXT(""));
		m_VerLink.clear();

		Document DecJson;
		DecJson.Parse<0>(strData.c_str());

		if (!DecJson.IsObject()) {

			return bRet;
		}

		Value& JsonVersion = DecJson["version"];
		string strVersion = JsonVersion.GetString();
		int inVersion = ::atoi(strVersion.c_str());

		Value& JsonValue = DecJson["route"];
		string strHttpLink = JsonValue.GetString();
		wstring wstrLink = CA2W(strHttpLink.c_str());

		m_pVerLink = ::make_pair(inVersion, wstrLink);

		Value& JsonObject = DecJson["smallversions"];

		if (JsonObject.IsArray()) {

			for (size_t i = 0; i < JsonObject.Size(); ++i) {

				Value& JsionItem = JsonObject[i];

				if (JsionItem.IsObject()) {

					Value& JsonItemVer = JsionItem["version"];
					string strVersion = JsonItemVer.GetString();
					int inItemVersion = ::atoi(strVersion.c_str());

					Value& JsonItemLink = JsionItem["route"];
					string strHttpLink = JsonItemLink.GetString();
					wstring wstrItemLink = CA2W(strHttpLink.c_str());

					m_VerLink.insert(::make_pair(inItemVersion, wstrItemLink));
				}
			}
		}

		if (((m_pVerLink.first > 0) && (m_pVerLink.second.length() > 0)) /*&& (m_VerLink.size() > 0)*/) {

			bRet = true;
		}
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}

std::wstring CGetUpdate::GetAppPath()
{
	wstring wstrRet = TEXT("");

	try {

		wchar_t wchAppPath[1024] = { 0 };
		::GetModuleFileName(::GetModuleHandle(NULL), wchAppPath, ARRAYSIZE(wchAppPath));

		namespace fs = boost::filesystem;
		fs::path path(wchAppPath);

		path.remove_filename();

		if (fs::is_directory(path)) {

			wstrRet = path.c_str();
		}
	}
	catch (...) {

		wstrRet = TEXT("");
	}

	return wstrRet;
}

bool CGetUpdate::CheckVersion(void* pThis)
{
	bool bRet = false;

	try {

		if ((NULL == pThis) || ((m_pVerLink.first <= 0) || (m_pVerLink.second.length() <= 0)) /*|| (m_VerLink.size() <= 0)*/) {

			return bRet;
		}

		m_pMainWnd = pThis;

		wstring wstrPath = GetAppPath();

		if (wstrPath.length() <= 0) {

			return bRet;
		}

		boost::filesystem::path path(wstrPath.c_str());
		path.append(TEXT("update.ini"));

		if (!boost::filesystem::exists(path)) {

			return bRet;
		}

		CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(m_pMainWnd);

		if (NULL == pManager) {

			return bRet;
		}

		static CTextUI* pTileText = NULL;

		if (NULL == pTileText) {

			pTileText = static_cast<CTextUI*>(pManager->FindControl(TEXT("Update.Title.Version")));
		}

		if (pTileText) {

			wchar_t wchTemp[128] = { 0 };
			::swprintf_s(wchTemp, ARRAYSIZE(wchTemp), TEXT("(版本:%d.0) 平台正在更新..."), m_pVerLink.first);

			pManager->SendNotify(pTileText, TEXT("msg.Title.Text"), (WPARAM)wchTemp);
		}

		//主版本检测
		bRet = CheckMajor(wstrPath.c_str(), path.c_str(), m_pVerLink.first, m_pVerLink.second.c_str());

		if (false == bRet) {

			return bRet;
		}

		bRet = g_DownloadZip.UnpackZip(wstrPath.c_str(), m_pVerLink.first, path.c_str(), TEXT("version"));

		//子版本检测
		for (tvVerLink::iterator i = m_VerLink.begin(); i != m_VerLink.end(); ++i) {

			if (pTileText) {

				wchar_t wchTemp[128] = { 0 };
				::swprintf_s(wchTemp, ARRAYSIZE(wchTemp), TEXT("(版本:%d.%d) 平台正在更新..."), m_pVerLink.first, i->first);

				pManager->SendNotify(pTileText, TEXT("msg.Title.Text"), (WPARAM)wchTemp);
			}

			bRet = CheckMinVer(wstrPath.c_str(), path.c_str(), i->first, i->second.c_str());

			if (false == bRet) {

				return bRet;
			}

			bRet = g_DownloadZip.UnpackZip(wstrPath.c_str(), i->first, path.c_str(), TEXT("smallversion"));
		}

		bRet = true;
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}

bool CGetUpdate::CheckMajor(LPCTSTR lptDirecotry, LPCTSTR lptConfig, int inVerions, LPCTSTR lptLink)
{
	bool bRet = false;

	try {

		if ((NULL == lptDirecotry) || (NULL == lptConfig) || (inVerions < 0) || (NULL == lptLink)) {

			return bRet;
		}

		wchar_t wchTemp[1024] = { 0 };
		::GetPrivateProfileString(TEXT("config"), TEXT("version"), TEXT("错误"), wchTemp, ARRAYSIZE(wchTemp), lptConfig);

		if ((::wcscmp(wchTemp, TEXT("错误")) != 0) && (::wcslen(wchTemp) > 0)) {

			int inCurVersion = ::wcstol(wchTemp, NULL, 10);

			if (inCurVersion < inVerions) {
				//需要更新大版本

				bRet = g_DownloadZip.DownLoadZip(lptDirecotry, lptLink, inVerions, lptConfig, m_pMainWnd);
			}
			else {

				bRet = true;
			}
		}
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}

bool CGetUpdate::CheckMinVer(LPCTSTR lptDirecotry, LPCTSTR lptConfig, int inVerions, LPCTSTR lptLink)
{
	bool bRet = false;

	try {

		if ((NULL == lptDirecotry) || (NULL == lptConfig) || (inVerions < 0) || (NULL == lptLink)) {

			return bRet;
		}

		wchar_t wchTemp[1024] = { 0 };
		::GetPrivateProfileString(TEXT("config"), TEXT("smallversion"), TEXT("错误"), wchTemp, ARRAYSIZE(wchTemp), lptConfig);

		if ((::wcscmp(wchTemp, TEXT("错误")) != 0) && (::wcslen(wchTemp) > 0)) {

			int inCurVersion = ::wcstol(wchTemp, NULL, 10);

			if (inCurVersion < inVerions) {
				//需要更新小版本

				bRet = g_DownloadZip.DownLoadZip(lptDirecotry, lptLink, inVerions, lptConfig, m_pMainWnd);
			}
			else {

				bRet = true;
			}
		}
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}
