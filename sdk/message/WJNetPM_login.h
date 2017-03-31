#ifndef _WJ_NetPM_login_H_
#define _WJ_NetPM_login_H_

#include "common/WJBaseType.h"
#include "../common/WJUserInfoBaseStruct.h"

namespace WJ
{
	//e_hs_mess_login = 100,//登陆
	enum E_HALL_ASSID_enum_hs_mess_login
	{
		e_hs_ass_login = 1,//普通登陆
		e_hs_ass_login_c_finish = 2,//客户端告诉服务器玩家登陆完成
		e_gs_ass_login_c_force_out = 3,//强退
		e_hs_ass_login_WX = 4,//微信登陆
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_login
	{
		e_hs_hcid_login_suc = 1,//登陆成功
		e_hs_hcid_login_err_unexist = 2,//登陆失败_不存在该用户
		e_hs_hcid_login_err_password = 3,//登陆失败_密码错误
		e_hs_hcid_login_err_online = 4,//登陆失败_已在线
	};

	//客户端登陆请求结构体
	struct struct_data_hs_mess_login
	{
		char username[NAME_LENGTH];//用户名
		char password[PASSWORD_LENGTH];//密码

		struct_data_hs_mess_login()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_login));
		}
	};

	//客户端微信登陆请求结构体
	struct struct_data_hs_mess_login_WX
	{
		char username[NAME_LENGTH];//用户名
		UINT userid;//用户id
		char wx_key[200];//微信返回的key
		struct_data_hs_mess_login_WX()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_login_WX));
		}
	};

	//登陆成功结构体
	typedef UserInfoInHallStruct struct_data_hs_hcid_login_suc;

}

#endif//_WJ_NetPM_login_H_