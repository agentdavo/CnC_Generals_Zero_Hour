#pragma once

#ifndef BITTYPE_H
#define BITTYPE_H

#include <cstdint>
#include "common/win32_compat.h"

// Standard sized aliases
using uint8  = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint   = unsigned int;

using sint8  = std::int8_t;
using sint16 = std::int16_t;
using sint32 = std::int32_t;
using sint   = int;

using float32 = float;
using float64 = double;

// Win32 style aliases guarded against redefinition
#ifndef DWORD
using DWORD = std::uint32_t;
#endif
#ifndef WORD
using WORD  = std::uint16_t;
#endif
#ifndef BYTE
using BYTE  = std::uint8_t;
#endif
#ifndef BOOL
using BOOL  = int;
#endif
#ifndef USHORT
using USHORT = std::uint16_t;
#endif
#ifndef LPCSTR
using LPCSTR = const char*;
#endif
#ifndef UINT
using UINT = unsigned int;
#endif
#ifndef ULONG
using ULONG = unsigned long;
#endif

#endif // BITTYPE_H

