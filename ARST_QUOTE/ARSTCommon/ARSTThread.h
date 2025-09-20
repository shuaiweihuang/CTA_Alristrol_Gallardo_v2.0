#ifndef ARSTCOMMON_ARSTTHREAD_H_
#define ARSTCOMMON_ARSTTHREAD_H_

#include <pthread.h>

class ARSTThread 
{
	private:
		pthread_t  m_tid;
		int        m_nRunning;
		int        m_nDetached;

	public:
		ARSTThread();
		virtual ~ARSTThread();

		int Start();
		int Join();
		int Detach();
		pthread_t Self();

		bool IsTerminated();

		virtual void* Run() = 0;
};
#endif
