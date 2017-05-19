#include "stdafx.h"
#include "cmdWind.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include "message/WJNetPlatformMessage.h"
#include "clientconfig.h"

#define TIME_ID 100



CcmdWind::CcmdWind()
{
	m_hWindow = nullptr;
	m_bRunning = false;//正在运行
	std::thread recvThread_wind(std::bind(&CcmdWind::WindowMsgThread, this));
	recvThread_wind.detach();
}

CcmdWind::~CcmdWind()
{
	//关闭窗口句柄
	if ((m_hWindow != 0) && (::IsWindow((HWND)m_hWindow) == TRUE)) ::SendMessage((HWND)m_hWindow, WM_CLOSE, 0, 0);

	clear_socketlogic();
}

//清屏
void CcmdWind::clearWind()
{
	system("cls");
}

//造套接字逻辑线程
int CcmdWind::MakeSocketThread()
{
	std::cout << "==>正在启动机器人，请稍后..." << std::endl;
	try
	{
		int inum = 0;
		while (1)
		{
			if (inum >= clientconfig().GetData()->AInum)
			{
				break;
			}

			{
				std::lock_guard <std::mutex> autoLock(m_dataMutex_test_client);
				WJ::WJPlatformLogic* plogic_struct = new WJ::WJPlatformLogic(inum);
				m_vector_logic_struct.push_back(plogic_struct);
			}

			++inum;
		}
		std::cout << "==>机器人启动完成，请进行后续操作。" << std::endl;
	}
	catch (...)
	{
		clear_socketlogic();
		std::cout << "==>启动机器人失败，已经关闭机器人" << std::endl;
	}

	std::cout << "==>启动机器人完成！" << std::endl;
	return 0;
}

//清理接字逻辑队列
void CcmdWind::clear_socketlogic()
{
	std::lock_guard <std::mutex> autoLock(m_dataMutex_test_client);
	auto _iter = m_vector_logic_struct.begin();
	while (_iter != m_vector_logic_struct.end())
	{
		(*_iter)->close();
		WJ_SAFE_DELETE((*_iter));
		_iter = m_vector_logic_struct.erase(_iter);
	}
}

//用户操作计时器
void CcmdWind::OnTimeMesage(int itimeid)
{
	if (itimeid == TIME_ID)
	{
		for (UINT i = 0; i < m_vector_logic_struct.size(); ++i)
		{
			m_vector_logic_struct.at(i)->run();
		}
	}
}

//执行连接动作
void CcmdWind::onsocketconnect()
{
	std::cout << "==>正在执行连接动作..." << std::endl;
	SetTimer(TIME_ID, 1000);

	//执行连接操作
	for (UINT i = 0; i < m_vector_logic_struct.size(); ++i)
	{
		m_vector_logic_struct.at(i)->connect((char*)clientconfig().GetData()->hall_server_address.c_str(), 0);
	}
	std::cout << "==>执行连接动作完成。" << std::endl;
}

//执行关闭动作
void CcmdWind::onsocketClose()
{
	std::cout << "==>正在执行关闭动作..." << std::endl;
	for (UINT i = 0; i < m_vector_logic_struct.size(); ++i)
	{
		m_vector_logic_struct.at(i)->close();
	}
	std::cout << "==>执行关闭动作完成。" << std::endl;
}

#define GWL_USERDATA        (-21)
//窗口回调函数
LRESULT CALLBACK WindowProcFunc_CcmdWind(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:		//窗口建立消息
	{
		DWORD iIndex = TlsAlloc();
		CcmdWind * p = (CcmdWind *)(((CREATESTRUCT *)lParam)->lpCreateParams);
		TlsSetValue(iIndex, p);
		::SetWindowLong((HWND)hWnd, GWL_USERDATA, iIndex);
		break;
	}
	case WM_TIMER:		//计时器消息
	{
		DWORD iIndex = ::GetWindowLong((HWND)hWnd, GWL_USERDATA);
		CcmdWind * p = (CcmdWind *)::TlsGetValue(iIndex);//::TlsGetValue(iIndex);
		if (p != nullptr){ p->OnTimeMesage((int)wParam); }
		break;
	}
	case WM_CLOSE:		//窗口关闭消息
	{
		DestroyWindow((HWND)hWnd);
		break;
	}
	case WM_DESTROY:	//窗口关闭消息
	{
		DWORD iIndex = ::GetWindowLong((HWND)hWnd, GWL_USERDATA);
		CcmdWind * p = (CcmdWind *)::TlsGetValue(iIndex);
		if (p != NULL) p->sethWindow(0);
		::TlsFree(iIndex);
		PostQuitMessage(0);
		//ExitProcess(0);
		break;
	}
	}
	return ::DefWindowProc((HWND)hWnd, uMsg, wParam, lParam);
}

HWND CcmdWind::gethWindow()
{
	return m_hWindow;
}

void CcmdWind::sethWindow(HWND hWindow)
{
	m_hWindow = hWindow;
}

//WINDOW 消息循环线程
void CcmdWind::WindowMsgThread()
{
	try
	{
		//注册窗口类
		LOGBRUSH		LogBrush;
		WNDCLASS		WndClass;
		TCHAR			szClassName[] = TEXT("CcmdWindow");

		LogBrush.lbColor = RGB(0, 0, 0);
		LogBrush.lbStyle = BS_SOLID;
		LogBrush.lbHatch = 0;
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hCursor = NULL;
		WndClass.hIcon = NULL;
		WndClass.lpszMenuName = NULL;
		WndClass.lpfnWndProc = WindowProcFunc_CcmdWind;
		WndClass.lpszClassName = szClassName;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		WndClass.hInstance = NULL;
		WndClass.hbrBackground = (HBRUSH)::CreateBrushIndirect(&LogBrush);
		::RegisterClass(&WndClass);

		//建立窗口
		this->m_hWindow = ::CreateWindow(szClassName, NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, this);
		if (this->m_hWindow == NULL) throw TEXT("窗口建立失败");
	}
	catch (...)
	{
		//TRACE("CATCH:%s with %s\n", __FILE__, __FUNCTION__);
		//启动错误
	}

	//消息循环
	MSG	Message;
	while (::GetMessage(&Message, NULL, 0, 0))
	{
		if (!::TranslateAccelerator(Message.hwnd, NULL, &Message))
		{
			::TranslateMessage(&Message);
			::DispatchMessage(&Message);
		}
		Sleep(0);
	}
}

//设定定时器
bool CcmdWind::SetTimer(UINT uTimerID, UINT uElapse)
{
	if ((m_hWindow != nullptr) && (IsWindow(m_hWindow) == TRUE))
	{
		::SetTimer(m_hWindow, uTimerID, uElapse, nullptr);
		return true;
	}
	return false;
}

//清除定时器
bool CcmdWind::KillTimer(UINT uTimerID)
{
	if ((m_hWindow != nullptr) && (::IsWindow(m_hWindow) == TRUE))
	{
		::KillTimer(m_hWindow, uTimerID);
		return true;
	}
	return false;
}

//初始化页面
void CcmdWind::ShowMainWind()
{
	char str[200] = { 0 };
	clearWind();
	std::cout << "################################首页##################################" << std::endl;
	std::cout << "#欢迎使用本程序，请您根据以下提示选项进行操作界面                    #" << std::endl;
	std::cout << "#1、设置界面                                                         #" << std::endl;
	std::cout << "#2、运行界面                                                         #" << std::endl;
	std::cout << "#   如需退出系统，请在任何时候输入【q】                              #" << std::endl;
	std::cout << "######################################################################" << std::endl;
	do
	{
		std::cout << "请选择：";
		std::cin >> str;
		if (!ShowCommond(str))
		{
			if (0 == strcmp(str, "1"))
			{
				ShowSet();
				return;
			}
			else if (0 == strcmp(str, "2"))
			{
				ShowRun();
				return;
			}
			else
			{
				std::cout << "输入错误，请重新选择！" << std::endl;
				continue;
			}
		}
		else
		{
			return;
		}

	} while (1);
}

//设置页面
void CcmdWind::ShowSet()
{
	char str[200] = { 0 };
	clearWind();
	std::cout << "###############################设置页面###############################" << std::endl;
	std::cout << "#1、清屏                                                             #" << std::endl;
	std::cout << "#2、查询设置                                                         #" << std::endl;
	std::cout << "#3、重新设置                                                         #" << std::endl;
	std::cout << "#4、返回首页                                                         #" << std::endl;
	std::cout << "######################################################################" << std::endl;

	do
	{
		std::cout << "==>请输入：";
		std::cin >> str;
		if (!ShowCommond(str))
		{
			if (0 == strcmp(str, "1"))
			{
				ShowSet();
				return;
			}
			else if (0 == strcmp(str, "2"))
			{
				std::cout << "==>当前设置结果：" << std::endl;
				std::cout << "机器人数目：" << clientconfig().GetData()->AInum << std::endl;
				std::cout << "登陆服务器ip：" << clientconfig().GetData()->hall_server_address.c_str() << std::endl;
				std::cout << "比赛id：" << clientconfig().GetData()->contestid << std::endl;
				std::cout << "房间id：" << clientconfig().GetData()->roomid << std::endl;
				continue;
			}
			else if (0 == strcmp(str, "3"))
			{
				std::cout << "机器人数目（不能大于" << AI_NUM << "）：";
				std::cin >> str;
				clientconfig().GetData()->AInum = atoi(str) < AI_NUM ? atoi(str) : AI_NUM;
				std::cout << "登陆服务器ip：";
				std::cin >> str;
				clientconfig().GetData()->hall_server_address = str;
				std::cout << "比赛id：";
				std::cin >> str;
				clientconfig().GetData()->contestid = atoi(str);
				std::cout << "房间id：";
				std::cin >> str;
				clientconfig().GetData()->roomid = atoi(str);
				std::cout << "==>设置已生效。" << std::endl;
				continue;
			}
			else if (0 == strcmp(str, "4"))
			{
				ShowMainWind();
				return;
			}
			else
			{
				std::cout << "输入错误，请重新选择！" << std::endl;
				continue;
			}
		}
		else
		{
			return;
		}

	} while (1);
}

//运行页面
void CcmdWind::ShowRun()
{
	char str[200] = { 0 };
	clearWind();
	std::cout << "###############################运行页面###############################" << std::endl;
	std::cout << "#1、清屏                                                             #" << std::endl;
	std::cout << "#2、查询运行数据                                                     #" << std::endl;
	std::cout << "#3、启动                                                             #" << std::endl;
	std::cout << "#4、停止                                                             #" << std::endl;
	std::cout << "#5、返回首页                                                         #" << std::endl;
	std::cout << "######################################################################" << std::endl;
	do
	{
		std::cout << "==>请输入：";
		std::cin >> str;
		if (!ShowCommond(str))
		{
			if (0 == strcmp(str, "1"))
			{
				ShowRun();
				return;
			}
			else if (0 == strcmp(str, "2"))
			{
				if (m_bRunning)
				{
					std::cout << "==>当前正在运行..." << std::endl;
				}
				else
				{
					std::cout << "==>当前已经停止..." << std::endl;
				}
				continue;
			}
			else if (0 == strcmp(str, "3"))
			{
				if (m_bRunning)
				{
					std::cout << "==>当前正在运行中..." << std::endl;
					continue;
				}
				std::cout << "==>开始运行..." << std::endl;
				m_bRunning = true;
				MakeSocketThread();
				onsocketconnect();
				continue;
			}
			else if (0 == strcmp(str, "4"))
			{
				std::cout << "==>开始停止..." << std::endl;
				onsocketClose();
				KillTimer(TIME_ID);
				m_bRunning = false;
				std::cout << "==>停止完成..." << std::endl;
				continue;
			}
			else if (0 == strcmp(str, "5"))
			{
				ShowMainWind();
				return;
			}
			else
			{
				std::cout << "输入错误，请重新选择！" << std::endl;
				continue;
			}
		}
		else
		{
			return;
		}
	} while (1);
}

bool CcmdWind::ShowCommond(char *str)
{
	if (nullptr != str)
	{
		if (0 == strcmp(str, "q"))
		{
			std::cout << "退出系统成功！" << std::endl;
			return true;
		}
	}
	return false;
}