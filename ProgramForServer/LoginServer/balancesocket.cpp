#include "balancesocket.h"

#define ONLINE 1
#define OFFLINE 2
#define UPDATE 3
#define KEEPLIVE 4

CBalanceSocket::CBalanceSocket(std::string strIP, int nPort)
{
	m_hSendThread = NULL;
	m_hRecvThread = NULL;
	m_hHeartThread = NULL;
	m_strIP = strIP;
	m_nPort = nPort;
	m_bLogin = false;
	m_bExit = false;
	m_nBufferLen = 0;
	m_nMaxLen = 1024;
	m_nRecvLen = -1;
	m_RecvBuffer = new char[m_nMaxLen];
	m_ClientSocket = NULL;
}

void CBalanceSocket::OnHeartBeat()
{
	
	if (m_bLogin)
	{
		Json::Value root;
		root["cmd"] = KEEPLIVE;
		
		std::string strString = root.toStyledString();
		int nLogLen = strString.length();
		char *pSendData = new char[8 + nLogLen];
		*(long long *)pSendData = nLogLen;
		memcpy(pSendData + 8, strString.c_str(), nLogLen);
		CSendData *pSendDataInfo = new CSendData();
		pSendDataInfo->m_pData = pSendData;
		pSendDataInfo->m_nLen = 8 + nLogLen;
		std::lock_guard <std::recursive_mutex> autoLock(m_SendMutex);
		m_deqSendData.push_back(pSendDataInfo);
	}
	else
	{
		/*QMutexLocker	stLock(&m_SendMutex);
		int nSize = m_deqSendData.size();
		for (int i = 0; i < nSize; i++)
		{
			if (m_deqSendData[i])
			{
				delete m_deqSendData[i];
				m_deqSendData[i] = NULL;
			}
		}
		m_deqSendData.clear();*/
	}
}

void CBalanceSocket::SendInit()
{

	Json::Value root;
	// 根据实际情况填入相应信息，ip  本机的外网ip地址， port 本机登陆服务端口  id 所在的区id
	root["cmd"] = ONLINE;
	root["ip"] = "127.0.0.1";
	root["port"] = 12000;
	root["id"] = 1;
	std::string strString = root.toStyledString();

	int nLogLen = strString.length();
	char *pSendData = new char[8 + nLogLen];
	*(long long *)pSendData = nLogLen;
	memcpy(pSendData + 8, strString.c_str(), nLogLen);
	CSendData *pSendDataInfo = new CSendData();
	pSendDataInfo->m_pData = pSendData;
	pSendDataInfo->m_nLen = 8 + nLogLen;
	{
		std::lock_guard <std::recursive_mutex> autoLock(m_SendMutex);
		m_deqSendData.push_back(pSendDataInfo);
	}
}

bool CBalanceSocket::ConnectBalanceServer()
{
	std::lock_guard <std::recursive_mutex> autoLock(m_stSockMutex);
	
	if (m_ClientSocket)
	{
		::closesocket(*m_ClientSocket);
		delete m_ClientSocket;
		m_ClientSocket = NULL;
	}
	if (NULL == m_ClientSocket)
	{
		m_ClientSocket = new SOCKET();
		*m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (*m_ClientSocket == INVALID_SOCKET)
		{
			return false;
		}
		struct sockaddr_in ServerAddr;
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_addr.s_addr = inet_addr(m_strIP.c_str());
		ServerAddr.sin_port = htons(m_nPort);
		memset(ServerAddr.sin_zero, 0x00, 8);

		int Ret = ::connect(*m_ClientSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
		if (Ret == SOCKET_ERROR)
		{
			return false;
		}
		else
		{
		}
		if (NULL == m_hSendThread)
		{
			m_hSendThread = CreateThread(NULL, 0, SendDataFunc, this, 0, NULL);
		}
		if (NULL == m_hRecvThread)
		{
			m_hRecvThread = CreateThread(NULL, 0, RecvDataFunc, this, 0, NULL);
		}
		if (NULL == m_hHeartThread)
		{
			m_hHeartThread = CreateThread(NULL, 0, HeartFunc, this, 0, NULL);
		}
		m_bLogin = true;
		SendInit();
	}
	return true;
}
DWORD WINAPI CBalanceSocket::HeartFunc(LPVOID arg)
{
	CBalanceSocket *pThis = (CBalanceSocket *)arg;
	if (!pThis)
	{
		return false;
	}
	while (pThis->m_bExit == false)
	{
		Sleep(1000);
		pThis->OnHeartBeat();
	}
	return 0;
}

CBalanceSocket::~CBalanceSocket()
{
	if (m_RecvBuffer)
	{
		delete[]m_RecvBuffer;
		m_RecvBuffer = NULL;
	}
}

DWORD WINAPI CBalanceSocket::SendDataFunc(LPVOID arg)
{
	CBalanceSocket *pThis = (CBalanceSocket *)arg;
	if (!pThis)
	{
		return false;
	}
	while (pThis->m_bExit == false)
	{
		Sleep(2);
		CSendData *pData = NULL;

		{
			std::lock_guard <std::recursive_mutex> autoLock(pThis->m_SendMutex);
			if (pThis->m_deqSendData.size() > 0)
			{
				pData = pThis->m_deqSendData.front();
				pThis->m_deqSendData.pop_front();
			}
		}

		{
			//QMutexLocker stLock(&pThis->m_stSockMutex);
			if (pThis->m_ClientSocket && pData && pData->m_pData)
			{
				int nRet = ::send(*pThis->m_ClientSocket, pData->m_pData, pData->m_nLen, 0);

				if (nRet < 0)
				{
					//QK_WarnLog("DB connection send Msg Error");
				}
				else
				{
					//	QK_WarnLog("DB connection send Msg ok , len : %d", nRet);
				}

				delete pData;
			}
		}
	}

	return true;
}

DWORD WINAPI CBalanceSocket::RecvDataFunc(LPVOID arg)
{
	CBalanceSocket *pThis = (CBalanceSocket *)arg;
	if (!pThis)
	{
		return false;
	}
	char RecvBuffer[10240];
	//	FILE *fp = fopen("E://rev.data", "ab+");
	while (pThis->m_bExit == false)
	{
		Sleep(2);
		int nRet = 1;
		{
			//QMutexLocker stLock(&pThis->m_stSockMutex);
			if (pThis->m_ClientSocket)
			{
				memset(RecvBuffer, 0, 10240);
				nRet = ::recv(*pThis->m_ClientSocket, RecvBuffer, 10240, 0);
			}
		}

		if (nRet == 0 || nRet == SOCKET_ERROR)
		{
			if (nRet == 0)
			{
				if (pThis->m_bExit)
				{
					break;
				}
				pThis->m_bLogin = false;
				{
					std::lock_guard <std::recursive_mutex> autoLock(pThis->m_SendMutex);
					int nSize = pThis->m_deqSendData.size();
					for (int i = 0; i < nSize; i++)
					{
						if (pThis->m_deqSendData[i])
						{
							delete  pThis->m_deqSendData[i];
							pThis->m_deqSendData[i] = NULL;
						}
					}
					pThis->m_deqSendData.clear();
				}
			//	QK_WarnLog("服务器断开!");
				Sleep(2000);
				bool bRet = false;
				do
				{
					Sleep(1000);
					bRet = pThis->ConnectBalanceServer();

				} while (bRet == false);
			}
		}
		if (nRet < 0)
		{
			if (pThis->m_bExit)
			{
				break;
			}
			pThis->m_bLogin = false;
			{
				std::lock_guard <std::recursive_mutex> autoLock(pThis->m_SendMutex);
				int nSize = pThis->m_deqSendData.size();
				for (int i = 0; i < nSize; i++)
				{
					if (pThis->m_deqSendData[i])
					{
						delete  pThis->m_deqSendData[i];
						pThis->m_deqSendData[i] = NULL;
					}
				}
				pThis->m_deqSendData.clear();
			}
			Sleep(2000);
			bool bRet = false;
			do
			{
				Sleep(1000);
				bRet = pThis->ConnectBalanceServer();

			} while (bRet == false);

		}
		else
		{
			/*if (fp)
			{
			fwrite(RecvBuffer, nRet, 1, fp);
			fflush(fp);
			}*/
			pThis->ReadMesg(RecvBuffer, nRet);
		}
	}

	return true;
}

void CBalanceSocket::ReadMesg(char *pData, long nLen)
{
	if (pData == NULL || nLen <= 0)
	{
		return;
	}
	//QK_WarnLog("get data len is:%d", jsonarray.size());
	//QK_WarnLog("m_RecvBuffer before len is:%d", m_RecvBuffer.size());
	if (pData)
	{
		if (m_nBufferLen + nLen <= m_nMaxLen)
		{
			memcpy(m_RecvBuffer + m_nBufferLen, pData, nLen);
			m_nBufferLen += nLen;
		}
		else
		{
			char *pOrgData = m_RecvBuffer;
			char *pNewData = new char[m_nMaxLen + m_nBufferLen + nLen];
			memcpy(pNewData, pOrgData, m_nBufferLen);
			delete[]pOrgData;
			m_RecvBuffer = pNewData;
			m_nMaxLen = m_nMaxLen + m_nBufferLen + nLen;
			memcpy(m_RecvBuffer + m_nBufferLen, pData, nLen);
			m_nBufferLen += nLen;
		}
	}
	//QK_WarnLog("m_RecvBuffer after len is:%d", m_RecvBuffer.size());
	//	QK_WarnLog("The content len is:%ld", m_nRecvLen);
	while (1)
	{
		if (m_nBufferLen <= 0)
		{
			break;
		}
		if (m_nBufferLen >= 8 && m_nRecvLen <= 0)
		{
			m_nRecvLen = *((long long *)m_RecvBuffer);
			memmove(m_RecvBuffer, m_RecvBuffer + 8, m_nBufferLen - 8);
			m_nBufferLen = m_nBufferLen - 8;
		}
		if (m_nBufferLen >= m_nRecvLen && m_nRecvLen > 0)
		{
			char *pTemp = new char[m_nRecvLen + 1];
			memset(pTemp, 0, m_nRecvLen + 1);
			memcpy(pTemp, m_RecvBuffer, m_nRecvLen);
			memmove(m_RecvBuffer, m_RecvBuffer + m_nRecvLen, m_nBufferLen - m_nRecvLen);
			m_nBufferLen = m_nBufferLen - m_nRecvLen;
			std::string strJsonData = pTemp;
			ParasJsonData(strJsonData);
			m_nRecvLen = -1;
			delete[]pTemp;
		}
		else
		{
			break;
		}
	}
	//QK_WarnLog("The leave");
}
void CBalanceSocket::ParasJsonData(std::string &strJsonString)
{
	
	
		if (strJsonString.length() > 0)
		{
			Json::Reader reader;
			Json::Value root;
			if (reader.parse(strJsonString, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
			{
				int nCmd = root["cmd"].asInt();
				if (nCmd == KEEPLIVE)
				{
					// 这里处理均衡服务器的keeplive的返回信息 目前 均衡服务器没有发送信息给登陆服务器 
					/*Json::Value root;
					root["cmd"] = ONLINE;
					std::string strString = root.toStyledString();
					int nLogLen = strString.length();
					char *pSendData = new char[8 + nLogLen];
					*(long long *)pSendData = nLogLen;
					memcpy(pSendData + 8, strString.c_str(), nLogLen);
					CSendData *pSendDataInfo = new CSendData();
					pSendDataInfo->m_pData = pSendData;
					pSendDataInfo->m_nLen = 8 + nLogLen;
					{
						std::lock_guard <std::recursive_mutex> autoLock(m_SendMutex);
						m_deqSendData.push_back(pSendDataInfo);
					}*/
				}
			}
		}
}


