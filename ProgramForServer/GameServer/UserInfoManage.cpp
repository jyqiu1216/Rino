
#include "UserInfoManage.h"
#include "common/WJCommonMarco.h"
#include "GameServer.h"

namespace WJ
{
	extern CGameServer * g_pGameServer;

	//构造函数
	CGameUserInfoManage::CGameUserInfoManage(void)
	{
	}

	//析构函数
	CGameUserInfoManage::~CGameUserInfoManage(void)
	{
		auto it = m_userid2userinfo.begin(), end = m_userid2userinfo.end();
        for (; it != end; ++it)
        {
            if (it->second)
            {
                WJ_SAFE_DELETE(it->second);
            }
        }
        m_userid2userinfo.clear();
        m_username2userid.clear();
		return;
	}


	//查找用户
	CGameUserInfo * CGameUserInfoManage::GetOnLineUserInfo(UINT dwUserID)
	{
		GameUserStruct* p = GetGameUserStruct(dwUserID, usertype_online);
		if (nullptr == p)
		{
			return nullptr;
		}
		return &(p->userinfo);
	}

	///获取在线用户
	CGameUserInfo* CGameUserInfoManage::GetOnLineUserInfoEx(UINT socketId)
	{
		GameUserStruct* p = GetGameUserStructEx(socketId, usertype_online);
		if (nullptr == p)
		{
			return nullptr;
		}
		return &(p->userinfo);
	}

	///获取在线用户
	CGameUserInfo* CGameUserInfoManage::GetOnLineUserInfo(char* username)
	{
		GameUserStruct* p = GetGameUserStruct(username, usertype_online);
		if (nullptr == p)
		{
			return nullptr;
		}
		return &(p->userinfo);
	}

	//查找用户
	CGameUserInfo * CGameUserInfoManage::GetNetCutUser(UINT dwUserID)
	{
		GameUserStruct* p = GetGameUserStruct(dwUserID, usertype_netcut);
		if (nullptr == p)
		{
			return nullptr;
		}
		return &(p->userinfo);
	}

	///获取在线用户
	CGameUserInfo* CGameUserInfoManage::GetNetCutUser(char* username)
	{
		GameUserStruct* p = GetGameUserStruct(username, usertype_netcut);
		if (nullptr == p)
		{
			return nullptr;
		}
		return &(p->userinfo);
	}

	CGameUserInfo* CGameUserInfoManage::GetUserInfo(UINT dwUserID)
	{
		GameUserStruct* p = GetGameUserStruct(dwUserID);
		if (nullptr == p)
		{
			return nullptr;
		}
		return &(p->userinfo);
	}

	CGameUserInfo* CGameUserInfoManage::GetUserInfoEx(UINT socketid)
	{
		GameUserStruct* p = GetGameUserStructEx(socketid);
		if (nullptr == p)
		{
			return nullptr;
		}
		return &(p->userinfo);
	}

	CGameUserInfo* CGameUserInfoManage::GetUserInfo(char* username)
	{
		GameUserStruct* p = GetGameUserStruct(username);
		if (nullptr == p)
		{
			return nullptr;
		}
		return &(p->userinfo);
	}

	///获取用户信息
	GameUserStruct* CGameUserInfoManage::GetGameUserStruct(UINT dwUserID, ENUM_usertype usertype)
	{
        auto it = m_userid2userinfo.find(dwUserID);
        if (it != m_userid2userinfo.end())
        {
			if (it->second->usertype == usertype)
            return it->second; 
        }
		return nullptr;
	}

	GameUserStruct* CGameUserInfoManage::GetGameUserStructEx(UINT socketid, ENUM_usertype usertype)
	{
		GameUserStruct* p = GetGameUserStructEx(socketid);
		if (nullptr != p)
		{
			if (p->usertype == usertype)
				return p;

			return nullptr;
		}

		return nullptr;
	}

	///获取用户信息
	GameUserStruct* CGameUserInfoManage::GetGameUserStruct(char* username, ENUM_usertype usertype)
	{
		if (nullptr == username)
		{
			return nullptr;
		}
		auto it = m_username2userid.find(username);
		if (it == m_username2userid.end())
		{
			return nullptr;
		}
		return GetGameUserStruct(it->second, usertype);
	}

	GameUserStruct* CGameUserInfoManage::GetGameUserStruct(UINT dwUserID)
	{
		auto it = m_userid2userinfo.find(dwUserID);
		if (it != m_userid2userinfo.end())
		{
			return it->second;
		}
		return nullptr;
	}

	GameUserStruct* CGameUserInfoManage::GetGameUserStructEx(UINT socketid)
	{
		auto it = m_socketid2userid.find(socketid);
		if (it != m_socketid2userid.end())
		{
			return GetGameUserStruct(it->second);
		}
		return nullptr;
	}

	GameUserStruct* CGameUserInfoManage::GetGameUserStruct(char* username)
	{
		if (nullptr == username)
		{
			return nullptr;
		}
		auto it = m_username2userid.find(username);
		if (it == m_username2userid.end())
		{
			return nullptr;
		}
		return GetGameUserStruct(it->second);
	}

	//激活用户
	CGameUserInfo * CGameUserInfoManage::ActiveUser(UserInfoInRoomStruct * pUserInfo, UINT uSocketID)
	{
		if (nullptr == pUserInfo)
		{
			return nullptr;
		}
		CWJSSocket *pSocket = g_pGameServer->getCWJSSocket(uSocketID);
		if (nullptr == pSocket)
		{
			return nullptr;
		}

		auto it = m_userid2userinfo.find(pUserInfo->basedata.dwUserID);
		if (it == m_userid2userinfo.end())
		{
			auto retPair = m_userid2userinfo.insert(make_pair(pUserInfo->basedata.dwUserID, new GameUserStruct));
			it = retPair.first;

			m_socketid2userid.insert(make_pair(uSocketID, pUserInfo->basedata.dwUserID));
		}

		it->second->userinfo.SetUserData(*pUserInfo, uSocketID);
		it->second->usertype = usertype_online;
		//pSocket->SetUserObj(it->second);  // wushuisheng memory problem
		return &it->second->userinfo;
	}

	//释放用户
	bool CGameUserInfoManage::FreeUser(UINT uSocketId)
	{
		GameUserStruct* p = nullptr;
		auto it = m_socketid2userid.find(uSocketId);
		if (it != m_socketid2userid.end())
		{
			p = GetGameUserStruct(it->second);
			m_socketid2userid.erase(uSocketId);
		}

		if (nullptr != p)
		{
			m_userid2userinfo.erase(p->userinfo.GetUserData()->basedata.dwUserID);
			p->userinfo.CleanData();
			p->usertype = usertype_def;
			WJ_SAFE_DELETE(p);

			return true;
		}

		return false;
	}

	///释放断线用户
	bool CGameUserInfoManage::FreeUserEx(UINT userid)
	{
		auto it = m_userid2userinfo.find(userid);
		if (it != m_userid2userinfo.end())
		{
			m_socketid2userid.erase(it->second->userinfo.GetSocketId());
			WJ_SAFE_DELETE(it->second);
			m_userid2userinfo.erase(it);
			
			return true;
		}
		return false;
	}

	///获取断线人数, 少调用，O(n)
	UINT CGameUserInfoManage::GetOnLineUserCount()
	{
		UINT icount = 0;
		auto iter = m_userid2userinfo.begin(), end = m_userid2userinfo.end();
		for (; iter != end; ++iter)
		{
			if (usertype_online == iter->second->usertype)
			{
				++icount;
			}
		}
		return icount;
	}

	///获取断线人数, 少调用，O(n)
	UINT CGameUserInfoManage::GetNetCutUserCount()
	{
		UINT icount = 0;
		auto iter = m_userid2userinfo.begin(), end = m_userid2userinfo.end();
		for (; iter != end; ++iter)
		{
			if (usertype_netcut == iter->second->usertype)
			{
				++icount;
			}
		}

		return icount;
	}

	///获取全部人数
	UINT CGameUserInfoManage::GetGameRoomCount(UINT uRoomid)
	{
		if (0 == uRoomid)
		{
			return 0;
		}

		UINT icount = 0;
		auto iter = m_userid2userinfo.begin(), end = m_userid2userinfo.end();
		for (; iter != end; ++iter)
		{
			if (usertype_online == iter->second->usertype
				&& uRoomid == iter->second->userinfo.GetUserData()->uRoomid)
			{
				++icount;
			}
		}
		return icount;
	}

	//注册用户断线
	CGameUserInfo * CGameUserInfoManage::RegNetCutUser(UINT socketId)
	{
		//CWJSSocket *pSocket = g_pGameServer->getCWJSSocket(socketId);
		//if (nullptr != pSocket)
		//{
		//	pSocket->SetUserObj(nullptr);
		//}

		auto iter = m_userid2userinfo.begin(), end = m_userid2userinfo.end();
		for (; iter != end; ++iter)
		{
			if (socketId == iter->second->userinfo.GetSocketId())
			{
				iter->second->usertype = usertype_netcut;
				iter->second->userinfo.GetUserData()->basedata.bUserState = US_R_NetCut;
				iter->second->userinfo.SetUserData(*(iter->second->userinfo.GetUserData()), INVALID_SOCKET_ID);
				return &(iter->second->userinfo);
			}
		}

		return nullptr;
	}
}