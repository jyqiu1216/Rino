#pragma once

#include <vector>
#include <mutex>
#include <unordered_map>
#include "common/WJBaseType.h"
#include "UserInfo.h"
namespace WJ
{
	//用户队列管理时候用到--------------------------------------------
	enum ENUM_usertype
	{
		usertype_def = 0,//默认状态
		usertype_online,//在线
		usertype_netcut,//断线
	};

	struct GameUserStruct
	{
		ENUM_usertype usertype;//用户数据类型
		CGameUserInfo userinfo;//用户数据信息

		GameUserStruct()
		{
			usertype = usertype_def;
		}
		~GameUserStruct()
		{
			int x;
			x = 1;
		}
	};
	//----------------------------------------------------------------

	///用户管理类
	class CGameUserInfoManage
	{
		///变量定义
	private:
		std::unordered_map<UINT, GameUserStruct*>	m_userid2userinfo;	//用户信息
		std::unordered_map<std::string, UINT>       m_username2userid;  //用户名到用户id映射,目前没用,以后最好也不要用,gameserver就按userid来
		std::unordered_map<UINT, UINT>	m_socketid2userid;	//socketid到用户id映射
	public:
		///构造函数
		CGameUserInfoManage(void);
		///析构函数
		~CGameUserInfoManage(void);

		//新建用户数据
		CGameUserInfo* ActiveUser(UserInfoInRoomStruct * pUserInfo, UINT uSocketIndex);

		///通过套接字id释放用户（不适用于断线用户）
		bool FreeUser(UINT uSocketIndex);
		///释放用户（在线非在线都适用）
		bool FreeUserEx(UINT userid);
		///注册用户断线
		CGameUserInfo* RegNetCutUser(UINT socketId);
		///获取在线用户
		CGameUserInfo* GetOnLineUserInfo(UINT dwUserID);
		///获取在线用户
		CGameUserInfo* GetOnLineUserInfo(char* username);
		///获取在线用户
		CGameUserInfo* GetOnLineUserInfoEx(UINT socketId);


		///查找用户
		CGameUserInfo* GetNetCutUser(UINT dwUserID);
		///获取在线用户
		CGameUserInfo* GetNetCutUser(char* username);

		CGameUserInfo* GetUserInfo(UINT dwUserID);
		CGameUserInfo* GetUserInfo(char* username);
		CGameUserInfo* GetUserInfoEx(UINT socketid);

		std::unordered_map<UINT, GameUserStruct*>* GetGameUserStruct_map(){ return &m_userid2userinfo; }
		///获取在线人数
		UINT GetOnLineUserCount();
		///获取断线人数
		UINT GetNetCutUserCount();
		///获取全部人数
		UINT GetGameRoomCount(UINT uRoomid);
	private:
		///获取用户信息
		GameUserStruct* GetGameUserStruct(UINT dwUserID, ENUM_usertype usertype);
		GameUserStruct* GetGameUserStructEx(UINT socketid, ENUM_usertype usertype);
		GameUserStruct* GetGameUserStruct(char* username, ENUM_usertype usertype);
		GameUserStruct* GetGameUserStruct(UINT dwUserID);
		GameUserStruct* GetGameUserStruct(char* username);
		GameUserStruct* GetGameUserStructEx(UINT socketid);
	};
}
