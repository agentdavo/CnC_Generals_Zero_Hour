#pragma once

#include "common/win32_compat.h"

#include <chrono>
#include <thread>

#ifndef WINAPI
#define WINAPI
#endif

#ifndef CALLBACK
#define CALLBACK
#endif

using LPVOID = void *;
using LPCVOID = const void *;
using LPBYTE = unsigned char *;
using LPSTR = char *;
using LPCSTR = const char *;
using LPWSTR = wchar_t *;
using LPCWSTR = const wchar_t *;

struct POINT
{
    LONG x;
    LONG y;
};
struct RECT
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
};

#ifndef INFINITE
#define INFINITE 0xFFFFFFFFu
#endif

#ifndef WAIT_OBJECT_0
#define WAIT_OBJECT_0 0
#endif
#ifndef WAIT_TIMEOUT
#define WAIT_TIMEOUT 258
#endif
#ifndef WAIT_FAILED
#define WAIT_FAILED 0xFFFFFFFFu
#endif

static inline void Sleep(DWORD ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

static inline DWORD timeGetTime()
{
    using namespace std::chrono;
    return static_cast<DWORD>(duration_cast<milliseconds>(
                                  steady_clock::now().time_since_epoch())
                                  .count());
}

// Minimal Win32 synchronization stubs
#ifndef _WIN32
#include <mutex>
#include <strings.h>
#include <ctype.h>
#include <atomic>
#include <thread>
#include <cstdint>
#include <condition_variable>
#include <new>
#include <cerrno>

#define stricmp strcasecmp
#define _stricmp strcasecmp
#define strnicmp strncasecmp
#define _strnicmp strncasecmp
static inline char *strupr(char *s)
{
    for (char *p = s; *p; ++p)
        *p = static_cast<char>(toupper(static_cast<unsigned char>(*p)));
    return s;
}

static inline char *_strupr(char *s) { return strupr(s); }

static inline char *strlwr(char *s)
{
    for (char *p = s; *p; ++p)
        *p = static_cast<char>(tolower(static_cast<unsigned char>(*p)));
    return s;
}

static inline char *_strlwr(char *s) { return strlwr(s); }

#define HAVE_STRUPR_DECL 1
#define HAVE_STRLWR_DECL 1

#define SEVERITY_ERROR 1
#define FACILITY_ITF 4
#define MAKE_HRESULT(sev, fac, code) \
    ((HRESULT)(((unsigned long)(sev) << 31) | ((unsigned long)(fac) << 16) | \
               ((unsigned long)(code))))
#ifndef S_OK
#define S_OK 0
#endif
#ifndef E_FAIL
#define E_FAIL ((HRESULT)0x80004005L)
#endif

// Minimal registry API stubs for non-Windows builds
using HKEY = void *;
#define HKEY_CURRENT_USER   ((HKEY)1)
#define HKEY_LOCAL_MACHINE  ((HKEY)2)

#define KEY_READ   0x20019
#define KEY_WRITE  0x20006
#define REG_OPTION_NON_VOLATILE 0
#define REG_SZ     1
#define REG_DWORD  4

#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0
#endif
#ifndef ERROR_FILE_NOT_FOUND
#define ERROR_FILE_NOT_FOUND ENOENT
#endif

static inline LONG RegOpenKeyEx(HKEY, LPCSTR, DWORD, DWORD, HKEY *)
{
    return ERROR_FILE_NOT_FOUND;
}

static inline LONG RegQueryValueEx(HKEY, LPCSTR, DWORD *, DWORD *, LPBYTE,
                                   DWORD *)
{
    return ERROR_FILE_NOT_FOUND;
}

static inline LONG RegSetValueEx(HKEY, LPCSTR, DWORD, DWORD, const BYTE *,
                                 DWORD)
{
    return ERROR_SUCCESS;
}

static inline LONG RegCreateKeyEx(HKEY, LPCSTR, DWORD, LPCSTR, DWORD, DWORD,
                                  LPVOID, HKEY *out, DWORD *disp)
{
    if (out)
        *out = nullptr;
    if (disp)
        *disp = 0;
    return ERROR_FILE_NOT_FOUND;
}

static inline LONG RegCloseKey(HKEY)
{
    return ERROR_SUCCESS;
}

struct WinMutex {
    std::mutex m;
};

static inline HANDLE CreateMutex(LPVOID, BOOL initialOwner, LPCSTR)
{
    auto *wm = new WinMutex();
    if (initialOwner)
        wm->m.lock();
    return wm;
}

static inline BOOL ReleaseMutex(HANDLE h)
{
    auto *wm = static_cast<WinMutex *>(h);
    wm->m.unlock();
    return 1;
}

static inline DWORD WaitForSingleObject(HANDLE h, DWORD ms)
{
    auto *wm = static_cast<WinMutex *>(h);
    if (ms == INFINITE) {
        wm->m.lock();
        return WAIT_OBJECT_0;
    }
    using namespace std::chrono;
    auto deadline = steady_clock::now() + milliseconds(ms);
    while (!wm->m.try_lock()) {
        if (steady_clock::now() >= deadline)
            return WAIT_TIMEOUT;
        std::this_thread::sleep_for(milliseconds(1));
    }
    return WAIT_OBJECT_0;
}

static inline BOOL CloseHandle(HANDLE h)
{
    delete static_cast<WinMutex *>(h);
    return 1;
}

struct CRITICAL_SECTION {
    std::recursive_mutex m;
};

struct CONDITION_VARIABLE {
    std::condition_variable_any cv;
};

static inline void InitializeCriticalSection(CRITICAL_SECTION *cs)
{
    new (cs) CRITICAL_SECTION();
}

static inline void DeleteCriticalSection(CRITICAL_SECTION *cs)
{
    cs->~CRITICAL_SECTION();
}

static inline void EnterCriticalSection(CRITICAL_SECTION *cs)
{
    cs->m.lock();
}

static inline void LeaveCriticalSection(CRITICAL_SECTION *cs)
{
    cs->m.unlock();
}

static inline void InitializeConditionVariable(CONDITION_VARIABLE *cv)
{
    new (cv) CONDITION_VARIABLE();
}

static inline BOOL SleepConditionVariableCS(CONDITION_VARIABLE *cv,
                                            CRITICAL_SECTION *cs,
                                            DWORD ms)
{
    std::unique_lock<std::recursive_mutex> lock(cs->m, std::adopt_lock);
    BOOL ret = 1;
    if (ms == INFINITE) {
        cv->cv.wait(lock);
    } else {
        auto status = cv->cv.wait_for(lock, std::chrono::milliseconds(ms));
        if (status == std::cv_status::timeout)
            ret = 0;
    }
    lock.release();
    return ret;
}

static inline void WakeConditionVariable(CONDITION_VARIABLE *cv)
{
    cv->cv.notify_one();
}

static inline void WakeAllConditionVariable(CONDITION_VARIABLE *cv)
{
    cv->cv.notify_all();
}

struct LARGE_INTEGER { long long QuadPart; };

static inline BOOL QueryPerformanceFrequency(LARGE_INTEGER *li)
{
    li->QuadPart = 1000000000LL; // nanoseconds per second
    return 1;
}

static inline BOOL QueryPerformanceCounter(LARGE_INTEGER *li)
{
    using namespace std::chrono;
    li->QuadPart =
        duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count();
    return 1;
}

static inline unsigned long GetCurrentThreadId()
{
    auto id = std::this_thread::get_id();
    return std::hash<std::thread::id>()(id);
}
#endif // _WIN32
