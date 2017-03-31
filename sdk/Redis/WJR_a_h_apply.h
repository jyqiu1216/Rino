#ifndef _WJ_R_a_h_apply_H_
#define _WJ_R_a_h_apply_H_

#include "common/WJBaseType.h"
namespace WJ
{
#define Redis_Order_h_messid_apply 3//玩家报名
#define Redis_Order_h_messid_apply_restitution 4//玩家退费


	//请求报名的redis结构体
	struct RRLine_c_apply : public RedisDataBaseLine
	{
		LLONG changemoney;//所扣的娱币
		UINT changepoint;//所扣的积分
		ENUM_ContestKind uContestKind;//比赛场次类型
		UINT userid;//用户id
		UINT ucontestid;//比赛id
		RRLine_c_apply()
		{
			std::memset(this, 0, sizeof(RRLine_c_apply));
			uRediscommandid = Redis_Order_h_messid_apply;
		}
	};

	//请求报名的结果返回
	struct RRLine_s_apply : public RedisResultLine
	{
		LLONG changemoney;//所扣的娱币
		UINT changepoint;//所扣的积分
		bool bSuc;//是否成功
		ENUM_ContestKind uContestKind;//比赛场次类型
		UINT userid;//用户id
		UINT ucontestid;//比赛id
		RRLine_s_apply()
		{
			std::memset(this, 0, sizeof(RRLine_s_apply));
			uRediscommandid = Redis_Order_h_messid_apply;
		}
	};

	//报名事务对象
	struct CTransaction_apply
	{
		RRLine_c_apply m_in;
		RRLine_s_apply m_out;
	};

//Redis_Order_h_messid_apply_restitution 4//玩家退费
	//请求退费的redis结构体
	struct RRLine_c_apply_restitution : public RedisDataBaseLine
	{
		LLONG changemoney;//所扣的娱币
		UINT changepoint;//所扣的积分
		ENUM_ContestKind uContestKind;//比赛场次类型
		UINT userid;//用户id
		UINT ucontestid;//比赛id
		RRLine_c_apply_restitution()
		{
			std::memset(this, 0, sizeof(RRLine_c_apply_restitution));
			uRediscommandid = Redis_Order_h_messid_apply_restitution;
		}
	};

	//请求退费的结果返回
	struct RRLine_s_apply_restitution : public RedisResultLine
	{
		LLONG changemoney;//所扣的娱币
		UINT changepoint;//所扣的积分
		bool bSuc;//是否成功
		ENUM_ContestKind uContestKind;//比赛场次类型
		UINT userid;//用户id
		UINT ucontestid;//比赛id
		RRLine_s_apply_restitution()
		{
			std::memset(this, 0, sizeof(RRLine_s_apply_restitution));
			uRediscommandid = Redis_Order_h_messid_apply_restitution;
		}
	};

	//请求退费事务对象
	struct CTransaction_apply_restitution
	{
		RRLine_c_apply_restitution m_in;
		RRLine_s_apply_restitution m_out;
	};
}

#endif//_WJ_R_a_h_apply_H_