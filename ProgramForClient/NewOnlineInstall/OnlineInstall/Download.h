#pragma once

#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "GetHttpUrl.h"
#include "UnzipFile.h"
#include "InstallTask.h"
#include "Complete.h"

#define ID_TIMERIMAGEHIDE																		WM_USER + 1
#define ID_TIMERIMAGESHOW																		WM_USER + 2
#define ID_TIMERIMAGESWITCH																		WM_USER + 3

class CDownload : public WindowImplBase {

private:
	//背景图
	static CLabelUI*																		m_pBkImage;
	//当前图序号
	static int																				m_CurIndex;
	//下次图序号
	static int																				m_NextIndex;
	//透明度值
	static int																				m_Opacity;
	//进度文字
	CTextUI*																				m_pProgrText;
	//进度条
	CProgressUI*																			m_pProgress;
	//速度文字
	CTextUI*																				m_pSpeedText;
	//下载
	CGetHttpUrl																				m_GetHttpUrl;
	//解压
	CUnzipFile																				m_UnZipFile;
	//安装任务
	CInstallTask																			m_InstallTask;
	//安装完成
	CComplete*																				m_pComplete;
	//关闭
	bool																					m_bClose;
protected:
	//换背景图
	static VOID CALLBACK																	SwitchTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	//慢慢隐藏背景图
	static VOID CALLBACK																	HideTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	//慢慢显示背景图
	static VOID CALLBACK																	ShowTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	//下载线程
	static DWORD WINAPI																		DownLoadThreadProc(LPVOID lpParameter);
public:
	CDownload();
	~CDownload();

	//皮肤文件
	CDuiString																				GetSkinFile();
	//窗口类名称
	LPCTSTR																					GetWindowClassName() const;
	//初始化
	void																					InitWindow();
	//点击事件
	void																					Notify(TNotifyUI& msg);
};

#endif