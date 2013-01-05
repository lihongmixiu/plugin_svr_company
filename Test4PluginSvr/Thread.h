#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED
#include<windows.h>
static unsigned int _stdcall threadFunction(void*);
class Thread
{
	friend unsigned int _stdcall threadFunction(void*);
	public:
		Thread();
		virtual ~Thread();
		int start(void *para=NULL);
		void *wait();
		void stop();
		void detach();
		static void sleep(unsigned int );
    protected:
		virtual void*run(void*)=0;
	private:
		HANDLE threadHandle;
		bool started;
		bool detached;
		void *param;
		unsigned int threadID;

};


#endif // THREAD_H_INCLUDED
