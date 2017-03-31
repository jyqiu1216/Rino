/*
 * PA_Logger.h
 *
 *  Created on: 2012-11-1
 *      Author: Administrator
 */

#ifndef PA_LOGGER_H__
#define PA_LOGGER_H__


#define GLOG_NO_ABBREVIATED_SEVERITIES


#ifdef WIN32
#include "src/utilities.h"
#else
#include "glog_linux/src/utilities.h"
#endif

#include "glog/logging.h"
#include "glog/raw_logging.h"


const int kMaxLoggingBufferSize = 4096;

//为了跨平台和避免在此引入过多的头文件，此函数只在此使用。


#ifdef WIN32

inline int vc_vsnprintf(char* buffer, size_t size, const char* format, va_list arguments)
{
	int length = _vsprintf_p(buffer, size, format, arguments);
	if (length < 0)
	{
		if (size > 0)
			buffer[0] = 0;
		return _vscprintf_p(format, arguments);
	}
	return length;
}


inline int logger_snprintf(char* buffer, size_t size, const char* format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	int result = vc_vsnprintf(buffer, size, format, arguments);
	va_end(arguments);
	return result;
};
#else
#define logger_snprintf  snprintf
#endif


//void initLoggingSystem(int argc, char* argv[]);

// 调试日志
#define QK_DebugLog(fmt,...)\
if (FLAGS_minloglevel <= google::GLOG_INFO) \
{\
if (FLAGS_v == 1)\
{\
	char logger_buffer[kMaxLoggingBufferSize]; \
	logger_snprintf(logger_buffer, sizeof(logger_buffer), fmt, ##__VA_ARGS__); \
	logger_buffer[kMaxLoggingBufferSize - 1] = 0; \
	VLOG(1) << logger_buffer << "\n"; \
}\
};

// 一般的日志
#define QK_InfoLog(fmt,...)\
if (FLAGS_minloglevel <= google::GLOG_INFO) \
{\
	char logger_buffer[kMaxLoggingBufferSize]; \
	logger_snprintf(logger_buffer, sizeof(logger_buffer), fmt, ##__VA_ARGS__); \
	logger_buffer[kMaxLoggingBufferSize - 1] = 0; \
	LOG(INFO) << logger_buffer << "\n"; \
};

// 一般来说,不是代码bug的错误,都归为warn.比如解析协议中,发现接收到的格式不符合要求等
#define QK_WarnLog(fmt,...)\
if(FLAGS_minloglevel <= google::GLOG_WARNING) \
{\
	char logger_buffer[kMaxLoggingBufferSize]; \
	logger_snprintf(logger_buffer, sizeof(logger_buffer), fmt, ##__VA_ARGS__); \
	logger_buffer[kMaxLoggingBufferSize-1] = 0; \
	LOG(WARNING) << logger_buffer << "\n"; \
};

#define QK_ErrorLog(fmt,...)\
if(FLAGS_minloglevel <= google::GLOG_ERROR) \
{\
	char logger_buffer[kMaxLoggingBufferSize]; \
	logger_snprintf(logger_buffer, sizeof(logger_buffer), fmt, ##__VA_ARGS__); \
	logger_buffer[kMaxLoggingBufferSize-1] = 0; \
	LOG(ERROR) << logger_buffer << "\n"; \
};

// 说明代码中存在bug
// 错误日志,可以把当时的异常栈打印出来
#define QK_Assert(fmt,...)\
if(FLAGS_minloglevel <= google::GLOG_FATAL) \
{\
	char logger_buffer[kMaxLoggingBufferSize]; \
	logger_snprintf(logger_buffer, sizeof(logger_buffer), fmt, ##__VA_ARGS__); \
	logger_buffer[kMaxLoggingBufferSize-1] = 0; \
	ACE_Stack_Trace stStackTrace; \
	LOG(FATAL) << logger_buffer << "\n" << stStackTrace.c_str()<< "\n"; \
};


#endif /* PA_LOGGER_H_ */
