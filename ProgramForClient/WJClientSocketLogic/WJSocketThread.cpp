#include <windows.h>
#include "clientsocket/WJSocketThread.h"
#include "WJSocketProtocolData.h"
#include "clientsocket/WJSocketEventDelegate.h"
#include "common/WJCommonMarco.h"
#include "qdebug.h"

namespace WJ
{
	WJSocketThread::WJSocketThread(ISocketEventDelegate* delegate)
		:_delegate(delegate)
	{
		m_pCTCPClientSocket = new CTCPClientSocket_load(this);
	}

	WJSocketThread::~WJSocketThread(void)
	{
		WJ_SAFE_DELETE(m_pCTCPClientSocket);

		for (auto iter = _socketMessageQueue.begin(); iter != _socketMessageQueue.end();)
		{
			WJSocketMessage* message = *iter;
			WJSocketMessage::releaseMessage(message);
			iter = _socketMessageQueue.erase(iter);
		}
		_socketMessageQueue.clear();
	}

	///网络读取消息
	bool WJSocketThread::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket_load * pClientSocket)
	{
		WJSocketMessage* SocketMessage = WJSocketMessage::getMessage();
		if (nullptr != SocketMessage)
		{
			SocketMessage->setMessage(pNetHead, (CHAR*)pNetData, uDataSize);
			SocketMessage->socketStatus = SocketStatus_RECV;

			std::lock_guard <std::mutex> autoLock(_dataMutex);
			_socketMessageQueue.push_back(SocketMessage);
			//_delegate->onReadComplete();
			return true;
		}

		return false;
	}

	///网络连接消息
	bool WJSocketThread::OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket_load * pClientSocket)
	{
		WJSocketMessage* SocketMessage = WJSocketMessage::getMessage();
		if (nullptr != SocketMessage)
		{
			if (0 == uErrorCode)
			{
				SocketMessage->socketStatus = SocketStatus_SUCCESS;
				{
					std::lock_guard <std::mutex> autoLock(_dataMutex);
					_socketMessageQueue.push_back(SocketMessage);
				}
			}
			else
			{
				SocketMessage->socketStatus = SocketStatus_DISCONNECT;
				{
					std::lock_guard <std::mutex> autoLock(_dataMutex);
					_socketMessageQueue.push_back(SocketMessage);
				}
			}

			//_delegate->onReadComplete();
			return true;
		}

		return false;
	}

	///网络关闭消息
	bool WJSocketThread::OnSocketCloseEvent()
	{
		WJSocketMessage* SocketMessage = WJSocketMessage::getMessage();
		if (nullptr != SocketMessage)
		{
			SocketMessage->socketStatus = SocketStatus_DISCONNECT;
			{
				std::lock_guard <std::mutex> autoLock(_dataMutex);
				_socketMessageQueue.push_back(SocketMessage);
			}
			//_delegate->onReadComplete();
			return true;
		}

		return false;
	}

	bool WJSocketThread::openWithIp(const char* ip, int port)
	{
		return m_pCTCPClientSocket->Connect(ip, port);
	}

	bool WJSocketThread::openWithHost(const char* host, int port)
	{
		return m_pCTCPClientSocket->Connect(host, port);
	}

	bool WJSocketThread::close()
	{
		OutputDebugStringA("sdp_test client 关闭 SOCKET 10");
		return m_pCTCPClientSocket->CloseSocket(false);
	}

	BYTE WJSocketThread::GetSocketState()
	{
		return m_pCTCPClientSocket->GetSocketState();
	}

	bool WJSocketThread::connected()
	{ 
		int tag = m_pCTCPClientSocket->GetSocketState();

		//char str[MAX_PATH] = {0};
		//sprintf_s(str, "sdp_test client WJSocketThread::connected tag=%d", tag);
		//OutputDebugStringA(str);
		return CONNECTED == tag;
	}

	// 发送数据
	int WJSocketThread::sendData(void * pData, UINT uSize, UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{
		return m_pCTCPClientSocket->SendData(pData, uSize, bMainID, bAssistantID, bHandleCode);
	}

	// 发送数据
	int WJSocketThread::sendData(UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{
		return m_pCTCPClientSocket->SendData(bMainID, bAssistantID, bHandleCode);
	}

	void WJSocketThread::transform_socket(std::function<void(WJSocketMessage* socketMessage)> func)
	{
		int queueSize = 0;
		do 
		{
			std::lock_guard < std::mutex > autoLock(_dataMutex);
			WJSocketMessage* socketMessage = nullptr;

			queueSize = _socketMessageQueue.size();
			if (queueSize > 0)
			{
				socketMessage = _socketMessageQueue.front();
				_socketMessageQueue.pop_front();
			}

			if (nullptr != socketMessage)
			{
				func(socketMessage);
				WJSocketMessage::releaseMessage(socketMessage);
			}

			Sleep(1);
		} while (queueSize > 0);
	}

}
