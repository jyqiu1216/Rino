
#ifndef WJ_USERINFO_H
#define WJ_USERINFO_H

#include "common/WJBaseType.h"
#include "common/WJUserInfoBaseStruct.h"
using namespace std;

namespace WJ
{
	///用户资料类
	class CLoginUserInfo
	{
		///变量定义
	private:
		UserInfoInHallStruct				m_UserData;							///用户资料
		UINT								m_uSocketIndex;						///网络索引
		///函数定义 
	public:
		///构造函数 
		CLoginUserInfo(void);
		///析构函数
		~CLoginUserInfo(void);

		///功能函数
	public:
		///设置信息
		bool SetUserData(UserInfoInHallStruct & UserInfo, UINT uSocketIndex);
		///设置状态
		bool SetUserState(ENUM_UserStation bUserState);
		///更改用户分数
		bool ChangePoint(__int64 dwPoint, __int64 dwTaxCom, bool bWin, bool bLost, bool bMid, bool bCut, long int dwPlayCount, __int64 &dwMoney);
		///清理用户信息
		bool CleanData();
		///获取用户信息
		UserInfoInHallStruct * GetUserData();

		///获取网络索引
		UINT GetSocketIndex();

		//是否可以切换用户状态（比如报名中不能进行退费，应该要等报名状态变为空闲状态）
		bool bCanChangeUserStation(ENUM_UserStation	bUserState);
	};

	/*******************************************************************************************************/



	/*******************************************************************************************************/
}

#endif//WJ_USERINFO_H