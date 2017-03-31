#ifndef _WJ_WJNetRM_room_H_
#define _WJ_WJNetRM_room_H_

#include "common/WJBaseType.h"

namespace WJ
{
	//e_gs_mess_room = 102,//房间信息
	enum E_ROOM_ASSID_enum_gs_mess_room
	{
		e_gs_ass_room_dissolve_desk = 1,//解散桌子
		e_gs_ass_room_chat = 2,//聊天
		e_gs_ass_room_info = 3,//发送给客户端的房间数据
	};

	enum E_hcid_enum_gs_room_chat
	{
		e_gs_hcid_gs_room_chat_for_room = 1,//广播到房间内
		e_gs_hcid_gs_room_chat_for_desk = 2,//组播到桌子
		e_gs_hcid_gs_room_chat_for_player = 3,//单播给个人
	};

	//聊天包
	struct RoomMessDate_room_chat
	{
		char from_username[NAME_LENGTH];//发起者用户名
		char to_username[NAME_LENGTH];//接收者用户名

		char mess[CHAT_LENDTH];//内容
		UINT strleng;		//内容长度
		RoomMessDate_room_chat()
		{
			std::memset(this, 0, sizeof(RoomMessDate_room_chat));
		}
	};
}

#endif//_WJ_WJNetRM_room_H_