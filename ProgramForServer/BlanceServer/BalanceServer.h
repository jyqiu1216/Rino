#ifndef INC_BALANCESERVER_H
#define INC_BALANCESERVER_H
#ifdef __cplusplus
extern "C"
{
#endif

	//包含所需要的头文件
#include "event2/event.h"
#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "event2/thread.h"
#include "event2/buffer.h"

#ifdef __cplusplus
};
#endif
#include <deque>
#include <map>
#include "json/json.h"
#ifdef _MSC_VER
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libevent_core.lib")
#endif
#define MAX_LINE    256 
#include "BalanceSelect.h"

struct ClientData
{
	ClientData()
	{
		m_strIP = "";
		m_nLastKeepAlive = 0;
		m_nBufferLen = 0;
		m_nMaxLen = 128;
		m_nRecvLen = -1;
		m_RecvBuffer= new char[m_nMaxLen];
	}
	~ClientData()
	{
		if (m_RecvBuffer)
		{
			delete[]m_RecvBuffer;
		}
	}
	std::string m_strIP;//ip地址
	long m_nRecvLen;//接收数据长度
	long m_nBufferLen;//发送数据长度
	long m_nMaxLen;//最大长度
	char * m_RecvBuffer;//接收数据内容
	long long m_nLastKeepAlive;
};

void initLoggingSystem(int argc, char* argv[]);
typedef struct tagNetMessageHead
{
	UINT						uMessageSize;			// 数据包大小
	UINT						bMainID;				// 处理主类型
	UINT						bAssistantID;			// 辅助处理类型 ID
	UINT						bHandleCode;			// 第三级id
	UINT						bReserve;				// 保留字段

	//tagNetMessageHead()
	//{
	//	std::memset(this, 0, sizeof(tagNetMessageHead));
	//}
}  NetMessageHead;
struct CMessage
{
	long m_nFd;
	std::string m_strData;
};
class CBalanceServer
{
public:
	CBalanceServer();
	~CBalanceServer();
	void StartService();
	void StopService();
	static void ParasJsonData(CMessage &msg);
	static void ParasLoginJsonData(std::string &strJsonString, bufferevent *bev);
	//监听回调函数
	static void listener_cb(evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sock, int socklen, void *arg);

	//从Socket接收消息的回调函数
	static void socket_read_cb(bufferevent *bev, void *arg);

	//从Socket事件的回调函数
	static void socket_event_cb(bufferevent *bev, short events, void *arg);


	//监听回调函数
	static void loginlistener_cb(evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sock, int socklen, void *arg);

	//从Socket接收消息的回调函数
	static void loginsocket_read_cb(bufferevent *bev, void *arg);

	//从Socket事件的回调函数
	static void loginsocket_event_cb(bufferevent *bev, short events, void *arg);

	static DWORD WINAPI  CheckKeepLiveFunc(LPVOID arg);

	static DWORD WINAPI  ClientProcessFunc(LPVOID arg);
private:
	static long long m_nConnect;
	static CBalanceSelect m_BalanceSelect;
	static std::recursive_mutex m_MutexMapClient;//互斥量，方便线程对互斥量上锁
	static std::recursive_mutex m_MutexMapLoginServer;//互斥量，方便线程对互斥量上锁
	static std::map<evutil_socket_t, ClientData*>   m_MapClient;
	static std::map<evutil_socket_t, ClientData*>   m_MapLoginServer;

	static std::recursive_mutex m_MutexMessage;//互斥量，方便线程对互斥量上锁
	static std::deque<CMessage> m_deqMessage;
	int  m_nLoginPort;//中心服务器端口（面向ls）
	int  m_nBalancePort;//中心服务器端口（面向客户端）
	bool m_bExit;
	HANDLE m_hCheckKeepLive;
	HANDLE m_hClientProcess;
};
#endif