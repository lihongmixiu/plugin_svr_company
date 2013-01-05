#include"ThreadWrapper.h"
#include"SocketUtil.h"
#include<iostream>
using namespace std;

ThreadWrapper::ThreadWrapper()
{
	//no -op
}
ThreadWrapper::~ThreadWrapper()
{
	//no -op
}


void* ThreadWrapper::run(void *para)
{
	SocketUtil *socketUtil;
	socketUtil->ConnectToSvr();
	socketUtil->SendPkg();
	return (void*)NULL;
}
