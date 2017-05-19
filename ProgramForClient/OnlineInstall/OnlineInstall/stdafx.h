// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

/*Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "UIlib.h"
using namespace DuiLib;

using namespace std;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif

char char_info[];
int IsTextUTF8(const char* str, ULONGLONG length);
//Utf8ToGb2312
char* U2G(const char* utf8);
//GB2312到UTF-8的转换
char* G2U(const char* gb2312);

//std::string 转 UTF - 8：
char *  string_To_UTF8(const std::string & str);
//std::wstring 转 UTF - 8：
char * wstring_To_UTF8(const std::wstring & str);
//UTF - 8 转 std::string：
char* UTF8_To_string(const std::string & str);

//获取根路径
std::string GetAppPath();
// TODO: 在此处引用程序需要的其他头文件
