#include "stdafx.h"
#include "server/WJListenSocket.h"
namespace WJ
{
	//构造函数
	CWJListenSocket::CWJListenSocket(void)
	{
		m_hSocket = INVALID_SOCKET;
		m_uListenType = 0;
	}

	//析构函数
	CWJListenSocket::~CWJListenSocket(void)
	{
		OutputDebugStringA("sdp_test CloseSocket(10)");
		CloseSocket();
	}

	//建立 SOCKET
	bool CWJListenSocket::Create()
	{
		m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
		return (m_hSocket != INVALID_SOCKET);
	}

	//绑定地址
	bool CWJListenSocket::Bind(const char *bindIp, UINT uPort)
	{
		::memset(&m_SocketAddr, 0, sizeof(m_SocketAddr));
		m_SocketAddr.sin_family = AF_INET;
		m_SocketAddr.sin_addr.s_addr = inet_addr(bindIp);
		m_SocketAddr.sin_port = htons(uPort);
		return (::bind(m_hSocket, (SOCKADDR*)&m_SocketAddr, sizeof(m_SocketAddr)) != SOCKET_ERROR);
	}

	//监听网络
	bool CWJListenSocket::Listen(UINT uListenType)
	{
		m_uListenType = uListenType;
		return (::listen(m_hSocket, 4096) != SOCKET_ERROR);
	}

	//关闭 SOCKET
	bool CWJListenSocket::CloseSocket()
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		return true;
	}
}
