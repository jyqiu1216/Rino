#ifndef _WJ_R_a_h_load_hall_H_
#define _WJ_R_a_h_load_hall_H_

#include "common/WJBaseType.h"


namespace WJ
{
#define Redis_Order_h_messid_load_hall 2//用户登录大厅
#define Redis_Order_h_messid_update_userinfo 9//更新用户数据
	//请求登录大厅的redis结构体
	struct RRLine_c_load_hall : public RedisDataBaseLine
	{
		struct_data_hs_mess_login data;
		struct_data_hs_mess_login_WX data2;
		bool bWX;//是否为微信登陆
		RRLine_c_load_hall()
		{
			std::memset(this, 0, sizeof(RRLine_c_load_hall));
			//uRediscommandid = Redis_Order_h_messid_load_hall;
		}
	};


	//请求登录大厅的结果返回
	struct RRLine_s_load_hall : public RedisResultLine
	{
		enum E_RES
		{
			e_login_err_unexist,//玩家不存在
			e_login_err_password,//密码错误
			e_login_err_online,//玩家已经存在
			e_login_suc,//登陆成功
		};

		struct_data_hs_hcid_login_suc _data;
		RRLine_s_load_hall()
		{
			std::memset(this, 0, sizeof(RRLine_s_load_hall));
			//uRediscommandid = Redis_Order_h_messid_load_hall;
		}
	};

	struct CTransaction_load_hall
	{
		RRLine_c_load_hall m_in;
		RRLine_s_load_hall m_out;
	};

	//刷新用户数据的结果返回
	struct RRLine_s_Update_Userinfo : public RedisResultLine
	{
		UINT				iuserid;
		UINT				itype;							//1积分，2娱币
		UINT				dwPoint;						//积分
		LLONG				i64Money;						//娱币
		RRLine_s_Update_Userinfo()
		{
			std::memset(this, 0, sizeof(RRLine_s_Update_Userinfo));
			uRediscommandid = Redis_Order_h_messid_update_userinfo;
		}
	};

	struct CTransaction_Update_Userinfo
	{
		RedisDataBaseLine m_in;
		std::vector<RRLine_s_Update_Userinfo> m_out_vector;
	};
}

#endif//_WJ_R_a_h_load_hall_H_