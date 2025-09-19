#pragma once
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdexcept>

class ARSTSharedMemory {
public:
    ARSTSharedMemory(const ARSTSharedMemory&) = delete;
    ARSTSharedMemory& operator=(const ARSTSharedMemory&) = delete;

    ARSTSharedMemory(key_t key, size_t size);
    ~ARSTSharedMemory();

    void* Get() const;
    void Attach();
    void Detach();
    void Remove();

private:
    static constexpr mode_t SHM_MODE_READ_WRITE =
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    int m_shmid;
    void* m_ptr;
};

