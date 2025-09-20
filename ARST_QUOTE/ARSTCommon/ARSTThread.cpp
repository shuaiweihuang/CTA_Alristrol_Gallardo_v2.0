#include "ARSTThread.h"

#include "../ARSTWebConns.h"
extern struct MNTRMSGS g_MNTRMSG;
pthread_mutex_t mtxThreadCount = PTHREAD_MUTEX_INITIALIZER;

static void *ThreadExecution(void* pArg)////////////
{
	return ((ARSTThread*)pArg)->Run();
}

ARSTThread::ARSTThread(): m_tid(0), m_nRunning(0), m_nDetached(0)
{
	pthread_mutex_lock(&mtxThreadCount);
	g_MNTRMSG.num_of_thread_Current++;
	g_MNTRMSG.num_of_thread_Max = (g_MNTRMSG.num_of_thread_Current > g_MNTRMSG.num_of_thread_Max) ? g_MNTRMSG.num_of_thread_Current : g_MNTRMSG.num_of_thread_Max;
	pthread_mutex_unlock(&mtxThreadCount);
}

ARSTThread::~ARSTThread()
{
	pthread_mutex_lock(&mtxThreadCount);
	g_MNTRMSG.num_of_thread_Current--;
	pthread_mutex_unlock(&mtxThreadCount);

	if ( m_nRunning == 1 && m_nDetached == 0)
	{
		pthread_detach(m_tid);
	}

	if ( m_nRunning == 1)
	{
		pthread_cancel(m_tid);
	}
}

int ARSTThread::Start()
{
	int result = pthread_create(&m_tid, NULL, ThreadExecution, this);

	if (result == 0)
	{
		m_nRunning = 1;
	}
	return result;
}

int ARSTThread::Join()
{
	int result = -1;

	if ( m_nRunning == 1) 
	{
		result = pthread_join(m_tid, NULL);

		if (result == 0) 
		{
			m_nDetached = 0;
		}
	}
	return result;
}

int ARSTThread::Detach()
{
	int result = -1;

	if (m_nRunning == 1 && m_nDetached == 0) 
	{
		result = pthread_detach(m_tid);

		if (result == 0) 
		{
			m_nDetached = 1;
		}
	}
	return result;
}

pthread_t ARSTThread::Self()//pthread_self();
{
	return m_tid;
}

bool ARSTThread::IsTerminated()
{
	return (m_nRunning==1)? true:false;
}
