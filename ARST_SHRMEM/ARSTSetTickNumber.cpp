#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <cassert>
#include <stdexcept>
#include "ARSTSharedMemory.h"

#define SERIAL_NUM_KEY 13

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <key> <13-digit serial>\n", argv[0]);
        return 1;
    }

    ARSTSharedMemory sharedMemory(static_cast<key_t>(std::atoi(argv[1])), sizeof(long));
    sharedMemory.Attach();

    long* pSerial = static_cast<long*>(sharedMemory.Get());

    if (std::strlen(argv[2]) != SERIAL_NUM_KEY) {
        fprintf(stderr, "Serial number must be 13 digits\n");
        return 1;
    }

    try {
        *pSerial = std::stol(argv[2]);
    } catch (const std::exception& e) {
        fprintf(stderr, "Invalid serial number: %s\n", e.what());
        return 1;
    }

    sharedMemory.Detach();

    return 0;
}

