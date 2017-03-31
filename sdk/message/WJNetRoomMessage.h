#ifndef _WJ_NetRoomMessage_H_
#define _WJ_NetRoomMessage_H_

#include "message/WJNetRM_game_frame.h"
#include "message/WJNetRM_queue.h"
#include "message/WJNetRM_room.h"
#include "message/WJNetRM_user_action.h"
#include "message/WJNetRM_user_list.h"

namespace WJ
{
	//////////////////////////////////////////////////////////////////////////
	//房间或游戏通讯指令宏定义

	enum E_ROOM_MAINID
	{
		e_gs_mess_user_list = 100,//用户列表
		e_gs_mess_user_action = 101,//用户动作
		e_gs_mess_room = 102,//房间信息
		e_gs_mess_queue = 103,//排队消息
		e_gs_mess_game_frame = 104,//游戏框架消息
		e_gs_mess_game_notify = 105,//游戏普通消息
	};

	//游戏结束通用类型
#define GAME_FINISH_TYPE_NORMAL 1	//正常结束
#define GAME_FINISH_TYPE_FLEE 2		//逃跑结束
#define GAME_FINISH_TYPE_FORCE 3	//系统强制结束
}

#endif // _WJ_NetRoomMessage_H_