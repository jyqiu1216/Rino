#ifndef WJ_USERINFOMANAGE_H
#define WJ_USERINFOMANAGE_H

#include <vector>
#include <map>
#include <mutex>
#include "common/WJBaseType.h"
#include "LUserInfo.h"
namespace WJ
{
	//用户队列管理时候用到--------------------------------------------
	enum ENUM_usertypeL
	{
		usertype_def = 0,//默认状态
		usertype_online,//在线
	};

	struct LUserStruct
	{
		ENUM_usertypeL usertype;//用户数据类型
		CLoginUserInfo userinfo;//用户数据信息

		LUserStruct()
		{
			usertype = usertype_def;
		}
	};
	//----------------------------------------------------------------

	///用户管理类
	class CLUserInfoManage
	{
		///变量定义
	private:
		map<long ,LUserStruct*>					m_UserInfo;				///用户信息
		map<std::string, long>                  m_mapUserNameID;        // 用户名-》ID 映射 加快查找速度
		map<long, long>                  m_mapUserIDSocketID;        // 用户名-》ID 映射 加快查找速度
		LUserStruct *m_puserinfo_online_data;//内部申请的内存空间，不直接使用
		long m_nMaxCount;
		///函数定义
	public:
		///构造函数
		CLUserInfoManage(void);
		///析构函数
		~CLUserInfoManage(void);

		///初始化函数
		bool Init_CLUserInfoManage(UINT uMaxOnLineCount);
		///取消初始化
		bool UnInit_CLUserInfoManage();
		//清理所有玩家
		void clear();
		///激活用户
		CLoginUserInfo* ActiveUser(UserInfoInHallStruct * pUserInfo, UINT uSocketIndex);
		///释放用户
		bool FreeUser(UINT uSocketIndex);
		///获取在线用户
		CLoginUserInfo* GetOnLineUserInfo(UINT dwUserID);
		///获取在线用户
		CLoginUserInfo* GetOnLineUserInfo(char username[]);
		///获取在线用户
		CLoginUserInfo* GetOnLineUserInfoEx(UINT uSocketIndex);
	

		///获取在线人数
		UINT GetOnLineUserCount();
		///获取全部人数
		UINT GetLoginCount();
		
		///获取用户信息test
		LUserStruct* GetLUserStructEx_TEST(UINT uSocketIndex);

		map<long, LUserStruct*>* GetLUserStruct_map(){ return &m_UserInfo; }
	private:
		///获取用户信息
		LUserStruct* GetLUserStruct(UINT dwUserID, ENUM_usertypeL usertype);
		///获取用户信息
		LUserStruct* GetLUserStruct(char username[], ENUM_usertypeL usertype);
		///获取用户信息
		LUserStruct* GetLUserStructEx(UINT uSocketIndex, ENUM_usertypeL usertype);
	};
}

#endif //WJ_USERINFOMANAGE_H