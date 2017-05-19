#ifndef _WJ_commonfunction_H_
#define _WJ_commonfunction_H_

#include <string>
#include <xstring>
#include "common/WJBaseType.h"
int IsTextUTF8(const char* str, ULONGLONG length);

//Utf8ToGb2312
std::string U2G(const char* utf8);

//GB2312到UTF-8的转换
std::string G2U(const char* gb2312);

//打印日志debugview
void MyDebugString(char* lpPre, char* lpFormat, ...);

//获取当前路径
std::string GetAppPath();

//wstring高字节不为0，返回FALSE
bool WStringToString(const std::wstring &wstr, std::string &str);
bool StringToWString(const std::string &str, std::wstring &wstr);
#endif//_WJ_commonfunction_H_