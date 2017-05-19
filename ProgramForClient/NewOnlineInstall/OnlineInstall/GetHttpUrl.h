#pragma once

#ifndef GETHTTPURL_H
#define GETHTTPURL_H

class CGetHttpUrl {

private:
	CURL*																				m_pCurl;
	//文件总长度
	long																				m_lFileLen;
	//下载进度
	double																				m_dDownLen;
	//界面管理
	CPaintManagerUI*																	m_pManager;
	//结束下载
	bool																				m_bEndDownLoad;
	//验证下载的文件
	bool																				CheckDownloadFile();
protected:
	//下载数据
	static int																			WriteHttpData(char* pchData, size_t size, size_t nmemb, string* pWriterData);
	//解析获取链接地址
	bool																				CheckHttpLink(string strData, wstring* pwstrData, int* pinVerison);
	//写入文件
	static size_t																		WriteFunc(char* pstr, size_t size, size_t nmemb, void* pstream);
	//下载进度
	static size_t																		ProgressFunc(void* pData, double dDwonLoadCount, double dCurDwonLoadCount, double uUpCount, double uUpCurCount);
public:
	CGetHttpUrl();
	~CGetHttpUrl();

	//获取下载地址
	bool																				GetHttpUrl(wstring* pwstrUrl, int& inVersion);

	//下载文件
	bool																				GetInstallZip(LPCTSTR lptFileName, LPCTSTR lptLink, CPaintManagerUI* pManager, DWORD& dwFileSize);

	//关闭停止下载
	void																				CloseDownload();

	//提交安装数据
	bool																				RepointInstall(LPCTSTR lptData);
};

#endif