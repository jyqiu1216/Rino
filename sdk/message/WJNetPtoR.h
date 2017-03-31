#ifndef _WJ_NetPtoR_H_
#define _WJ_NetPtoR_H_
//此文件保存登陆服务器与游戏服务器之间的通信协议
#include "message/WJNetPtoR_roominfo.h"
#include "message/WJNetPtoR_userinfo.h"
namespace WJ
{
	//////////////////////////////////////////////////////////////////////////
	//房间或游戏通讯指令宏定义

	enum E_PtoR_MAINID
	{
		e_pr_mess_roominfo = 500,//房间信息
		e_pr_mess_userinfo = 501,//用户信息
	};
}

#endif // _WJ_NetPtoR_H_