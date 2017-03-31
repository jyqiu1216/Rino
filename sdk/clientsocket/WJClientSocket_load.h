#ifndef _WJ_CTCPClientSocket_load_H_
#define _WJ_CTCPClientSocket_load_H_

#include <vector>
#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"
#include "WJSocketMessage.h"
namespace WJ
{
#define NO_CONNECT				0								//没有连接
#define CONNECTING				1								//正在连接
#define CONNECTED				2								//成功连接
	class IClientSocketService_load;
	//客户端 SOKET
	class WJCLIENTSOCKETLOGIC_EXPORT CTCPClientSocket_load
	{
		//变量定义
		private:
		int							m_iReadBufLen;					//缓存区长度
		BYTE						m_bConnectState;				//连接状态
		/*SOCKET*/LLONG				m_hSocket;						//SOCKET 句柄
		IClientSocketService_load		* m_pIService;					//处理接口
		BYTE						m_szBuffer[MAX_SEND_SIZE];			//数据缓存区

		int							m_iCheckCode;

		/*HWND*/int					m_hWindow;					///窗口句柄
		bool							m_bRunSafeThreading_c_wind;			//是否窗口线程启动
		std::vector<char>				m_vRecvBuf;

		long m_dwServerIP;//ip
		UINT m_uPort;//端口

		int								m_nKeepAliveCount;//连接倒计时
		//函数定义
	public:
		//构造函数
		explicit CTCPClientSocket_load(IClientSocketService_load * pIService);
		//析构函数
		virtual ~CTCPClientSocket_load();
		//是否已经连接
		inline BYTE GetSocketState() { return m_bConnectState; }
		//关闭 SOCKET
		inline bool CloseSocket(bool bNotify = true);
		//连接服务器
		inline bool Connect(const char * szServerIP, int uPort);
		//连接服务器
		inline bool Connect(long int dwServerIP, int uPort);
		//发送函数
		inline int SendData(void * pData, UINT uSize, UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//简单命令发送函数
		inline int SendData(UINT bMainID, UINT bAssistantID, UINT bHandleCode);

		//设置校验码，密文与密钥
		void SetCheckCode(__int64 iCheckCode, int isecretkey);

		bool CheckReConnect();
		bool CheckKeepLive();
		int gethWindow();
		void sethWindow(int hWindow);
		//SOCKET 消息处理程序
		/*LRESULT*/long	OnSocketNotifyMesage(/*WPARAM*/int wParam, /*LPARAM*/long lParam);
		//内部函数
	private:
		
		///WINDOW 消息循环线程
		void WindowMsgThread();
	};

	///客户端网络服务接口
	class IClientSocketService_load
	{
		///接口函数 
	public:
		///网络读取消息
		virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket_load * pClientSocket) = 0;
		///网络连接消息
		virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket_load * pClientSocket) = 0;
		///网络关闭消息
		virtual bool OnSocketCloseEvent() = 0;
	};
}

#endif //_WJ_CTCPClientSocket_load_H_