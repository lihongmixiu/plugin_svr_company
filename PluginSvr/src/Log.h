#ifndef _LOG_H_
#define _LOG_H_
enum logLevel{
	GG_TRACE=1,
	GG_DEBUG=2,
	GG_WARN=3,
	GG_ERROR=4,
	GG_FATAL=5,
	GG_NONE,
};
class Log
{
	public:
		Log();
		~Log();
		void SetLogLevel(int logLevel);
		int GetLogLevel();
		void EnableLog();
		void DisableLog();
		bool IsLogEnabled();
		int OpenLog(const char *logPath,int logLevel);
		int WriteLog(int logLevel,const char *logInfo,...);
		int GetCurDateTimeString(char *dateTimeString);
		const char* GetLogLevelString(int logLevel);
	private:
		bool m_LogEnabled;
		int m_LogLevel;
		char m_LogPath[256];
	
		
};



#endif
