
#ifndef WJ_USERINFO_H
#define WJ_USERINFO_H

#include "common/WJBaseType.h"
#include "common/WJUserInfoBaseStruct.h"
using namespace std;

namespace WJ
{
	///用户资料类
	class CGameUserInfo
	{
		///变量定义
	private:
		UserInfoInRoomStruct				m_UserData;							///用户资料
		UINT								m_uSockId;						    ///网络索引
		///函数定义 
	public:
		///构造函数 
		CGameUserInfo(void);
		///析构函数
		~CGameUserInfo(void);

		///功能函数
	public:
		///设置信息
		bool SetUserData(UserInfoInRoomStruct & UserInfo, UINT uSocketId);
		///设置状态
		bool SetUserState(ENUM_UserStation bUserState, UINT bDeskNO, UINT uDeskStation, bool bSetDeskStation);
		///更改用户分数
		bool ChangePoint(__int64 dwPoint, __int64 dwTaxCom, bool bWin, bool bLost, bool bMid, bool bCut, long int dwPlayCount, __int64 &dwMoney);
		///清理用户信息
		bool CleanData();
		///获取用户信息
		UserInfoInRoomStruct * GetUserData();

		///获取网络索引
		inline UINT GetSocketId() { return m_uSockId; }
	};

	/*******************************************************************************************************/



	/*******************************************************************************************************/
}

#endif//WJ_USERINFO_H