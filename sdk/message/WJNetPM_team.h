#ifndef _WJ_NetPM_team_H_
#define _WJ_NetPM_team_H_

#include "common/WJBaseType.h"
namespace WJ
{
	//e_hs_mess_team = 103,//团队
	enum E_HALL_ASSID_enum_hs_mess_team
	{
		e_hs_ass_team_create = 1,//创建团队
		e_hs_ass_team_join = 2,//加入团队
		e_hs_ass_team_leave = 3,//退出团队
		e_hs_ass_team_dissolve = 4,//解散团队
		e_hs_ass_team_start = 5,//队长发起开始请求
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_team_create
	{
		e_hs_hcid_team_create_suc = 1,//创建团队成功
		e_hs_hcid_team_create_err_contest_inexistence = 2,//创建团队失败_比赛不存在
		e_hs_hcid_team_create_err_room_inexistence = 3,//创建团队失败_房间不存在
		e_hs_hcid_team_create_err_nomoney = 4,//创建团队失败_创建费不够
		e_hs_hcid_team_create_err_exist = 5,//创建团队失败_已在团队中，还未解散
		e_hs_hcid_team_create_err_applyed = 6,//创建团队失败_已报名比赛，还未结束
		e_hs_hcid_team_create_err_contest_rule = 7,//创建团队失败_比赛模式不是团队赛
		e_hs_hcid_team_create_err_PayType = 8,//创建团队失败_支付类型未知
		e_hs_hcid_team_create_err_unknown = 9,//创建团队失败_未知错误
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_team_join
	{
		e_hs_hcid_team_join_suc = 1,//加入团队成功
		e_hs_hcid_team_join_err_contest_inexistence = 2,//加入团队失败_比赛不存在
		e_hs_hcid_team_join_err_nomoney = 3,//加入团队失败_报名费不够
		e_hs_hcid_team_join_err_exist = 4,//加入团队失败_已在团队中，还未解散
		e_hs_hcid_team_join_err_applyed = 5,//加入团队失败_已报名比赛，还未结束
		e_hs_hcid_team_join_err_contest_rule = 6,//加入团队失败_比赛模式不是团队赛
		e_hs_hcid_team_join_err_password = 7,//加入团队失败_团队暗号错误
		e_hs_hcid_team_join_err_full = 8,//加入团队失败_人数已满
		e_hs_hcid_team_join_err_unknown = 9,//加入团队失败_未知错误
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_team_leave
	{
		e_hs_hcid_team_leave_suc = 1,//退出团队成功
		e_hs_hcid_team_leave_err_contest_inexistence = 2,//退出团队失败_比赛不存在
		e_hs_hcid_team_leave_err_created = 3,//退出团队失败_自己不能退自己创建的团队
		e_hs_hcid_team_leave_err_playing = 4,//退出团队失败_游戏进行中
		e_hs_hcid_team_leave_err_contest_rule = 5,//退出团队失败_比赛模式不是团队赛
		e_hs_hcid_team_leave_err_applyed = 6,//退出团队失败_已报名比赛，还未结束
		e_hs_hcid_team_leave_err_password = 7,//退出团队失败_团队暗号错误
		e_hs_hcid_team_leave_err_money = 8,//退出团队失败_退参赛费失败
		e_hs_hcid_team_leave_err_unknown = 9,//退出团队失败_未知错误
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_team_dissolve
	{
		e_hs_hcid_team_dissolve_suc = 1,//解散团队成功
		e_hs_hcid_team_dissolve_err_contest_inexistence = 2,//解散团队失败_比赛不存在
		e_hs_hcid_team_dissolve_err_contest_rule = 3,//解散团队失败_比赛模式不是团队赛
		e_hs_hcid_team_dissolve_err_uncreated = 4,//解散团队失败_自己并没有创建团队
		e_hs_hcid_team_dissolve_err_playing = 5,//解散团队失败_游戏进行中
		e_hs_hcid_team_dissolve_err_applyed = 6,//解散团队失败_已报名比赛，还未结束
		e_hs_hcid_team_dissolve_err_password = 7,//解散团队失败_团队暗号错误
		e_hs_hcid_team_dissolve_err_money = 8,//解散团队失败_退参赛费失败
		e_hs_hcid_team_dissolve_err_unknown = 9,//解散团队失败_未知错误
	};

	//客户端创建团队请求结构体
	struct struct_data_hs_mess_team_create
	{
		UINT ucontestid;//比赛id
		ENUM_TeamPayType m_TeamPayType;//支付方式

		struct_data_hs_mess_team_create()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_team_create));
		}
	};

	//创建团队成功后返回数据
	typedef TeamStruct struct_data_hs_mess_team_create_res;
	
	//客户端加入团队请求结构体
	struct struct_data_hs_mess_team_join
	{
		UINT ucontestid;//比赛id
		char m_Team_password[TEAM_PASS_LENGTH];//团队暗号
		struct_data_hs_mess_team_join()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_team_join));
		}
	};

	//加入团队成功后返回数据
	typedef TeamStruct struct_data_hs_mess_team_join_res;

	//退出团队请求
	typedef struct_data_hs_mess_team_join struct_data_hs_mess_team_leave;

	//退出团队成功后返回数据给团队内其他玩家
	struct struct_data_hs_mess_team_leave_res
	{
		UINT userid;//退出的人的userid

		struct_data_hs_mess_team_leave_res()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_team_leave_res));
		}
	};

	//解散团队请求
	typedef struct_data_hs_mess_team_join struct_data_hs_mess_team_dissolve;

	//队长请求开始匹配比赛
	typedef struct_data_hs_mess_team_join struct_data_hs_mess_team_start;

	enum E_HALL_HandleCodeID_enum_hs_mess_team_start
	{
		e_hs_hcid_team_start_suc = 1,//启动匹配成功
		//e_hs_hcid_team_start_err_no_free_room = 2,//启动匹配失败_还没有空闲房间
	};

	//启动匹配成功, 回复给客户端
	struct struct_data_hs_hcid_team_start_suc
	{
		char strIP[30];//房间ip地址
		UINT uport;//房间端口号
		TeamStruct_VS data;

		struct_data_hs_hcid_team_start_suc()
		{
			std::memset(this, 0, sizeof(struct_data_hs_hcid_team_start_suc));
		}
	};

}

#endif//_WJ_NetPM_team_H_