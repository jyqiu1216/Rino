#ifndef _WJ_commonfunction_H_
#define _WJ_commonfunction_H_

#include "common/WJBaseType.h"
WJCLIENTSOCKETLOGIC_EXPORT int IsTextUTF8(const char* str, ULONGLONG length);

//Utf8ToGb2312
WJCLIENTSOCKETLOGIC_EXPORT char* U2G(const char* utf8);

//GB2312到UTF-8的转换
WJCLIENTSOCKETLOGIC_EXPORT char* G2U(const char* gb2312);

//打印日志debugview
WJCLIENTSOCKETLOGIC_EXPORT void MyDebugString(char* lpPre, char* lpFormat, ...);

//获取当前路径
WJCLIENTSOCKETLOGIC_EXPORT std::string GetAppPath();
#endif//_WJ_commonfunction_H_