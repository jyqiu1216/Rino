#ifndef _WJ_commonfunction_H_
#define _WJ_commonfunction_H_

#include "WJBaseType.h"

WJSERVERMANAGELIB_API int IsTextUTF8(const char* str, ULONGLONG length);

//Utf8ToGb2312
WJSERVERMANAGELIB_API char* U2G(const char* utf8);

//GB2312到UTF-8的转换
WJSERVERMANAGELIB_API char* G2U(const char* gb2312);

//打印日志debugview
WJSERVERMANAGELIB_API void MyDebugString(char* lpPre, char* lpFormat, ...);

//获取当前路径
WJSERVERMANAGELIB_API std::string GetAppPath();
#endif//_WJ_commonfunction_H_