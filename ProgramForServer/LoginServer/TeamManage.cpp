#include "TeamManage.h"
#include <windows.h>
namespace WJ
{

	CTeamManage::CTeamManage()
	{

	}
	
	CTeamManage::~CTeamManage()
	{

	}

	//添加
	bool CTeamManage::add(TeamStruct &data)
	{
		TeamStruct data2;
		if (find(data.m_uCaptain_userid, data2))
		{//该玩家已经在团队中
			return false;
		}
		
		//团队暗号是随机生成的，但不能和现有的冲突
		std::string str_passwor;
		while (1)
		{
			if (0 < m_pass.size())
			{//暗号池一旦有，则再次使用
				str_passwor = m_pass.at(0);
				m_pass.erase(m_pass.begin());
			}
			else
			{
				//随机生成团队暗号
				GetRandText(str_passwor, TEAM_PASS_LENGTH - 1);
			}

			if (bExistTeam_password(const_cast<char*>(str_passwor.c_str())))
			{//如果团队管理列表中已经有则丢弃
				continue;
			}

			//上层业务确保数据正确，这里只是生成团队暗号
			sprintf_s(data.m_Team_password, "%s", str_passwor.c_str());
			data.m_Userid[0] = data.m_uCaptain_userid;
			m_list.push_back(data);
			break;
		};

		return true;
	}

	//删除
	void CTeamManage::del(UINT uCaptain_userid)
	{
		auto iter = m_list.begin();
		while (iter != m_list.end())
		{
			if ((*iter).m_uCaptain_userid == uCaptain_userid)
			{				
				if (!bExistTeam_password((*iter).m_Team_password))
				{
					std::string str_passwor;
					str_passwor = (*iter).m_Team_password;
					m_pass.push_back(str_passwor);
				}

				m_list.erase(iter);
				return;
			}
			++iter;
		}
	}

	void CTeamManage::del(char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return;
		}

		auto iter = m_list.begin();
		while (iter != m_list.end())
		{
			if (0 == strcmp((*iter).m_Team_password, Team_password))
			{
				if (!bExistTeam_password((*iter).m_Team_password))
				{
					std::string str_passwor;
					str_passwor = (*iter).m_Team_password;
					m_pass.push_back(str_passwor);
				}

				m_list.erase(iter);
				return;
			}
			++iter;
		}
	}

	void CTeamManage::delAll()
	{
		m_list.clear();
	}

	//查找
	bool CTeamManage::find(UINT userid, TeamStruct &data)
	{
		auto iter = m_list.begin();
		while (iter != m_list.end())
		{
			for (int i = 0; i < (*iter).m_uTemaPlayerNum; ++i)
			{
				if ((*iter).m_Userid[i] == userid)
				{
					std::memcpy(&data, &(*iter), sizeof(TeamStruct));
					return true;
				}
			}

			++iter;
		}

		return false;
	}

	bool CTeamManage::find(char* Team_password, TeamStruct &data)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		auto iter = m_list.begin();
		while (iter != m_list.end())
		{
			if (0==strcmp((*iter).m_Team_password, Team_password))
			{
				std::memcpy(&data, &(*iter), sizeof(TeamStruct));
				return true;
			}
			++iter;
		}

		return false;
	}

	//是否队列里面存在某队伍有该暗号
	bool CTeamManage::bExistTeam_password(char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//如果团队管理列表中已经有则丢弃
				return true;
			}
		}

		return false;
	}

	//队伍人数是否满了
	bool CTeamManage::bFull(char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//找到该队伍
				UINT uTemaPlayerNum = m_list.at(i).m_uTemaPlayerNum < TEAM_USER_NUM ? m_list.at(i).m_uTemaPlayerNum : TEAM_USER_NUM;
				UINT uplayers = 0;
				for (int j = 0; j < uTemaPlayerNum; ++j)
				{//人数统计
					if (0 != m_list.at(i).m_Userid[j])
					{
						++uplayers;
					}
				}
				//人数等于总人数则人满
				return uplayers == uTemaPlayerNum;
			}
		}

		return false;
	}

	//获得团队用户信息
	bool CTeamManage::GetUsers(char* Team_password, UINT Userid[], UINT &uTemaPlayerNum)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//找到该队伍
				uTemaPlayerNum = m_list.at(i).m_uTemaPlayerNum < TEAM_USER_NUM ? m_list.at(i).m_uTemaPlayerNum : TEAM_USER_NUM;

				for (int j = 0; j < uTemaPlayerNum; ++j)
				{
					Userid[j] = m_list.at(i).m_Userid[j];
				}
				return true;
			}
		}

		return false;
	}

	//队长同意开始匹配
	void CTeamManage::setButVS(char* Team_password, bool ballow)
	{
		if (nullptr == Team_password)
		{
			return;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//找到该队伍
				m_list.at(i).bButVS = ballow;
				break;
			}
		}
	}

	//查询队长是否同意开始匹配
	bool CTeamManage::getButVS(char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//找到该队伍
				return m_list.at(i).bButVS;
			}
		}

		return false;
	}

	//玩家加入团队
	bool CTeamManage::userAdd(UINT userid, char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		TeamStruct data2;
		if (find(userid, data2))
		{//该玩家已经创建了团队
			return false;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//找到该队伍
				UINT uTemaPlayerNum = m_list.at(i).m_uTemaPlayerNum < TEAM_USER_NUM ? m_list.at(i).m_uTemaPlayerNum : TEAM_USER_NUM;
				for (int j = 1; j < uTemaPlayerNum; ++j)
				{//从非队长位置开始查
					if (0 == m_list.at(i).m_Userid[j])
					{//在规定人数范围内对空位子进行赋值
						m_list.at(i).m_Userid[j] = userid;
						return true;
					}
				}
			}
		}

		return false;
	}

	//玩家离开团队
	bool CTeamManage::userDel(UINT userid, char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		TeamStruct data2;
		if (find(userid, data2))
		{//该玩家已经创建了团队
			return false;
		}
		
		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//找到该队伍
				UINT uTemaPlayerNum = m_list.at(i).m_uTemaPlayerNum < TEAM_USER_NUM ? m_list.at(i).m_uTemaPlayerNum : TEAM_USER_NUM;
				for (int j = 1; j < uTemaPlayerNum; ++j)
				{//从非队长位置开始查
					if (userid == m_list.at(i).m_Userid[j])
					{//在规定人数范围内对空位子进行赋值
						m_list.at(i).m_Userid[j] = 0;
						return true;
					}
				}
			}
		}

		return false;
	}

	//设置团队被分配到房间
	void CTeamManage::AllotRoom(char* Team_password, UINT uroomid)
	{
		if (nullptr == Team_password)
		{
			return;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//找到该队伍
				m_list.at(i).bAllot = true;
				m_list.at(i).uroomid = uroomid;
				return;
			}
		}
	}

	//设置团队未被分配到房间
	void CTeamManage::unAllotRoom(char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//找到该队伍
				m_list.at(i).bAllot = false;
				m_list.at(i).uroomid = 0;
				return;
			}
		}
	}

	//获得该比赛有多少组团队还没有处理
	UINT CTeamManage::getteamcount(UINT ucontestid)
	{
		UINT ucount = 0;

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (ucontestid == m_list.at(i).m_uContest
				&& !m_list.at(i).bAllot)
			{//找到该队伍
				++ucount;
			}
		}

		return ucount;
	}

	//获得团队赛有多少组团队还没有处理
	UINT CTeamManage::getteamcount_unplaying()
	{
		UINT ucount = 0;

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (!m_list.at(i).bAllot)
			{//找到该队伍
				++ucount;
			}
		}

		return ucount;
	}

	//获得团队赛有多少组团队已经在比赛了
	UINT CTeamManage::getteamcount_playing()
	{
		UINT ucount = 0;

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (m_list.at(i).bAllot)
			{//找到该队伍
				++ucount;
			}
		}

		return ucount;
	}

	//获得团队列表数据
	std::vector<TeamStruct> CTeamManage::getTeamStruct_list()
	{
		return m_list;
	}

	int CTeamManage::MyRand()
	{
		static int innerSeed = GetTickCount();

		return (((innerSeed = innerSeed * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	//获得随机数字
	void CTeamManage::GetRandText(std::string &str, int strleng)
	{
		str.clear();
		do
		{
			int a = MyRand() % 10;
			char s[2] = "0";
			_itoa_s(a, s, 2, 10);
			str += s;
			--strleng;
		} while (strleng > 0);
	}

	//匹配的数量
	UINT CTeamManage::Count_VS()
	{
		return m_vs_list.size();
	}

	//添加匹配节点
	bool CTeamManage::add_VS(char* Team_password, TeamStruct_VS &_TeamStruct_VS, UINT uroomid)
	{
		TeamStruct data;
		//是否能找到
		if (!find(Team_password, data))
		{
			return false;
		}
		//是否人满
		if (!bFull(Team_password))
		{
			return false;
		}

		if (data.bVS)
		{//已经匹配了
			return false;
		}

		if (data.bButVS)
		{//队长未同意
			return false;
		}
		
		for (int i = 0; i < m_list.size();++i)
		{
			if (m_list.at(i).m_uContest == data.m_uContest
				&& m_list.at(i).m_uCaptain_userid != data.m_uCaptain_userid
				&& bFull(m_list.at(i).m_Team_password)
				&& m_list.at(i).bButVS
				&& !m_list.at(i).bVS)
			{//同一比赛中，不是同一队伍,且人数满编，且队长同意匹配，且未匹配过就可以匹配
				
				_TeamStruct_VS._TeamStruct[0] = data;
				_TeamStruct_VS._TeamStruct[1] = m_list.at(i);

				m_list.at(i).bVS = true;
				m_list.at(i).bAllot = true;
				m_list.at(i).uroomid = uroomid;
				setbVS(Team_password, true);
				AllotRoom(Team_password, uroomid);
				return true;
			}
		}
		return true;
	}

	//设置被匹配标记
	void CTeamManage::setbVS(char* Team_password, bool bVS)
	{
		if (nullptr == Team_password)
		{
			return;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//找到该队伍
				m_list.at(i).bVS = bVS;
				return;
			}
		}
	}

	//删除匹配节点
	void CTeamManage::del_VS(UINT index)
	{
		if (index >= m_vs_list.size())
		{
			return;
		}

		m_vs_list.erase(m_vs_list.begin() + index);
	}

	//得到匹配节点
	void CTeamManage::at_vs(UINT index, TeamStruct_VS &_TeamStruct_VS)
	{
		if (index >= m_vs_list.size())
		{
			return;
		}

		_TeamStruct_VS = m_vs_list.at(index);
	}
}