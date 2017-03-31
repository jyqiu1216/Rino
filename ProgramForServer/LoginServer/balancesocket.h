#ifndef INC_BALANCESOCKET_H
#define INC_BALANCESOCKET_H
#include "windows.h"
#include <string>
#include <mutex>
#include <deque>
#include "json/json.h"
struct CSendData
{
	CSendData()
	{
		m_pData = NULL;
		m_nLen = 0;
	}
	~CSendData()
	{
		if (m_pData)
		{
			delete[]m_pData;
		}
	}
	char *m_pData;
	int  m_nLen;
};

class CBalanceSocket
{
public:
	CBalanceSocket(std::string strIP, int nPort);
	~CBalanceSocket();
	static DWORD WINAPI  SendDataFunc(LPVOID arg);
	static DWORD WINAPI  RecvDataFunc(LPVOID arg);
	static DWORD WINAPI  HeartFunc(LPVOID arg);
	bool ConnectBalanceServer();
	void ReadMesg(char *pData, long nLen);
	void ParasJsonData(std::string &strJsonString);
	void SendInit();
	void OnHeartBeat();
private:
	std::string m_strIP;
	int  m_nPort;
	SOCKET  *m_ClientSocket;
	HANDLE m_hSendThread;
	HANDLE m_hRecvThread;
	HANDLE m_hHeartThread;
	bool   m_bExit;
	std::recursive_mutex m_stSockMutex;
	std::recursive_mutex m_SendMutex;
	std::deque<CSendData*> m_deqSendData;
	long m_nRecvLen;
	long m_nBufferLen;
	long m_nMaxLen;
	char* m_RecvBuffer;
	bool m_bLogin;
};
#endif