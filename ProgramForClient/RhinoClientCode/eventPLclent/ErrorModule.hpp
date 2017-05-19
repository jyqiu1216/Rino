#pragma once
#ifndef ERRORMODULE_H
#define ERRORMODULE_H

typedef enum _MEMORY_INFORMATION_CLASS {

	MemoryBasicInformation
#if DEVL
	,MemoryWorkingSetInformation
#endif
	,MemoryMappedFilenameInformation
	,MemoryRegionInformation
	,MemoryWorkingSetExInformation

} MEMORY_INFORMATION_CLASS;

typedef NTSTATUS (NTAPI* Ole_ZwQueryVirtualMemory)(	__in HANDLE ProcessHandle,
	__in PVOID BaseAddress,
	__in MEMORY_INFORMATION_CLASS MemoryInformationClass,
	__out_bcount(MemoryInformationLength) PVOID MemoryInformation,
	__in SIZE_T MemoryInformationLength,
	__out_opt PSIZE_T ReturnLength);

void DisplayVolumePaths( __in PWCHAR VolumeName, LPCTSTR lptDosPath, int inLen )
{
	if ( (NULL == VolumeName) || (NULL == lptDosPath) || (inLen <= 0) ) {

		return ;
	}

	DWORD  CharCount = MAX_PATH + 1;
	PWCHAR Names     = NULL;
	PWCHAR NameIdx   = NULL;
	BOOL   Success   = FALSE;

	for ( ;; ) {

		Names = (PWCHAR) new BYTE [CharCount * sizeof(WCHAR)];

		Success = ::GetVolumePathNamesForVolumeNameW(VolumeName, Names, CharCount, &CharCount);

		if ( Success ) {

			break;
		}

		if ( GetLastError() != ERROR_MORE_DATA ) {

			break;
		}

		delete [] Names;
		Names = NULL;
	}

	if ( Success ) {

		for ( NameIdx = Names; NameIdx[0] != L'\0'; NameIdx += wcslen(NameIdx) + 1 ) {

			::wcscpy_s((wchar_t*)lptDosPath, inLen, NameIdx);
		}
	}

	if ( Names != NULL ) {

		delete [] Names;
		Names = NULL;
	}

	return;
}

bool NtPathToDosPath(LPCTSTR lptVolumePath, LPCTSTR lptDosPath, int inLen, int& inDeviceNameLen)
{
	bool bRet = false;

	if ((NULL == lptVolumePath) || (NULL == lptDosPath) || (inLen <= 0)) {

		return bRet;
	}

	DWORD  CharCount            = 0;
	wchar_t WchVolumeName[512]	= { 0 };
	WCHAR  DeviceName[MAX_PATH] = L"";
	wchar_t wchDosPath[128]		= { 0 };
	wchar_t WchDeviceName[512]	= { 0 };

	HANDLE FindHandle = ::FindFirstVolume(WchVolumeName, ARRAYSIZE(WchVolumeName));

	if (INVALID_HANDLE_VALUE == FindHandle) {

		return bRet;
	}

	for ( ;; ) {

		int Index = ::wcslen(WchVolumeName) - 1;

		if (WchVolumeName[0]     != L'\\' ||
			WchVolumeName[1]     != L'\\' ||
			WchVolumeName[2]     != L'?'  ||
			WchVolumeName[3]     != L'\\' ||
			WchVolumeName[Index] != L'\\') {

				break;
		}

		WchVolumeName[Index] = L'\0';

		CharCount = ::QueryDosDeviceW(&WchVolumeName[4], DeviceName, ARRAYSIZE(DeviceName)); 
		::wcscpy_s(WchDeviceName, ARRAYSIZE(WchDeviceName), DeviceName);

		WchVolumeName[Index] = L'\\';

		if ( CharCount == 0 ) {

			break;
		}

		DisplayVolumePaths(WchVolumeName, wchDosPath, ARRAYSIZE(wchDosPath));

		if ( ::wcsncmp(lptVolumePath, WchDeviceName, (inDeviceNameLen = ::wcslen(WchDeviceName))) == 0 ) {

			::wcscpy_s((wchar_t*)lptDosPath, inLen, wchDosPath);
			bRet = true;
			break;
		}

		BOOL Success = ::FindNextVolumeW(FindHandle, WchVolumeName, ARRAYSIZE(WchVolumeName));

		if ( !Success ) {

			break;
		}
	}

	::FindVolumeClose(FindHandle);

	return bRet;
}

//这个很重要,根据异常地址找到错误是来自哪个模块;
bool AddrGetModule(DWORD dwAddr, LPCTSTR lptModuleName, int inLen)
{
	bool bRet = false;

	if ( (NULL == dwAddr) || (dwAddr >= 0x80000000) || (NULL == lptModuleName) || (inLen <= 0) ) {

		return bRet;
	}

	Ole_ZwQueryVirtualMemory ZwQueryVirtualMemory = (Ole_ZwQueryVirtualMemory)::GetProcAddress(::GetModuleHandle(TEXT("ntdll.dll")), "ZwQueryVirtualMemory");

	if ( NULL == ZwQueryVirtualMemory ) {

		return bRet;
	}

	MEMORY_BASIC_INFORMATION mbi;
	if ( ZwQueryVirtualMemory(::GetCurrentProcess(), (PVOID)dwAddr, MemoryBasicInformation, &mbi, sizeof(mbi), NULL) > 0 ) {

		return bRet;
	}

	if ( MEM_IMAGE != mbi.Type ) {

		return bRet;
	}

	wchar_t wchModuleName[512] = { 0 };
	if ( ZwQueryVirtualMemory(::GetCurrentProcess(), (PVOID)dwAddr, MemoryRegionInformation, wchModuleName, sizeof(wchModuleName), NULL) > 0 ) {

		return bRet;
	}

	wchar_t* pWchVolumePath = ::wcsstr(wchModuleName, TEXT("\\"));

	if ( NULL == pWchVolumePath ) {

		return bRet;
	}

	wchar_t wchDosPath[512] = { 0 };
	int inDeviceLen = 0;
	bRet = NtPathToDosPath(pWchVolumePath, wchDosPath, ARRAYSIZE(wchDosPath), inDeviceLen);

	if ( bRet ) {

		::swprintf_s((wchar_t*)lptModuleName, inLen, TEXT("%ws%ws"), wchDosPath, &pWchVolumePath[inDeviceLen]);
	}

	return bRet;
}


#endif