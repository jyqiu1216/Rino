#ifndef _WJ_NetPlatformMessage_H_
#define _WJ_NetPlatformMessage_H_

#include "common/WJBaseType.h"
#include "message/WJNetPM_apply.h"
#include "message/WJNetPM_gamelist.h"
#include "message/WJNetPM_login.h"
#include "message/WJNetPM_team.h"

namespace WJ
{
	//////////////////////////////////////////////////////////////////////////
	//大厅通讯指令宏定义

	enum E_HALL_MAINID
	{
		e_hs_mess_login = 100,//登陆
		e_hs_mess_gamelist = 101,//游戏列表
		e_hs_mess_apply = 102,//报名
		e_hs_mess_team = 103,//团队
	};
}

#endif // _WJ_NetPlatformMessage_H_