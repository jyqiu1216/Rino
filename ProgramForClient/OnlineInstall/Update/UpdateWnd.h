/*Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/#pragma once
#include "http_request_manager.h"
#include "resource.h"
#include <vector>
#include <deque>
#include "MyFileOperation.h"
#include "backgroundWnd.h"

using namespace std;

enum _E_RSULT
{
	OP_OK=0,
	OP_FAILD
};

struct _S_HALL_DOWNLOAD_DATA
{
	_S_HALL_DOWNLOAD_DATA(std::string strFileName, std::string strRecvData):
		m_strFileName(strFileName), m_strRecvData(strRecvData)
	{
	}
	std::string m_strFileName;	//文件名
	std::string m_strRecvData;	//文件内容
};

class CUpdateWnd : public WindowImplBase, public FCHttpRequestManager
{
public:
	explicit CUpdateWnd(LPTSTR lpCmdLine);

	~CUpdateWnd();

	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("UpdateWnd");
	}

	virtual CDuiString GetSkinFile()
	{
		return _T("UpdateWnd.xml");
	}

	virtual CDuiString GetSkinFolder()
	{
		return  _T("skin\\");
	}

	virtual UILIB_RESOURCETYPE GetResourceType() const
	{
		return UILIB_ZIPRESOURCE;
		return UILIB_FILE;
	}

	virtual LPCTSTR GetResourceID() const
	{
		return MAKEINTRESOURCE(IDR_ZIPRES1);
		return _T("");
	}

	virtual void InitWindow();
	
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual void Notify(TNotifyUI& msg);
public:

	bool ParseCmdLine();

	void UpdateProcess();

public://执行拷贝安装文件线程
	static unsigned int __stdcall Thread_Install(LPVOID lpVoid);

protected:
    
	//下载请求发送完成后调用此函数
    virtual void OnAfterRequestSend(FCHttpRequest& rTask);
    
	//文件下载完成后调用此函数
    virtual void OnAfterRequestFinish(FCHttpRequest& rTask);

	bool AddDownloadEx(LPCTSTR strFile);

	CDuiString GetFormatKB(LONGLONG nByte);

	CDuiString GetStatusText (FCHttpRequest& rTask);

	//void MakeFiles();

	static bool CreateMuliteDirectory(CString P);

	static bool FolderExists(CString s);

	//BOOL LoadImageFromResource(ATL::CImage *pImage, UINT nResID,LPCTSTR lpTyp);
	//显示当前下载进度
	void onShowProgress(int iprogress);
protected:
	CbackgroundWnd* m_pCbackgroundWnd;//退出窗口

	CTextUI*  m_pTxt_Progress;//进度描述
	CButtonUI* m_pBtn_cancel;//取消按钮
	CTextUI* m_pTxt_UpdateClient;//超链接文字，下载客户端

	CProgressUI *m_pProgress;//进度条控件

	CWndShadow m_WndShadow;
	bool m_bIsErr;	//更新失败的标记
private:

	CDuiString m_strCmdLine;

	CDuiString m_strMainUrl;

	DWORD		m_dwTotalSize;

	DWORD		m_dwDownLoadSize;



	vector<CDuiString> m_vecDownFiles;


	std::deque<std::string>	m_main_wait_download;

	bool m_bDownloading;

	int m_iCurTaskID;
public:
	static CString	m_strFileName;

	static CDuiString	CUpdateWnd::m_str_version;//主版本
	static CDuiString	CUpdateWnd::m_str_smallversion;//子版本
	static vector<_S_HALL_DOWNLOAD_DATA> m_vecHallDownData;
	static CMyFileOperation m_MyFileOperation;  //文件夹操作
};