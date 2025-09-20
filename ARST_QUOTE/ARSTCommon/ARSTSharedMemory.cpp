#include <cstdio>
#include <cstddef>

#include "ARSTSharedMemory.h"

ARSTSharedMemory::ARSTSharedMemory(key_t kKey, int nSize): m_pSharedMemory(NULL), m_nDetached(0)
{
	m_nShmid = shmget(kKey, nSize, 0666 | IPC_CREAT);

	if(m_nShmid == -1)
	{
		perror("Get shared memory error");
	}
}

ARSTSharedMemory::~ARSTSharedMemory() 
{
	if ( m_nDetached == 0) 
	{
		shmdt(m_pSharedMemory);	
	}
}

void ARSTSharedMemory::AttachSharedMemory()
{
	m_pSharedMemory = shmat(m_nShmid, NULL, 0);

	if(m_pSharedMemory == (void*)-1)
	{
		perror("Attach memory error");
	}
	else
	{
		m_nDetached = 0;
	}
}

int ARSTSharedMemory::DetachSharedMemory()
{
	int nResult = shmdt(m_pSharedMemory);

	if(nResult == -1)
	{
		perror("Detach memory error");
	}
	else
	{
		m_nDetached = 1;
	}
}

int ARSTSharedMemory::RemoveSharedMemory()
{
	int nResult = shmctl(m_nShmid, IPC_RMID, 0);

	if(nResult == -1)
	{
		perror("Remove memory error");
	}
}

void* ARSTSharedMemory::GetSharedMemory()
{
	return m_pSharedMemory;
}
