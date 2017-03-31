#ifndef _WJ_CTCPClientSocket_H_
#define _WJ_CTCPClientSocket_H_

#include <vector>
#include <WinSock2.h>
#include "common/WJBaseType.h"
#include "server/WJInterface.h"

namespace WJ
{
#define NO_CONNECT				0								//没有连接
#define CONNECTING				1								//正在连接
#define CONNECTED				2								//成功连接

	//客户端 SOKET
	class WJSERVERMANAGELIB_API CTCPClientSocket
	{
		//变量定义
	private:
		int							m_iReadBufLen;					//缓存区长度
		BYTE						m_bConnectState;				//连接状态
		SOCKET						m_hSocket;						//SOCKET 句柄
		IClientSocketService		* m_pIService;					//处理接口
		BYTE						m_szBuffer[MAX_SEND_SIZE];			//数据缓存区

		int							m_iCheckCode;

		HWND							m_hWindow;					///窗口句柄
		bool							m_bRunSafeThreading_c_wind;			//是否窗口线程启动
		std::vector<char>				m_vRecvBuf;

		long m_dwServerIP;//ip
		UINT m_uPort;//端口

		int								m_nKeepAliveCount;//连接倒计时
		//函数定义
	public:
		//构造函数
		explicit CTCPClientSocket(IClientSocketService * pIService);
		//析构函数
		virtual ~CTCPClientSocket();
		//是否已经连接
		inline BYTE GetSocketState() { return m_bConnectState; }
		//关闭 SOCKET
		inline bool CloseSocket(bool bNotify = true);
		//连接服务器
		inline bool Connect(const char * szServerIP, UINT uPort);
		//连接服务器
		inline bool Connect(long int dwServerIP, UINT uPort);
		//发送函数
		inline int SendData(void * pData, UINT uSize, UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//简单命令发送函数
		inline int SendData(UINT bMainID, UINT bAssistantID, UINT bHandleCode);

		//设置校验码，密文与密钥
		void SetCheckCode(__int64 iCheckCode, int isecretkey);

		bool CheckReConnect();
		bool CheckKeepLive();
		//内部函数
	private:
		//SOCKET 消息处理程序
		inline LRESULT	OnSocketNotifyMesage(WPARAM wParam, LPARAM lParam);

		///WINDOW 消息循环线程
		void WindowMsgThread();
		///窗口回调函数
		static LRESULT CALLBACK WindowProcFunc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	};
}

#endif //_WJ_CTCPClientSocket_H_