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
#endif // _WIN32
