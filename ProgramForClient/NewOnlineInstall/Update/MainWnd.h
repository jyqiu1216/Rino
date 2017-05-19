#pragma once

#ifndef MAINWND_H
#define MAINWND_H

#include "Cancel.h"
#include "GetUpdate.h"

class CMainWnd : public WindowImplBase {

private:
	//提示文本
	CTextUI*																				m_pTilText;
	//速度
	CTextUI*																				m_pTilSpeed;
	//进度
	CProgressUI*																			m_pProgress;
	//取消或者重试
	CButtonUI*																				m_pCancelReDownLoad;
protected:
	//工作线程
	static DWORD WINAPI																		TaskThreadProc(LPVOID lpParameter);
	//启动平台
	void																					RunMainroutine();
	//提权
	BOOL																					EnablePrivilege(LPCTSTR lptName); 
public:
	CMainWnd();
	~CMainWnd();

	//皮肤
	CDuiString																				GetSkinFile();
	//类名
	LPCTSTR																					GetWindowClassName() const;
	//初始化
	void																					InitWindow();
	//响应
	void																					Notify(TNotifyUI& msg);
};

#endif