#include "Log.h"
#include <io.h>
#include <iostream>
#include <qstring.h>
#include "common/WJCommonMarco.h"
#include "common/WJEncrypt.h"
#include "common/WJSpinlock.h"
using namespace std;

// 静态单实例指针
CSystemLog* g_mspInstance = nullptr;

WJ::CSpinlock g_logLock;

	// 文件加密  
void encrypt(std::string &str)
{
	std::string str2;
	char a;
	int i = 0, s, t;

	while (i < str.length())
	{
		s = str.at(i);
		t = genFunEx(ENCRYPT_KEY) ^ s;  // 加密  // 解密 (一模一样重新再做一次就是解密。。。)^运算可以用于简单的加密，比如原始值int a = 19880516;密钥 int key =1314520; 进行加密 int data=key^a = 20665500;解密 data^key == a；
		a = t;
		str2 += a;

		++i;
	}

	str = str2;
}

int CSystemLog::IsTextUTF8(const char* str, ULONGLONG length)
{
	int i;
	DWORD nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
	UCHAR chr;
	BOOL bAllAscii = TRUE; //如果全部都是ASCII, 说明不是UTF-8
	for (i = 0; i < length; i++)
	{
		chr = *(str + i);
		if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
			bAllAscii = FALSE;
		if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
		{
			if (chr >= 0x80)
			{
				if (chr >= 0xFC && chr <= 0xFD)
					nBytes = 6;
				else if (chr >= 0xF8)
					nBytes = 5;
				else if (chr >= 0xF0)
					nBytes = 4;
				else if (chr >= 0xE0)
					nBytes = 3;
				else if (chr >= 0xC0)
					nBytes = 2;
				else
				{
					return FALSE;
				}
				nBytes--;
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if ((chr & 0xC0) != 0x80)
			{
				return FALSE;
			}
			nBytes--;
		}
	}
	if (nBytes > 0) //违返规则
	{
		return FALSE;
	}
	if (bAllAscii) //如果全部都是ASCII, 说明不是UTF-8
	{
		return FALSE;
	}
	return TRUE;
}

//Utf8ToGb2312
char* CSystemLog::U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

//GB2312到UTF-8的转换
char* CSystemLog::G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

CSystemLog::CSystemLog()
{

}

CSystemLog::~CSystemLog()
{
	g_logLock.Lock();
	if (mFSream.is_open())
		mFSream.close();
	g_logLock.UnLock();
};

CSystemLog* CSystemLog::getInstance()
{
	if (!g_mspInstance)
		g_mspInstance = new CSystemLog();
	return g_mspInstance;
}

void CSystemLog::release()
{
	WJ_SAFE_DELETE(g_mspInstance);
}

void CSystemLog::initialize()
{
	g_logLock.Lock();
	//先进入log所在目录路径
	char path[MAX_PATH] = { 0 };
	memset(path, 0, sizeof(path));
	HMODULE  handle = GetModuleHandle(NULL);
	GetModuleFileNameA(handle, path, MAX_PATH);
	char* pwchExeName = strrchr(path, '\\');
	::ZeroMemory(pwchExeName + 1, ::strlen(pwchExeName));
	strcat_s(path, "Log");
	if (_access(path, 0))
		CreateDirectoryA(path, NULL);

	//再创建并打开以当天日期为名称的日志文件
	char buffer[64] = { 0 };
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	sprintf_s(buffer, "%4d-%02d-%02d", sys.wYear, sys.wMonth, sys.wDay);
	strcat_s(buffer, ".log");
	strcat_s(path, "\\");
	strcat_s(path, buffer);

	
	if (mFSream.is_open())
		mFSream.close();

	mFSream.open(path, std::ios::out | std::ios::app);
	memset(mBuffer, 0, BUFFERSIZE);
	g_logLock.UnLock();
}

char* CSystemLog::getSysTimeBuffer()
{
	static char buffer[64] = {0};
	SYSTEMTIME sys;
	GetLocalTime(&sys);

	sprintf_s(buffer, "%4d-%02d-%02d %02d:%02d:%02d",
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	return buffer;
};

void CSystemLog::LogToFile(char* format, ...)
{
	try
	{
		g_logLock.Lock();
		va_list va;
		va_start(va, format);
		vsprintf_s(mBuffer, BUFFERSIZE, format, va);

		std::string str;
		if (IsTextUTF8(mBuffer, sizeof(mBuffer)))
		{
			char* p = U2G(mBuffer);
			if (nullptr != p)
			{
				str = p;
				delete p;
			}
		}
		else
		{
			str = mBuffer;
		}
		
		std::string sss;
		sss = "[";
		sss += getSysTimeBuffer();
		sss += "][1]" + str + "\r\n";

		encrypt(sss);
		mFSream.write(sss.c_str(), sss.length());
		mFSream.flush();

		OutputDebugStringA(str.c_str());
		g_logLock.UnLock();
	}
	catch (...)
	{

	}
};

void CSystemLog::LogToFile(const char* buf)
{
	try
	{
		g_logLock.Lock();
		std::string str;
		if (IsTextUTF8(buf, strlen(buf)))
		{
			char* p = U2G(buf);
			if (nullptr != p)
			{
				str = p;
				delete p;
			}
		}
		else
		{
			str = buf;
		}

		std::string sss;
		sss = "[";
		sss += getSysTimeBuffer();
		sss += "][2]" + str + "\r\n";

		encrypt(sss);

		mFSream.write(sss.c_str(), sss.length());
		mFSream.flush();

		OutputDebugStringA(str.c_str());
		g_logLock.UnLock();
	}
	catch (...)
	{

	}
}

void CSystemLog::LogToFile(std::string str)
{
	try
	{
		g_logLock.Lock();
		std::string str2;
		if (IsTextUTF8(str.c_str(), str.length()))
		{
			char* p = U2G(str.c_str());
			if (nullptr != p)
			{
				str2 = p;
				delete p;
			}
		}

		std::string sss;
		sss = "[";
		sss += getSysTimeBuffer();
		sss += "][3]" + str2 + "\r\n";

		encrypt(sss);
		mFSream.write(sss.c_str(), sss.length());
		mFSream.flush();

		OutputDebugStringA(str2.c_str());
		g_logLock.UnLock();
	}
	catch (...)
	{

	}
}

void CSystemLog::LogToFile(QString str)
{
	std::string str2 = str.toStdString();
	LogToFile(str2);
}

void CSystemLog::LogToDebug(char* lpPre, char* format, ...)
{
	try
	{
		g_logLock.Lock();
		int ilen = strlen(format) + 1;
		//char* strMsg = new char[ilen];
		char strMsg[1024] = { 0 };
		std::memset(strMsg, 0, sizeof(strMsg));
		va_list pArg;
		va_start(pArg, format);

		//vsprintf_s(strMsg, ilen, lpFormat, pArg);
		vsprintf_s(strMsg, sizeof(strMsg), format, pArg);
		va_end(pArg);

		std::string str;
		str = lpPre;
		str += "------";
		str += strMsg;
		str += "\n";
		OutputDebugStringA(str.c_str());
		printf("%s", str.c_str());
		//delete[] strMsg;
		g_logLock.UnLock();
	}
	catch (...)
	{
		//
	}
}