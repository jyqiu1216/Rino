#ifndef INC_BALANCESELECT_H
#define INC_BALANCESELECT_H
#include <string>
#include <map>
#include<mutex>
struct CLoginServer
{
	CLoginServer()
	{
		m_strIP = "";
		m_nPort = -1;
		m_nConnect = 0;
		m_nID = -1;
	}
	std::string m_strIP;
	int m_nPort;
	long long m_nConnect;
	int  m_nID;
};

enum eSelectType
{
	ROBIN = 0,

};


class CBalanceSelect
{
public:
	CBalanceSelect();
	~CBalanceSelect();
	void SetSelectType(eSelectType selecttype);
	void UpdateLoginServer(CLoginServer &loginserver);
	void AddLoginServer(CLoginServer &loginserver);
	void DelLoginServer(std::string &strLoginIP, int nPort);
	bool SelectLoginServer(std::string &strUserName, CLoginServer &loginserver);

private:
	std::recursive_mutex m_deqLoginServer;//互斥量，方便线程对互斥量上锁
	std::map<std::string, CLoginServer> m_mapIPLoginServer;
	eSelectType  m_eSelectType;
	long long m_nIndex;
};
#endif