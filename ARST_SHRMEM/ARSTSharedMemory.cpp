#include "ARSTSharedMemory.h"
#include <cstdio>

ARSTSharedMemory::ARSTSharedMemory(key_t key, size_t size)
    : m_shmid(-1), m_ptr(nullptr)
{
    m_shmid = shmget(key, size, SHM_MODE_READ_WRITE | IPC_CREAT);
    if (m_shmid == -1) {
        perror("shmget error");
        throw std::runtime_error("Failed to get shared memory");
    }
    Attach();
}

ARSTSharedMemory::~ARSTSharedMemory() {
    Detach();
}

void* ARSTSharedMemory::Get() const { return m_ptr; }

void ARSTSharedMemory::Attach() {
    if (!m_ptr) {
        m_ptr = shmat(m_shmid, nullptr, 0);
        if (m_ptr == (void*)-1) {
            m_ptr = nullptr;
            perror("shmat error");
            throw std::runtime_error("Failed to attach shared memory");
        }
    }
}

void ARSTSharedMemory::Detach() {
    if (m_ptr) {
        if (shmdt(m_ptr) == -1) {
            perror("shmdt error");
        }
        m_ptr = nullptr;
    }
}

void ARSTSharedMemory::Remove() {
    if (shmctl(m_shmid, IPC_RMID, nullptr) == -1) {
        perror("shmctl IPC_RMID error");
    }
}

