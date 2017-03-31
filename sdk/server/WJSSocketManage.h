
#ifndef _WJ_SSocketManage_H_
#define _WJ_SSocketManage_H_

#include <mutex>
#include <thread>
#include <functional>
#include <atomic>
#include <unordered_map>
#include <deque>
#include <vector>
#include "message/WJNetMessageHead.h"
#include "server/WJListenSocket.h"
#include "server/WJSSocket.h"
#include "common/WJSpinlock.h"

namespace WJ
{
	//套接字运行环境设置
	void Init_WinSock();
	void UnInit_WinSock();

    struct EventItem
    {//套接字事件节点
        EventItem(CWJSSocket *pSocket, int eventType, DWORD sentBytes = 0, void *pUserData = 0)
		: m_pSocket(pSocket), m_eventType(eventType), m_sentBytes(sentBytes), pUserData(pUserData)
        {
        }
        CWJSSocket  *m_pSocket;
        int          m_eventType;
        DWORD        m_sentBytes;
		void        *pUserData;  //回传参数,主动连接时用到
    };

	struct ConnectReq
	{
		char   ip[16];
		int    port;
		int    type;
		void  *pUserData; //回传
	};

	//TCP 网络 SOCKET 管理类
	class WJSERVERMANAGELIB_API CWJSSocketManage
	{
	private:
		std::atomic<bool>			m_bRun;							//工作标志
		std::vector<CWJListenSocket*> m_ListenList;					//监听 SOCKET
		std::unordered_map<UINT, CWJSSocket*>		m_id2socket;    // socket 对象管理, key 是socketid, value 是CWJSSocket 的地址
        UINT                        m_nMaxSocketCount;				//设定的最大连接数
		UINT						m_uRSThreadCount;				//读写线程数目
		HANDLE						m_hCompletionPortRS;			//读写完成端口
        std::deque<EventItem>       m_eventQueue;                   // ThreadAccept  ThreadPullCompletedStatus 这两个线程通过此队列和CWJSSocketManage所在线程通信
        CSpinlock                   m_eventQueueLock;
        std::vector<EventItem>      m_tmpEventResults;
		std::deque<ConnectReq>      m_connectQueue;
		CSpinlock                   m_connectQueueLock;
	public:
		//构造函数
		CWJSSocketManage(UINT nMaxSocketCount);
		//析构函数
		virtual ~CWJSSocketManage();

	public:
		//开始服务
		bool Start_WJSSocketManage(UINT uSocketThradCount);
		//停止服务
		bool Stop_WJSSocketManage();

		//作为服务器角色发起端口侦听
		void AsyncListen(const std::string &bindIp, int listenPort, UINT nListenType);
		//作为客户端发角色起端口连接
		void AsyncConnect(const char *dstIp, int dstPort, int type, void *pUserData = NULL); // pUserData 回调参数，回传


		//消息处理函数, 重写该函数实现业务逻辑
		virtual bool ProccessNetMessage(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize) = 0;
		/**
		 * 连接在底层已经建立成功,马上回调该函数. 
		 * pUserData ： 
		 *    [客户端角色]主动连接时的回传参数,对应AsyncConnect中的参数pUserData，
		 *    [服务端角色]被动连接时无用（始终为NULL）
		 */
		virtual void OnNewConnection(CWJSSocket * pSocket, void *pUserData) = 0; 
		//连接在底层回调此函数，在关闭前回调
		virtual void OnEndConnection(CWJSSocket * pSocket) = 0;
		/**
		 * [客户端角色]主动连接失败时回调, 失败了就不会回调OnNewConnection， 而是回调该函数. 
		 * type 对应AsyncConnect中的参数type, pUserData 对应AsyncConnect中的参数pUserData
		 */
		virtual void OnConnectFailed(int type, void *pUserData) = 0;  

		//功能函数
	public:
		//发送数据函数
		void SendData(UINT sockId, const char * pData, UINT uBufLen);
		//发送数据函数
		void SendData(UINT sockId, UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//发送数据函数
		void SendData(UINT sockId, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//发送数据函数
		void SendData(CWJSSocket * pSocket, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode);

		//关闭连接
		void CloseSocket(UINT socketId);
		//判断是否连接

		//获得节点
		CWJSSocket* getCWJSSocket(UINT uSockedId);

	public:
        //该函数执行在上层业务主线程中
        int ProcessNetEvents();
	private:
		//接收新连接的线程
		int ThreadAccept(SOCKET listenSock, UINT nListenType);
		//IO完成结果拉取线程
		int ThreadPullCompletedStatus();
		//主动连接线程
		int ThreadConnect();


	};
}
#endif //_WJ_SSocketManage_H_