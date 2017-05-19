#ifndef _HOOKMANAGER_H
#define _HOOKMANAGER_H

#include <Windows.h>

enum EnHookStatus
{
	E_HOOK_SUCCESS = 0,				/* 成功 */
	E_HOOK_DLL_NULLPTR,				/* 指定的DLL为空 */
	E_HOOK_DLL_NOT_EXIST,			/* DLL不存在 */
	E_HOOK_INJECT_FAILE,			/* 注入失败 */
	E_HOOK_SET_PRIVILEGE_FAILED,	/* 设置安全令牌失败 */
	E_HOOK_OPEN_PROCESS_FAILED,		/* 打开进程失败 */
	E_HOOK_CREATE_REMOTE_THREAD_FAILED,	/* 创建远程线程失败 */
	E_HOOK_WINDOW_NOT_EXIST,	/* 窗口不存在 */
};


class HookManager
{
public:
	/**
	* @date:	2016/08/20
	* @brief:	远程注入DLL
	* @param:	[_processId]:	进程ID
	* @return:	[_dllName]:		DLL名称
	* @usage:	
	*/
	static EnHookStatus injectDll(unsigned long _processId, TCHAR* _dllName);

	
	static EnHookStatus ganmetaskinjectDll(HWND _hWnd, const char* _path, const char* _dllName);

private:
	static BOOL setPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);

	static bool MyCreateRemoteThread(HANDLE _hProcess, LPTHREAD_START_ROUTINE _pThreadProc, LPVOID _pRemoteBuf);
};

#endif // !_HOOKMANAGER_H
