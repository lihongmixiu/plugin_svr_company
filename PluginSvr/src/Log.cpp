#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdarg.h>
#include "Log.h"
Log::Log()
{
	DisableLog();
	SetLogLevel(GG_NONE);
	m_LogPath[0]=0;
}

Log::~Log()
{
	//no -op
}

void Log::SetLogLevel(int logLevel)
{
	m_LogLevel = logLevel;
}

int Log::GetLogLevel()
{
	return m_LogLevel;
}

void Log::EnableLog()
{
	m_LogEnabled = true;
}

void Log::DisableLog()
{
	m_LogEnabled = false;
}

bool Log::IsLogEnabled()
{
	return m_LogEnabled;
}

int Log::OpenLog(const char *logPath,int logLevel)
{
	assert(logPath);
	SetLogLevel(logLevel);
	snprintf(m_LogPath,sizeof(m_LogPath),"%s",logPath);
	EnableLog();
    return 0;
}

int Log::WriteLog(int logLevel,const char *logInfo,...)
{
	int ret = 0;
	char logPath[256];
	char curDateTime[256];
	snprintf(logPath,sizeof(logPath),"%s.log",m_LogPath);
	//snprintf(logPath)
	if(IsLogEnabled() && logLevel >= GetLogLevel())
	{
		va_list ap;
		va_start(ap,logInfo);
		FILE *logFile = fopen(logPath,"a");
		GetCurDateTimeString(curDateTime);
		ret = fprintf(logFile,"[%s][%s]",curDateTime,GetLogLevelString(logLevel));
		vfprintf(logFile,logInfo,ap);
		fprintf(logFile,"\n");
		va_end(ap);
		ret = fflush(logFile);
			
	}
	return ret;
}

inline int Log::GetCurDateTimeString(char *dateTimeString)
{
	time_t timer = time(NULL);
	strftime(dateTimeString, 256, "%Y-%m-%d %H:%M:%S", localtime(&timer));
	return 0;
}
inline const char* Log::GetLogLevelString(int logLevel)
{
	if(logLevel<1 || logLevel>6)
	{
		return "ERROR LEVEL";
	}
	switch(logLevel)
	{
		case GG_TRACE:
			return "TRACE";
		case GG_DEBUG:
			return "DEBUG";
		case GG_WARN:
			return "WARN ";
		case GG_ERROR:
			return "ERROR";
		case GG_FATAL:
			return "FATAL";
		case GG_NONE:
			return "NONE ";
		default:
			return "ERROR LEVEL";
	}
	return "NULL";
}
