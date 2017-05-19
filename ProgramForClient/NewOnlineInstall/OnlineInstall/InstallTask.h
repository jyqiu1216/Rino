#pragma once

#ifndef INSTALLTASK_H
#define INSTALLTASK_H

#include "GetHttpUrl.h"

class CInstallTask {

private:
	//标记
	wstring																					m_SetupFlags;
	//POST提交
	CGetHttpUrl																				m_HttpPost;
	//添加桌面快捷方式
	bool																					AddDesktopLink(LPCTSTR lptLink);
	//获取桌面快捷方式名称
	bool																					GetDesktopLinkName(LPTSTR lptName, int inLen);
	//产品列表中添加我们的产品
	void																					AddInstall(LPCTSTR lptAppPath, LPCTSTR lptDirectory);
	//写入版本和安装信息
	void																					WriteVersionInfo(int inVersion, LPCTSTR lptDirectory);
	//启动监控服务
	void																					StartMonitor(LPCTSTR lptDirectory);
	//上报安装处理
	void																					ReportSetup();
protected:
	//获取硬件序列号
	bool																					GetDiskKey(LPWSTR lpwstr, int inLen);
public:
	CInstallTask();
	~CInstallTask();

	//执行安装任务
	bool																					InstallTask(LPCTSTR lptInstallDir, CPaintManagerUI* pManager, int inVersion);
};

#endif