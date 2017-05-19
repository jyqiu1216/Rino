#include "stdafx.h"
#include "GetHttpUrl.h"


CGetHttpUrl::CGetHttpUrl()
{
	m_pCurl = NULL;
	m_lFileLen = 0;
	m_dDownLen = 0;
	m_pManager = NULL;
	m_bEndDownLoad = false;

	curl_global_init(CURL_GLOBAL_DEFAULT);
}

CGetHttpUrl::~CGetHttpUrl()
{
	curl_global_cleanup();
}

bool CGetHttpUrl::GetHttpUrl(wstring* pwstrUrl, int& inVersion)
{
	bool bRet = false;

	try {

		if (NULL == pwstrUrl) {

			return bRet;
		}

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

			bRet = CheckHttpLink(strBuffer, pwstrUrl, &inVersion);
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

int CGetHttpUrl::WriteHttpData(char* pchData, size_t size, size_t nmemb, string* pWriterData)
{
	unsigned long sizes = size * nmemb;

	if (NULL == pWriterData) {

		return 0;
	}

	pWriterData->append(pchData, sizes);

	return sizes;
}

bool CGetHttpUrl::CheckHttpLink(string strData, wstring* pwstrData, int* pinVerison)
{
	bool bRet = false;

	try {

		if ((strData.length() <= 0) || (NULL == pwstrData) || (NULL == pinVerison)) {

			return bRet;
		}

		Document DecJson;
		DecJson.Parse<0>(strData.c_str());

		if (!DecJson.IsObject()) {

			return bRet;
		}

		Value& JsonValue = DecJson["route"];
		string strHttpLink = JsonValue.GetString();

		Value& JsonVersion = DecJson["version"];
		string strVersion = JsonVersion.GetString();

		if (strVersion.length() > 0) {

			*pinVerison = ::atoi(strVersion.c_str());
		}

		if (strHttpLink.length() > 0) {

			*pwstrData = CA2W(strHttpLink.c_str());

			bRet = true;
		}
	}
	catch (...) {

		bRet = false;
	}

	return bRet;
}

size_t CGetHttpUrl::WriteFunc(char* pstr, size_t size, size_t nmemb, void* pstream)
{
	return ::fwrite(pstr, size, nmemb, (FILE*)pstream);
}

size_t CGetHttpUrl::ProgressFunc(void* pData, double dDwonLoadCount, double dCurDwonLoadCount, double uUpCount, double uUpCurCount)
{
	__try {

		if (dDwonLoadCount > -0.1 && dDwonLoadCount < 0.1) {

			return 0;
		}

		CGetHttpUrl* pThis = static_cast<CGetHttpUrl*>(pData);

		if (NULL == pThis) {

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

				::swprintf_s(wchTemp, ARRAYSIZE(wchTemp), TEXT("请耐心等待，马上就好！当前速度：%0.3fKB/S"), dKb);
			}
			else {

				::swprintf_s(wchTemp, ARRAYSIZE(wchTemp), TEXT("请耐心等待，马上就好！当前速度：%0.3fMB/S"), dKb / 1024);
			}

			static CTextUI* pText = NULL;

			if (NULL == pText) {

				pText = static_cast<CTextUI*>(pThis->m_pManager->FindControl(TEXT("Text.Download")));
				pThis->m_pManager->SendNotify(pText, TEXT("Msg.DwonLoad.Show"), (WPARAM)pText);
			}

			if (pText) {

				pThis->m_pManager->SendNotify(pText, TEXT("Msg.DwonLoad.Speed"), (WPARAM)pText, (LPARAM)wchTemp);
			}
		}

		pThis->m_dDownLen = dCurDwonLoadCount;
		pThis->m_lFileLen = (long)dDwonLoadCount;
		int nPos = (int)((dCurDwonLoadCount / dDwonLoadCount) * 100);

		static CTextUI* pTileText = NULL;

		if (NULL == pTileText) {

			pTileText = static_cast<CTextUI*>(pThis->m_pManager->FindControl(TEXT("Text.Progress")));
		}

		if (pTileText) {

			wchar_t wchTemp[256] = { 0 };
			::swprintf_s(wchTemp, ARRAYSIZE(wchTemp), TEXT("安装进度... %d%%"), nPos);

			pThis->m_pManager->SendNotify(pTileText, TEXT("Msg.DwonLoad.TilText"), (WPARAM)pTileText, (LPARAM)wchTemp);
		}

		static CProgressUI* pProgress = NULL;

		if (NULL == pProgress) {

			pProgress = static_cast<CProgressUI*>(pThis->m_pManager->FindControl(TEXT("Progress.Download")));

			pThis->m_pManager->SendNotify(pProgress, TEXT("Msg.DwonLoad.Progress"), (WPARAM)pProgress, (LPARAM)0);
		}

		if (pProgress) {

			pThis->m_pManager->SendNotify(pProgress, TEXT("Msg.DwonLoad.Progress"), (WPARAM)pProgress, (LPARAM)nPos);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

	}

	return 0;
}

bool CGetHttpUrl::GetInstallZip(LPCTSTR lptFileName, LPCTSTR lptLink, CPaintManagerUI* pManager, DWORD& dwFileSize)
{
	bool bRet = false;

	try {

		if ((NULL == lptFileName) || (NULL == lptLink) || (NULL == pManager)) {

			return bRet;
		}

		m_pManager = pManager;
		m_bEndDownLoad = false;

		namespace fs = boost::filesystem;
		fs::path path(lptLink);

		if (!fs::is_directory(lptFileName)) {

			fs::create_directories(lptFileName);
		}

		wstring wstrFileName = path.filename().wstring();
		fs::path filenames(lptFileName);
		filenames.append(wstrFileName);
		::wcscpy_s(g_wchDownloadPath, ARRAYSIZE(g_wchDownloadPath), filenames.c_str());

		string strLink = CW2A(lptLink);

		string strBuffer = "";
		m_pCurl = curl_easy_init();

		::DeleteFile(filenames.c_str());

		FILE* pFile = NULL;
		::fopen_s(&pFile, filenames.string().c_str(), "ab+");

		if (NULL == pFile) {

			curl_easy_cleanup(m_pCurl);
			return bRet;
		}

		curl_easy_setopt(m_pCurl, CURLOPT_URL, strLink.c_str());
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

			bRet = CheckDownloadFile();

			if (bRet) {

				dwFileSize = m_lFileLen;
			}
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

void CGetHttpUrl::CloseDownload()
{
	m_bEndDownLoad = true;
}

bool CGetHttpUrl::RepointInstall(LPCTSTR lptData)
{
	bool bRet = false;

	try {

		if (NULL == lptData) {

			return bRet;
		}

		string strSendGet = CW2A(lptData);

		string strBuffer = "";
		m_pCurl = curl_easy_init();

		for (int i = 0; (i < 5) && (false == bRet); i++) {

			char chErrorBuffer[CURL_ERROR_SIZE] = { 0 };

			curl_easy_setopt(m_pCurl, CURLOPT_ERRORBUFFER, chErrorBuffer);
			curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L);
			curl_easy_setopt(m_pCurl, CURLOPT_URL, strSendGet.c_str());
			curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);
			curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteHttpData);
			curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &strBuffer);

			CURLcode RetCode = curl_easy_perform(m_pCurl);

			long retcode = 0;
			RetCode = curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &retcode);

			if ((RetCode == CURLE_OK) && (retcode == 200)) {

				int inValue = ::atoi(strBuffer.c_str());

				if (1 == inValue) {

					bRet = true;
				}
			}
			else {

				bRet = false;
			}

			if (bRet) {

				break;
			}
			else {

				::Sleep(1000);
			}
		}

		curl_easy_cleanup(m_pCurl);
	}
	catch (...) {

		bRet = true;
	}

	return bRet;
}

bool CGetHttpUrl::CheckDownloadFile()
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