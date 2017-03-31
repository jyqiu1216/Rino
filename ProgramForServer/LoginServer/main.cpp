#include "loginserver.h"
#include "common/QkLogger.h"
#include <DbgHelp.h>
#include "ErrorModule.hpp"

#pragma comment(lib, "ws2_32.lib")

#ifdef _DEBUG
#pragma comment(lib, "hiredis_d.lib")
#pragma comment(lib, "WJServerManagelib_d.lib")
#pragma comment(lib, "json_d.lib")
#pragma comment(lib, "libglog_static.lib")
#else
#pragma comment(lib, "hiredis_r.lib")
#pragma comment(lib, "WJServerManagelib_r.lib")
#pragma comment(lib, "json_r.lib")
#endif

LONG WINAPI bad_exception(struct _EXCEPTION_POINTERS* ExceptionInfo)
{

	wchar_t buffer[64] = { 0 };
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	swprintf_s(buffer, L"ls_%4d-%02d-%02d_%02d-%02d-%02d",
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);

	std::wstring str = buffer;
	str += L".dmp";
	//创建 Dump 文件
	HANDLE hDumpFile = CreateFile(str.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDumpFile != INVALID_HANDLE_VALUE)
	{
		//Dump信息
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ExceptionPointers = ExceptionInfo;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ClientPointers = TRUE;
		//写入Dump文件内容
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
	}

	wchar_t wchBuffer[1024] = { 0 };
	wchar_t wchModuleName[512] = { 0 };
	AddrGetModule((DWORD)ExceptionInfo->ExceptionRecord->ExceptionAddress, wchModuleName, ARRAYSIZE(wchModuleName));

	::swprintf_s(wchBuffer, ARRAYSIZE(wchBuffer), L"异常模块:\n%ws\nCRASH CODE:0x%.8X\nADDR=0x%.8X\nFLAGS=0x%.8X\nPARAMS=0x%.8X\n",
		wchModuleName,
		ExceptionInfo->ExceptionRecord->ExceptionCode,
		(DWORD)ExceptionInfo->ExceptionRecord->ExceptionAddress,
		ExceptionInfo->ExceptionRecord->ExceptionFlags,
		ExceptionInfo->ExceptionRecord->NumberParameters);

	::MessageBox(NULL, wchBuffer, L"崩溃了", MB_OK);
	return true;
}

void initLoggingSystem(int argc, char* argv[])
{
	(void)argc;
	std::string strGroup("LOG");
	std::string strMinLevel = "debug";

	// 设置最小的日志等级
	int iMinLogLevel = google::GLOG_INFO; //默认值
	if (strMinLevel == "warning")
	{
		iMinLogLevel = google::GLOG_WARNING;
	}
	else if (strMinLevel == "error")
	{
		iMinLogLevel = google::GLOG_ERROR;
	}
	else if (strMinLevel == "fatal")
	{
		iMinLogLevel = google::GLOG_FATAL;
	}

	if (strMinLevel == "debug")
	{
		FLAGS_v = 1;
	}
	else
	{
		FLAGS_v = 0;
	}

	std::string strLoggingPath = ".\\lslog\\";
	//std::string strLoggingPath = "D:\\lslog\\";

	google::InitGoogleLogging("LoginServer");
	google::SetLogDestination(google::GLOG_INFO, strLoggingPath.c_str());

	FLAGS_log_dir = strLoggingPath;
	FLAGS_max_log_size = 10; //最大log文件的大小，10M字节
	FLAGS_minloglevel = google::GLOG_INFO;
	FLAGS_logbuflevel = google::GLOG_INFO; //只有buf info的，其他都是sync.
	FLAGS_logbufsecs = 5;
	FLAGS_logtostderr = false;

	if (0)
	{	//是否开启控制台日志
		FLAGS_alsologtostderr = true;
		FLAGS_colorlogtostderr = true;
		FLAGS_stderrthreshold = google::GLOG_INFO;
	}
	else
	{
		//所有的日志都不出现在控制台
		FLAGS_stderrthreshold = google::NUM_SEVERITIES;
	}

}

int main(int argc, char *argv[])
{
	::SetUnhandledExceptionFilter(::bad_exception);

	initLoggingSystem(__argc, __argv);
	QK_InfoLog("hello Loginserver");
	LoginServer w;
	w.StatServer();
}
