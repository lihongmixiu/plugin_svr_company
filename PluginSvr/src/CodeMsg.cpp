#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include"CodeMsg.h"

void  CodeMsg::HeadNtoh(tagCSMsgHead &msgHead)
{
	printf("net pkgLength = %d\n",msgHead.pkgLength);
	msgHead.pkgLength = ntohl(msgHead.pkgLength);
	printf("recv PkgLength = %d\n",msgHead.pkgLength);
	msgHead.cmdID = ntohl(msgHead.cmdID);
	msgHead.protoVersion = ntohl(msgHead.protoVersion);	
}

void  CodeMsg::HeadHton(tagCSMsgHead &msgHead)
{
	msgHead.pkgLength = htonl(msgHead.pkgLength);
	msgHead.cmdID = htonl(msgHead.cmdID);
	msgHead.protoVersion = htonl(msgHead.protoVersion);	
}
