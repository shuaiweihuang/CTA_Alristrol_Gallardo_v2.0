#ifndef ARSTCOMMON_ARSTSHAREDMEMORY_H_
#define ARSTCOMMON_ARSTSHAREDMEMORY_H_

#include <sys/shm.h>

class ARSTSharedMemory
{
	private:
		void* 	m_pSharedMemory;
		int 	m_nShmid;
		int 	m_nDetached;

	public:
		ARSTSharedMemory(key_t kKey, int nSize);
		virtual ~ARSTSharedMemory();

		//int GetSharedMemory();
		void AttachSharedMemory();
		int DetachSharedMemory();
		int RemoveSharedMemory();

		void* GetSharedMemory();
};
#endif
