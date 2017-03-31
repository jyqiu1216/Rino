#ifndef _WJ_R_a_h_team_H_
#define _WJ_R_a_h_team_H_

#include "common/WJBaseType.h"
namespace WJ
{
#define Redis_Order_h_messid_team_create 5//玩家创建团队
#define Redis_Order_h_messid_team_join 6//玩家加入团队
#define Redis_Order_h_messid_team_leave 7//玩家退出团队
#define Redis_Order_h_messid_team_dissolve 8//玩家解散团队
	//请求创建团队的redis结构体
	struct RRLine_c_team_create : public RedisDataBaseLine
	{
		LLONG changemoney;//所扣的娱币
		UINT changepoint;//所扣的积分
		ENUM_ContestKind uContestKind;//比赛场次类型
		UINT userid;//用户id
		UINT ucontestid;//比赛id
		ENUM_TeamPayType m_TeamPayType;//支付方式
		ENUM_RoomRule uRoomRule;//房间规则
		RRLine_c_team_create()
		{
			std::memset(this, 0, sizeof(RRLine_c_team_create));
			uRediscommandid = Redis_Order_h_messid_team_create;
		}
	};

	//请求创建团队的结果返回
	struct RRLine_s_team_create : public RedisResultLine
	{
		LLONG changemoney;//所扣的娱币
		UINT changepoint;//所扣的积分
		bool bSuc;//是否成功
		ENUM_ContestKind uContestKind;//比赛场次类型
		UINT userid;//用户id
		UINT ucontestid;//比赛id
		ENUM_TeamPayType m_TeamPayType;//支付方式
		ENUM_RoomRule uRoomRule;//房间规则
		RRLine_s_team_create()
		{
			std::memset(this, 0, sizeof(RRLine_s_team_create));
			uRediscommandid = Redis_Order_h_messid_team_create;
		}
	};

	struct CTransaction_team_create
	{
		RRLine_c_team_create m_in;
		RRLine_s_team_create m_out;
	};

	//请求加入团队的redis结构体
	struct RRLine_c_team_join : public RedisDataBaseLine
	{
		LLONG changemoney;//所扣的娱币
		UINT changepoint;//所扣的积分
		ENUM_ContestKind uContestKind;//比赛场次类型
		UINT userid;//用户id
		UINT ucontestid;//比赛id
		char m_Team_password[TEAM_PASS_LENGTH];//团队暗号
		RRLine_c_team_join()
		{
			std::memset(this, 0, sizeof(RRLine_c_team_join));
			uRediscommandid = Redis_Order_h_messid_team_join;
		}
	};

	//请求加入团队的结果返回
	struct RRLine_s_team_join : public RedisResultLine
	{
		LLONG changemoney;//所扣的娱币
		UINT changepoint;//所扣的积分
		bool bSuc;//是否成功
		ENUM_ContestKind uContestKind;//比赛场次类型
		UINT userid;//用户id
		UINT ucontestid;//比赛id
		char m_Team_password[TEAM_PASS_LENGTH];//团队暗号
		RRLine_s_team_join()
		{
			std::memset(this, 0, sizeof(RRLine_s_team_join));
			uRediscommandid = Redis_Order_h_messid_team_join;
		}
	};

	struct CTransaction_team_join
	{
		RRLine_c_team_join m_in;
		RRLine_s_team_join m_out;
	};

	//请求退出团队的redis结构体
	struct RRLine_c_team_leave : public RedisDataBaseLine
	{
		LLONG changemoney;//所扣的娱币
		UINT changepoint;//所扣的积分
		ENUM_ContestKind uContestKind;//比赛场次类型
		UINT userid;//用户id
		UINT ucontestid;//比赛id
		char m_Team_password[TEAM_PASS_LENGTH];//团队暗号
		RRLine_c_team_leave()
		{
			std::memset(this, 0, sizeof(RRLine_c_team_leave));
			uRediscommandid = Redis_Order_h_messid_team_leave;
		}
	};

	//请求退出团队的结果返回
	struct RRLine_s_team_leave : public RedisResultLine
	{
		LLONG changemoney;//所扣的娱币
		UINT changepoint;//所扣的积分
		bool bSuc;//是否成功
		ENUM_ContestKind uContestKind;//比赛场次类型
		UINT userid;//用户id
		UINT ucontestid;//比赛id
		char m_Team_password[TEAM_PASS_LENGTH];//团队暗号
		RRLine_s_team_leave()
		{
			std::memset(this, 0, sizeof(RRLine_s_team_leave));
			uRediscommandid = Redis_Order_h_messid_team_leave;
		}
	};

	struct CTransaction_team_leave
	{
		RRLine_c_team_leave m_in;
		RRLine_s_team_leave m_out;
	};

	//请求退出团队的redis结构体
	struct RRLine_c_team_dissolve : public RedisDataBaseLine
	{
		LLONG changemoney[TEAM_USER_NUM];//所扣的娱币
		UINT changepoint[TEAM_USER_NUM];//所扣的积分
		ENUM_ContestKind uContestKind;//比赛场次类型
		UINT userid[TEAM_USER_NUM];//用户id
		UINT ucontestid;//比赛id
		char m_Team_password[TEAM_PASS_LENGTH];//团队暗号
		UINT m_uTemaPlayerNum;//队伍人数
		RRLine_c_team_dissolve()
		{
			std::memset(this, 0, sizeof(RRLine_c_team_dissolve));
			uRediscommandid = Redis_Order_h_messid_team_dissolve;
		}
	};

	//请求退出团队的结果返回
	struct RRLine_s_team_dissolve : public RedisResultLine
	{
		LLONG changemoney[TEAM_USER_NUM];//所扣的娱币
		UINT changepoint[TEAM_USER_NUM];//所扣的积分
		bool bSuc;//是否成功
		ENUM_ContestKind uContestKind;//比赛场次类型
		UINT userid[TEAM_USER_NUM];//用户id
		UINT ucontestid;//比赛id
		char m_Team_password[TEAM_PASS_LENGTH];//团队暗号
		UINT m_uTemaPlayerNum;//队伍人数
		RRLine_s_team_dissolve()
		{
			std::memset(this, 0, sizeof(RRLine_s_team_dissolve));
			uRediscommandid = Redis_Order_h_messid_team_dissolve;
		}
	};

	struct CTransaction_team_dissolve
	{
		RRLine_c_team_dissolve m_in;
		RRLine_s_team_dissolve m_out;
	};
}

#endif//_WJ_R_a_h_team_H_