#ifndef LOG_H
#define LOG_H
#include<event2/event.h>
#include<stdio.h>

static FILE *logfile = NULL;

class Log
{
//	private:
//		static FILE *logfile = NULL;
	public:
//		static FILE *logfile = NULL;

		static void discard_cb(int severity, const char *msg);
		static void writeLog(int severity,const char *msg,...);
		static void log_helper(int severity,const char*errstr,const char *msg,va_list ap);
		static void write_to_file_cb(int severity,const char *msg);
		static void suppress_logging(void);
		static void set_logfile(FILE *f);
		static void set_logfile_Name(const char*fileName);

};
#endif
