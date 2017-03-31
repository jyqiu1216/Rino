#ifndef _WJ_R_a_h_gamelist_H_
#define _WJ_R_a_h_gamelist_H_

#include "common/WJBaseType.h"
#include "server/WJServerManagelib.h"
#include "server/DataLine.h"
namespace WJ
{
#define Redis_Order_h_messid_gamelist 1//获取游戏列表的命令

	//请求游戏列表的redis结构体
	struct RRLine_c_gamelist : public RedisDataBaseLine
	{
		RRLine_c_gamelist()
		{
			std::memset(this, 0, sizeof(RRLine_c_gamelist));
			uRediscommandid = Redis_Order_h_messid_gamelist;
		}
	};

	//请求游戏列表结果返回
	struct RRLine_s_gamelist : public RedisResultLine
	{
		enum E_RES
		{
			e_gamelist_game_node,//游戏节点
			e_gamelist_erea_node,//区服节点
			e_gamelist_contest_node,//比赛节点
			e_gamelist_room_node,//房间节点
		};
		RRLine_s_gamelist()
		{
			std::memset(this, 0, sizeof(RRLine_s_gamelist));
			uRediscommandid = Redis_Order_h_messid_gamelist;
		}
	};
	//请求游戏列表结果返回(游戏节点)
	struct RRLine_s_gamelist_Gameinfo : public RRLine_s_gamelist
	{
		ComGameinfo _data;
		RRLine_s_gamelist_Gameinfo()
		{
			std::memset(this, 0, sizeof(RRLine_s_gamelist_Gameinfo));
			uHandleRusult = e_gamelist_game_node;
			uRediscommandid = Redis_Order_h_messid_gamelist;
		}
	};
	//请求游戏列表结果返回(区服节点)
	struct RRLine_s_gamelist_Ereainfo : public RRLine_s_gamelist
	{
		ComEreainfo _data;
		RRLine_s_gamelist_Ereainfo()
		{
			std::memset(this, 0, sizeof(RRLine_s_gamelist_Ereainfo));
			uHandleRusult = e_gamelist_erea_node;
			uRediscommandid = Redis_Order_h_messid_gamelist;
		}
	};
	//请求游戏列表结果返回(比赛节点)
	struct RRLine_s_gamelist_ContestInfo : public RRLine_s_gamelist
	{
		ComContestInfo _data;
		RRLine_s_gamelist_ContestInfo()
		{
			std::memset(this, 0, sizeof(RRLine_s_gamelist_ContestInfo));
			uHandleRusult = e_gamelist_contest_node;
			uRediscommandid = Redis_Order_h_messid_gamelist;
		}
	};
	//请求游戏列表结果返回(房间节点)
	struct RRLine_s_gamelist_RoomInfo : public RRLine_s_gamelist
	{
		ComRoomInfo _data;
		RRLine_s_gamelist_RoomInfo()
		{
			std::memset(this, 0, sizeof(RRLine_s_gamelist_RoomInfo));
			uHandleRusult = e_gamelist_room_node;
			uRediscommandid = Redis_Order_h_messid_gamelist;
		}
	};
}

#endif//_WJ_R_a_h_gamelist_H_