// MemeryShare.cpp


#include "macrodefinition.h"
#include <process.h>

#include "ShareMemory.h"
#include "EventSignal.h"

EventSignal::EventSignal()
{
	m_log.initialize("sharememory");
	m_eventArray.resize(3);
	m_isServer = true;
	m_isAutoSend = false;
}
EventSignal::~EventSignal()
{
	SafeReleasePtr(m_pServerSM);
	SafeReleasePtr(m_pClientSM);


	if (m_threadHandle)
	{
		SetEvent(m_eventArray[EVENT_EXIT_INDEX]);
		if (0 == WaitForSingleObject(m_threadHandle, 100))
		{
			CloseHandle(m_threadHandle);
		}
		else
		{
			::TerminateThread(m_threadHandle, 1);
		}
	}

	m_threadHandle = NULL;
	SafeReleaseHandle(m_eventArray[EVENT_EXIT_INDEX]);
	SafeReleaseHandle(m_eventArray[EVENT_SERVER_INDEX]);
	SafeReleaseHandle(m_eventArray[EVENT_CLIENT_INDEX]);

	SafeReleasePtr(m_pServerSM);
	SafeReleasePtr(m_pClientSM);
}



BOOL EventSignal::initEventSignal(LPCTSTR serverEventName, LPCTSTR clientEventName,
	LPCTSTR exitEventName, IDataRecive* pDataRecive, bool isServer)
{
	LPCTSTR pEventName = exitEventName ? exitEventName : EVENT_EXIT_DEFAULT_NAME;

	m_eventArray[EVENT_EXIT_INDEX] = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (!m_eventArray[EVENT_EXIT_INDEX])
	{
		return FALSE;
	}

	pEventName = serverEventName ? serverEventName : EVENT_SERVER_DEFAULT_NAME;
	m_eventArray[EVENT_SERVER_INDEX] = OpenEvent(EVENT_ALL_ACCESS, FALSE, pEventName);
	if (!m_eventArray[EVENT_SERVER_INDEX])
	{
		m_eventArray[EVENT_SERVER_INDEX] = CreateEvent(NULL, FALSE, FALSE, pEventName);
	}
	if (!m_eventArray[EVENT_SERVER_INDEX])
	{
		return FALSE;
	}

	pEventName = clientEventName ? clientEventName : EVENT_CLIENT_DEFAULT_NAME;
	m_eventArray[EVENT_CLIENT_INDEX] = OpenEvent(EVENT_ALL_ACCESS, FALSE, pEventName);
	if (!m_eventArray[EVENT_CLIENT_INDEX])
	{
		m_eventArray[EVENT_CLIENT_INDEX] = CreateEvent(NULL, FALSE, FALSE, pEventName);
	}
	if (!m_eventArray[EVENT_CLIENT_INDEX])
	{
		return FALSE;
	}

	this->m_isServer = isServer;
	m_pServerSM = new CShareMemory(pDataRecive, TEXT("ZSSHAREMOMERY_SERVER"));
	m_pClientSM = new CShareMemory(pDataRecive, TEXT("ZSSHAREMOMERY_CLIENT"));
	BOOL result = m_pServerSM->OpenShareFile(WANGJING_PLATFORM_SERVER_SHARE_MEMORY_NAME);
	result &= m_pClientSM->OpenShareFile(WANGJING_PLATFORM_CLIENT_SHARE_MEMORY_NAME);

	return  result;

}


BOOL EventSignal::StartThread()
{

	if (m_threadHandle)
	{
		//SetEvent(m_eventArray[EVENT_EXIT_INDEX]);
		SetEvent(m_threadHandle);
		if (0 == WaitForSingleObject(m_threadHandle, 100))
		{
			CloseHandle(m_threadHandle);
		}
		else
		{
			::TerminateThread(m_threadHandle, 1);
		}
	}

	m_threadHandle = NULL;

	m_threadHandle = (HANDLE)_beginthreadex(nullptr, 0, &EventSignal::mainLoop, this, 0, nullptr);
	if (m_threadHandle)
		return TRUE;

	OutputDebugStringA("eventsignal start false!");
	return FALSE;
}

BOOL EventSignal::SendEvent(LPVOID pData, size_t size)
{
	if (!pData)
		return FALSE;

	if (m_isServer)//Send data to the client 
	{
		m_pServerSM->WriteShareMemory(pData, size);
		SetEvent(m_eventArray[EVENT_CLIENT_INDEX]);
		m_log.writeLog(XLog::E_LOG_DEBUG, "Write Share Memory Data [Server]");
	}
	else
	{
		m_pClientSM->WriteShareMemory(pData, size);
		SetEvent(m_eventArray[EVENT_SERVER_INDEX]);
		m_log.writeLog(XLog::E_LOG_DEBUG, "Write Share Memory Data [Client]");
	}
	return TRUE;
}


BOOL EventSignal::CallExit()
{

	if (m_threadHandle)
	{
		SetEvent(m_threadHandle);
		if (0 == WaitForSingleObject(m_threadHandle, 100))
		{
			CloseHandle(m_threadHandle);
		}
		else
		{
			::TerminateThread(m_threadHandle, 1);
		}
	}
	else
	{
		::TerminateThread(m_threadHandle, 1);
	}
	m_threadHandle = nullptr;
	return TRUE;
}



UINT EventSignal::mainLoop(void* r)
{
	EventSignal* pThis = (EventSignal*)r;

	HANDLE waitEvents[2];
	waitEvents[0] = pThis->m_eventArray[EVENT_EXIT_INDEX];
	if (pThis->m_isServer)
	{
		waitEvents[1] = pThis->m_eventArray[EVENT_SERVER_INDEX];
	}
	else
	{
		waitEvents[1] = pThis->m_eventArray[EVENT_CLIENT_INDEX];
	}


	int logLineCount = 0;
	while (true)
	{
		switch (WaitForMultipleObjects(2, waitEvents, FALSE, 50/*INFINITE*/))
		{
		case 0:
			return 0;
		case 1:
			if (pThis->m_isServer)
				pThis->m_pClientSM->ReadFromShareMemory();
			else
				pThis->m_pServerSM->ReadFromShareMemory();

			if (logLineCount <= 50)
			{
				pThis->getLog().writeLog(XLog::E_LOG_DEBUG, "Read Share Memory Data");
				logLineCount++;
			}
			break;
		default:
			break;
		}
		//本地模式采用，一直向服务器一直发送数据获取
		//0---------------------通过定时器进行发送
		if (pThis->m_isAutoSend)
		{
			pThis->m_pClientSM->WriteShareMemory();
			SetEvent(pThis->m_eventArray[EVENT_SERVER_INDEX]);
		}
	}
}