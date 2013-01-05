#include "Thread.h"
#include<process.h>
unsigned int _stdcall threadFunction(void *object)
{
	Thread *thread = (Thread*)object;
	return(unsigned int)thread->run(thread->param);
}

Thread::Thread()
{
	started = false;
	detached = false;
}

Thread::~Thread()
{
	stop();
}

int Thread::start( void*para)
{
	if(!started)
	{
		param = para;
		if(threadHandle = (HANDLE)_beginthreadex(NULL,0,threadFunction,this,0,&threadID))
		{
			if(detached)
			{
				CloseHandle(threadHandle);
			}
			started = true;
		}
	}
	return started;
}

void* Thread::wait()
{
	DWORD status = (DWORD)NULL;
	if(started&&!detached)
	{
		WaitForSingleObject(threadHandle,INFINITE);
		GetExitCodeThread(threadHandle,&status);
		CloseHandle(threadHandle);
		detached = true;

	}
	return (void*)status;
}

void Thread::detach()
{
	if(started&&!detached)
	{
		CloseHandle(threadHandle);
	}
	detached = true;
}

void Thread::stop()
{
	if(started&&!detached)
	{
		TerminateThread(threadHandle,0);
		CloseHandle(threadHandle);
		detached = true;
	}
}

void Thread::sleep(unsigned int delay)
{
	::Sleep(delay);
}



