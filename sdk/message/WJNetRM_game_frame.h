#ifndef _WJ_WJNetRM_game_frame_H_
#define _WJ_WJNetRM_game_frame_H_

#include "common/WJBaseType.h"
#include "common/WJUserInfoBaseStruct.h"
namespace WJ
{
	//e_gs_mess_game_frame = 104,//游戏框架消息
	enum E_ROOM_ASSID_enum_gs_mess_game_frame
	{
		e_gs_ass_game_frame_info = 1,//请求游戏桌子数据消息
		e_gs_ass_game_frame_station = 2,//发送游戏状态消息
		e_gs_ass_game_frame_agree = 3,//玩家举手同意消息
		e_gs_ass_game_frame_begin = 4,//游戏正式开始
		e_gs_ass_game_frame_end_c = 5,//玩家告诉服务器游戏结束,并且服务器会转发
		e_gs_ass_game_frame_end_s = 6,//游戏正式结束
		e_gs_ass_game_frame_user_leave = 7,//玩家在非游戏进行中离开桌子
	};

	///游戏信息
	struct RoomMessDate_deskinfo
	{
		UINT								bGameStation;						///游戏状态
		UINT								bReserve;							///保留字段

		RoomMessDate_deskinfo()
		{
			std::memset(this, 0, sizeof(RoomMessDate_deskinfo));
		}
	};

	//玩家举手准备
	struct RoomMessDate_user_frame_agree
	{
		UINT udeskid;//桌子id
		UINT udeskstation;//座位
		UserInfoInRoomStruct userdata;//玩家数据
		RoomMessDate_user_frame_agree()
		{
			std::memset(this, 0, sizeof(RoomMessDate_user_frame_agree));
		}
	};

	//第三方创建房间的结果
	enum E_hcid_enum_game_frame_third_party_room_create
	{
		e_gs_hcid_game_frame_third_party_room_create_suc = 1,//创建成功
		e_gs_hcid_game_frame_third_party_room_create_fail = 2,//创建失败
	};
}

#endif//_WJ_WJNetRM_game_frame_H_