#pragma once

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <io.h>
#undef DELETE


typedef struct{
    enum class RW {
        Read,
        Wirte
    };
    RW flag;
    SRWLOCK lock;
}WinRWLock;

typedef HANDLE               sem_t;
typedef HANDLE               mutex_t;
typedef WinRWLock            rwlock_t;
typedef HINSTANCE            plugin_t;
typedef CRITICAL_SECTION     spinlock_t;
#define IS_FILE(Path)        _access(Path,0)
#endif
