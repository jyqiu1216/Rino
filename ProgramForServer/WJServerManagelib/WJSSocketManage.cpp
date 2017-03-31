#include "stdafx.h"
#include <CommCtrl.h>
#include "server/WJSSocketManage.h"
#include "common/WJCommonMarco.h"
#include "common/commonfunction.h"
#include "common/QkLogger.h"

using namespace std;

namespace WJ
{
	void Init_WinSock()
	{
		//用来保存WinSock库的版本号  
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;

		wVersionRequested = MAKEWORD(2, 2);
		err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			return;
		}
		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
			//若不是所请求的版本号2.2,则终止WinSock库的使用  
			WSACleanup();
			return;
		}
	}

	void UnInit_WinSock()
	{
		WSACleanup();
	}

	//构造函数
	CWJSSocketManage::CWJSSocketManage(UINT nMaxSocketCount)
	{
		m_bRun = false;
		m_hCompletionPortRS = nullptr;
		m_uRSThreadCount = 0;
        m_nMaxSocketCount = nMaxSocketCount;
		if (m_nMaxSocketCount == 0)
		{
			m_nMaxSocketCount = 1024;
		}
        m_tmpEventResults.reserve(1024);
		return;
	}

	//析构函数
	CWJSSocketManage::~CWJSSocketManage()
	{
        auto it = m_id2socket.begin(), end = m_id2socket.end();
        for (; it != end; ++it)
        {
            if(it->second)
            {
                delete it->second;
                it->second = NULL;
            }
        }
        m_id2socket.clear();
		return;
	}

	//开始服务
	bool CWJSSocketManage::Start_WJSSocketManage(UINT uSocketThradCount)
	{
		if (m_bRun)
		{
			return false;
		}
		m_bRun = true;
		//获取系统信息
		SYSTEM_INFO SystemInfo;
		::GetSystemInfo(&SystemInfo);
		if (uSocketThradCount == 0 || uSocketThradCount > SystemInfo.dwNumberOfProcessors)
		{
			uSocketThradCount = SystemInfo.dwNumberOfProcessors;
		}
		//建立完成端口
		m_hCompletionPortRS = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, uSocketThradCount);
		if (m_hCompletionPortRS == nullptr)
		{
			return false;
		}
		//建立拉取完成包的线程
		m_uRSThreadCount = 0;
		for (UINT i = 0; i < uSocketThradCount; i++)
		{
			std::thread recvThread(std::bind(&CWJSSocketManage::ThreadPullCompletedStatus, this));
			recvThread.detach();
			m_uRSThreadCount++;
		}
		////建立异步连接线程(作为客户端主动发起连接服务器用)
		std::thread recvThread(std::bind(&CWJSSocketManage::ThreadConnect, this));
		recvThread.detach();
		return true;
	}

	//停止服务
	bool CWJSSocketManage::Stop_WJSSocketManage()
	{
		//关闭 SOCKET
		if (!m_bRun)
		{
			return false;
		}
		m_bRun = false;
		for (size_t i = 0; i < m_ListenList.size(); ++i)
		{
			WJ_SAFE_DELETE(m_ListenList[i]);
		}
		m_ListenList.clear();
		if (m_hCompletionPortRS != nullptr)
		{
			::CloseHandle(m_hCompletionPortRS);
			m_hCompletionPortRS = nullptr;
			m_uRSThreadCount = 0;
		}
		return true;
	}

	void CWJSSocketManage::AsyncListen(const std::string &bindIp, int listenPort, UINT nListenType)
	{
		//建立监听SOCKET
		CWJListenSocket *pListenSocket = new CWJListenSocket;
		
		if (!pListenSocket->Create())
		{
			QK_ErrorLog("listenSocket.Create() error : %d", errno);
			return;
		}
		if (!pListenSocket->Bind(bindIp.c_str(), listenPort))
		{
			QK_ErrorLog("listenSocket.Bind error : %d", errno);
			return ;
		}
		if (!pListenSocket->Listen(nListenType))
		{
			QK_ErrorLog("listenSocket.Listen error : %d", errno);
			return ;
		}
		m_ListenList.push_back(pListenSocket);
		////建立accept新连接线程
		std::thread recvThread(&CWJSSocketManage::ThreadAccept, this, pListenSocket->GetSafeSocket(), pListenSocket->GetListenType());
		recvThread.detach();
	}

	void CWJSSocketManage::AsyncConnect(const char *dstIp, int dstPort, int type, void *pUserData)
	{
		ConnectReq connReq;
		int copyLen = min((int)strlen(dstIp), sizeof(connReq.ip));
		memcpy(connReq.ip, dstIp, copyLen);
		connReq.ip[copyLen] = '\0';
		connReq.port = dstPort;
		connReq.type = type;
		connReq.pUserData = pUserData;
		m_connectQueueLock.Lock();
		m_connectQueue.push_back(connReq);
		m_connectQueueLock.UnLock();
	}

	//发送数据函数
	void CWJSSocketManage::SendData(UINT sockId, const char * pData, UINT uBufLen)
	{
        auto it = m_id2socket.find(sockId);
        if (it != m_id2socket.end() && it->second)
        {
            it->second->SendData(pData, uBufLen);
        }
	}
    
    //发送数据函数
	void CWJSSocketManage::SendData(UINT sockId, UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{
        auto it = m_id2socket.find(sockId);
        if (it != m_id2socket.end() && it->second)
        {
            it->second->SendData(bMainID, bAssistantID, bHandleCode);
        }
	}

	//发送数据函数
	void CWJSSocketManage::SendData(UINT sockId, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{
        auto it = m_id2socket.find(sockId);
        if (it != m_id2socket.end() && it->second)
        {
            it->second->SendData(pData, uBufLen, bMainID, bAssistantID, bHandleCode);
        }
	}

	//发送数据函数
	void CWJSSocketManage::SendData(CWJSSocket * pSocket, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{
		if ((pSocket != nullptr) && (m_bRun == true))
		{
			pSocket->SendData(pData, uBufLen, bMainID, bAssistantID, bHandleCode);
		}
	}

	//关闭连接
	void CWJSSocketManage::CloseSocket(UINT sockId)
	{
		auto it = m_id2socket.find(sockId);
		if (it != m_id2socket.end())
		{
            if (it->second)
			{
                it->second->CloseSocket();
            }
            m_id2socket.erase(it);
		}
	}


	//获得节点
	CWJSSocket* CWJSSocketManage::getCWJSSocket(UINT sockId)
	{
		auto it = m_id2socket.find(sockId);
		if (it != m_id2socket.end())
		{
			return it->second;
		}
		return NULL;
	}

    int CWJSSocketManage::ProcessNetEvents()
    {
        m_tmpEventResults.clear();
 
        m_eventQueueLock.Lock();
        while (!m_eventQueue.empty())
        {
            m_tmpEventResults.push_back(m_eventQueue.front());
            m_eventQueue.pop_front();
        }
        m_eventQueueLock.UnLock();

        int count = (int)m_tmpEventResults.size();
        EventItem *pEvItem = NULL;
        for (int i = 0; i < count; ++i)
        {
            pEvItem = &m_tmpEventResults[i];
            if (pEvItem->m_pSocket)
            {
                switch (pEvItem->m_eventType)
                {
                    case SOCKET_NEW:
                    {
						if (m_id2socket.size() < m_nMaxSocketCount)
						{
							auto retPair = m_id2socket.insert(std::make_pair(pEvItem->m_pSocket->GetSockID(), pEvItem->m_pSocket));
                            if (retPair.second)
                            {
								pEvItem->m_pSocket->SetKeep();
								OnNewConnection(pEvItem->m_pSocket, pEvItem->pUserData);
							    pEvItem->m_pSocket->RecvData();
                            }
                            else 
                            {
                                pEvItem->m_pSocket->DecRef(); // 释放并关闭连接
                            }
						}
						else
						{
							delete pEvItem->m_pSocket; 
							pEvItem->m_pSocket = NULL;
						}
                        break;
                    }
                    case SOCKET_REV:
                    {
						pEvItem->m_pSocket->SetKeep();
                        bool ret = pEvItem->m_pSocket->OnRecvCompleted();
						if (!ret)
						{
                            UINT sockid = pEvItem->m_pSocket->GetSockID();
							OnEndConnection(pEvItem->m_pSocket);
                            m_id2socket.erase(sockid);
                            pEvItem->m_pSocket->DecRef();
						}
                        pEvItem->m_pSocket->DecRef();
                        break;
                    }
                    case SOCKET_SND:
                    {
                        if (pEvItem->m_sentBytes > 0)
                        {
                            pEvItem->m_pSocket->OnSendCompleted(pEvItem->m_sentBytes);
                        }
                        pEvItem->m_pSocket->DecRef();
                        break;
                    }
                    default:
                    {
                        QK_ErrorLog("unknown pEvItem->m_eventType : %d", pEvItem->m_eventType);
                    }
                }
            }
			else // 主动连接失败
			{
				OnConnectFailed(pEvItem->m_eventType, pEvItem->pUserData);
			}
        }
        return count;
    }

	//SOCKET 连接应答线程
	int CWJSSocketManage::ThreadAccept(SOCKET listenSock, UINT nListenType)
	{
		//数据定义
		sockaddr_in			socketAddr;
		int					iAddrBuferLen = sizeof(socketAddr);
		SOCKET				hSocket = INVALID_SOCKET;
		CWJSSocket			* pNewTCPSocket = nullptr;

		while (m_bRun)
		{
			//接收连接
			hSocket = ::WSAAccept(listenSock, (sockaddr *)&socketAddr, &iAddrBuferLen, nullptr, 0);
			int iErrorCode = WSAGetLastError();
			if (hSocket == INVALID_SOCKET )
			{
				QK_ErrorLog("WSAAccept error iErrorCode=%d", iErrorCode);
				return 0;
			}
            pNewTCPSocket = new CWJSSocket;
            assert(pNewTCPSocket);
            pNewTCPSocket->IncRef();
            pNewTCPSocket->Attach(hSocket, socketAddr);
            pNewTCPSocket->SetSocketManagePtr(this);
			pNewTCPSocket->SetType(nListenType);
			if (NULL == ::CreateIoCompletionPort((HANDLE)hSocket, m_hCompletionPortRS, (ULONG_PTR)pNewTCPSocket, 0))
            {
                iErrorCode = WSAGetLastError();
                ::closesocket(hSocket);
                delete pNewTCPSocket;
				QK_ErrorLog("CreateIoCompletionPort faield, iErrorCode=%d", iErrorCode);
                continue;
            }
            m_eventQueueLock.Lock();
            m_eventQueue.push_back(EventItem(pNewTCPSocket, SOCKET_NEW, 0));
            m_eventQueueLock.UnLock();
		}
		return 0;
    }
    
	int CWJSSocketManage::ThreadPullCompletedStatus()
	{
		//重叠数据
		DWORD							dwThancferred = 0;							    //接收数量
		CWJSSocket					  * pTCPSocket = nullptr;							//完成键
		CWJSSocket::OverLappedStruct  * pOverData = nullptr;							//重叠数据
		while (m_bRun)
		{
			//等待完成端口
			dwThancferred = 0;
			pTCPSocket = nullptr;
			pOverData = nullptr;
			::GetQueuedCompletionStatus(m_hCompletionPortRS, &dwThancferred, (PULONG_PTR)&pTCPSocket, (LPOVERLAPPED *)&pOverData, INFINITE);

			if ((pTCPSocket == nullptr) || (pOverData == nullptr) || (0xFFFFFFFF == dwThancferred))
			{
				return 0;
			}
			
			//处理完成端口
            /*
			if ((dwThancferred == 0) && (pOverData->uOperationType == SOCKET_SND))
			{
				continue;
			}
            */
			
			switch (pOverData->uOperationType)
			{
				case SOCKET_REV:	//SOCKET 数据读取
				{
                    m_eventQueueLock.Lock();
                    m_eventQueue.push_back(EventItem(pTCPSocket, SOCKET_REV, 0));
                    m_eventQueueLock.UnLock();
					break;
				}
				case SOCKET_SND:	//SOCKET 数据发送
				{
                    m_eventQueueLock.Lock();
                    m_eventQueue.push_back(EventItem(pTCPSocket, SOCKET_SND, dwThancferred));
                    m_eventQueueLock.UnLock();
					break;
				}
			}
		}
		return 0;
	}

	int CWJSSocketManage::ThreadConnect()
	{
		deque<ConnectReq> connectRequests;
		while (m_bRun)
		{
			connectRequests.clear();
			m_connectQueueLock.Lock();
			while (!m_connectQueue.empty())
			{
				connectRequests.push_back(m_connectQueue.front());
				m_connectQueue.pop_front();
			}
			m_connectQueueLock.UnLock();

			if (connectRequests.empty())
			{
				Sleep(20);
			}
			else
			{
				ConnectReq req;
				while (!connectRequests.empty())
				{
					req = connectRequests.front();
					connectRequests.pop_front();
					SOCKET hSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
					if (hSocket == INVALID_SOCKET)
					{
						QK_ErrorLog("CWJSSocketManage::ThreadConnect : WSASocket error coed: %d", WSAGetLastError());
						continue;
					}
					sockaddr_in serverAddr;
					serverAddr.sin_family = AF_INET;
					serverAddr.sin_port = htons(req.port);
					serverAddr.sin_addr.S_un.S_addr = inet_addr(req.ip);
                    int nTimeOut = 10000;
                    int nOldRecvTimeout = 0, nOldSendTimeout = 0;
                    int optLen = 0;
                    getsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO,(char*)nOldRecvTimeout, &optLen);
                    getsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO,(char*)nOldSendTimeout, &optLen);
                    setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO,(char*)&nTimeOut, sizeof(nTimeOut));
                    setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO,(char*)&nTimeOut, sizeof(nTimeOut));
					if ((::connect(hSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR))
					{
						QK_ErrorLog("CWJSSocketManage::ThreadConnect : connect %s:%d, error coed: %d", req.ip, req.port, WSAGetLastError());
						closesocket(hSocket);
						m_eventQueueLock.Lock();
						m_eventQueue.push_back(EventItem(NULL, req.type, 0, req.pUserData));
						m_eventQueueLock.UnLock();
						continue;
					}
                    setsockopt(hSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&nOldRecvTimeout,sizeof(nOldRecvTimeout));
                    setsockopt(hSocket,SOL_SOCKET,SO_SNDTIMEO,(char*)&nOldSendTimeout,sizeof(nOldSendTimeout));
					CWJSSocket * pNewTCPSocket = new CWJSSocket;
					assert(pNewTCPSocket);
                    pNewTCPSocket->IncRef();
					pNewTCPSocket->Attach(hSocket, serverAddr);
					pNewTCPSocket->SetSocketManagePtr(this);
                    pNewTCPSocket->SetType(req.type);
					if (NULL == ::CreateIoCompletionPort((HANDLE)hSocket, m_hCompletionPortRS, (ULONG_PTR)pNewTCPSocket, 0))
					{
						int iErrorCode = WSAGetLastError();
						::closesocket(hSocket);
						delete pNewTCPSocket;
						QK_ErrorLog("CWJSSocketManage::ThreadConnect ： CreateIoCompletionPort faield, iErrorCode=%d", iErrorCode);
						continue;
					}
					m_eventQueueLock.Lock();
					m_eventQueue.push_back(EventItem(pNewTCPSocket, SOCKET_NEW, 0, req.pUserData));
					m_eventQueueLock.UnLock();
				}
			}
		}
		return 0;
	}


}