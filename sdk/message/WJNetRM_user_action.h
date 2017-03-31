#ifndef _WJ_WJNetRM_user_action_H_
#define _WJ_WJNetRM_user_action_H_

#include "common/WJBaseType.h"
#include "common/WJUserInfoBaseStruct.h"

namespace WJ
{
	//e_gs_mess_user_action = 101,//用户动作
	enum E_ROOM_ASSID_enum_gs_mess_user_action
	{
		e_gs_ass_user_action_login = 1,//登陆游戏服务器
		e_gs_ass_user_action_login_cancel = 2,//取消登录
		e_gs_ass_user_action_team_out = 3,//团队赛的时候，玩家离开了
		e_gs_ass_user_action_team_come = 4,//团队赛的时候，玩家回来了
	};

	//客户端发起登陆房间请求
	struct RoomMessDate_user_action_login
	{
		UINT uuserid;//用户id
		UINT contestid;

		RoomMessDate_user_action_login()
		{
			std::memset(this, 0, sizeof(RoomMessDate_user_action_login));
		}
	};

	//服务端回复登陆结果
	enum E_ROOM_HandleCodeID_enum_hs_mess_login
	{
		e_gs_hcid_login_suc_ = 1,//登陆成功
		e_gs_hcid_login_suc_recome = 2,//登陆成功_断线重回
		e_gs_hcid_login_err_ = 3,//登陆失败
	};

	//登陆成功（包括断线重回）返回以下结构体
	typedef UserInfoInRoomStruct struct_data_gs_mess_login_suc;

	//客户端取消登录请求
	struct RoomMessDate_user_action_login_cancel
	{
		UINT uuserid;//用户id
		RoomMessDate_user_action_login_cancel()
		{
			std::memset(this, 0, sizeof(RoomMessDate_user_action_login_cancel));
		}
	};

	//服务端回复取消登陆结果
	enum E_ROOM_HandleCodeID_enum_hs_mess_login_cancel
	{
		e_gs_hcid_login_cancel_suc_ = 1,//取消登陆成功
		e_gs_hcid_login_cancel_err_ = 2,//取消登陆失败
	};
}

#endif//_WJ_WJNetRM_user_action_H_