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

using LPVOID = void*;
using LPCVOID = const void*;
using LPBYTE = unsigned char*;
using LPSTR = char*;
using LPCSTR = const char*;
using LPWSTR = wchar_t*;
using LPCWSTR = const wchar_t*;

struct POINT { LONG x; LONG y; };
struct RECT  { LONG left; LONG top; LONG right; LONG bottom; };

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
        steady_clock::now().time_since_epoch()).count());
}

