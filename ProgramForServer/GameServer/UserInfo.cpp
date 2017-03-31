#include "UserInfo.h"
#include "server/WJSSocket.h"

namespace WJ
{
	//构造函数
	CGameUserInfo::CGameUserInfo(void)
	{
		m_uSockId = INVALID_SOCKET_ID;
		memset(&m_UserData, 0, sizeof(m_UserData));
	}

	//析构函数
	CGameUserInfo::~CGameUserInfo(void)
	{
	}


	bool CGameUserInfo::SetUserData(UserInfoInRoomStruct & UserInfo, UINT uSocketId)
	{
		memcpy(&m_UserData, &UserInfo, sizeof(UserInfoInRoomStruct));
		m_uSockId = uSocketId;
		return true;
	}

	//设置用户状态
	bool CGameUserInfo::SetUserState(ENUM_UserStation bUserState, UINT uDeskNO, UINT uDeskStation, bool bSetDeskStation)
	{
		if (bSetDeskStation)
		{
			m_UserData.uDeskid = uDeskNO;
			m_UserData.uDeskStation = uDeskStation;
		}
		m_UserData.basedata.bUserState = bUserState;
		return true;
	}

	//更改用户分数
	bool CGameUserInfo::ChangePoint(__int64 dwPoint, __int64 dwTaxCom, bool bWin, bool bLost, bool bMid, bool bCut, long int dwPlayCount, __int64 &dwMoney)
	{
		return false;
	}

	//清理用户信息
	bool CGameUserInfo::CleanData()
	{
		m_uSockId = INVALID_SOCKET_ID;
		m_UserData.reset();
		return true;
	}

	///获取用户信息
	UserInfoInRoomStruct * CGameUserInfo::GetUserData()
	{
		return &m_UserData;
	}
}

