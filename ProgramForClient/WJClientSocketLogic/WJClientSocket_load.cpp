
#include "clientsocket/WJClientSocket_load.h"
#include "message/WJNetMessageHead.h"
#include <thread>
#include <WinSock2.h>
#include "common/commonfunction.h"
namespace WJ
{

#define WM_SOCKET_MESSAGE		WM_USER+12						//SOCKET 消息
	//构造函数
	CTCPClientSocket_load::CTCPClientSocket_load(IClientSocketService_load * pIService)
	{
		m_iReadBufLen = 0;
		m_pIService = pIService;
		m_bConnectState = NO_CONNECT;
		m_hSocket = INVALID_SOCKET;
		m_hWindow = 0;
		m_iCheckCode = INVALID_VALUE;
		m_bRunSafeThreading_c_wind = true;
		std::thread recvThread_wind(std::bind(&CTCPClientSocket_load::WindowMsgThread, this));
		recvThread_wind.detach();

		m_vRecvBuf.clear();
		std::vector<char> tmp;
		m_vRecvBuf.swap(tmp);
		m_dwServerIP = 0;
		m_uPort = 0;
		m_nKeepAliveCount = 0;

		//用来保存WinSock库的版本号  
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;
		//printf("This is a Server side application!\n");
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

	//析构函数
	CTCPClientSocket_load::~CTCPClientSocket_load()
	{
		//关闭窗口句柄
		if ((m_hWindow != 0) && (::IsWindow((HWND)m_hWindow) == TRUE)) ::SendMessage((HWND)m_hWindow, WM_CLOSE, 0, 0);
		m_bRunSafeThreading_c_wind = false;
		OutputDebugStringA("sdp_test client 关闭 SOCKET 1");
		CloseSocket(false);
	}

#define GWL_USERDATA        (-21)
	//窗口回调函数
	LRESULT CALLBACK WindowProcFunc_CTCPClientSocket_load(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CREATE:		//窗口建立消息
		{
			DWORD iIndex = TlsAlloc();
			CTCPClientSocket_load * pClientSocket = (CTCPClientSocket_load *)(((CREATESTRUCT *)lParam)->lpCreateParams);
			TlsSetValue(iIndex, pClientSocket);
			::SetWindowLong((HWND)hWnd, GWL_USERDATA, iIndex);
			break;
		}
		case WM_SOCKET_MESSAGE:		//套接字消息
		{
			DWORD iIndex = ::GetWindowLong((HWND)hWnd, GWL_USERDATA);
			CTCPClientSocket_load * pClientSocket = (CTCPClientSocket_load *)::TlsGetValue(iIndex);//::TlsGetValue(iIndex);
			if (pClientSocket != nullptr){ pClientSocket->OnSocketNotifyMesage((int)wParam, (int)lParam); }
			break;
		}
		case WM_CLOSE:		//窗口关闭消息
		{
			DestroyWindow((HWND)hWnd);
			break;
		}
		case WM_DESTROY:	//窗口关闭消息
		{
			DWORD iIndex = ::GetWindowLong((HWND)hWnd, GWL_USERDATA);
			CTCPClientSocket_load * pClientSocket = (CTCPClientSocket_load *)::TlsGetValue(iIndex);
			if (pClientSocket != NULL) pClientSocket->sethWindow(0);
			::TlsFree(iIndex);
			PostQuitMessage(0);
			//ExitProcess(0);
			break;
		}
		}
		return ::DefWindowProc((HWND)hWnd, uMsg, wParam, lParam);
	}

	int CTCPClientSocket_load::gethWindow()
	{
		return m_hWindow;
	}

	void CTCPClientSocket_load::sethWindow(int hWindow)
	{
		m_hWindow = hWindow;
	}

	//WINDOW 消息循环线程
	void CTCPClientSocket_load::WindowMsgThread()
	{
		try
		{
			//注册窗口类
			LOGBRUSH		LogBrush;
			WNDCLASS		WndClass;
			TCHAR			szClassName[] = TEXT("CClientSocketWindow");

			LogBrush.lbColor = RGB(0, 0, 0);
			LogBrush.lbStyle = BS_SOLID;
			LogBrush.lbHatch = 0;
			WndClass.cbClsExtra = 0;
			WndClass.cbWndExtra = 0;
			WndClass.hCursor = NULL;
			WndClass.hIcon = NULL;
			WndClass.lpszMenuName = NULL;
			WndClass.lpfnWndProc = WindowProcFunc_CTCPClientSocket_load;
			WndClass.lpszClassName = szClassName;
			WndClass.style = CS_HREDRAW | CS_VREDRAW;
			WndClass.hInstance = NULL;
			WndClass.hbrBackground = (HBRUSH)::CreateBrushIndirect(&LogBrush);
			::RegisterClass(&WndClass);

			//建立窗口
			this->m_hWindow = (int)::CreateWindow(szClassName, NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, this);
			if (this->m_hWindow == NULL) throw TEXT("窗口建立失败");
		}
		catch (...)
		{
			//TRACE("CATCH:%s with %s\n", __FILE__, __FUNCTION__);
			//启动错误
		}

		//消息循环
		MSG	Message;
		while (::GetMessage(&Message, NULL, 0, 0))
		{
			if (!::TranslateAccelerator(Message.hwnd, NULL, &Message))
			{
				::TranslateMessage(&Message);
				::DispatchMessage(&Message);
			}
			Sleep(0);
		}
	}

	//SOCKET 消息处理程序
	long/*LRESULT*/	CTCPClientSocket_load::OnSocketNotifyMesage(/*WPARAM*/int wParam, /*LPARAM*/long lParam)
	{
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_CONNECT:	//连接信息
		{
			UINT uErrorCode = WSAGETSELECTERROR(lParam);
			if (uErrorCode == 0)
			{
				m_bConnectState = CONNECTED;
			}
			else
			{
				//xlog_out("Log_AFCComEx", "CTCPClientSocket", P_ERROR, "连接失败，断开套接字");
				OutputDebugStringA("sdp_test client 关闭 SOCKET 2");
				CloseSocket();
			}
			if (m_pIService != NULL)
			{
				m_pIService->OnSocketConnectEvent(uErrorCode, this);
			}

			return 0;
		}
		case FD_READ:	//读取数据
		{
			///////////////////////////////////////////////////////////////
			//接收数据
			int iRecvCode = ::recv(m_hSocket, (char *)(m_szBuffer + m_iReadBufLen), MAX_SEND_SIZE - m_iReadBufLen, 0);

			if (iRecvCode > 0)
			{
				//处理数据
				m_iReadBufLen += iRecvCode;
				NetMessageHead * pNetHead = (NetMessageHead *)m_szBuffer;

				while ((m_iReadBufLen >= sizeof(NetMessageHead)) && (m_iReadBufLen >= pNetHead->uMessageSize))
				{
					//效验数据
					if (pNetHead->uMessageSize < sizeof(NetMessageHead))
					{
						OutputDebugStringA("sdp_test client 关闭 SOCKET 3");
						CloseSocket(true);
						return 0;
					}

					//打包数据
					UINT uMessageSize = pNetHead->uMessageSize;
					UINT uHandleSize = uMessageSize - sizeof(NetMessageHead);
					if (uMessageSize > MAX_SEND_SIZE)
					{
						OutputDebugStringA("sdp_test client 关闭 SOCKET 4");
						CloseSocket(true);
						return 0;
					}

					{//有时候没收到连接成功消息，但是能收到消息就认为连接服务器成功
						m_bConnectState = CONNECTED;
						//OutputDebugStringA("sdp_bbb 用收到消息来判断连接成功");
					}

					if (NET_M_CONNECT == pNetHead->bMainID && ASS_NET_CONNECT_1 == pNetHead->bAssistantID)
					{//心跳包底层处理
						SendData(NET_M_CONNECT, ASS_NET_CONNECT_1, 0);
					}
					else
					{
						if (NET_M_CONNECT == pNetHead->bMainID && ASS_NET_CONNECT_2 == pNetHead->bAssistantID)
						{
							Net_ConnectSuccess* p = (Net_ConnectSuccess*)(pNetHead + 1);
							SetCheckCode(p->i64CheckCode, SECRECTKEY);
						}
						//MyDebugString("sdp_test2", "client 底层消息(%d,%d,%d)", pNetHead->bMainID, pNetHead->bAssistantID, pNetHead->bHandleCode);
						if (!m_pIService->OnSocketReadEvent(pNetHead, uHandleSize ? pNetHead + 1 : nullptr, uHandleSize, this))
						{
							char sdp[MAX_PATH] = { 0 };
							sprintf_s(sdp, "sdp_test client 关闭 SOCKET 5(%d,%d,%d)", pNetHead->bMainID, pNetHead->bAssistantID, pNetHead->bHandleCode);
							OutputDebugStringA(sdp);
							CloseSocket(true);
							return 0;
						}
					}

					//删除缓存数据
					::MoveMemory(m_szBuffer, m_szBuffer + uMessageSize, m_iReadBufLen - uMessageSize);
					m_iReadBufLen -= uMessageSize;
				}

			}
			///////////////////////////////////////////////////////////////
			return 0;
		}
		case FD_CLOSE:	//关闭消息
		{
			OutputDebugStringA("sdp_test client 关闭 SOCKET 6");
			CloseSocket();
			return 0;
		}
		}
		return 0;
	}

	//连接服务器
	bool CTCPClientSocket_load::Connect(const char * szServerIP, int uPort)
	{
		m_dwServerIP = inet_addr(szServerIP);
		if (m_dwServerIP == INADDR_NONE)
		{
			LPHOSTENT lpHost = ::gethostbyname(szServerIP);
			if (lpHost == NULL) return false;
			m_dwServerIP = ((LPIN_ADDR)lpHost->h_addr)->s_addr;
		}

		m_uPort = uPort;
		return Connect(m_dwServerIP, m_uPort);
	}

	//连接服务器
	bool CTCPClientSocket_load::Connect(long int dwServerIP, int uPort)
	{
		//效验数据
		if (dwServerIP == INADDR_NONE)
		{
			//xlog_out("Log_AFCComEx", "CTCPClientSocket", P_ERROR, "连接服务器 失败1");
			return false;
		}

		if (NO_CONNECT != m_bConnectState)
		{//不是断开状态就不要继续连接，先断开前面发起的连接
			OutputDebugStringA("sdp_test client 关闭 SOCKET 7");
			CloseSocket(false);
		}

		m_hSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_hSocket == INVALID_SOCKET)
		{
			//xlog_out("Log_AFCComEx", "CTCPClientSocket", P_ERROR, "连接服务器 失败2");
			return false;
		}
		if (::WSAAsyncSelect(m_hSocket, (HWND)m_hWindow, WM_SOCKET_MESSAGE, FD_READ | FD_CONNECT | FD_CLOSE) == SOCKET_ERROR)
		{
			int res = ::WSAGetLastError();
			//xlog_out("Log_AFCComEx", "CTCPClientSocket", P_ERROR, "连接服务器 失败3");
			return false;
		}

		//解释服务器地址
		sockaddr_in SocketAddr;
		SocketAddr.sin_family = AF_INET;
		SocketAddr.sin_port = htons(uPort);
		SocketAddr.sin_addr.S_un.S_addr = dwServerIP;

		//连接操作
		if ((::connect(m_hSocket, (sockaddr *)&SocketAddr, sizeof(SocketAddr)) == SOCKET_ERROR))
		{
			int res = ::WSAGetLastError();
			if (::WSAGetLastError() != WSAEWOULDBLOCK)
			{
				//xlog_out("Log_AFCComEx", "CTCPClientSocket", P_WARN, "连接服务器 失败4");
				OutputDebugStringA("sdp_test client 关闭 SOCKET 8");
				CloseSocket(false);
				return false;
			}
		}

		//设置数据
		m_bConnectState = CONNECTING;
		return true;
	}

	//关闭 SOCKET
	bool CTCPClientSocket_load::CloseSocket(bool bNotify)
	{
		bool bClose = (m_hSocket != INVALID_SOCKET);
		m_bConnectState = NO_CONNECT;
		if (m_hSocket != INVALID_SOCKET)
		{
			::WSAAsyncSelect(m_hSocket, (HWND)this->m_hWindow, WM_SOCKET_MESSAGE, 0);
			::closesocket(m_hSocket);
			m_hSocket = INVALID_SOCKET;
			m_iReadBufLen = 0;
		}
		if ((bNotify == true) && (bClose == true) && (m_pIService != NULL))
			m_pIService->OnSocketCloseEvent();

		return bClose;
	}

	//发送函数
	int CTCPClientSocket_load::SendData(void * pData, UINT uSize, UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{
		if ((m_hSocket != INVALID_SOCKET)
			&& (uSize <= MAX_SEND_SIZE)
			&& (CONNECTED == m_bConnectState)
			&& (INVALID_VALUE != m_iCheckCode))
		{

			//定义数据
			int iErrorCode = 0;
			char bSendBuffer[MAX_SEND_SIZE];
			UINT uSendSize = uSize + sizeof(NetMessageHead), uSended = 0;

			//打包数据
			NetMessageHead * pNetHead = (NetMessageHead *)bSendBuffer;
			pNetHead->uMessageSize = uSendSize;
			pNetHead->bMainID = bMainID;
			pNetHead->bAssistantID = bAssistantID;
			pNetHead->bHandleCode = bHandleCode;
			pNetHead->bReserve = m_iCheckCode;
			if (uSize > 0) ::CopyMemory(bSendBuffer + sizeof(NetMessageHead), pData, uSize);

			//发送数据
			do
			{
				iErrorCode = ::send(m_hSocket, bSendBuffer + uSended, uSize + sizeof(NetMessageHead)-uSended, 0);
				if (iErrorCode == SOCKET_ERROR)
				{
					int res = ::WSAGetLastError();
					if (res == WSAEWOULDBLOCK)
					{
						return uSize;
					}
					else
					{
						return SOCKET_ERROR;
					}
				}
				uSended += iErrorCode;
				Sleep(0);
			} while (uSended < uSize + sizeof(NetMessageHead));

			return uSize;
		}

		return SOCKET_ERROR;
	}

	//简单命令发送函数
	int CTCPClientSocket_load::SendData(UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{
		return SendData(NULL, 0, bMainID, bAssistantID, bHandleCode);
	}

	//设置校验码，密文与密钥
	void CTCPClientSocket_load::SetCheckCode(__int64 iCheckCode, int isecretkey)
	{
		m_iCheckCode = ((int)iCheckCode - isecretkey) / 23;
	}

	bool CTCPClientSocket_load::CheckReConnect()
	{
		if (m_bConnectState == NO_CONNECT)
		{
			if (!Connect(m_dwServerIP, m_uPort))
			{
				return false;
			}
		}
		return true;
	}

	bool CTCPClientSocket_load::CheckKeepLive()
	{
		if (m_bConnectState == CONNECTED)
		{
			m_nKeepAliveCount++;
			if (m_nKeepAliveCount > 5)
			{
				OutputDebugStringA("sdp_test client 关闭 SOCKET 9");
				CloseSocket(true);
			}
		}
		return true;
	}
}