#include "mutex.h"
#include "wwdebug.h"
#include "common/windows.h"
#ifndef _WIN32
#include <mutex>
#include <chrono>
#endif

MutexClass::MutexClass(const char* name) : handle(nullptr), locked(false)
{
#ifdef _WIN32
    handle = CreateMutex(NULL,false,name);
    WWASSERT(handle);
#else
    (void)name;
    handle = new std::timed_mutex();
#endif
}

MutexClass::~MutexClass()
{
#ifdef _WIN32
    WWASSERT(!locked);
    CloseHandle(handle);
#else
    delete static_cast<std::timed_mutex*>(handle);
#endif
}

bool MutexClass::Lock(int time)
{
#ifdef _WIN32
    int res = WaitForSingleObject(handle, time==WAIT_INFINITE ? INFINITE : time);
    if (res != WAIT_OBJECT_0) return false;
    locked++;
    return true;
#else
    auto m = static_cast<std::timed_mutex*>(handle);
    if (time == WAIT_INFINITE) {
        m->lock();
        locked++;
        return true;
    }
    if (m->try_lock_for(std::chrono::milliseconds(time))) {
        locked++;
        return true;
    }
    return false;
#endif
}

void MutexClass::Unlock()
{
#ifdef _WIN32
    WWASSERT(locked);
    locked--;
    int res = ReleaseMutex(handle);
    (void)res;
    WWASSERT(res);
#else
    WWASSERT(locked);
    locked--;
    static_cast<std::timed_mutex*>(handle)->unlock();
#endif
}

MutexClass::LockClass::LockClass(MutexClass& m,int time) : mutex(m)
{
    failed = !mutex.Lock(time);
}

MutexClass::LockClass::~LockClass()
{
    if(!failed) mutex.Unlock();
}

CriticalSectionClass::CriticalSectionClass() : handle(nullptr), locked(false)
{
#ifdef _WIN32
    handle = W3DNEWARRAY char[sizeof(CRITICAL_SECTION)];
    InitializeCriticalSection((CRITICAL_SECTION*)handle);
#else
    handle = new std::timed_mutex();
#endif
}

CriticalSectionClass::~CriticalSectionClass()
{
#ifdef _WIN32
    WWASSERT(!locked);
    DeleteCriticalSection((CRITICAL_SECTION*)handle);
    delete[] static_cast<char*>(handle);
#else
    delete static_cast<std::timed_mutex*>(handle);
#endif
}

void CriticalSectionClass::Lock()
{
#ifdef _WIN32
    EnterCriticalSection((CRITICAL_SECTION*)handle);
#else
    static_cast<std::timed_mutex*>(handle)->lock();
#endif
    locked++;
}

void CriticalSectionClass::Unlock()
{
#ifdef _WIN32
    WWASSERT(locked);
    locked--;
    LeaveCriticalSection((CRITICAL_SECTION*)handle);
#else
    WWASSERT(locked);
    locked--;
    static_cast<std::timed_mutex*>(handle)->unlock();
#endif
}

CriticalSectionClass::LockClass::LockClass(CriticalSectionClass& cs) : CriticalSection(cs)
{
    CriticalSection.Lock();
}

CriticalSectionClass::LockClass::~LockClass()
{
    CriticalSection.Unlock();
}
