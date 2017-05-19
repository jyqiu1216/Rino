#ifndef _UTIL_H_
#define _UTIL_H_

#include <Windows.h>
#include <string>

//选择文件夹对话框  
#include<Shlobj.h>  
#pragma comment(lib,"Shell32.lib") 

class Util
{
public:
	static std::string strToUtf8(const std::string& _str)
	{
		static char sBuffer[1024];
		static wchar_t sBufferTemp[1024];

		ZeroMemory(sBufferTemp, 1024);
		ZeroMemory(sBuffer, 1024);

		int len = MultiByteToWideChar(CP_ACP, 0, _str.c_str(), -1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, _str.c_str(), _str.length(), sBufferTemp, len);

		int length = WideCharToMultiByte(CP_UTF8, 0, sBufferTemp, -1, NULL, NULL, NULL, NULL);
		WideCharToMultiByte(CP_UTF8, 0, sBufferTemp, len, sBuffer, length, NULL, NULL);
		return std::string(sBuffer);
	}


	static bool WStringToString(const std::wstring &wstr, std::string &str)
	{
		int len = (int)wstr.length();
		str.resize(len, ' ');

		int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), len, (LPSTR)str.c_str(), len, NULL, NULL);
		if (nResult == 0)
		{
			return false;
		}

		return true;
	}

	static bool openFile(std::string& _file)
	{
		TCHAR szFileName[MAX_PATH] {0};
		OPENFILENAME openFileName = { 0 };
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.nMaxFile = MAX_PATH;  //这个必须设置，不设置的话不会出现打开文件对话框  
		openFileName.lpstrFilter = TEXT("所有文件(*.*)\0*.*\0\0");
		openFileName.lpstrFile = szFileName;
		openFileName.lpstrInitialDir = TEXT("F:\\01-Development\\lixySample\\lixySample\\bin\\images");

		openFileName.nFilterIndex = 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


		if (!::GetOpenFileName(&openFileName))
			return false;

		return WStringToString(szFileName, _file);
	}

	static bool openFolder(std::string& _dir)
	{
		TCHAR szBuffer[MAX_PATH] = { 0 };
		BROWSEINFO bi = { 0 };
		bi.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
		bi.pszDisplayName = szBuffer;//接收文件夹的缓冲区  
		bi.lpszTitle = TEXT("选择一个文件夹");//标题  
		bi.ulFlags = BIF_NEWDIALOGSTYLE;
		LPITEMIDLIST idl = SHBrowseForFolder(&bi);
		if (!SHGetPathFromIDList(idl, szBuffer))
			return false;

		return WStringToString(szBuffer, _dir);
	}


};

#endif // _UTIL_H_