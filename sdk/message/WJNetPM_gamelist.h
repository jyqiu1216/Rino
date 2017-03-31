#ifndef _WJ_NetPM_gamelist_H_
#define _WJ_NetPM_gamelist_H_

#include "common/WJBaseType.h"
#include "server/comstruct.h"
namespace WJ
{
	//e_hs_mess_gamelist = 101,//游戏列表
	enum E_HALL_ASSID_enum_hs_mess_gamelist
	{
		e_hs_ass_gamelist_game = 1,//获取游戏节点
		e_hs_ass_gamelist_erea = 2,//获取区服节点
		e_hs_ass_gamelist_contest = 3,//获取比赛节点
		e_hs_ass_gamelist_room = 4,//获取房间节点
		e_hs_ass_gamelist_contest_online = 5,//获取比赛节点(单节点数据)
		e_hs_ass_gamelist_onlien_count = 6,//在线人数
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_gamelist
	{
		e_hs_hcid_gamelist_part = 1,//一部分，还没有发完
		e_hs_hcid_gamelist_finish = 2,//所请求列表发完了
	};

	//客户端请求获取区服节点结构体
	struct struct_data_hs_ass_gamelist_erea
	{
		UINT gameid;//游戏id

		struct_data_hs_ass_gamelist_erea()
		{
			std::memset(this, 0, sizeof(struct_data_hs_ass_gamelist_erea));
		}
	};

	//客户端请求获取比赛节点结构体
	struct struct_data_hs_ass_gamelist_contest
	{
		UINT ereaid;//区服id

		struct_data_hs_ass_gamelist_contest()
		{
			std::memset(this, 0, sizeof(struct_data_hs_ass_gamelist_contest));
		}
	};

	//客户端请求获取比赛节点结构体
	struct struct_data_hs_ass_gamelist_contest_online
	{
		UINT contestid;//比赛id

		struct_data_hs_ass_gamelist_contest_online()
		{
			std::memset(this, 0, sizeof(struct_data_hs_ass_gamelist_contest_online));
		}
	};

	//客户端请求获取房间节点结构体
	struct struct_data_hs_ass_gamelist_room
	{
		UINT contestid;//比赛id

		struct_data_hs_ass_gamelist_room()
		{
			std::memset(this, 0, sizeof(struct_data_hs_ass_gamelist_room));
		}
	};

	///游戏类型数据
	struct ComGameinfo
	{
		UINT gameid;//游戏id
		char gamename[GAMENAME_LENGTH];//游戏名称

		ComGameinfo()
		{
			reset();
		}

		void reset()
		{
			std::memset(this, 0, sizeof(ComGameinfo));
		}
	};

	///区服数据
	struct ComEreainfo
	{
		UINT ereaid;//游戏区服id
		char ereaname[EREANAME_LENGTH];//区服名称
		UINT gameid;//游戏id
		ComEreainfo()
		{
			reset();
		}

		void reset()
		{
			std::memset(this, 0, sizeof(ComEreainfo));
		}
	};

	///比赛类型数据
	struct ComContestInfo
	{
		UINT contestid;//比赛id
		char contestname[CONTESTNAME_LENGTH];//比赛名称
		UINT gameid;//游戏id
		UINT ereaid;//游戏区服id
		char cContestDescribe[200];//比赛描述
		ENUM_ContestRule uContestRule;//比赛模式
		ENUM_ContestKind uContestKind;//比赛场次类型
		LLONG _i64cost;//报名费用
		ENUM_AwardType awardType;//奖励类型
		UINT award;//奖励
		ENUM_RoomRule uRoomRule;//房间规则
		//报名人数
		ComContestInfo()
		{
			reset();
		}

		void reset()
		{
			std::memset(this, 0, sizeof(ComContestInfo));
			uContestRule = US_ContestRule_solo;
			awardType = ENUM_AwardType_point;
			uRoomRule = US_RoomRule_1v1;

		}
	};

	///房间类型数据
	struct ComRoomInfo
	{
		UINT roomid;//房间id
		char roomname[ROOMNAME_LENGTH];//房间名称
		UINT gameid;//游戏id
		UINT ereaid;//游戏区服id
		UINT contestid;//比赛id
		char strIP[30];//房间ip地址
		UINT uport;//房间端口号
		UINT uOlineCount;//在线人数
		ComRoomInfo()
		{
			reset();
		}
		void reset()
		{
			std::memset(this, 0, sizeof(ComRoomInfo));
		}
	};
}

#endif//_WJ_NetPM_gamelist_H_