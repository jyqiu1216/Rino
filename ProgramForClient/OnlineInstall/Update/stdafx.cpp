// stdafx.cpp : 只包括标准包含文件的源文件
// Update.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

/*Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用


CString GetAppPath()
{
	//char lpBuffer[MAX_PATH];
	//ZeroMemory(lpBuffer, sizeof(lpBuffer));
	//GetCurrentDirectoryA(MAX_PATH, lpBuffer);

	char szFilePath[MAX_PATH + 1];
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //删除文件名，只获得路径
	CString ss = szFilePath;

	//CString ss = lpBuffer;
	//ss += "\\";
	return ss;
}