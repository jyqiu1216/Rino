
#define WM_HALL_MESS		WM_USER+1000	//大厅消息
#define WM_GAME_MESS		WM_USER+2000	//房间消息

////////////////////////////////////////////////////////////////////////////////////////
//大厅内传递消息
////////////////////////////////////////////////////////////////////////////////////////
#define WM_HALL_MESS_1001	WM_HALL_MESS+1						//任务栏ICO 响应消息

#define WM_HALL_MESS_1002	WM_HALL_MESS+2						//大厅服务器连接超时
#define WM_HALL_MESS_1003	WM_HALL_MESS+3						//大厅服务器连接失败，服务器主动断开
#define WM_HALL_MESS_1004	WM_HALL_MESS+4						//大厅服务连接成功
#define WM_HALL_MESS_1005	WM_HALL_MESS+5						//游戏信息返回完成
#define WM_HALL_MESS_1006	WM_HALL_MESS+6						//报名失败_未加入房间
#define WM_HALL_MESS_1007	WM_HALL_MESS+7						//报名失败_已加入房间
#define WM_HALL_MESS_1008	WM_HALL_MESS+8						//游戏正常结束(任务赛)
#define WM_HALL_MESS_1010	WM_HALL_MESS+10						//lol solo赛匹配页面
//发起结果页请求流程
#define WM_HALL_MESS_1009	WM_HALL_MESS+9						//CrossProxy.exe启动了
#define WM_HALL_MESS_1012	WM_HALL_MESS+12						//任务完成状态返回失败
#define WM_HALL_MESS_1013	WM_HALL_MESS+13						//第二个客户端通知主窗口显示
#define WM_HALL_MESS_1014	WM_HALL_MESS+14						//收到监视LOL进程关闭消息，转发Socket3013消息给H5
#define WM_HALL_MESS_1015	WM_HALL_MESS+15					    //LOL 任务赛结果页成功连接

////////////////////////////////////////////////////////////////////////////////////////
//房间或游戏内传递消息
////////////////////////////////////////////////////////////////////////////////////////
#define WM_GAME_MESS_2001	WM_GAME_MESS+1						//炉石注入结果消息
#define WM_GAME_MESS_2002	WM_GAME_MESS+2						//炉石加好友请求
#define WM_GAME_MESS_2003	WM_GAME_MESS+3						//炉石加好友请求回应
#define WM_GAME_MESS_2004	WM_GAME_MESS+4						//炉石发起挑战请求
#define WM_GAME_MESS_2005	WM_GAME_MESS+5						//炉石发起挑战回应
#define WM_GAME_MESS_2006	WM_GAME_MESS+6						//炉石检测出游戏结束了

#define WM_GAME_MESS_2007	WM_GAME_MESS+7						//游戏服务器连接超时
#define WM_GAME_MESS_2008	WM_GAME_MESS+8						//游戏服务器连接失败
#define WM_GAME_MESS_2009	WM_GAME_MESS+9						//游戏服务器登陆成功
#define WM_GAME_MESS_2010	WM_GAME_MESS+10						//游戏服务器断线重回
#define WM_GAME_MESS_2011	WM_GAME_MESS+11						//solo赛游戏服务器匹配成功
#define WM_GAME_MESS_2012	WM_GAME_MESS+12						//游戏服务器游戏正式开始
#define WM_GAME_MESS_2013	WM_GAME_MESS+13						//游戏服务器玩家举手同意消息
#define WM_GAME_MESS_2014	WM_GAME_MESS+14						//游戏服务器游戏房间信息返回

#define WM_GAME_MESS_2015	WM_GAME_MESS+15						//炉石自动化手工干预

#define WM_GAME_MESS_2016	WM_GAME_MESS+16						//炉石显示或隐藏内嵌结束页面
#define WM_GAME_MESS_2017	WM_GAME_MESS+17						//游戏过程中异常错误，需要清理不正确的数据，保证可以再次点击一键开赛(初始化整个房间数据)

#define WM_GAME_MESS_2018	WM_GAME_MESS+18						//炉石自动化手工干预操作完成，杀死刚才的计时器，防止判断超时

#define WM_GAME_MESS_2019	WM_GAME_MESS+19						//炉石内嵌窗口点击收起或展开效果的指令
#define WM_GAME_MESS_2020	WM_GAME_MESS+20						//炉石内嵌窗口创建并显示

#define WM_GAME_MESS_2021	WM_GAME_MESS+21						//收到房间列表消息

#define WM_GAME_MESS_2022	WM_GAME_MESS+22						//报名失败理由

#define WM_GAME_MESS_2023	WM_GAME_MESS+23						//LOL退出游戏消息

#define WM_GAME_MESS_2024	WM_GAME_MESS+24						//LOL SOLO弹出押注结果页




/////////////////////////////////////////////////////////////////////////////////////////
//窗口消息发送函数封装
#include <windows.h>
//#include <WinUser.h>
#include <qwidget.h>
#define WJ_SAFE_QSendMessage(_p, _messid, _wParam, _lParam) \
	if (nullptr != _p)\
		::SendMessage((HWND)((QWidget*)_p->winId()), _messid, _wParam, _lParam);

#define WJ_SAFE_QPostMessage(_p, _messid, _wParam, _lParam) \
	if (nullptr != _p)\
	::PostMessage((HWND)((QWidget*)_p->winId()), _messid, _wParam, _lParam);

