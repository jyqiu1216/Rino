// NewTest_Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "cmdWind.h"

///////////
#include "clientsocket/commonfunction_c.h"
#include <atltime.h>
//////////////
//using namespace WJ;

void updateOnline(std::string _filename, std::string strDownLoadUpdatepADDR, std::string startgamename);

int _tmain(int argc, _TCHAR* argv[])
{
	int a = 1;
	if (0 == a)
	{
		//updateOnline("Client9--9--10--12.zip", "http://cdn.xiniugame.com//xiniuclient/version/20160922141701", "Dbgview.exe");
		updateOnline("RhinoClient.zip", "http://img.wangyuhudong.com/xiniugame/client", "Dbgview.exe");
	} 
	else
	{
		CcmdWind _cmdWind;
		_cmdWind.ShowMainWind();
	}

	system("pause");
	return 0;
}

void updateOnline(std::string _filename, std::string strDownLoadUpdatepADDR, std::string startgamename)
{
	CTime LastServerTime = CTime::GetCurrentTime();

	// 构造命令行
	char strCommonLine[MAX_PATH] = { 0 };
	std::string str_CommonLine;
	char _timedata[100] = { 0 };
	sprintf_s(_timedata, "%d-%d-%d %d:%d:%d", LastServerTime.GetYear(), LastServerTime.GetMonth(), LastServerTime.GetDay(), LastServerTime.GetHour(), LastServerTime.GetMinute(), LastServerTime.GetSecond());
#ifdef _DEBUG
	sprintf_s(strCommonLine, ("Update_d.exe /nb:%s%s/ne /tb:%s/te /ub:%s/ue /pb:"), GetAppPath().c_str(), startgamename.c_str(), _timedata, strDownLoadUpdatepADDR.c_str());
#else
	sprintf_s(strCommonLine, ("Update.exe /nb:%s%s/ne /tb:%s/te /ub:%s/ue /pb:"), GetAppPath().c_str(), startgamename.c_str(), _timedata, strDownLoadUpdatepADDR.c_str());
#endif

	DWORD dwSize = 0;
	str_CommonLine += strCommonLine;
	str_CommonLine += _filename;

	char _str[MAX_PATH * 10] = { 0 };
	sprintf_s(_str, "%s/pe /sb:%lu/se", str_CommonLine.c_str(), dwSize);
	STARTUPINFOA StartInfo;
	memset(&StartInfo, 0, sizeof(StartInfo));
	StartInfo.cb = sizeof(StartInfo);
	StartInfo.wShowWindow = SW_SHOWNORMAL;
	PROCESS_INFORMATION 	m_ProcessInfo;
	BOOL bSuccess = CreateProcessA(NULL, _str, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &StartInfo, &m_ProcessInfo);
	ExitProcess(0);
}

