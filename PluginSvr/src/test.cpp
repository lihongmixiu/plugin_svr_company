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

#include "LogWrapper.h"
#include "Config.h"
#include "ConfigUtil.h"
#include "Proto.h"
#include "PkgUtil.h"
//log and config
int Initialize()
{
	int ret = 0;
	LOG_UTIL->OpenLog("/home/xxz/PluginSvr/log/server",2);
	CONFIG_UTIL->SetCfgFile("/home/xxz/PluginSvr/cfg/server.cfg");
	CONFIG_UTIL->ReadCfg();
	TRACESVR(GG_DEBUG,"svr ip:%s",CONFIG_UTIL->GetServerIp());
	TRACESVR(GG_DEBUG,"svr port:%d",CONFIG_UTIL->GetServerPort());
	return ret;
	
}
void Do_Read(struct bufferevent *bev,void *ctx)
{
	printf("readcb start\n");
	TRACESVR(GG_DEBUG,"do read call back begin!");
    struct evbuffer *input, *output;
	input = bufferevent_get_input(bev);
    size_t len = evbuffer_get_length(input);
	printf("len is:%d\n",len);	
	output = bufferevent_get_output(bev);
	char *inputBuff = (char*)malloc(MAX_SVR_PKG_LENGTH);
	if(inputBuff == NULL)
	{
	   TRACESVR(GG_ERROR,"input buff malloc fail!");
	   return;
	}
	if(evbuffer_remove(input,inputBuff,len)<0)
	{
	   TRACESVR(GG_ERROR,"evbuffer remove failed!");
	   return;
    }
    PkgUtil pkgUtil;
	int pkgType = 0;
	if(( pkgUtil.ProcessClientPkg(inputBuff,len,pkgType))<0)
	{
	   TRACESVR(GG_ERROR,"deal pkg fail,the value is:%d",pkgType);
	   return;
	}
	char* replyPkg =(char*)malloc(MAX_SVR_PKG_LENGTH);
	size_t replyPkgLen = 0;
	if(replyPkg == NULL)
	{
	   TRACESVR(GG_ERROR,"reply pkg malloc fail!");	
	   return;
	}
	memset(replyPkg,0,MAX_SVR_PKG_LENGTH);
	pkgUtil.BuildPkgByType(pkgType,replyPkg,replyPkgLen);
	evbuffer_add(output,(void*)replyPkg,replyPkgLen);/*send the reply pkg*/
	size_t outputLen = evbuffer_get_length(output);
    TRACESVR(GG_DEBUG,"reply and the pkg len is:%d",outputLen);	
	free(replyPkg);
	free(inputBuff);	   
}

void Do_Write(struct bufferevent *bev,void *ctx)
{
	TRACESVR(GG_DEBUG,"do write call back begin!");
}
/*the error happen call back func*/
void Do_Error(struct bufferevent *bev, short error, void *ctx)
{
	    if (error & BEV_EVENT_EOF) 
		{
			int sockfd = bufferevent_getfd(bev);
		    close(sockfd);
			TRACESVR(GG_DEBUG,"close sockfd:%d",sockfd);	
		} else if (error & BEV_EVENT_ERROR) 
		{
			TRACESVR(GG_ERROR,"BEV_EVENT_ERROR,error num:%d",error);			    
		} else if (error & BEV_EVENT_TIMEOUT) 
		{
			/* must be a timeout event handle, handle it */
            /*if time out,close directly in case blocking*/
			int sockfd = bufferevent_getfd(bev);
			close(sockfd);
			TRACESVR(GG_DEBUG,"timeout event,so close it,sockfd is:%d",sockfd);
		}
	    bufferevent_free(bev);/*free the bufferevent*/
}


void Do_Accept(evutil_socket_t listener, short event, void *arg)
{
	printf("%s\n","there is a client");
	TRACESVR(GG_DEBUG,"there is a client coming~");    
	struct event_base *base = (event_base*)arg;
	struct sockaddr_storage ss;
	socklen_t slen = sizeof(ss);
	int fd = accept(listener, (struct sockaddr*)&ss, &slen);
	if (fd < 0) 
	{
		perror("accept");
		TRACESVR(GG_ERROR,"accept client error!");
	} 
	else if (fd > FD_SETSIZE) 
	{
		close(fd);
		TRACESVR(GG_ERROR,"no more fd!");
	}
	else 
	{
		struct bufferevent *bev;
		evutil_make_socket_nonblocking(fd);
		bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	    bufferevent_setcb(bev,Do_Read, Do_Write, Do_Error, NULL);
	    bufferevent_setwatermark(bev, EV_READ,0,4096 );
	    bufferevent_setwatermark(bev, EV_WRITE,1, 10);
	    bufferevent_enable(bev, EV_READ|EV_WRITE);
	}
}


void  Run()
{
	TRACESVR(GG_DEBUG,"server start.");
	evutil_socket_t listener;/*evutil_socket_t equals int ,redefine by libevent */
	struct sockaddr_in sin;
	struct event_base *base;
	struct event *listener_event;
	base = event_base_new();
	if (!base)
	{
		TRACESVR(GG_ERROR,"event_base_new error!");
		return; 
	}
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(CONFIG_UTIL->GetServerIp()); 
	sin.sin_port = htons(CONFIG_UTIL->GetServerPort());
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
		 TRACESVR(GG_ERROR,"bind listener failed!");
		 return;	
	 }
	if (listen(listener, 16)<0) {
		TRACESVR(GG_ERROR,"listen failed!");
		return;
	}
	/*new the event */
	listener_event = event_new(base, listener, EV_READ|EV_PERSIST, Do_Accept, (void*)base);/*where to free*/
	/*add the event to the list*/
	event_add(listener_event, NULL);
	/*the event loop*/
	event_base_dispatch(base);/*how to control stop*/

}

void ignore_pipe_new()
{
  struct sigaction sig;

  sig.sa_handler = SIG_IGN;
  sig.sa_flags = 0;
  sigemptyset(&sig.sa_mask);
  sigaction(SIGPIPE,&sig,NULL);
}

void InitDaemon(void)
{
	pid_t pid;

	if ((pid = fork()) != 0)
		exit(0);

	setsid();

	signal(SIGINT,  SIG_IGN);
	signal(SIGHUP,  SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGHUP,  SIG_IGN);
	ignore_pipe_new();

	if ((pid = fork()) != 0)
		exit(0);

	umask(0);
}

int main(int argc,char **argv)
{
	bool daemon = true;//if is background run
	if(argc > 1)
	{
		for(int i = 0;i < argc;i++)
		{
			if(0 == strcasecmp(argv[i],"-d"))
			{
				daemon = false;
			}
		}
		if(0 == strcasecmp(argv[1],"-v"))
		{
			printf("******no version info now,heihei******");
			exit(0);
		}
	}
	if(daemon)
	{
		InitDaemon();
	}
	Initialize();
	Run();
	return 0;
}
