#pragma once

#ifndef GETUPDATE_H
#define GETUPDATE_H

#include <set>
#include <map>
using namespace std;
#include "DownloadZip.h"

//下载列表
typedef pair<int, wstring> tpVerLink;
typedef set<pair<int, wstring> > tvVerLink;

class CGetUpdate {

private:
	CURL*																				m_pCurl;
	//下载列表
	tpVerLink																			m_pVerLink;
	tvVerLink																			m_VerLink;
	//主窗口指针
	void*																				m_pMainWnd;
	//主版本检测
	bool																				CheckMajor(LPCTSTR lptDirecotry, LPCTSTR lptConfig, int inVerions, LPCTSTR lptLink);
	//子版本检测
	bool																				CheckMinVer(LPCTSTR lptDirecotry, LPCTSTR lptConfig, int inVerions, LPCTSTR lptLink);
protected:
	//网页数据获取
	static int																			WriteHttpData(char* pchData, size_t size, size_t nmemb, string* pWriterData);
	//解析版本数据
	bool																				ParseVersion(string& strData);
	//获取程序路径
	wstring																				GetAppPath();
public:
	CGetUpdate();
	~CGetUpdate();

	//拉取更新服务器信息
	bool																				GetUpdate(void* pThis);

	//校验版本信息
	bool																				CheckVersion(void* pThis);
};

#endif