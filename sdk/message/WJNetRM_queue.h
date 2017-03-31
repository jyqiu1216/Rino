#ifndef _WJ_WJNetRM_queue_H_
#define _WJ_WJNetRM_queue_H_

#include "common/WJBaseType.h"

namespace WJ
{
	//enum_gs_mess_queue = 103,//排队消息
	enum E_ROOM_ASSID_enum_gs_mess_queue
	{
		e_gs_ass_queue_allot = 1,//分配玩家去房间
	};

	//分配桌子座位给玩家，并可能设置该玩家为房主
	struct RoomMessDate_queue_allot
	{
		UINT uuserid;//用户id
		UINT udeskid;//桌子id
		UINT udeskstation;//座位
		bool bOwerUserid;//是否为房主

		RoomMessDate_queue_allot()
		{
			std::memset(this, 0, sizeof(RoomMessDate_queue_allot));
		}
	};
}

#endif//_WJ_WJNetRM_queue_H_