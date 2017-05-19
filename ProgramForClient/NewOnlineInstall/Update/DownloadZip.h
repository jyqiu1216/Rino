#pragma once

#ifndef DOWNLOADZIP_H
#define DOWNLOADZIP_H

class CDownloadZip {

private:
	CURL*																				m_pCurl;
	//文件总长度
	long																				m_lFileLen;
	//取消下载
	bool																				m_bEndDownLoad;
	//主窗口指针
	void*																				m_pMainWnd;
	//下载总大小
	double																				m_dDownLen;
	//包路径
	wstring																				m_InstallPath;
protected:
	//写文件
	static size_t																		WriteFunc(char* pstr, size_t size, size_t nmemb, void* pstream);
	//进度
	static size_t																		ProgressFunc(void* pData, double dDwonLoadCount, double dCurDwonLoadCount, double uUpCount, double uUpCurCount);
	//校验文件是否下载完成
	bool																				CheckFileComplete();
	//路径替换
	wstring																				RepPath(LPCTSTR lptPath);
public:
	CDownloadZip();
	~CDownloadZip();

	//下载ZIP包
	bool																				DownLoadZip(LPCTSTR lptInstallPath, LPCTSTR lptLink, int inVersion, LPCTSTR lptConfig, void* pThis);

	//取消下载
	void																				CancelDownLoad();

	//结束占用进程
	void																				ExitProcessList();

	//解包
	bool																				UnpackZip(LPCTSTR lptInstallPath, int inVersion, LPCTSTR lptConfig, LPCTSTR lptTilName);
};

#endif