// RhinoLauncher.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <ShellAPI.h>

#include "commonfunction_c.h"
#include "RhinoLauncher.h"

//隐藏dos窗口
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

int main(int argc, _TCHAR* argv[])
{
	//::MessageBox(NULL, NULL, _T("I am comeing!"), MB_OK);
	HANDLE hMutex = ::CreateMutex(NULL, TRUE, L"RhinoLauncher");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{//只运行开一个实例
		return 0;
	}


	//如果判断在线更新窗口在启动，则不要再启动在线更新了
	std::string str_Update = GetAppPath().c_str();
#ifdef _DEBUG
	str_Update += "Update_d.exe";
#else
	str_Update += "Update.exe";
#endif // _DEBUG
	
	while (true)
	{
		if (0 != FindProcess((char*)str_Update.c_str()))
		{//存在update就一直等待
			OutputDebugStringA("sdp_aaa 存在Update.exe");
			continue;
		}
		else
		{//不存在就可以去启动update了
			break;
		}
	}

	std::string s_Client = GetAppPath().c_str();
#ifdef _DEBUG
	s_Client += "Client_d.exe";
#else
	s_Client += "Client.exe";
#endif // _DEBUG

	CRhinoLauncher cmd;
	if (!cmd.VersionUpdate())
	{//不需要更新则启动客户端
		SHELLEXECUTEINFOA ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = s_Client.c_str();
		ShExecInfo.lpParameters = NULL;
		ShExecInfo.lpDirectory = GetAppPath().c_str();
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = NULL;
		::ShellExecuteExA(&ShExecInfo);
	}
	return 0;
}