#ifndef _WJ_SSocket_H_
#define _WJ_SSocket_H_


#include <WinSock2.h>
#include <mutex>
#include <atomic>
#include <list>
#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"

namespace WJ
{
	//控制标识
#define SOCKET_NEW              0                               //新的 SOCKET 连接
#define SOCKET_SND				1								//SOCKET 发送完成事件
#define SOCKET_REV				2								//SOCKET 接收完成事件

#define INVALID_SOCKET_ID      -1                               // 约定 -1 为无效socket id

	class CWJSSocketManage;

	struct SendBuf
	{
		SendBuf(char *buf, UINT len) : m_buf(buf), m_len(len)
		{

		}
		char  *m_buf;
		UINT   m_len;
	};
	//TCP SOCKET 类
	class WJSERVERMANAGELIB_API CWJSSocket
	{
		friend class CWJSSocketManage;
	public:
		//结构定义
		struct OverLappedStruct		//SOCKET 重叠 IO 结构
		{
			//变量定义
			OVERLAPPED				OverLapped;							//重叠结构
			WSABUF					WSABuffer;							//数据缓冲
			UINT					uOperationType;						//操作类型

			OverLappedStruct()
			{
				ZeroMemory(this, sizeof(OverLappedStruct));
			}
		};

	private:
		static std::atomic<UINT>    m_sockidSeed;                       //socketid 生成种子

        std::atomic<UINT>           m_refCount;                         //socketid 生成种子
		UINT						m_sockid;							//SOCKET id
		SOCKET						m_hSocket;							//SOCKET 句柄
		UINT                        m_type;                             //连接类型，例如区分客户端连接还是LoginServer的连接
		//void                       *m_pUserObj;                         //指向用户对象
		std::list<SendBuf>          m_sendList;                         //发送队列
		UINT						m_sendPendingSize;					//发送队列中数据总长度
		bool                        m_bSending;                         //正在发送队列中数据(IOCP 发送请求已递交等待完成包)
		char						m_szRecvBuf[MAX_SEND_SIZE];			//数据接收缓冲区
		DWORD						m_dwRecvBuffLen;					//接收缓冲区长度

		CWJSSocketManage		  * m_pManage;						    //SOCKET 管理类指针
		struct sockaddr_in			m_peerAddr;						    //地址信息

		OverLappedStruct			m_SendOverData;						//发送数据重叠结构
		OverLappedStruct			m_RecvOverData;						//接收数据重叠结构

		UINT m_Keep;//保持心跳连接所用
		//函数定义
	public:
        inline void IncRef() { ++ m_refCount; }
        inline void DecRef()
        {
            --m_refCount;
            if (0 == m_refCount)
            {
                delete this;
            }
        }
		//析够函数
		virtual ~CWJSSocket(void);
		//功能函数
	public:

		//发送数据函数
		void SendData(UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//发送数据函数
		void SendData(void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//发送数据函数
		void SendData(const char * pData, UINT uBufLen);
		//判断是否连接
		inline bool IsConnect() { return (m_hSocket != INVALID_SOCKET); }
		// 获取连接类型
		inline UINT GetType() { return m_type; }
		inline void SetType(UINT uSockType) { m_type = uSockType; }
		//inline void SetUserObj(void *pUserObj) { m_pUserObj = pUserObj; }
		//inline void *GetUserObj() { return m_pUserObj;  }
		inline UINT GetSendPendingSize() { return m_sendPendingSize; }
		//获取连接信息
		//ULONG dwAccessIP = this->GetPeerAddr()->sin_addr.S_un.S_addr 可获取ip地址
		inline const sockaddr_in *GetPeerAddr() { return &m_peerAddr; }
		inline UINT GetSockID() { return m_sockid; }
		inline UINT64 GetSocket() { return (UINT64)m_hSocket; }
		//设置管理类指针
		inline void SetSocketManagePtr(CWJSSocketManage * pManage) { m_pManage = pManage; }
		//关闭 SOCKET
		void CloseSocket();
		//发送完成函数
		bool OnSendCompleted(DWORD dwThancferred);
		//接收完成函数
		bool OnRecvCompleted();
		//设置当前套接字最新使用时间
		void SetKeep();
		//获得套接字最近使用时间
		UINT GetKeep();
    private:
		//构造函数
		CWJSSocket(void);
		//合并 SOCKET
		bool Attach(SOCKET hSocket, sockaddr_in & peerAddr);
		//投递接收 IO
		bool RecvData();
		//检测发送数据函数
		bool CheckSendData();
	};
}


#endif //_WJ_SSocket_H_