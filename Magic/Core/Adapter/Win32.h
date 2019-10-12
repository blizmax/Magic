#pragma once

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
typedef struct{
	enum class RW {
		Read,
		Wirte
	};
	RW flag;
	SRWLOCK lock;
}WinRWLock;

typedef HANDLE				sem_t;
typedef HANDLE				mutex_t;
typedef CRITICAL_SECTION	spinlock_t;
typedef WinRWLock			rwlock_t;
#endif
