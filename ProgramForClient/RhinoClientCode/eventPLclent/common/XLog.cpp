#include "XLog.h"

XLog::XLog() : mIsOpen(false)
{
}

XLog::~XLog()
{
	if (mFSream.is_open())
		mFSream.close();
};


void XLog::release()
{
}


void XLog::initialize(const char* _logFile)
{
	char file[1024] = { 0 };
	CreateDirectoryA("./log", NULL);

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	sprintf_s(file, "./log/%s_%04d%02d%02d.log", _logFile, sys.wYear, sys.wMonth, sys.wDay);

	if (mFSream.is_open())
		mFSream.close();
	
	mFSream.open(file, std::ios::out | std::ios::binary | std::ios::app);
	memset(mBuffer, 0, LOG_BUF_SIZE);
	mFSream << "\n===== Open log file " << getSysTimeBuffer() << " ===================================\n";
}


char* XLog::getSysTimeBuffer()
{
	static char buffer[64] = {0};

	SYSTEMTIME sys; 
	GetLocalTime( &sys);

	sprintf_s(buffer, "%4d-%02d-%02d %02d:%02d:%02d.%03d", 
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);

	return buffer;
};

void XLog::writeLog(const EnLogLevel& _level, const char* _format, ...)
{
	if (!mFSream.is_open())
	{
		return;
	}

//	if (E_LOG_DEBUG == _level)
//	{
//#ifndef _DEBUG
//		return;
//#endif // DEBUG
//	}

	static const char* const title[] = {"DEBUG", "NOTICE", "WARNING", "ERROR", "FATAL"};
	
	va_list va;
	va_start(va, _format);
	vsprintf_s(mBuffer, LOG_BUF_SIZE - 1, _format, va);
	
	// 写日志文件
	/*mFSream << "[" << _fileName.substr(_fileName.find_last_of('\\')+1).c_str() << ": " << _funcName << "() line:" << _line << "] " 
		<< "["<< title[_level] << " " << getSysTimeBuffer() << "] " << " ["<< GetCurrentThreadId() << "] "<< mBuffer << std::endl;*/
	
	mFSream << "[" << getSysTimeBuffer() << "] [" << title[_level] << "] [thread:" << GetCurrentThreadId() << "] "<< mBuffer << std::endl;
	mFSream.flush();
};