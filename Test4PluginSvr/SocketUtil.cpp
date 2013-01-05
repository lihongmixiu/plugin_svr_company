#include"SocketUtil.h"
#include<iostream>
using namespace std;

//#pragma comment(lib,"ws2_32.lib")
SocketUtil::SocketUtil()
{
	//no -op
}

SocketUtil::~SocketUtil()
{
	//no -op
}

int SocketUtil::ConnectToSvr()
{
	WSADATA wsaData;
	SOCKET s;
	SOCKADDR_IN serverAddr;
	int port = 6666;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.S_un.S_addr = inet_addr("172.25.40.148");
	int result = connect(s,(SOCKADDR*)&serverAddr,sizeof(serverAddr));
	if(0 == result)
	{
		cout<<"connect to svr succ!"<<endl;
	}
	return result;
}

int SocketUtil::SendPkg()
{
	return 0;
}
