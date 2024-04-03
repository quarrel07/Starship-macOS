#include "sys.h"

s32 sMemoryBuffer[0x8800];
s32* sMemoryPtr;

void Memory_FreeAll(void) {
    sMemoryPtr = sMemoryBuffer;
}

void* Memory_Allocate(s32 size) {
    void* addr = sMemoryPtr;

    sMemoryPtr = (void*) (((size + 0xF) & ~0xF) + (uintptr_t) sMemoryPtr);
    return addr;
}
