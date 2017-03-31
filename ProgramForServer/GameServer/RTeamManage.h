#ifndef WJ_RTeamManage_H
#define WJ_RTeamManage_H

#include "common/WJBaseType.h"
#include "server/comstruct.h"
#include <vector>

/*
房间内的团队是做整体处理的，不允许团队内部单独离开或者开始
*/

namespace WJ
{//团队管理
	class CRTeamManage
	{
	public:
		CRTeamManage();
		~CRTeamManage();

		//添加
		bool add(TeamStruct &data);
		//删除
		void del(UINT uCaptain_userid);
		void del(char* Team_password);
		void delAll();
		//查找
		bool find(UINT uCaptain_userid, TeamStruct &data);
		bool find(char* Team_password, TeamStruct &data);

		//队伍人数是否满了
		bool bFull(char* Team_password);

		//获得团队用户信息
		bool GetUsers(char* Team_password, UINT Userid[], UINT &uTemaPlayerNum);
	private:
		std::vector<TeamStruct> m_list;//团队队列
	};
}

#endif//WJ_RTeamManage_H