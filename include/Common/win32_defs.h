#pragma once

// Minimal Win32/MSVC type replacements for cross-platform builds
#ifndef _WIN32
using BOOL   = int;
using BYTE   = unsigned char;
using WORD   = unsigned short;
using DWORD  = unsigned long;
using UINT   = unsigned int;
using HANDLE = void*;
using HWND   = void*;
using HMODULE = void*;
using LPVOID = void*;
using LPCVOID = const void*;
using LPSTR  = char*;
using LPCSTR = const char*;
using LPWSTR = wchar_t*;
using LPCWSTR = const wchar_t*;
using HRESULT = long;

#define WINAPI
#define CALLBACK
#define __stdcall
#define __cdecl
#define MAX_PATH 260
#endif
