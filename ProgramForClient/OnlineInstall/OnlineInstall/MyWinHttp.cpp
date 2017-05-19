#include "StdAfx.h"
#include <winhttp.h> 
#include "MyWinHttp.h"
#include "HDSerial.h"
#include "atlbase.h"

#pragma comment(lib,"winhttp.lib")
#pragma comment(lib,"user32.lib")

CMyWinHttp::CMyWinHttp(void)
{
	//tellHttpServerDownFinish_test("啊啊发头发哥哥2");
	//tellHttpServerDownFinish();
}

CMyWinHttp::~CMyWinHttp(void)
{
}

//通知服务器下载安装成功
void CMyWinHttp::tellHttpServerDownFinish(int& iSource)
{
	std::string strRoot = GetAppPath();
	iSource = 0;
	//下载完成后更新时间
	CDuiString strIni;
	strIni.Format("%s\\%s", strRoot.c_str(), "set.ini");
	int itag = GetPrivateProfileInt("config", "Source", 0, strIni.GetData());
	iSource = itag;
	std::string str_res;
	wchar_t ws[MAX_PATH] = {0};
	MyHDSerial _MyHDSerial;
	char sHDSerial[100] = {0};
	_MyHDSerial.GetHDSerial(sHDSerial);
	std::wstring wstr = CA2W(sHDSerial);
	swprintf_s(ws, L"/count/installrecord?source=%d&serialnum=%s", itag, wstr.c_str());
	wstr = L"sdp_test 通知服务器下载安装成功";
	wstr += ws;
	OutputDebugStringW(wstr.c_str());

	GetHttpData(HTTP_WEB_ADDRESS_SERVER2, INTERNET_DEFAULT_HTTP_PORT, ws, str_res);

	char res[MAX_PATH] = { 0 };
	sprintf_s(res, "sdp_test CMyWinHttp res=%s", str_res.c_str());
	OutputDebugStringA(res);
}

//通知服务器下载安装成功_测试
void CMyWinHttp::tellHttpServerDownFinish_test(char* sHDSerial)
{
	//char sHDSerial[100] = { 0 };
	std::string str_res;
	wchar_t ws[MAX_PATH] = { 0 };
	std::wstring wstr = CA2W(sHDSerial);
	swprintf_s(ws, L"/count/installrecord?source=测试&serialnum=%s", wstr.c_str());
	wstr = L"sdp_test 通知服务器下载安装成功";
	wstr += ws;
	OutputDebugStringW(wstr.c_str());

	GetHttpData(HTTP_WEB_ADDRESS_SERVER2, INTERNET_DEFAULT_HTTP_PORT, ws, str_res);
	char res[MAX_PATH] = {0};
	sprintf_s(res, "sdp_test CMyWinHttp res2=%s", str_res.c_str());
	OutputDebugStringA(res);
}

//获取当前最新版本下载地址
void CMyWinHttp::GetHttpDownAddress(std::string &str_res)
{
	GetHttpData(HTTP_WEB_ADDRESS_SERVER2, INTERNET_DEFAULT_HTTP_PORT, L"/api/fight/version", str_res);
}

void CMyWinHttp::GetHttpData(std::wstring str_web_addr, int iport, std::wstring str_web_content, std::string &str_res)
{
	str_res = "";
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer = NULL;
	std::string str_res2;//结果数据
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	BOOL  bResults = FALSE;

	hSession = WinHttpOpen(L"User Agent", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	if (hSession)
	{
		hConnect = WinHttpConnect(hSession, str_web_addr.c_str(), /*INTERNET_DEFAULT_HTTP_PORT*/iport, 0);
		if (nullptr == hConnect)
		{
			DWORD res_err = GetLastError();
			printf("%ld", res_err);
		}
	}

	if (hConnect)
	{
		hRequest = WinHttpOpenRequest(hConnect, L"GET", str_web_content.c_str(), L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
	}

	if (hRequest)
	{
		bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	}

	if (bResults)
	{
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	}

	if (bResults)
	{
		do
		{
			// Check for available data.

			dwSize = 0;

			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
			{
				printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());

				break;
			}

			if (!dwSize)
				break;

			pszOutBuffer = new char[dwSize + 1];

			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				break;
			}

			ZeroMemory(pszOutBuffer, dwSize + 1);

			if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
			{
				printf("Error %u in WinHttpReadData.\n", GetLastError());
			}
			else
			{
				//printf("%s", pszOutBuffer);
				str_res2 += pszOutBuffer;
			}

			delete[] pszOutBuffer;

			if (!dwDownloaded)
				break;

		} while (dwSize > 0);
	}



	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	char* res = nullptr;
	res = U2G(str_res2.c_str());
	str_res = res;
	delete res;
}