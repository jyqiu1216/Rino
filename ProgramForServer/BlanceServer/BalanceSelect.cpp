#include "BalanceSelect.h"
#include <iterator>
CBalanceSelect::CBalanceSelect()
{
	m_eSelectType = ROBIN;
	m_nIndex = 0;
}

CBalanceSelect::~CBalanceSelect()
{
	m_mapIPLoginServer.clear();
}
void CBalanceSelect::SetSelectType(eSelectType selecttype)
{
	m_eSelectType = selecttype;
}

void CBalanceSelect::UpdateLoginServer(CLoginServer &loginserver)
{
	std::lock_guard <std::recursive_mutex> autoLock(m_deqLoginServer);
	std::map<std::string, CLoginServer>::iterator iter = m_mapIPLoginServer.find(loginserver.m_strIP);
	if (iter != m_mapIPLoginServer.end())
	{
		m_mapIPLoginServer[loginserver.m_strIP].m_nConnect = loginserver.m_nConnect;
	}
	else
	{
		m_mapIPLoginServer.insert(std::map<std::string, CLoginServer>::value_type(loginserver.m_strIP, loginserver));
	}
}

void CBalanceSelect::AddLoginServer(CLoginServer &loginserver)
{
	std::lock_guard <std::recursive_mutex> autoLock(m_deqLoginServer);
	std::map<std::string, CLoginServer>::iterator iter = m_mapIPLoginServer.find(loginserver.m_strIP);
	if (iter != m_mapIPLoginServer.end())
	{
		m_mapIPLoginServer[loginserver.m_strIP] = loginserver;
	}
	else
	{
		m_mapIPLoginServer.insert(std::map<std::string, CLoginServer>::value_type(loginserver.m_strIP, loginserver));
	}
}

void CBalanceSelect::DelLoginServer(std::string &strLoginIP, int nPort)
{
	std::lock_guard <std::recursive_mutex> autoLock(m_deqLoginServer);
	std::map<std::string, CLoginServer>::iterator iter = m_mapIPLoginServer.find(strLoginIP);
	if (iter != m_mapIPLoginServer.end())
	{
		m_mapIPLoginServer.erase(iter);
	}
}

bool CBalanceSelect::SelectLoginServer(std::string &strUserName, CLoginServer &loginserver)
{
	bool bRet = false;
	std::lock_guard <std::recursive_mutex> autoLock(m_deqLoginServer);
	int nSize = m_mapIPLoginServer.size();
	if (nSize > 0)
	{
		std::map<std::string, CLoginServer>::iterator iter = m_mapIPLoginServer.begin();
		advance(iter, (m_nIndex % nSize));
		loginserver = iter->second;
		m_nIndex++;
		bRet = true;
	}
	return bRet;
}