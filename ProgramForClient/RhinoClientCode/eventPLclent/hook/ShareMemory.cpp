// MemeryShare.cpp


#include "ShareMemory.h"
#include "macrodefinition.h"
#include <process.h>
#include <tchar.h>

CShareMemory::CShareMemory(IDataRecive* pDataRecive, LPCTSTR shareMemoryName)
	: m_shareMemoryHandle(nullptr)
	, m_lpBuffer(nullptr)
	, m_pIDataRecive(pDataRecive)
	//, m_hMutex(nullptr)
{
	m_shareMemoryName = shareMemoryName;
}
CShareMemory::~CShareMemory()
{
	SafeReleaseHandle(m_shareMemoryHandle);
	//SafeReleaseHandle(m_hMutex);
}

BOOL CShareMemory::CreateShareFile(LPCTSTR shareName)
{
	if (shareName)
		m_shareMemoryName = shareName;

	SafeReleaseHandle(m_shareMemoryHandle);

	m_shareMemoryHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT,
		0, MEMERY_DEFAULT_SIZE, m_shareMemoryName.c_str());

	if (!m_shareMemoryHandle)
	{
		DWORD ec = GetLastError();
		OutputDebugString(m_shareMemoryName.c_str());
		OutputDebugString(TEXT("--CREATE SHAREFILE ERROR!!!!! \n"));
		return FALSE;
	}
	m_lpBuffer = MapViewOfFile(m_shareMemoryHandle, FILE_MAP_ALL_ACCESS, 0, 0, MEMERY_DEFAULT_SIZE);

	if (!m_lpBuffer)
	{
		return FALSE;
	}
	return TRUE;

}
BOOL CShareMemory::ReadFromShareMemory(LPVOID pData, size_t size)
{
	if (!m_lpBuffer)
		return FALSE;

	//DWORD dw = WaitForSingleObject(m_hMutex, 100);
	//switch (dw)
	//{
	//case WAIT_OBJECT_0:
	//{

	memcpy(pData, m_lpBuffer, size);
	//		ReleaseMutex(m_hMutex);
	return TRUE;
	//}
	//case WAIT_TIMEOUT:
	//{
	//	OutputDebugStringA("share momery WAIT_TIMEOUT !\n");
	//}break;
	//case WAIT_FAILED:
	//{
	//	OutputDebugStringA("share momery WAIT_FAILED !\n");
	//}
	//default:
	//	break;
	//}
	//return FALSE;
}

BOOL CShareMemory::ReadFromShareMemory(IDataRecive* pDataRecive)
{

	//DWORD dw = WaitForSingleObject(m_hMutex, 100);
	//switch (dw)
	//{
	//case WAIT_OBJECT_0:
	//{
	if (pDataRecive)
	{
		m_pIDataRecive->DoData(m_lpBuffer, 0);
	}
	if (m_pIDataRecive)
		m_pIDataRecive->DoData(m_lpBuffer, 0);

	//		ReleaseMutex(m_hMutex);
	return TRUE;
	//}
	//	case WAIT_TIMEOUT:
	//	{
	//		OutputDebugStringA("share momery WAIT_TIMEOUT !\n");
	//	}break;
	//	case WAIT_FAILED:
	//	{
	//		OutputDebugStringA("share momery WAIT_FAILED !\n");
	//	}
	//	default:
	//		break;
	//	}
	//
	//	return FALSE;
}
BOOL CShareMemory::WriteShareMemory(LPVOID pData, size_t size)
{
	if (!m_lpBuffer)
		return FALSE;

	//DWORD dw = WaitForSingleObject(m_hMutex, 100);
	//switch (dw)
	//{
	//case WAIT_OBJECT_0:
	//{
	memcpy(m_lpBuffer, pData, size);
	//ReleaseMutex(m_hMutex);
	return TRUE;
	//}

	//case WAIT_TIMEOUT:
	//{
	//	OutputDebugStringA("share momery WAIT_TIMEOUT !\n");
	//}break;
	//case WAIT_FAILED:
	//{
	//	OutputDebugStringA("share momery WAIT_FAILED !\n");
	//}
	//default:
	//	break;
	//}
	//
	//return FALSE;
}
BOOL CShareMemory::WriteShareMemory()
{
	//DWORD dw = WaitForSingleObject(m_hMutex, 100);
	//switch (dw)
	//{
	//case WAIT_OBJECT_0:
	//{
	if (m_pIDataRecive)
		m_pIDataRecive->SendData(m_lpBuffer, MEMERY_DEFAULT_SIZE);
	//	ReleaseMutex(m_hMutex);
	return TRUE;
	//}
	//case WAIT_TIMEOUT:
	//{
	//	OutputDebugStringA("share momery WAIT_TIMEOUT !\n");
	//}
	//	break;
	//case WAIT_FAILED:
	//{
	//	OutputDebugStringA("share momery WAIT_FAILED !\n");
	//}
	//default:
	//	break;
	//}
	//return FALSE;
}
BOOL CShareMemory::InitMutex()
{
	//	SafeReleaseHandle(m_hMutex);
	//
	//	tstring mutexName = m_shareMemoryName + TEXT("_mutex");
	//	m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexName.c_str());
	//	if (!m_hMutex)
	//	{
	//		m_hMutex = CreateMutex(NULL, FALSE, mutexName.c_str());
	//	}
	//#ifdef _DEBUG 
	//	int err = GetLastError();
	//	TCHAR buf[128] = { 0 };
	//	_stprintf_s(buf, 128, TEXT("error %d  memory name is %s --\n"), err, mutexName.c_str());
	//	OutputDebugString(buf);
	//#endif


	//if (!m_hMutex)
	//	return FALSE;
	return TRUE;
}

BOOL CShareMemory::OpenShareFile(LPCTSTR shareName)
{
	SafeReleaseHandle(m_shareMemoryHandle);
	InitMutex();

	if (shareName)
	{
		m_shareMemoryName = shareName;
	}
	m_shareMemoryHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, m_shareMemoryName.c_str());

	if (!m_shareMemoryHandle)
	{
		return CreateShareFile(m_shareMemoryName.c_str());
	}

	m_lpBuffer = MapViewOfFile(m_shareMemoryHandle, FILE_MAP_ALL_ACCESS, 0, 0, MEMERY_DEFAULT_SIZE);

	if (!m_lpBuffer)
	{
		return FALSE;
	}
	return TRUE;
}

