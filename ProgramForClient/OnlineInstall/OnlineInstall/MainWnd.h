/*Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/#pragma once
#include "http_request_manager.h"
#include "MyWinHttp.h"
#include <shlwapi.h>
#pragma comment (lib, "shlwapi.lib")

class CMainWnd : public WindowImplBase, public FCHttpRequestManager
{
public:
	CMainWnd(void);
	~CMainWnd(void);

	virtual LPCTSTR GetWindowClassName(void) const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;

	virtual void InitWindow();
	virtual void OnFinalMessage( HWND hWnd );
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void Notify(TNotifyUI& msg);
	void SwitchCustomImage(LPCTSTR lpBkImage);
public:
	void OnAfterRequestSend (FCHttpRequest& rTask);
	void OnAfterRequestFinish (FCHttpRequest& rTask);
	CDuiString GetStatusText (FCHttpRequest& rTask);
	void SetProcess();

	void DownloadProcess();
	void SmoothSwitchBK();
	void OnTimer(int iEventID);

	void SetErrorMsg(LPCTSTR lpStr)
	{
		if (m_pTxtError)
		{
			m_pTxtError->SetText(lpStr);
		}
	}

	CDuiString GetFormatKB (LONGLONG nByte)
	{
		TCHAR strRes[128] ;
		StrFormatKBSize(nByte, strRes, sizeof(strRes)) ;
		return strRes;
	}

	void SetProgress(int iValue)
	{
		if (m_pProgress)
		{
			m_pProgress->SetValue(iValue);
		}
	}

public:
	static unsigned int __stdcall Thread_Install(LPVOID lpVoid);

public:
	//创建快捷方式
	void CreateLinkFile(LPCTSTR szStartAppPath,LPCTSTR szDestLnkPath);
	//获取桌面路径
	CDuiString GetDesktop();
	//获取系统是64位还是32位
	bool Is64Bit_OS();
	//获取分区剩余空间
	int GetDiskFree(LPCTSTR strDrive);
	//验证路径合法性以及磁盘空间大小
	void VerifyPath(LPCTSTR strPath);
	//产品列表中添加我们的产品
	void AddInstall(LPCSTR lptAppPath, LPCTSTR lptDirectory);
	//静默安装
	void NoWindowInstall();
private:
	//注册表写入时，判断该路径字符串在原path字符串中是不是存在
	bool IsPathExist(char *PerfData, char *myPath);
	//往系统环境变量path中加入游戏安装路径
	bool AddPath_registry(char *szPath);
	//获取下载地址信息json
	void getDownloadAddressInfo(string strValue);

	//运行各种需要的运行环境程序
	void Exec_dx_exe();
protected:
	CWndShadow m_WndShadow;
	CHorizontalLayoutUI* m_pHorCustom;
	CTabLayoutUI* m_pMainTab;//切换到主界面和用户协议界面
	CTabLayoutUI* m_pTab;
	CControlUI* m_pCtrlTheme;
	CProgressUI* m_pProgress;
	CRichEditUI*	m_pTxtinfo;//用户协议信息
	CTextUI*	m_pTxtStatues;
	CTextUI*	m_pTxtError;
	CTextUI*	m_pTxtShowMsg;
	CEditUI*	m_pEdtPath;
	CFadeButtonUI* m_pBtnCustom;
	COptionUI* m_pOptAgree;//用户协议单选框
	CFadeButtonUI* m_pBtnInstall;//下载按钮
	//判断是否是静默安装
	bool			m_NoWndInstall;

	CMyWinHttp m_MyWinHttp;
	std::string m_strWeb_download;//当前最新版本下载地址//压缩包下载地址http://172.16.2.18:8080/download
	std::string	m_str_version;//主版本

	int m_iSource;//推广平台标志
	//与下载相关的数据
private:
	int   m_task_id ;
	std::string   m_receive ;
	HTTP_REQUEST_HEADER   m_header ;

	//与界面相关的数据
private:
	int		m_iCurFade;	//当前背景图透明度
	int		m_iCurIndex;//当前图片索引
	bool	m_bToUp;	//自定义按钮是否从下到上(false-从上到下，true-从下到上）
	bool	m_bCanInstall;	//是否可以安装
	DWORD	m_dwMsgColor;	//默认消息背景色
};

