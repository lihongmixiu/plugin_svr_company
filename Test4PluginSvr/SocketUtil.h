#ifndef SOCKETUTIL_H_INCLUDED
#define SOCKETUTIL_H_INCLUDED

#include<winsock2.h>
class SocketUtil
{
	public:
		SocketUtil();
		~SocketUtil();
		int ConnectToSvr();
		int SendPkg();
};

#endif // SOCKETUTIL_H_INCLUDED
