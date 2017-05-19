#ifndef __cmdWind_H__
#define __cmdWind_H__

#include <vector>
#include "WJPlatformLogic.h"
#include <windows.h>

class CcmdWind
{
public:
	CcmdWind();
	~CcmdWind();

	//清屏
	void clearWind();
	//设置页面
	void ShowSet();
	//首页
	void ShowMainWind();
	//去初始化页面

	//运行页面
	void ShowRun();

	//通用全局命令界面
	bool ShowCommond(char *str);

	HWND gethWindow();
	void sethWindow(HWND hWindow);
	//计时器
	void OnTimeMesage(int itimeid);
	//内部函数
private:


	//执行连接动作
	void onsocketconnect();

	//执行关闭动作
	void onsocketClose();

	//清理接字逻辑队列
	void clear_socketlogic();

	///WINDOW 消息循环线程
	void WindowMsgThread();

	///设定定时器
	bool SetTimer(UINT uTimerID, UINT uElapse);
	///清除定时器
	bool KillTimer(UINT uTimerID);

	//造套接字逻辑线程
	int MakeSocketThread();

private:
	std::vector<WJ::WJPlatformLogic*> m_vector_logic_struct;
	std::mutex m_dataMutex_test_client;//批发同步锁互斥量，方便线程对互斥量上锁
	HWND m_hWindow;//窗口句柄
	bool m_bRunning;//正在运行
};

#endif//__cmdWind_H__