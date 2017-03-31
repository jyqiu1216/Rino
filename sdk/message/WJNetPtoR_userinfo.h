#ifndef _WJ_WJNetPtoR_userinfo_H_
#define _WJ_WJNetPtoR_userinfo_H_

#include "common/WJBaseType.h"
#include "common/WJUserInfoBaseStruct.h"
namespace WJ
{
	//e_pr_mess_userinfo = 501,//用户信息
	enum E_PtoR_ASSID_enum_userinfo
	{
		e_pr_ass_userinfo_login = 1,//玩家登录游戏服务器
		e_pr_ass_userinfo_allot_room = 2,//gs告诉ls玩家加入房间
		e_pr_ass_userinfo_apply_restitution = 3,//大厅询问房间，玩家是否可以退费
		e_pr_ass_userinfo_unplay = 4,//玩家离开比赛
		e_pr_ass_userinfo_gameover = 5,//游戏服务器通知大厅，玩家已经结束游戏
		e_pr_ass_userinfo_team_leave = 6,//大厅询问房间，玩家是否可以退出团队
		e_pr_ass_userinfo_team_dissolve = 7,//大厅询问房间，玩家是否可以解散团队
		e_pr_ass_userinfo_team_start = 8,//大厅通知相关比赛的房间，团队准备加入
		e_pr_ass_userinfo_team_vs_finish = 9,//游戏服务器通知大厅，团队已经结束游戏
	};

	//游戏服务器告知登陆服务器，有玩家登录游戏服务器，请求确认
	struct struct_data_pr_userinfo_login
	{
		UINT uuserid;//用户id
		UINT contestid;
		UINT uIndex;//玩家当时连接游戏服务器分配的套接字id
		struct_data_pr_userinfo_login()
		{
			std::memset(this, 0, sizeof(struct_data_pr_userinfo_login));
			uIndex = ERROR_SOCKETID;
		}
	};

	enum E_hcid_enum_pr_userinfo_login
	{
		e_pr_hcid_login_allow = 1,//允许加入
		e_pr_hcid_login_unallow = 2,//不允许加入
	};

	//玩家登录游戏服务器允许的消息包
	struct struct_data_pr_userinfo_login_res_ok
	{
		UserInfoInHallStruct data;
		UINT contestid;
		UINT uIndex;//玩家当时连接游戏服务器分配的套接字id
		struct_data_pr_userinfo_login_res_ok()
		{
			std::memset(this, 0, sizeof(struct_data_pr_userinfo_login_res_ok));
			uIndex = ERROR_SOCKETID;
		}
	};

	//玩家登录游戏服务器不允许的消息包
	struct struct_data_pr_userinfo_login_res_err
	{
		UINT userid;//用户id
		UINT uIndex;//玩家当时连接游戏服务器分配的套接字id
		struct_data_pr_userinfo_login_res_err()
		{
			std::memset(this, 0, sizeof(struct_data_pr_userinfo_login_res_err));
			uIndex = ERROR_SOCKETID;
		}
	};

	///////////////////////////////////////////////////////////////////////
	//游戏服务器告知登陆服务器，有玩家加入该房间
	struct struct_data_pr_userinfo_allot_room
	{
		UINT userid;//用户id
		UINT contestid;//比赛id
		UINT roomid;//房间id
		UINT uIndex;//玩家当时连接游戏服务器分配的套接字id
		struct_data_pr_userinfo_allot_room()
		{
			std::memset(this, 0, sizeof(struct_data_pr_userinfo_allot_room));
			uIndex = ERROR_SOCKETID;
		}
	};

	//退费成功与否
	enum E_hcid_enum_pr_userinfo_apply_restitution
	{
		e_pr_hcid_apply_restitution_suc = 1,//退费成功
		e_pr_hcid_apply_restitution_err = 2,//退费失败
	};

	//玩家向游戏服务器请求退费 和 游戏服务器回复登陆服务器结果的消息包
	struct struct_data_pr_userinfo_apply_restitution
	{
		UINT userid;//用户id

		struct_data_pr_userinfo_apply_restitution()
		{
			std::memset(this, 0, sizeof(struct_data_pr_userinfo_apply_restitution));
		}
	};

	//玩家没有在房间里面玩，比赛继续有效
	typedef struct_data_pr_userinfo_apply_restitution struct_data_pr_userinfo_unplay;
	//游戏服务器通知大厅，玩家已经结束游戏
	typedef struct_data_pr_userinfo_apply_restitution struct_data_pr_userinfo_gameover;

	//房间接受团队与否
	enum E_hcid_enum_pr_userinfo_team_start
	{
		e_pr_hcid_team_start_suc = 1,//房间接受团队成功
		e_pr_hcid_team_start_err = 2,//房间接受团队失败
	};

	//房间告知大厅，团队一局游戏结束
	struct struct_data_pr_userinfo_team_vs_finish
	{
		TeamStruct_VS _data;
		struct_data_pr_userinfo_team_vs_finish()
		{
			std::memset(this, 0, sizeof(struct_data_pr_userinfo_team_vs_finish));
		}
	};
}

#endif//_WJ_WJNetPtoR_userinfo_H_