// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>
#include <shlwapi.h>
#include <process.h>
#include <AtlBase.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//DUILIB
#include "UIlib.h"
using namespace DuiLib;

#ifdef _DEBUG
#pragma comment(lib, "DuiLib_d.lib")
#else
#pragma comment(lib, "DuiLib.lib")
#endif

//boost
#include <boost/filesystem.hpp>

//json
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/stringbuffer.h>

using namespace rapidjson;

//curl
#include <curl/curl.h>

#ifdef _DEBUG
#pragma comment(lib, "libcurld.lib")
#else
#pragma comment(lib, "libcurl.lib")
#endif

//zip
DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
typedef DWORD ZRESULT;

typedef struct {

	int index;                 // index of this file within the zip
	char name[MAX_PATH];       // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime, ctime, mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;

typedef struct {

	int index;                 // index of this file within the zip
	TCHAR name[MAX_PATH];      // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime, ctime, mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRYW;

#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
extern HZIP OpenZipU(void *z, unsigned int len, DWORD flags);
extern ZRESULT CloseZipU(HZIP hz);

#ifdef _UNICODE
#define ZIPENTRY ZIPENTRYW
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif

extern ZRESULT GetZipItemA(HZIP hz, int index, ZIPENTRY *ze);
extern ZRESULT GetZipItemW(HZIP hz, int index, ZIPENTRYW *ze);
extern ZRESULT FindZipItemA(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);
extern ZRESULT FindZipItemW(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRYW *ze);
extern ZRESULT UnzipItem(HZIP hz, int index, void *dst, unsigned int len, DWORD flags);

#define ZIP_OK		 0
#define ZIP_HANDLE   1
#define ZIP_FILENAME 2
#define ZIP_MEMORY   3

// TODO:  在此处引用程序需要的其他头文件
//下载地址
#define ID_GETHTTPADDR																				"http://api.xiniugame.com/api/fight/version"
//安装成功上报
#define ID_SETUPCOMPLETEADDR																		"http://api.xiniugame.com/count/installrecord?source=%d&serialnum=%s"
//安装目录名称
#define ID_INSTALLDIR																				TEXT("RhinoClient")
//互斥
extern HANDLE																						g_hSetupMutex;
//安装路径
extern wchar_t																						g_wchInstallPath[1024];
//下载ZIP路径
extern wchar_t																						g_wchDownloadPath[1024];
//主窗口指针
extern CPaintManagerUI*																				g_pMainWndManager;
//静默安装
extern bool																							g_bNoWindow;