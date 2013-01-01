#ifndef _LOG_WRAPPER_
#define _LOG_WRAPPER_
#include "Log.h"
class LogWrapper
{
	public:
		static Log m_Log;
		//static Log GetLogUtil(){return m_Log;}
};
#define LOG_UTIL (&(LogWrapper::m_Log))
#define WRITELOG LOG_UTIL->WriteLog

#define TRACESVR(level,fmt,...)\
	do{\
		if(level >= LOG_UTIL->GetLogLevel())\
		{\
		WRITELOG(level,fmt,##__VA_ARGS__);\
		}\
}while(0)


#endif
