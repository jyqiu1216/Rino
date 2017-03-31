#ifndef _WJ_NetPM_apply_H_
#define _WJ_NetPM_apply_H_

#include "common/WJBaseType.h"
#include "server/comstruct.h"
namespace WJ
{
	//e_hs_mess_apply = 102,//报名
	enum E_HALL_ASSID_enum_hs_mess_apply
	{
		e_hs_ass_apply = 1,//请求报名
		e_hs_ass_apply_login_recome_apply = 2,//玩家断线后回来发现还未加入房间，发送该消息给客户端
		e_hs_ass_apply_login_recome_playing = 3,//玩家断线后回来发现已经加入房间，发送该消息给客户端
		e_hs_ass_apply_restitution = 4,//退费
		e_hs_ass_apply_unplay = 5,//玩家没有在房间里面玩，比赛继续有效
		e_hs_ass_unapply = 6,//玩家并没有报名参加任何比赛
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_apply
	{
		e_hs_hcid_apply_suc = 1,//报名成功
		e_hs_hcid_apply_err_contest_inexistence = 2,//报名失败_比赛不存在
		e_hs_hcid_apply_err_room_inexistence = 3,//报名失败_房间不存在
		e_hs_hcid_apply_err_nomoney = 4,//报名失败_报名费不够
		e_hs_hcid_apply_err_applyed = 5,//报名失败_已报名，还未结束
		e_hs_hcid_apply_err_try_again = 6,//报名失败_上一局刚结束，等一会再报名
		e_hs_hcid_apply_err_unknown = 9,//报名失败_未知错误
	};

	//客户端报名请求结构体
	struct struct_data_hs_mess_apply
	{
		UINT ucontestid;//比赛id

		struct_data_hs_mess_apply()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_apply));
		}
	};

	//报名失败_上一局刚结束，等一会再报名
	struct struct_data_hs_mess_apply_try_again
	{
		LLONG lltimeout;//剩余等待时间(秒)

		struct_data_hs_mess_apply_try_again()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_apply_try_again));
		}
	};
	
	//登陆大厅，告诉玩家先前加入的比赛下所有房间(还未开始游戏)
	enum E_HALL_HandleCodeID_enum_hs_mess_login_recome_apply
	{
		e_hs_hcid_login_recome_apply_roomlist_part = 1,//一部分，还没有发完
		e_hs_hcid_login_recome_apply_roomlist_finish = 2,//所请求列表发完了
	};
	struct  ComRoomInfo;//房间节点
	//登陆大厅，告诉玩家先前加入的比赛id(已经开始游戏)
	struct struct_data_hs_mess_apply_login_recome_playing
	{
		ENUM_ContestKind uContestKind;//比赛场次类型
		UINT gameid;//游戏id
		UINT ereaid;//游戏区服id
		UINT ucontestid;//比赛id
		UINT roomid;//房间id
		char strIP[30];//房间ip地址
		UINT uport;//房间端口号
		struct_data_hs_mess_apply_login_recome_playing()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_apply_login_recome_playing));
		}
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_apply_restitution
	{
		e_hs_hcid_apply_restitution_suc = 1,//退费成功
		e_hs_hcid_apply_restitution_err = 2,//退费失败
	};

	//退费失败，肯定是因为已经游戏开始了
	typedef struct_data_hs_mess_apply_login_recome_playing struct_data_hs_mess_apply_restitution_recome;

	//通知客户端请继续为指定比赛寻找房间
	typedef struct_data_hs_mess_apply struct_data_hs_mess_apply_unplay;
}

#endif//_WJ_NetPM_apply_H_