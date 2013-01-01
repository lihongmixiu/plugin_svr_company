#include"log.h"
#include <string.h>
#include <time.h>

void Log::discard_cb(int severity, const char *msg)
{
	    /* This callback does nothing. */
}

void Log::writeLog(int logLevel,const char *msg,...)
{
	va_list ap;
	va_start(ap, msg);
	log_helper(logLevel, NULL, msg, ap);
	va_end(ap);
	
}

void Log::log_helper(int logLevel,const char *errstr,const char*fmt,va_list ap)
{
	char buf[1024];
	size_t len;
	if (fmt != NULL)
		evutil_vsnprintf(buf, sizeof(buf), fmt, ap);
	else
		buf[0] = '\0';
	if (errstr) {
		len = strlen(buf);
		if (len < sizeof(buf) - 3) {
			evutil_snprintf(buf + len, sizeof(buf) - len, ": %s", errstr);
		}
	}
	write_to_file_cb(logLevel,buf);
}
//static FILE *logfile = NULL;
 void Log::write_to_file_cb(int severity, const char *msg)
{
	    const char *s;
		if (!logfile)
		{
			printf("logfile NULL!\n");
			return;
		}
		switch (severity) {
			case _EVENT_LOG_DEBUG: s = "Debug"; 
								   break;									          
			case _EVENT_LOG_MSG:   s = "Msg";   
								   break;
			case _EVENT_LOG_WARN:  s = "Warn"; 
								   break;
			case _EVENT_LOG_ERR:   s = "Error"; 
								   break;
			 default:               s = "?";    
								   	break; /* never reached */
				}
		time_t rawtime;  
		struct tm * timeinfo;  
		time ( &rawtime );  
		timeinfo = localtime ( &rawtime );  
		int n = fprintf(logfile, "[%d-%d-%d %d:%d:%d][%s] %s\n",1900+timeinfo->tm_year,1+timeinfo->tm_mon,
				timeinfo->tm_mday, timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec, s, msg);
		fflush(logfile);		
}

/* Turn off all logging from Libevent. */
void Log::suppress_logging(void)
{
	    event_set_log_callback(discard_cb);
}

/* Redirect all Libevent log messages to the C stdio file 'f'. */
void Log::set_logfile(FILE *f)
{
	    logfile = f;
//	    event_set_log_callback(write_to_file_cb);
}
void Log::set_logfile_Name(const char*fileName)
{
	FILE *f = fopen(fileName,"a");
	printf("fileName:%s\n",fileName);
	set_logfile(f);
}

