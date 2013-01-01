#include<stdlib.h>
#include <netinet/in.h>
/* For socket functions */
#include <sys/socket.h>
/* For fcntl */
#include <fcntl.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
//#include <log-internal.h>

#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#include "proto.h"
#include "ReadCfg.h"
#include "log.h"
#define MAX_LINE 16384

static unsigned int clientNum = 0; /*count for client num*/
void do_read(evutil_socket_t fd, short events, void *arg);
void do_write(evutil_socket_t fd, short events, void *arg);

char
rot13_char(char c)
{
	    /* We don't want to use isalpha here; setting the locale would change
		        * which characters are considered alphabetical. */
	    if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
			        return c + 13;
		    else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
				        return c - 13;
			    else
					        return c;
}


/*read event call back*/
void
readcb(struct bufferevent *bev, void *ctx)
{
		printf("readcb start\n");
	    struct evbuffer *input, *output;
	   	input = bufferevent_get_input(bev);
        size_t len;
	    len = evbuffer_get_length(input);
	    printf("len is:%d\n",len);	
	    output = bufferevent_get_output(bev);
		char *inputBuff = (char*)malloc(MAX_SVR_PKG_LENGTH);
		if(inputBuff == NULL)
		{
			Log::writeLog(3,"inputBuff malloc fail");
			return;
		}
	    if(evbuffer_remove(input,inputBuff,len)<0)
	    {
			Log::writeLog(3,"evbuffer_remove fail");
			return;
    	}
      	Proto p;
		int pkgType = 0;
		if(( p.dealPkg(inputBuff,len,pkgType))<0)
		{
			Log::writeLog(3,"dealPkg fail,the value is:%d\n",pkgType);
			return;
		}
		Log::writeLog(1,"receive pkg type is:%d",pkgType);
		char* replyPkg =(char*)malloc(MAX_SVR_PKG_LENGTH);
		size_t replyPkgLen = 0;
		if(replyPkg == NULL)
		{
			Log::writeLog(3,"replyPkg malloc fail");
			return;
		}
		memset(replyPkg,0,MAX_SVR_PKG_LENGTH);
		Log::writeLog(1,"begin build reply pkg");
		p.buildPkgByType(pkgType,replyPkg,replyPkgLen);
	    
		evbuffer_add(output,(void*)replyPkg,replyPkgLen);/*send the reply pkg*/
		size_t outputLength = evbuffer_get_length(output);
		Log::writeLog(1,"reply and the pkg len is:%d",replyPkgLen);		
	   free(replyPkg);
	   free(inputBuff);	   
}

/*write event call back ,this func is not make full use of...*/
void 
writecb(struct bufferevent *bev,void *ctx)
{
	printf("%s\n","writecb start");
	struct evbuffer *input ,*output;
	output = bufferevent_get_output(bev);
	input = bufferevent_get_input(bev);
	int inputLen = evbuffer_get_length(input);
	printf("input len:%d\n",inputLen);
	if(output==NULL)
	{
		printf("is NULL");
			return;
	}
	int len = evbuffer_get_length(output);
	printf("output len:%d\n",len);
	if(evbuffer_get_length(input)>0)
	{
		int sockfd = bufferevent_getfd(bev);
		const char* backToClient = "hello,client\n";
		send(sockfd,backToClient,13,0);
	}
}
/*the error happen call back func*/
void
errorcb(struct bufferevent *bev, short error, void *ctx)
{
	    if (error & BEV_EVENT_EOF) 
		{
			int sockfd = bufferevent_getfd(bev);
		    close(sockfd);
			char conCloseBuffer[80];
			sprintf(conCloseBuffer,"%s%d%s","fd:",sockfd," connection has been closed");	
			Log::write_to_file_cb(1,conCloseBuffer);			
		} else if (error & BEV_EVENT_ERROR) 
		{
			char errorBuffer[80];
			sprintf(errorBuffer,"%s%d","BEV_EVENT_ERROR,error num:",error);
			Log::write_to_file_cb(3,errorBuffer);
					    
		} else if (error & BEV_EVENT_TIMEOUT) 
		{
			/* must be a timeout event handle, handle it */
            /*if time out,close directly in case blocking*/
			Log::write_to_file_cb(3,"BEV_EVENT_TIMEOUT");
			int sockfd = bufferevent_getfd(bev);
			close(sockfd);
		}
	    bufferevent_free(bev);
}

void
do_accept(evutil_socket_t listener, short event, void *arg)
{
	    printf("%s\n","there is a client");
		Log::writeLog(1,"The number of %d client has connected to the svr",clientNum);
	    struct event_base *base = (event_base*)arg;
		struct sockaddr_storage ss;
		socklen_t slen = sizeof(ss);
		int fd = accept(listener, (struct sockaddr*)&ss, &slen);
		if (fd < 0) {
			perror("accept");
		} else if (fd > FD_SETSIZE) {
			close(fd);
		} else {
			     struct bufferevent *bev;
		   		 evutil_make_socket_nonblocking(fd);
			     bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	             bufferevent_setcb(bev, readcb, writecb, errorcb, NULL);
	             bufferevent_setwatermark(bev, EV_READ,0,4096 );
	             bufferevent_setwatermark(bev, EV_WRITE,1, 10);
	            bufferevent_enable(bev, EV_READ|EV_WRITE);
		}
}

void
run(void)
{
	printf("%s\n","begin run");
	Log::writeLog(1,"the server is running");
	evutil_socket_t listener;/*evutil_socket_t equals int ,redefine by libevent */
	struct sockaddr_in sin;
	struct event_base *base;
	struct event *listener_event;
	base = event_base_new();
	if (!base)
	 return; /*XXXerr*/
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("172.25.40.148"); 
	sin.sin_port = htons(6666);
	printf("the listen port is:%d\n",sin.sin_port);
	listener = socket(AF_INET, SOCK_STREAM, 0);
	evutil_make_socket_nonblocking(listener);

#ifndef WIN32
	{
		int one = 1;
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
	}
#endif
	 if (bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0) {										
		Log::writeLog(3,"bind port error");
		 perror("bind");
		 return;	
	 }
	if (listen(listener, 16)<0) {
		Log::writeLog(3,"server listen fail");
		perror("listen");
		return;
	}
	/*new the event */
	listener_event = event_new(base, listener, EV_READ|EV_PERSIST, do_accept, (void*)base);/*where to free*/
	/*add the event to the list*/
	event_add(listener_event, NULL);
	/*the event loop*/
	event_base_dispatch(base);/*how to control stop*/
}
/*
string getTimeString()
{
	
		time_t rawtime;
		struct tm*timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		
}
*/
int
main(int c, char **v)
{
	   unsigned int test =48;
	   printf("the size is:%d\n",sizeof(test));
	   // setvbuf(stdout, NULL, _IONBF, 0);
		ReadCfg::setVersion("version.cfg");  /*set version by cfg file*/
		ReadCfg::readXml("hyperLink_Cfg.xml");/*load xml for update hyperlink*/
		char szBuf[256] = {0};
		time_t timer = time(NULL);
		strftime(szBuf, sizeof(szBuf), "%Y%m%d.log", localtime(&timer));
		Log::set_logfile_Name(szBuf);        /*according to the date,record the log*/
		Log::writeLog(1,"server start succ!");
		run();
	    return 0;
}

