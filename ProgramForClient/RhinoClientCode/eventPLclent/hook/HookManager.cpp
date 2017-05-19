#include "HookManager.h"

BOOL HookManager::setPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	HANDLE hToken;
	LUID luid;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}

	if (!LookupPrivilegeValue(
		NULL,             // lookup privilege on local system
		lpszPrivilege,    // privilege to lookup 
		&luid))          // receives LUID of privilege
	{
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.
	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		//The token does not have the specified privilege.
		return FALSE;
	}

	return TRUE;
}



typedef DWORD(WINAPI *PFNTCREATETHREADEX)
(
	PHANDLE                 ThreadHandle,
	ACCESS_MASK             DesiredAccess,
	LPVOID                  ObjectAttributes,
	HANDLE                  ProcessHandle,
	LPTHREAD_START_ROUTINE  lpStartAddress,
	LPVOID                  lpParameter,
	BOOL	                CreateSuspended,
	DWORD                   dwStackSize,
	DWORD                   dw1,
	DWORD                   dw2,
	LPVOID                  Unknown
	);

#pragma warning(disable:4996)
bool HookManager::MyCreateRemoteThread(HANDLE _hProcess, LPTHREAD_START_ROUTINE _pThreadProc, LPVOID _pRemoteBuf)
{
	HANDLE      hThread = NULL;
	FARPROC     pFunc = NULL;
	BOOL bHook;

	// 判断系统版本
	OSVERSIONINFO osvi;
	//BOOL bIsWindowsXPorLater;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	if (osvi.dwMajorVersion == 6)
	{
		bHook = TRUE;
	}
	else
	{
		bHook = FALSE;
	}

	if (bHook)    // Vista, 7, Server2008
	{
		pFunc = GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "NtCreateThreadEx");
		if (pFunc == NULL)
		{
			//GetLastError());
			return FALSE;
		}


		((PFNTCREATETHREADEX)pFunc)(&hThread,
			0x1FFFFF,
			NULL,
			_hProcess,
			_pThreadProc,
			_pRemoteBuf,
			FALSE,
			NULL,
			NULL,
			NULL,
			NULL);

		if (hThread == NULL)
		{
			return FALSE;
		}
	}
	else                    // 2000, XP, Server2003
	{
		hThread = CreateRemoteThread(_hProcess, NULL, 0, _pThreadProc, _pRemoteBuf, 0, NULL);
		if (hThread == NULL)
		{
			return FALSE;
		}
	}

	if (WAIT_FAILED == WaitForSingleObject(hThread, INFINITE))
	{
		return FALSE;
	}

	return TRUE;
}


EnHookStatus HookManager::ganmetaskinjectDll(HWND _hWnd, const char* _path, const char* _dllName)
{
	// 获取进程ID和主线程ID.
	DWORD dwProcId = 0;
	DWORD dwThreadId = GetWindowThreadProcessId(_hWnd, &dwProcId);

	char pathName[MAX_PATH] = { 0 };					//存放路径的变量

	strcat(pathName, _path);
	strcat(pathName, "/");
	strcat(pathName, _dllName);

	TCHAR t_pathName[MAX_PATH];
#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, pathName, -1, t_pathName, 100);
#else
	strcpy(t_pathName, pathName);
#endif

	EnHookStatus res = HookManager::injectDll(dwProcId, t_pathName);
	return res;
}


EnHookStatus HookManager::injectDll(unsigned long _processId, TCHAR* _dllName)
{
#ifdef UNICODE
	if (wcslen(_dllName) <= 0)
#else
	if (strlen(_dllName) <= 0)
#endif
	{
		return E_HOOK_DLL_NULLPTR;
}

	////判断dll是否存在
	//HANDLE hFile = CreateFile(aFullpath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//if (hFile != INVALID_HANDLE_VALUE)
	//{
	//	DWORD dwsize = GetFileSize(hFile, NULL);
	//	CloseHandle(hFile);
	//	if (dwsize < 10)
	//	{
	//		return EHOOK_DLLFILE_LITTLE;
	//	}
	//}
	//else
	//{
	//	return EHOOK_DLLFILE_NOEXITE;
	//}
	// 

	if (!setPrivilege(SE_DEBUG_NAME, TRUE))
	{
		return E_HOOK_SET_PRIVILEGE_FAILED;
	}


	HANDLE hProcess = NULL;
	LPVOID pRemoteBuf = NULL;
	FARPROC pThreadProc = NULL;
#ifdef UNICODE
	DWORD dwBufSize = wcslen(_dllName) * sizeof(wchar_t) + 2;
#else
	DWORD dwBufSize = strlen(_dllName) * sizeof(char) + 2;
#endif
	//open the process to get the HANDLE
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _processId)))
	{
		return E_HOOK_OPEN_PROCESS_FAILED;
	}

	//1  allock memory for param data ,here is to save the dll path and name
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);

	//2  cp the dll pathName to remote buffer
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)_dllName, dwBufSize, NULL);

	//3  find the LoadLibraryW ProcAddress from the kernel32.dll module
	pThreadProc = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryW");

	//4 excute the LoadLibraryW with the 
	if (!MyCreateRemoteThread(hProcess, (LPTHREAD_START_ROUTINE)pThreadProc, pRemoteBuf))
	{
		return E_HOOK_CREATE_REMOTE_THREAD_FAILED;
	}

	VirtualFreeEx(hProcess, pRemoteBuf, dwBufSize, MEM_RELEASE);
	CloseHandle(hProcess);

	return E_HOOK_SUCCESS;
}