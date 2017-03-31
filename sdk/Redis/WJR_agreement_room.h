#ifndef _WJ_R_agreement_room_H_
#define _WJ_R_agreement_room_H_

#include "WJRedis.h"
#include "message/WJNetMessageHead.h"
#include "message/WJNetRoomMessage.h"

namespace WJ
{
#define Redis_Order_r_gamefinish 1//游戏结束

	//游戏结束信息的redis结构体
	struct RRLine_c_gamefinish : public RedisDataBaseLine
	{
		RRLine_c_gamefinish()
		{
			std::memset(this, 0, sizeof(RRLine_c_gamefinish));
			uRediscommandid = Redis_Order_r_gamefinish;
		}
	};
}

#endif//_WJ_R_agreement_room_H_
