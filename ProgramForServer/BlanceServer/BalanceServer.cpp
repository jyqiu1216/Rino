#include "BalanceServer.h"
#include"QkLogger.h"
#include "windows.h"
#define LOGIN  0
#define ONLINE 1
#define OFFLINE 2
#define UPDATE 3
#define KEEPLIVE 4



CLoginServer GetLoginServer(std::string &strName)
{
	CLoginServer logininfo;
	logininfo.m_nPort = 1230;
	logininfo.m_strIP = "127.0.0.1";
	return logininfo;
}
std::recursive_mutex CBalanceServer::m_MutexMapClient;//互斥量，方便线程对互斥量上锁
std::recursive_mutex CBalanceServer::m_MutexMapLoginServer;//互斥量，方便线程对互斥量上锁
std::map<evutil_socket_t, ClientData*>   CBalanceServer::m_MapClient;
std::map<evutil_socket_t, ClientData*>   CBalanceServer::m_MapLoginServer;
long long CBalanceServer::m_nConnect = 0;
std::recursive_mutex CBalanceServer::m_MutexMessage;//互斥量，方便线程对互斥量上锁
std::deque<CMessage> CBalanceServer::m_deqMessage;

CBalanceSelect CBalanceServer::m_BalanceSelect;

CBalanceServer::CBalanceServer()
{
	m_nBalancePort = 3528;
	m_nLoginPort = 3258;
	m_bExit = false;
}

CBalanceServer::~CBalanceServer()
{
	
}

void CBalanceServer::StartService()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
	wVersionRequested = MAKEWORD(2, 2);
	//这里必须初始化网络，不然会创建Socket失败
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) 
	{
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		printf("WSAStartup failed with error: %d\n", err);
		return ;
	}
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_nBalancePort);

	struct sockaddr_in sinlogin;
	memset(&sinlogin, 0, sizeof(struct sockaddr_in));
	sinlogin.sin_family = AF_INET;
	sinlogin.sin_port = htons(m_nLoginPort);

	event_base *base = event_base_new();
	m_hCheckKeepLive = CreateThread(NULL, 0, CheckKeepLiveFunc, this, 0, NULL);
	// 绑定并监听IPV4端口
	evconnlistener *listener = evconnlistener_new_bind(base, listener_cb, base,LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,200, (struct sockaddr*)&sin,sizeof(sin));
	evconnlistener *listenerlogin = evconnlistener_new_bind(base, loginlistener_cb, base, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 200, (struct sockaddr*)&sinlogin, sizeof(sinlogin));
	QK_WarnLog("The login port is:%d, the balance port is:%d", m_nLoginPort, m_nBalancePort);
	//事件分发处理
	event_base_dispatch(base);

	evconnlistener_free(listener);
	evconnlistener_free(listenerlogin);
	event_base_free(base);
	WSACleanup();
}

DWORD WINAPI CBalanceServer::CheckKeepLiveFunc(LPVOID arg)
{
	CBalanceServer *pThis = (CBalanceServer *)arg;
	if (!pThis)
	{
		return false;
	}
	while (pThis->m_bExit == false)
	{
		Sleep(1000);
		{
			long long nTime = GetTickCount64();
			m_MutexMapLoginServer.lock();
			for (auto iter = m_MapLoginServer.begin(); iter != m_MapLoginServer.end();)
			{
				if (nTime - (iter->second)->m_nLastKeepAlive >= 10000)
				{
					//满足删除条件，删除当前结点，并指向下面一个结点
					closesocket(iter->first);
					m_BalanceSelect.DelLoginServer(iter->second->m_strIP, 0);
					m_MapLoginServer.erase(iter++);
					
				}
				else
				{
					//条件不满足，指向下面一个结点
					iter++;
				}
			}
			m_MutexMapLoginServer.unlock();
		}
	
	}
}

void CBalanceServer::StopService()
{
	
}

DWORD WINAPI CBalanceServer::ClientProcessFunc(LPVOID arg)
{
	CBalanceServer *pThis = (CBalanceServer *)arg;
	if (!pThis)
	{
		return false;
	}
	while (pThis->m_bExit == false)
	{
			m_MutexMapClient.lock();
			if (m_deqMessage.size() > 0)
			{
				CMessage msg = m_deqMessage.front();
				m_deqMessage.pop_front();
				m_MutexMapClient.unlock();
				pThis->ParasJsonData(msg);
			}
			else
			{
				m_MutexMapClient.unlock();
				Sleep(1);
			}
			
	

	}
}
void CBalanceServer::ParasJsonData(CMessage &msg)
{
	std::string strJsonString = msg.m_strData;
	do
	{
		if (strJsonString.length() > 0)
		{
			Json::Reader reader;
			Json::Value root;
			if (reader.parse(strJsonString, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
			{
				int nCmd = root["cmd"].asInt();
				if (nCmd == LOGIN)
				{

					std::string strName = root["name"].asString();
					std::string strPwd = root["pwd"].asString();
					CLoginServer info;
					bool bRet = m_BalanceSelect.SelectLoginServer(strName, info);
					Json::Value rootrsp;
					if (bRet == false)
					{
					//	QK_WarnLog("There is no loginserver.");
						rootrsp["msg"] = "error";
					}
					else
					{
						rootrsp["msg"] = "ok";
						rootrsp["ip"] = info.m_strIP;
						rootrsp["port"] = info.m_nPort;
						rootrsp["id"] = info.m_nID;
					}
					int nHeadLen = sizeof(NetMessageHead);
					std::string strString = rootrsp.toStyledString();

					int nLogLen = strString.length();
					char *pSendData = new char[nHeadLen + nLogLen];
					NetMessageHead *pHead = (NetMessageHead *)pSendData;
					pHead->uMessageSize = nHeadLen + nLogLen;
					pHead->bMainID = 1;
					memcpy(pSendData + nHeadLen, strString.c_str(), nLogLen);
					send(msg.m_nFd, pSendData, nLogLen + nHeadLen, 0);
					delete[]pSendData;
				}
			}
		}
	} while (0);
}

void CBalanceServer::ParasLoginJsonData(std::string &strJsonString, bufferevent *bev)
{
	do
	{
		if (strJsonString.length() > 0)
		{
			Json::Reader reader;
			Json::Value root;
			if (reader.parse(strJsonString, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
			{
				int nCmd = root["cmd"].asInt();
				if (nCmd == ONLINE)
				{
					printf("onlien \n");
					std::string strIP = root["ip"].asString();
					int nPort = root["port"].asInt();
					int nID = root["id"].asInt();
					CLoginServer info;
					info.m_nPort = nPort;
					info.m_strIP = strIP;
					info.m_nID = nID;
					m_BalanceSelect.AddLoginServer(info);
					evutil_socket_t fd = bufferevent_getfd(bev);
					{
						CBalanceServer::m_MutexMapLoginServer.lock();
						evutil_socket_t fd = bufferevent_getfd(bev);
						auto iter = m_MapLoginServer.find(fd);
						if (iter != m_MapLoginServer.end())
						{
							iter->second->m_nLastKeepAlive = GetTickCount64();
						}
						CBalanceServer::m_MutexMapLoginServer.unlock();
					}
				}
				else if (nCmd == OFFLINE)
				{
					std::string strIP = root["ip"].asString();
					int nPort = root["port"].asInt();
					m_BalanceSelect.DelLoginServer(strIP, nPort);
				}
				else if (nCmd == UPDATE)
				{

					std::string strName = root["ip"].asString();
					long long nConnect = root["connect"].asInt64();
					CLoginServer info;
					info.m_strIP = strName;
					info.m_nConnect = nConnect;
					m_BalanceSelect.UpdateLoginServer(info);
				}
				else if (nCmd == KEEPLIVE)
				{
					printf("keeplive\n");
					{
					m_MutexMapLoginServer.lock();
						evutil_socket_t fd = bufferevent_getfd(bev);
						auto iter = m_MapLoginServer.find(fd);
						if (iter != m_MapLoginServer.end())
						{
							iter->second->m_nLastKeepAlive = GetTickCount64();
						}
						m_MutexMapLoginServer.unlock();
					}

					//std::string strName = root["name"].asString();
					//std::string strPwd = root["pwd"].asString();
					//CLoginServer info = GetLoginServer(strName);
					//Json::Value root;
					//root["msg"] = "ok";
					//root["ip"] = info.m_strIP;
					//root["port"] = info.m_nPort;
					//std::string strString = root.toStyledString();

					//int nLogLen = strString.length();
					//char *pSendData = new char[8 + nLogLen];
					//*(long long *)pSendData = nLogLen;
					//memcpy(pSendData + 8, strString.c_str(), nLogLen);
					//bufferevent_write(bev, pSendData, nLogLen + 8);
					//delete[]pSendData;
				}
			}
		}
	} while (0);
}
//一个新客户端连接上服务器了  
//当此函数被调用时，libevent已经帮我们accept了这个客户端。该客户端的
//文件描述符为fd
void CBalanceServer::listener_cb(evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sock, int socklen, void *arg)
{
	sockaddr_in* addr = (sockaddr_in*)sock;
	std::string strIP = inet_ntoa(addr->sin_addr);
	printf("recv pc client  ip:%s\n", strIP.c_str());
	event_base *base = (event_base*)arg;
	bufferevent *bev = bufferevent_socket_new(base, fd,BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, socket_read_cb, NULL, socket_event_cb, NULL);
	bufferevent_enable(bev, EV_READ | EV_PERSIST);
	//为这个客户端分配一个bufferevent  
	ClientData * pData = new ClientData();
	//pData->m_strIP = strIP;
	m_MutexMapClient.lock();
	m_MapClient.insert(std::map<evutil_socket_t, ClientData *>::value_type(fd, pData));
	m_MutexMapClient.unlock();
}



void CBalanceServer::socket_read_cb(bufferevent *bev, void *arg)
{
	
	evutil_socket_t fd = bufferevent_getfd(bev);
	ClientData *pData = NULL;
	m_MutexMapClient.lock();
	std::map<evutil_socket_t, ClientData *>::iterator   iterfind = m_MapClient.find(fd);
	if (iterfind != m_MapClient.end())
	{
		pData = iterfind->second;
	}
	char line[MAX_LINE + 1];
	int n;
	

	while (n = bufferevent_read(bev, line, MAX_LINE), n > 0)
	{
		if (pData)
		{
			if (pData->m_nBufferLen + n <= pData->m_nMaxLen)
			{
				memcpy(pData->m_RecvBuffer + pData->m_nBufferLen, line, n);
				pData->m_nBufferLen += n;
			}
			else
			{
				char *pOrgData = pData->m_RecvBuffer;
				char *pNewData = new char[pData->m_nMaxLen + pData->m_nBufferLen + n];
				memcpy(pNewData, pOrgData, pData->m_nBufferLen);
				delete[]pOrgData;
				pData->m_RecvBuffer = pNewData;
				pData->m_nMaxLen = pData->m_nMaxLen + pData->m_nBufferLen + n;
				memcpy(pData->m_RecvBuffer + pData->m_nBufferLen, line, n);
				pData->m_nBufferLen += n;
			}
		}
	}
	if (pData)
	{
		int nHeadLen = sizeof(NetMessageHead);
		if (pData->m_nBufferLen >= nHeadLen && pData->m_nRecvLen <= 0)
		{
			NetMessageHead *pHead = (NetMessageHead*)pData->m_RecvBuffer;
			pData->m_nRecvLen = pHead->uMessageSize - nHeadLen;
			memmove(pData->m_RecvBuffer, pData->m_RecvBuffer + nHeadLen, pData->m_nBufferLen - nHeadLen);
			pData->m_nBufferLen = pData->m_nBufferLen - nHeadLen;
		}
		if (pData->m_nBufferLen >= pData->m_nRecvLen && pData->m_nRecvLen > 0)
		{
			
			char *pTemp = new char[pData->m_nRecvLen + 1];
			memset(pTemp, 0, pData->m_nRecvLen + 1);
			memcpy(pTemp, pData->m_RecvBuffer, pData->m_nRecvLen);
			memmove(pData->m_RecvBuffer, pData->m_RecvBuffer + pData->m_nRecvLen, pData->m_nBufferLen - pData->m_nRecvLen);
			pData->m_nBufferLen = pData->m_nBufferLen - pData->m_nRecvLen;
			// 数据处理在这里进行，这里先注释掉 根据需要自己实现处理函数
			CMessage msg;
			msg.m_nFd = fd;
			msg.m_strData = pTemp;
			m_MutexMessage.lock();
			m_deqMessage.push_back(msg);
			m_MutexMessage.unlock();
			pData->m_nRecvLen = -1;
			delete[]pTemp;
		}
	}
	
	m_MutexMapClient.unlock();
}

void CBalanceServer::socket_event_cb(bufferevent *bev, short events, void *arg)
{
	if (events & BEV_EVENT_EOF)
	{
//		printf("connection closed\n");
	}
		
	else if (events & BEV_EVENT_ERROR)
	{
	//	printf("some other error\n");
	}
	/*m_nConnect++;
	QK_WarnLog("the Connet is:%lld", m_nConnect);*/
	//这将自动close套接字和free读写缓冲区  
	evutil_socket_t fd = bufferevent_getfd(bev);
	m_MutexMapClient.lock();
	std::map<evutil_socket_t, ClientData *>::iterator   iterfind = m_MapClient.find(fd);
	if (iterfind != m_MapClient.end())
	{
		delete iterfind->second;
		m_MapClient.erase(iterfind);
		
	}
	bufferevent_free(bev);
	m_MutexMapClient.unlock();
}



//一个新客户端连接上服务器了  
//当此函数被调用时，libevent已经帮我们accept了这个客户端。该客户端的
//文件描述符为fd
void CBalanceServer::loginlistener_cb(evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sock, int socklen, void *arg)
{
	sockaddr_in* addr = (sockaddr_in*)sock;
	std::string strIP = inet_ntoa(addr->sin_addr);
	printf("recv login server ip:%s\n", strIP.c_str());
	event_base *base = (event_base*)arg;
	//为这个客户端分配一个bufferevent  
	bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, loginsocket_read_cb, NULL, loginsocket_event_cb, NULL);
	bufferevent_enable(bev, EV_READ | EV_PERSIST);
	ClientData * pData = new ClientData();
	pData->m_nLastKeepAlive = GetTickCount64();
	pData->m_strIP = strIP;
	m_MutexMapLoginServer.lock();
	m_MapLoginServer.insert(std::map<evutil_socket_t, ClientData *>::value_type(fd, pData));
	m_MutexMapLoginServer.unlock();
}



void CBalanceServer::loginsocket_read_cb(bufferevent *bev, void *arg)
{
	evutil_socket_t fd = bufferevent_getfd(bev);
	ClientData *pData = NULL;
	m_MutexMapLoginServer.lock();
	std::map<evutil_socket_t, ClientData *>::iterator   iterfind = m_MapLoginServer.find(fd);
	if (iterfind != m_MapLoginServer.end())
	{
		pData = iterfind->second;
	}
	char line[MAX_LINE + 1];
	int n;
	

	while (n = bufferevent_read(bev, line, MAX_LINE), n > 0)
	{
		if (pData)
		{
			if (pData->m_nBufferLen + n <= pData->m_nMaxLen)
			{
				memcpy(pData->m_RecvBuffer + pData->m_nBufferLen, line, n);
				pData->m_nBufferLen += n;
			}
			else
			{
				char *pOrgData = pData->m_RecvBuffer;
				char *pNewData = new char[pData->m_nMaxLen + pData->m_nBufferLen + n];
				memcpy(pNewData, pOrgData, pData->m_nBufferLen);
				delete[]pOrgData;
				pData->m_RecvBuffer = pNewData;
				pData->m_nMaxLen = pData->m_nMaxLen + pData->m_nBufferLen + n;
				memcpy(pData->m_RecvBuffer + pData->m_nBufferLen, line, n);
				pData->m_nBufferLen += n;
			}
		}

	}
	if (pData)
	{
		if (pData->m_nBufferLen >= 8 && pData->m_nRecvLen <= 0)
		{
			pData->m_nRecvLen = *((long long *)pData->m_RecvBuffer);
			memmove(pData->m_RecvBuffer, pData->m_RecvBuffer + 8, pData->m_nBufferLen - 8);
			pData->m_nBufferLen = pData->m_nBufferLen - 8;
		}
		if (pData->m_nBufferLen >= pData->m_nRecvLen && pData->m_nRecvLen > 0)
		{

			char *pTemp = new char[pData->m_nRecvLen + 1];
			memset(pTemp, 0, pData->m_nRecvLen + 1);
			memcpy(pTemp, pData->m_RecvBuffer, pData->m_nRecvLen);
			memmove(pData->m_RecvBuffer, pData->m_RecvBuffer + pData->m_nRecvLen, pData->m_nBufferLen - pData->m_nRecvLen);
			pData->m_nBufferLen = pData->m_nBufferLen - pData->m_nRecvLen;
			std::string strJsonData = pTemp;
			ParasLoginJsonData(strJsonData, bev);
			pData->m_nRecvLen = -1;
			delete[]pTemp;
		}
	}
	
	m_MutexMapLoginServer.unlock();
}

void CBalanceServer::loginsocket_event_cb(bufferevent *bev, short events, void *arg)
{
	if (events & BEV_EVENT_EOF)
		printf("connection closed\n");
	else if (events & BEV_EVENT_ERROR)
		printf("some other error\n");
	evutil_socket_t fd = bufferevent_getfd(bev);
	//这将自动close套接字和free读写缓冲区  
	m_MutexMapLoginServer.lock();
	std::map<evutil_socket_t, ClientData *>::iterator   iterfind = m_MapLoginServer.find(fd);
	if (iterfind != m_MapLoginServer.end())
	{
		delete iterfind->second;
		m_MapLoginServer.erase(iterfind);
	}
	bufferevent_free(bev);
	m_MutexMapLoginServer.unlock();
}


int makeDirRecursively(const char* strDirName)
{
	std::string strPath = strDirName;
	WIN32_FIND_DATA  wfd;
	bool bExit = false;
	HANDLE hFind = FindFirstFile(strPath.c_str(), &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		bExit = true;
	}
	FindClose(hFind);
	if (bExit == false)
	{
		CreateDirectory(strPath.c_str(), NULL);
	}
	return 0;
}

std::string getExecutableDir()
{
	char path[512] = "";

#ifdef _WIN32

	GetModuleFileNameA(NULL, path, sizeof(path));

#else

	ssize_t count = readlink("/proc/self/exe", path, sizeof(path));
	if (count <= 0)
	{
		return "";
	}

#endif
	std::string strpath = path;
	std::string::size_type pos = std::string(strpath).find_last_of("\\/");
	strpath = std::string(strpath).substr(0, pos);
	return strpath;
}

void initLoggingSystem(int argc, char* argv[])
{
	(void)argc;
	std::string strGroup("LOG");
	std::string strMinLevel = "debug";

	// 设置最小的日志等级
	int iMinLogLevel = google::GLOG_INFO; //默认值
	if (strMinLevel == "warning")
	{
		iMinLogLevel = google::GLOG_WARNING;
	}
	else if (strMinLevel == "error")
	{
		iMinLogLevel = google::GLOG_ERROR;
	}
	else if (strMinLevel == "fatal")
	{
		iMinLogLevel = google::GLOG_FATAL;
	}

	if (strMinLevel == "debug")
	{
		FLAGS_v = 1;
	}
	else
	{
		FLAGS_v = 0;
	}

	std::string strLoggingPath = getExecutableDir() + "\\log\\";
	makeDirRecursively(strLoggingPath.c_str());

	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::GLOG_INFO, strLoggingPath.c_str());


	FLAGS_log_dir = strLoggingPath;
	FLAGS_max_log_size = 10; //最大log文件的大小，10M字节
	FLAGS_minloglevel = google::GLOG_WARNING;
	FLAGS_logbuflevel = google::GLOG_INFO; //只有buf info的，其他都是sync.
	FLAGS_logbufsecs = 900;
	FLAGS_logtostderr = false;


	if (0)
	{	//是否开启控制台日志
		FLAGS_alsologtostderr = true;
		FLAGS_colorlogtostderr = true;
		FLAGS_stderrthreshold = google::GLOG_INFO;
	}
	else
	{
		//所有的日志都不出现在控制台
		FLAGS_stderrthreshold = google::NUM_SEVERITIES;
	}
}
