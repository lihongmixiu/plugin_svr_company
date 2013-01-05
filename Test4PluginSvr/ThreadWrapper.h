#ifndef THREADWRAPPER_H_INCLUDED
#define THREADWRAPPER_H_INCLUDED

#include "thread.h"
class ThreadWrapper:public Thread
{
	public:
		ThreadWrapper();
		~ThreadWrapper();
		virtual void* run(void*);
};


#endif // THREADWRAPPER_H_INCLUDED
