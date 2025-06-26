#pragma once

#ifndef _WIN32
#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#ifndef __cdecl
#define __cdecl
#endif
#ifndef _cdecl
#define _cdecl
#endif
#ifndef __stdcall
#define __stdcall
#endif
#ifndef _stdcall
#define _stdcall
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

#ifndef FILE_ATTRIBUTE_READONLY
#define FILE_ATTRIBUTE_READONLY 0x00000001u
#endif

using HANDLE = void*;
using HWND   = HANDLE;

#ifndef SUCCEEDED
#define SUCCEEDED(hr) ((hr) >= 0)
#endif
#ifndef FAILED
#define FAILED(hr) ((hr) < 0)
#endif

static inline int lstrcmpi(const char *a, const char *b) { return strcasecmp(a, b); }
static inline char *lstrcpy(char *d, const char *s) { return strcpy(d, s); }
static inline char *lstrcat(char *d, const char *s) { return strcat(d, s); }
static inline size_t lstrlen(const char *s) { return strlen(s); }
static inline char *lstrcpyn(char *d, const char *s, size_t n) {
    if (n == 0) return d; strncpy(d, s, n - 1); d[n - 1] = '\0'; return d;
}
static inline DWORD GetCurrentDirectory(DWORD n, char *b) {
    char *r = getcwd(b, n); return r ? strlen(r) : 0;
}
#ifndef GetFileAttributes
static inline DWORD GetFileAttributes(const char *p) {
    struct stat st;
    if (stat(p, &st) == 0) {
        DWORD attr = 0;
        if (!(st.st_mode & S_IWUSR))
            attr |= FILE_ATTRIBUTE_READONLY;
        return attr;
    }
    return 0xFFFFFFFFu;
}
#endif
static inline char *_strdup(const char *s) { return strdup(s); }
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#endif // _WIN32
