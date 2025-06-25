#pragma once

#ifndef _WIN32
#include <cstdint>
#ifndef __cdecl
#define __cdecl
#endif
#ifndef _cdecl
#define _cdecl
#endif

using DWORD = std::uint32_t;
using WORD  = std::uint16_t;
using BYTE  = std::uint8_t;
using HRESULT = std::int32_t;
using BOOL  = int;
using LONG  = std::int32_t;

#ifndef MAX_PATH
#define MAX_PATH 260
#endif
#ifndef _MAX_PATH
#define _MAX_PATH MAX_PATH
#endif

using HANDLE = void*;
using HWND   = HANDLE;

#ifndef SUCCEEDED
#define SUCCEEDED(hr) ((hr) >= 0)
#endif
#ifndef FAILED
#define FAILED(hr) ((hr) < 0)
#endif
#endif // _WIN32
