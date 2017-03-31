#ifndef WJ_TeamManage_H
#define WJ_TeamManage_H

#include <vector>
#include <mutex>
#include "common/WJBaseType.h"
#include "server/comstruct.h"

namespace WJ
{//团队管理
	class CTeamManage
	{
	public:
		CTeamManage();
		~CTeamManage();

		//添加
		bool add(TeamStruct &data);
		//删除
		void del(UINT uCaptain_userid);
		void del(char* Team_password);
		void delAll();
		//查找
		bool find(UINT userid, TeamStruct &data);
		bool find(char* Team_password, TeamStruct &data);

		//玩家加入团队
		bool userAdd(UINT userid, char* Team_password);
		//玩家离开团队
		bool userDel(UINT userid, char* Team_password);

		//设置团队被分配到房间
		void AllotRoom(char* Team_password, UINT uroomid);
		//设置团队未被分配到房间
		void unAllotRoom(char* Team_password);
		//获得该比赛有多少组团队还没有处理
		UINT getteamcount(UINT ucontestid);
		//获得团队赛有多少组团队还没有处理
		UINT getteamcount_unplaying();
		//获得团队赛有多少组团队已经在比赛了
		UINT getteamcount_playing();

		//获得团队列表数据
		std::vector<TeamStruct> getTeamStruct_list();

		//是否队列里面存在某队伍有该暗号
		bool bExistTeam_password(char* Team_password);

		//队伍人数是否满了
		bool bFull(char* Team_password);

		//获得团队用户信息
		bool GetUsers(char* Team_password, UINT Userid[], UINT &uTemaPlayerNum);

		//队长同意开始匹配
		void setButVS(char* Team_password, bool ballow);
		//查询队长是否同意开始匹配
		bool getButVS(char* Team_password);

		//已匹配的数量
		UINT Count_VS();
		//为某团队生成匹配
		bool add_VS(char* Team_password, TeamStruct_VS &_TeamStruct_VS, UINT uroomid);

		//删除匹配节点
		void del_VS(UINT index);
		//得到匹配节点
		void at_vs(UINT index, TeamStruct_VS &_TeamStruct_VS);
	private:
		//生成随机数
		int MyRand();
		//获得随机数字
		void GetRandText(std::string &str, int strleng);

		//设置被匹配标记
		void setbVS(char* Team_password, bool bVS);
	private:
		std::vector<TeamStruct> m_list;//团队队列
		std::vector<std::string> m_pass;//未使用的团队暗号存储池

		std::vector<TeamStruct_VS> m_vs_list;//匹配队列
	};
}

#endif//WJ_TeamManage_H