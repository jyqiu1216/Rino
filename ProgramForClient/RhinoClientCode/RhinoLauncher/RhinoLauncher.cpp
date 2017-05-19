#include "stdafx.h"
#include "RhinoLauncher.h"
#include <windows.h>
#include <winhttp.h> 
#include <Psapi.h>
#include "atlbase.h"
#include "commonfunction_c.h"
#include "json.h"
#pragma comment(lib,"winhttp.lib")
#pragma comment(lib,"user32.lib")

//
// GetDebugPriv
// 在 Windows NT/2000/XP 中可能因权限不够导致以上函数失败
// 如以　System 权限运行的系统进程，服务进程
// 用本函数取得　debug 权限即可,Winlogon.exe 都可以终止哦 :)
//
BOOL GetDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	return TRUE;
}

//
// FindProcess
// 这个函数唯一的参数是你指定的进程名，如:你的目标进程
// 是 "Notepad.exe",返回值是该进程的ID，失败返回0
//
DWORD FindProcess(char *strProcessName)
{
	DWORD aProcesses[1024], cbNeeded, cbMNeeded;
	HMODULE hMods[1024];
	HANDLE hProcess;
	char szProcessName[MAX_PATH];
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) return 0;
	for (int i = 0; i < (int)(cbNeeded / sizeof(DWORD)); i++)
	{
		//_tprintf(_T("%d/t"), aProcesses[i]);  
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
		EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
		GetModuleFileNameExA(hProcess, hMods[0], szProcessName, sizeof(szProcessName));

		if (strstr(szProcessName, strProcessName))
		{
			//_tprintf(_T("%s;"), szProcessName);  
			return (aProcesses[i]);
		}
		//_tprintf(_T("/n"));  
	}
	return 0;
}

//
// Function: ErrorForce
// 此函数中用上面的 FindProcess 函数获得你的目标进程的ID
// 用WIN API OpenPorcess 获得此进程的句柄，再以TerminateProcess
// 强制结束这个进程
//
void KillProcess(char *strProcessName)
{
	GetDebugPriv();
	// When the all operation fail this function terminate the "winlogon" Process for force exit the system.  
	HANDLE hYourTargetProcess = OpenProcess(PROCESS_TERMINATE |
		PROCESS_QUERY_INFORMATION |   // Required by Alpha  
		PROCESS_CREATE_THREAD |   // For CreateRemoteThread  
		PROCESS_VM_OPERATION |   // For VirtualAllocEx/VirtualFreeEx  
		PROCESS_VM_WRITE,             // For WriteProcessMemory  
		FALSE, FindProcess(strProcessName));
	if (hYourTargetProcess == NULL)
	{
		return;
	}
	bool bres = TerminateProcess(hYourTargetProcess, 0);
	return;
}

CRhinoLauncher::CRhinoLauncher()
{

}

CRhinoLauncher::~CRhinoLauncher()
{

}

/*
std::string version 主版本
std::string smallversion 子版本
std::string _filename 下载的文件
std::string strDownLoadUpdatepADDR 下载的根地址
std::string startgamename 启动的程序名称
*/
bool CRhinoLauncher::updateOnline(std::string version, std::string smallversion, std::string _filename, std::string strDownLoadUpdatepADDR, std::string startgamename)
{
	// 构造命令行
	char strCommonLine[MAX_PATH] = { 0 };
	std::string str_CommonLine;
	char _str_exe[100] = { 0 };
	std::string _strAppPath = G2U(GetAppPath().c_str());
#ifdef _DEBUG
	sprintf_s(_str_exe, "Update_d.exe");
#else
	sprintf_s(_str_exe, "Update.exe");
#endif // _DEBUG
	sprintf_s(strCommonLine, ("%s /nb:%s%s/ne /v1b:%s/v1e /v2b:%s/v2e /ub:%s/ue /pb:")
		, _str_exe, _strAppPath.c_str(), startgamename.c_str(), version.c_str(), smallversion.c_str(), strDownLoadUpdatepADDR.c_str());


	DWORD dwSize = 0;
	str_CommonLine += strCommonLine;
	str_CommonLine += _filename;

	char _str[MAX_PATH * 10] = { 0 };
	sprintf_s(_str, "%s/pe /sb:%lu/se", str_CommonLine.c_str(), dwSize);
	std::string strCommand = U2G(_str);
	std::string _address = strCommand;
	STARTUPINFOA StartInfo;
	memset(&StartInfo, 0, sizeof(StartInfo));
	StartInfo.cb = sizeof(StartInfo);
	StartInfo.wShowWindow = SW_SHOWNORMAL;
	PROCESS_INFORMATION 	m_ProcessInfo;
	MyDebugString("sdp_test", "在线更新数据 %s", _address.c_str());
	BOOL bSuccess = CreateProcessA(NULL, /*_str*/(LPSTR)_address.c_str(), NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &StartInfo, &m_ProcessInfo);

	if (bSuccess) {

		::CloseHandle(m_ProcessInfo.hThread);
		::CloseHandle(m_ProcessInfo.hProcess);
	}

	ExitProcess(0);
	return true;
}

#include <assert.h>
bool CRhinoLauncher::VersionUpdate()
{
	std::string route;		//主版本下载地址
	std::string version;	//主版本号
	std::string info;		//主版本描述信息
	Json::Value _smallversion;	//子版本数据
	std::string _webroot;		//网页根路径
	std::wstring _wwebroot;		//网页根路径
#ifdef _DEBUG
	std::string _exe_name = "RhinoClient_d.exe";	//启动程序名称
#else
	std::string _exe_name = "RhinoClient.exe";		//启动程序名称
#endif // _DEBUG
////////////////
	//文件操作
	std::string str_file = GetAppPath();
	str_file += "Config\\clientconfig.json";

	//配置文件只读处理
	DWORD dwAttributes = ::GetFileAttributesA(str_file.c_str());

	if (dwAttributes & FILE_ATTRIBUTE_READONLY) {

		DWORD dwTempAttribute = dwAttributes & (~FILE_ATTRIBUTE_READONLY);

		::SetFileAttributesA(str_file.c_str(), dwTempAttribute);
	}

	HANDLE hFile = ::CreateFileA(str_file.c_str(),		//创建文件的名称。
		GENERIC_READ,									// 写和读文件。
		0,												// 不共享读写。
		NULL,											// 缺省安全属性。
		OPEN_EXISTING,									// 文件必须已经存在。由设备提出要求。
		FILE_ATTRIBUTE_NORMAL,							// 一般的文件。       
		NULL);											// 模板文件为空。
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//获取出错码。
		DWORD dwError = GetLastError();
		OutputDebugString(TEXT("CreateFile fail!\r\n"));
	}

	const int BUFSIZE = 4096;
	char chBuffer[BUFSIZE];
	DWORD dwReadSize = 0;
	memset(chBuffer, 0, 4096);
	BOOL bRet = ::ReadFile(hFile, chBuffer, BUFSIZE-1, &dwReadSize, NULL);
	if (bRet)
	{
		MyDebugString("sdp_test", "ReadFile 读文件成功");
	}
	else
	{
		//获取出错码。
		DWORD dwError = GetLastError();
		//处理出错。            
		TCHAR chErrorBuf[1024];
		wsprintf(chErrorBuf, TEXT("GetLastError()=%d"), dwError);
		MyDebugString("sdp_test", "GetLastError()=%d", dwError);

		::MessageBox(NULL, chErrorBuf, L"配置文件读取失败", MB_OK);
		return false;
	}

	Json::Reader reader;
	Json::Value my_value;
	if (reader.parse(chBuffer, my_value) && my_value.isMember("SysSet"))
	{
		//获取根网址
		_webroot = my_value["SysSet"]["web_server_address"].asString();
		if (_webroot.empty())
		{//不能为空
			return false;
		}
		//要记得去掉“http://”
		std::string del = "http://";
		std::string::size_type _npos = _webroot.find(del, 0);  // p为找到的位置,-1为未找到
		if (_npos != std::string::npos)
		{
			_webroot.erase(_npos, del.length());
		}
		//要记得把字符串最后的"/"、或"\\"
		std::string _s = _webroot.substr(_webroot.size()-1, 1);
		if (0 == strcmp(_s.c_str(), "/") || 0 == strcmp(_s.c_str(), "\\"))
		{
			_webroot.erase(_webroot.size() - 1, _s.length());
		}
	}
	else
	{
		return false;
	}
	////////////////
	std::string str_res;//返回结果
	if (!StringToWString(_webroot, _wwebroot))
	{//转换失败
		MyDebugString("sdp_test", "_webroot(%s) 转换失败", _webroot.c_str());
		return false;
	}

	GetHttpData(_wwebroot, INTERNET_DEFAULT_HTTP_PORT, L"/api/fight/version", str_res);
	MyDebugString("sdp_test", "版本数据=%s", str_res.c_str());

	Json::Value my_value2;
	if (reader.parse(str_res, my_value2) && my_value2.isMember("route") && my_value2.isMember("version"))
	{//解析获得的服务器传来的最新客户端版本数据
		//主版本下载地址
		route = my_value2["route"].asString();
		//主版本号
		version = my_value2["version"].asString();
		//主版本描述信息
		info = my_value2["info"].asString();
		//子版本内容（是一个数组，含多个当前主版本下的子版本）
		_smallversion = my_value2["smallversions"];
	}

	//查看本地版本信息
	std::string str_file_update = GetAppPath().c_str();
	str_file_update += "\\update.ini";
	int iversion = GetPrivateProfileIntA("config", "version", 0, str_file_update.c_str());//主版本号
	int ismallversion = GetPrivateProfileIntA("config", "smallversion", 0, str_file_update.c_str());//子版本号

	//比较版本
	if (iversion == atoi(version.c_str()))
	{//大版本相同，比较子版本
		if (0 != _smallversion.size())
		{//存在子版本
			std::string version2;//子版本号
			std::string route2;//子版本下载地址

			/////////////////////////////////
			bool bNeed = false;//是否需要更新
			//初始化,将变量设置为不等于当前数据的节点的数据
			Json::Value str_ValueInfo;//节点数据
			for (unsigned int i = 0; i < _smallversion.size(); ++i)
			{
				str_ValueInfo = _smallversion[i];
				if (str_ValueInfo.isMember("route") && str_ValueInfo.isMember("version"))
				{
					//子版本下载地址
					route2 = str_ValueInfo["route"].asString();
					//子版本号
					version2 = str_ValueInfo["version"].asString();
				}

				if (atoi(version2.c_str()) > ismallversion)
				{//第一个节点跟当前本地版本不一样说明至少有版本要更新
					bNeed = true;
					break;
				}
			}

			if (bNeed)
			{
				MyDebugString("sdp_test", "在线更新 初始化,将变量设置为第一个节点的数据=====%d,%d", atoi(version2.c_str()), ismallversion);
				for (unsigned int i = 0; i < _smallversion.size(); ++i)
				{
					std::string version_midd;//子版本号(临时)
					std::string route_midd;//子版本下载地址(临时)
					str_ValueInfo = _smallversion[i];
					if (str_ValueInfo.isMember("route") && str_ValueInfo.isMember("version"))
					{
						//子版本下载地址
						route_midd = str_ValueInfo["route"].asString();
						//子版本号
						version_midd = str_ValueInfo["version"].asString();
					}

					MyDebugString("sdp_test", "在线更新 遍历子节点=====%d", atoi(version_midd.c_str()));

					if (atoi(version_midd.c_str()) <= ismallversion)
					{//比当前子版本小的不做考虑
						MyDebugString("sdp_test", "在线更新 比当前子版本小的不做考虑=====%d,%d", atoi(version_midd.c_str()), ismallversion);
						continue;
					}

					MyDebugString("sdp_test", "在线更新 选择比第一个节点版本还要小，并且大于当前版本的节点说明应该更新该版本=====%d,%d", atoi(version2.c_str()), atoi(version_midd.c_str()));

					if (atoi(version2.c_str()) > atoi(version_midd.c_str()))
					{//选择比第一个节点版本还要小，并且大于当前版本的节点说明应该更新该版本
						version2 = version_midd;
						route2 = route_midd;
					}
				}

				int v1 = 0, v2 = 0;
				v1 = ismallversion;
				v2 = atoi(version2.c_str());
				MyDebugString("sdp_test", "在线更新 本地子版本比查询的子版本小，则下载 （v1=%d < v2=%d）", v1, v2);
				std::string::size_type _npos = route2.rfind('/', route2.size() - 1);
				std::string _s;
				std::string _s2;
				if (_npos != std::string::npos)
				{
					_s = route2.substr(_npos, route2.size() - 1);
					_s2 = route2.substr(0, _npos);
				}
				return updateOnline(version, version2, _s, _s2, _exe_name.c_str());
			}

		}

		MyDebugString("sdp_test", "在线更新 版本不需要更新");
		return false;
	}
	else
	{//大版本不同，直接下载大版本整包
		MyDebugString("sdp_test", "在线更新 大版本不同，直接下载大版本整包 （version=%s < virsiontime=%d）", version.c_str(), iversion);
		std::string::size_type _npos = route.rfind('/', route.size() - 1);
		std::string _s;
		std::string _s2;
		if (_npos != std::string::npos)
		{
			_s = route.substr(_npos, route.size() - 1);
			_s2 = route.substr(0, _npos);
		}
		return updateOnline(version, "0", _s, _s2, _exe_name.c_str());
	}

	return false;
}

void CRhinoLauncher::GetHttpData(std::wstring str_web_addr, int iport, std::wstring str_web_content, std::string &str_res)
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

	std::string strRet = U2G(str_res2.c_str());
	str_res = strRet;
}