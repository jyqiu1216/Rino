#include "RTeamManage.h"
#include <windows.h>
namespace WJ
{

	CRTeamManage::CRTeamManage()
	{

	}
	
	CRTeamManage::~CRTeamManage()
	{

	}

	//添加
	bool CRTeamManage::add(TeamStruct &data)
	{
		TeamStruct data2;
		if (find(data.m_uCaptain_userid, data2))
		{//该玩家已经创建了团队
			return false;
		}

		m_list.push_back(data);

		return true;
	}

	//删除
	void CRTeamManage::del(UINT uCaptain_userid)
	{
		auto iter = m_list.begin();
		while (iter != m_list.end())
		{
			if ((*iter).m_uCaptain_userid == uCaptain_userid)
			{
				m_list.erase(iter);
				return;
			}
			++iter;
		}
	}

	void CRTeamManage::delAll()
	{
		m_list.clear();
	}

	//查找
	bool CRTeamManage::find(UINT uCaptain_userid, TeamStruct &data)
	{
		auto iter = m_list.begin();
		while (iter != m_list.end())
		{
			if ((*iter).m_uCaptain_userid == uCaptain_userid)
			{
				std::memcpy(&data, &(*iter), sizeof(TeamStruct));
				return true;
			}
			++iter;
		}

		return false;
	}

	bool CRTeamManage::find(char* Team_password, TeamStruct &data)
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

	//队伍人数是否满了
	bool CRTeamManage::bFull(char* Team_password)
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
	bool CRTeamManage::GetUsers(char* Team_password, UINT Userid[], UINT &uTemaPlayerNum)
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
}