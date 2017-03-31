#include "LUserInfo.h"
#include "common/commonfunction.h"
#include "common\QkLogger.h"

namespace WJ
{
	//构造函数
	CLoginUserInfo::CLoginUserInfo(void)
	{
		m_uSocketIndex = ERROR_SOCKETID;
		memset(&m_UserData, 0, sizeof(m_UserData));
	}

	//析构函数
	CLoginUserInfo::~CLoginUserInfo(void)
	{
	}

	UINT CLoginUserInfo::GetSocketIndex()
	{
		return m_uSocketIndex; 
	}

	bool CLoginUserInfo::SetUserData(UserInfoInHallStruct & UserInfo, UINT uSocketIndex)
	{
		if (uSocketIndex == ERROR_SOCKETID)
		{
			QK_InfoLog("sdp_testaaa 22222 m_uSocketIndex=%d", m_uSocketIndex);
		}

		memcpy(&m_UserData, &UserInfo, sizeof(UserInfoInHallStruct));
		m_uSocketIndex = uSocketIndex;
		return true;
	}

	//设置用户状态
	bool CLoginUserInfo::SetUserState(ENUM_UserStation bUserState)
	{
		m_UserData.basedata.bUserState = bUserState;
		QK_InfoLog("sdp_test hall 设置玩家状态(%d)", bUserState);
		return true;
	}

	//更改用户分数
	bool CLoginUserInfo::ChangePoint(__int64 dwPoint, __int64 dwTaxCom, bool bWin, bool bLost, bool bMid, bool bCut, long int dwPlayCount, __int64 &dwMoney)
	{
		return false;
	}

	//清理用户信息
	bool CLoginUserInfo::CleanData()
	{
		m_uSocketIndex = ERROR_SOCKETID;
		memset(&m_UserData, 0, sizeof(m_UserData));
		return true;
	}

	///获取用户信息
	UserInfoInHallStruct * CLoginUserInfo::GetUserData()
	{
		return &m_UserData;
	}

	//是否可以切换用户状态（比如报名中不能进行退费，应该要等报名状态变为空闲状态）
	bool CLoginUserInfo::bCanChangeUserStation(ENUM_UserStation	bUserState)
	{
		QK_InfoLog("sdp_test hall 判断玩家状态(%d, %d)", m_UserData.basedata.bUserState, bUserState);
		switch (m_UserData.basedata.bUserState)
		{
		case US_P_Wait:	//大厅空闲状态
		{
							return true;
		}break;
		case US_P_Applying:	//报名中状态
		case US_P_unApplying:	//取消报名中状态
		case US_P_CreateTeaming://创建团队中状态
		case US_P_AddTeaming://加入团队中状态
		case US_P_DissolveTeaming://解散团队中状态
		case US_P_LeaveTeaming://离开团队中状态
		case US_P_TeamVSing://团队VS中状态
		{
								return US_P_Wait == bUserState;
		}break;
		default:
		{
				   return false;
		}
		}

		return false;
	}
}

