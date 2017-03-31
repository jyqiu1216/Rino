#ifndef _WJ_ListenSocket_H_
#define _WJ_ListenSocket_H_
#include <WinSock2.h>
#include "common/WJBaseType.h"
namespace WJ
{
	//服务器 SOCKET
	class WJSERVERMANAGELIB_API CWJListenSocket
	{
		//变量定义
	private:
		SOCKET						m_hSocket;							//SOCKET 句柄
		SOCKADDR_IN					m_SocketAddr;						//本地网络信息
		UINT                        m_uListenType;
		//函数定义
	public:
		//构造函数
		CWJListenSocket(void);
		//析构函数
		virtual ~CWJListenSocket(void);

		//功能函数
	public:
		//建立 SOCKET
		bool Create();
		//监听网络
		bool Listen(UINT uListenType);
		//绑定地址
		bool Bind(const char *bindIp, UINT uPort);
		inline UINT GetListenType() { return m_uListenType; }
		//关闭 SOCKET
		inline bool CloseSocket();
		//获取 SOCKET 句柄
		inline SOCKET GetSafeSocket() { return m_hSocket; }
		//获取本地网络信息
		inline const sockaddr_in *GetConnectData() { return &m_SocketAddr; }
	};
}


#endif //_WJ_ListenSocket_H_