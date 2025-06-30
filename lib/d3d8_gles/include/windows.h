#ifndef WINDOWS_H
#define WINDOWS_H

#include <stdint.h>
#include <wchar.h>
#include <string.h>

#ifndef WINAPI
#define WINAPI
#endif
#ifndef CALLBACK
#define CALLBACK
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// Only define these types if they haven't been defined already
// Check if main compatibility types are already defined
#ifndef MAIN_COMPAT_TYPES_DEFINED
#ifndef BOOL
typedef int BOOL;
#endif
#ifndef BYTE
typedef unsigned char BYTE;
#endif
#ifndef WORD
typedef unsigned short WORD;
#endif
#ifndef UINT
typedef unsigned int UINT;
#endif
#ifndef LONG
typedef long LONG;
#endif
#ifndef ULONG
typedef unsigned long ULONG;
#endif
#ifndef DWORD
typedef unsigned long DWORD;
#endif
#ifndef FLOAT
typedef float FLOAT;
#endif
#ifndef INT
typedef int INT;
#endif
#endif // MAIN_COMPAT_TYPES_DEFINED

#ifndef LPVOID
typedef void *LPVOID;
#endif
#ifndef LPCVOID
typedef const void *LPCVOID;
#endif
#ifndef LPBYTE
typedef BYTE *LPBYTE;
#endif
#ifndef PBYTE
typedef BYTE *PBYTE;
#endif
#ifndef LPDWORD
typedef DWORD *LPDWORD;
#endif
#ifndef LPSTR
typedef char *LPSTR;
#endif
#ifndef LPCSTR
typedef const char *LPCSTR;
#endif
#ifndef LPCTSTR
typedef const char *LPCTSTR;
#endif
#ifndef VOID
typedef void VOID;
#endif
#ifndef WCHAR
typedef wchar_t WCHAR;
#endif
#ifndef LPWSTR
typedef WCHAR *LPWSTR;
#endif
#ifndef LPCWSTR
typedef const WCHAR *LPCWSTR;
#endif

// GUID structure
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;
#endif

#ifndef LPGUID
typedef GUID *LPGUID;
#endif

#ifndef LPDISPATCH
typedef void *LPDISPATCH;
#endif

// SetWindowPos flags
#ifndef SWP_NOMOVE
#define SWP_NOMOVE 0x0002
#endif
#ifndef SWP_NOZORDER
#define SWP_NOZORDER 0x0004
#endif
#ifndef SWP_NOSIZE
#define SWP_NOSIZE 0x0001
#endif

// Window positioning constants
#ifndef HWND_TOPMOST
#define HWND_TOPMOST ((HWND)-1)
#endif

// Window style constants
#ifndef GWL_STYLE
#define GWL_STYLE (-16)
#endif

// Windows API stubs for non-Windows platforms
static inline void ZeroMemory(void* ptr, size_t size) {
    memset(ptr, 0, size);
}

#ifndef STDAPI
#define STDAPI extern "C" HRESULT
#endif

// Ensure HMODULE is always available for DirectX headers
#ifndef HMODULE
#ifndef HANDLE
typedef void *HANDLE;
#endif
typedef HANDLE HMODULE;
#endif

#ifndef MAIN_COMPAT_TYPES_DEFINED
#ifndef HANDLE
typedef void *HANDLE;
#endif
#ifndef HINSTANCE
typedef HANDLE HINSTANCE;
#endif
#ifndef HMODULE
typedef HANDLE HMODULE;
#endif

#ifndef HDC
typedef void *HDC;
#endif

#ifndef HFONT
typedef void *HFONT;
#endif

#ifndef HBITMAP
typedef void *HBITMAP;
#endif
#endif // MAIN_COMPAT_TYPES_DEFINED

#ifndef PVOID
typedef void *PVOID;
#endif

#ifndef LOGFONT_DEFINED
typedef struct tagLOGFONT {
  int dummy;
} LOGFONT, *PLOGFONT, *LPLOGFONT;
#define LOGFONT_DEFINED
#endif

#ifndef GLYPHMETRICSFLOAT_DEFINED
typedef struct tagGLYPHMETRICSFLOAT {
  float dummy;
} GLYPHMETRICSFLOAT, *LPGLYPHMETRICSFLOAT;
#define GLYPHMETRICSFLOAT_DEFINED
#endif

typedef intptr_t INT_PTR;
typedef uintptr_t UINT_PTR;
typedef intptr_t LONG_PTR;
typedef uintptr_t ULONG_PTR;

#ifndef DECLARE_HANDLE
#define DECLARE_HANDLE(name)                                                   \
  typedef struct name##__ {                                                    \
    int unused;                                                                \
  } *name;
#endif

#ifndef MAIN_COMPAT_TYPES_DEFINED
DECLARE_HANDLE(HWND);
#endif // MAIN_COMPAT_TYPES_DEFINED
#ifndef HMONITOR_DECLARED
#define HMONITOR_DECLARED
DECLARE_HANDLE(HMONITOR);
#endif

#ifndef POINT_DEFINED
#define POINT_DEFINED
typedef struct tagPOINT {
  LONG x;
  LONG y;
} POINT, *PPOINT, *LPPOINT;
#endif

#ifndef RECT_DEFINED
#define RECT_DEFINED
typedef struct tagRECT {
  LONG left;
  LONG top;
  LONG right;
  LONG bottom;
} RECT, *PRECT, *LPRECT;
#endif

#ifndef PALETTEENTRY_DEFINED
#define PALETTEENTRY_DEFINED
typedef struct tagPALETTEENTRY {
  BYTE peRed;
  BYTE peGreen;
  BYTE peBlue;
  BYTE peFlags;
} PALETTEENTRY, *PPALETTEENTRY, *LPPALETTEENTRY;
#endif

#ifndef RGNDATA_DEFINED
#define RGNDATA_DEFINED
typedef struct _RGNDATA {
  char unused;
} RGNDATA, *PRGNDATA, *LPRGNDATA;
#endif

#ifndef LPARAM
typedef LONG_PTR LPARAM;
#endif
#ifndef WPARAM
typedef UINT_PTR WPARAM;
#endif
#ifndef LRESULT
typedef LONG_PTR LRESULT;
#endif

#ifndef MAIN_COMPAT_TYPES_DEFINED
#ifndef HRESULT
typedef long HRESULT;
#endif
#endif // MAIN_COMPAT_TYPES_DEFINED

#ifndef SUCCEEDED
#define SUCCEEDED(hr) ((HRESULT)(hr) >= 0)
#endif
#ifndef FAILED
#define FAILED(hr) ((HRESULT)(hr) < 0)
#endif

#ifndef S_OK
#define S_OK ((HRESULT)0L)
#endif
#ifndef S_FALSE
#define S_FALSE ((HRESULT)1L)
#endif
#ifndef E_FAIL
#define E_FAIL ((HRESULT)0x80004005L)
#endif
#ifndef E_NOINTERFACE
#define E_NOINTERFACE ((HRESULT)0x80004002L)
#endif
#ifndef E_OUTOFMEMORY
#define E_OUTOFMEMORY ((HRESULT)0x8007000EL)
#endif

#ifndef MAKE_HRESULT
#define MAKE_HRESULT(sev, fac, code)                                           \
  ((HRESULT)(((unsigned long)(sev) << 31) | ((unsigned long)(fac) << 16) |     \
             ((unsigned long)(code))))
#endif
#ifndef _FACDD
#define _FACDD 0x876
#endif
#ifndef MAKE_DDHRESULT
#define MAKE_DDHRESULT(code) MAKE_HRESULT(1, _FACDD, (code))
#endif

// Windows API function stubs
#ifdef __cplusplus
extern "C" {
#endif

static inline BOOL GetClientRect(HWND hWnd, RECT* lpRect) {
    // Stub implementation - return reasonable defaults
    if (lpRect) {
        lpRect->left = 0;
        lpRect->top = 0;
        lpRect->right = 800;
        lpRect->bottom = 600;
    }
    return TRUE;
}

static inline LONG GetWindowLong(HWND hWnd, int nIndex) {
    // Stub implementation
    return 0;
}

static inline BOOL AdjustWindowRect(RECT* lpRect, DWORD dwStyle, BOOL bMenu) {
    // Stub implementation - no adjustment needed
    return TRUE;
}

static inline BOOL SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags) {
    // Stub implementation
    return TRUE;
}

#ifdef __cplusplus
}
#endif

#endif // WINDOWS_H
