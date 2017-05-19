#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Download.h"

class CMainWindow : public WindowImplBase {

private:
	//路径修改按钮
	static CButtonUI*																		m_pBtnShow;
	//自定义安装
	static CVerticalLayoutUI*																m_pVerlayout;
	//路径框
	CEditUI*																				m_pPath;
	//盘符空间M
	DWORD64																					m_dwMSize;
	//下载界面
	CDownload*																				m_pDownload;
protected:
	//显示
	static VOID CALLBACK																	ShowTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	//隐藏
	static VOID CALLBACK																	HideTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
public:
	CMainWindow();
	~CMainWindow();

	//皮肤文件
	CDuiString																				GetSkinFile();
	//窗口类名称
	LPCTSTR																					GetWindowClassName() const;
	//初始化
	void																					InitWindow();
	//点击事件
	void																					Notify(TNotifyUI& msg);
	//显示或者隐藏路径
	void																					ShowHidePath(CButtonUI* pBtnShow);
	//设置安装程序路径
	void																					SetInstallPath();
	//选择目录
	void																					SwitchDirectory();
	//获取盘符信息
	void																					GetDiskInfo();
};

#endif