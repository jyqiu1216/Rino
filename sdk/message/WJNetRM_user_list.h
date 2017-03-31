#ifndef _WJ_WJNetRM_user_list_H_
#define _WJ_WJNetRM_user_list_H_

#include "common/WJBaseType.h"
#include "common/WJUserInfoBaseStruct.h"

namespace WJ
{
	//e_gs_mess_user_list=100,//用户列表
	enum E_ROOM_ASSID_enum_gs_mess_user_list
	{
		e_gs_ass_user_list_update_userinfo = 1,//更新玩家信息
		e_gs_ass_user_list_get_desk_userinfo = 2,//获取桌子里面所有玩家的数据
	};

	////更新玩家信息
	//struct RoomMessDate_update_userinfo
	//{
	//	UINT udeskid;//桌子id
	//	UINT udeskstation;//座位
	//	bool bOwerUserid;//是否为房主
	//	ENUM_UserStation bUserState;						//用户状态
	//	RoomMessDate_update_userinfo()
	//	{
	//		std::memset(this, 0, sizeof(RoomMessDate_update_userinfo));
	//	}
	//};

	enum E_HALL_HandleCodeID_enum_gs_mess_get_desk_userinfo
	{
		e_gs_hcid_userlist_part = 1,//一部分，还没有发完
		e_gs_hcid_userlist_finish = 2,//所请求列表发完了
	};

	//获取桌子里面所有玩家的数据中每一个玩家信息结构体
	struct WJ::UserInfoInRoomStruct;
}


#endif//_WJ_WJNetRM_user_list_H_