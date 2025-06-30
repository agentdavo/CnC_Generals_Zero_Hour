#ifndef OBJBASE_H
#define OBJBASE_H

#ifndef COM_NO_WINDOWS_H
#include <windows.h>
#endif

#include <stdint.h>

// Ensure basic Windows types are defined even when COM_NO_WINDOWS_H is set
// Only define if the game engine hasn't already defined them
#ifndef MAIN_COMPAT_TYPES_DEFINED
#ifndef HRESULT
typedef long HRESULT;
#endif
#ifndef ULONG
typedef unsigned long ULONG;
#endif
#ifndef DWORD
typedef unsigned long DWORD;
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef interface
#define interface struct
#endif

#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t Data4[8];
} GUID;
#endif

typedef GUID IID;
typedef GUID CLSID;
typedef const IID *REFIID;
typedef const CLSID *REFCLSID;
typedef const GUID *REFGUID;

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

#ifndef DEFINE_GUID
#ifdef INITGUID
#define DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
    EXTERN_C const GUID name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}
#else
#define DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
    EXTERN_C const GUID name
#endif
#endif

#ifdef __cplusplus
#define STDMETHOD(method) virtual HRESULT method
#define STDMETHOD_(type,method) virtual type method
#define STDMETHODIMP HRESULT
#define STDMETHODIMP_(type) type
#define PURE = 0
#define THIS
#define THIS_ 

#define DECLARE_INTERFACE_(iface, baseiface) \
    interface iface : public baseiface

#define DECLARE_INTERFACE(iface) DECLARE_INTERFACE_(iface, IUnknown)

#else /* C interface */

#define STDMETHOD(method) HRESULT (*method)
#define STDMETHOD_(type,method) type (*method)
#define STDMETHODIMP HRESULT WINAPI
#define STDMETHODIMP_(type) type WINAPI
#define PURE
#define THIS void *This
#define THIS_ void *This,

#define DECLARE_INTERFACE_(iface, baseiface) \
    typedef struct iface iface; \
    typedef struct iface##Vtbl iface##Vtbl; \
    struct iface##Vtbl
#define DECLARE_INTERFACE(iface) DECLARE_INTERFACE_(iface, IUnknown)

#endif

#ifdef __cplusplus

interface IUnknown {
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void **ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
};

#else /* C interface */

typedef interface IUnknown IUnknown;
struct IUnknownVtbl {
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void **ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
};
interface IUnknown {
    const struct IUnknownVtbl *lpVtbl;
};

#endif

// Minimal IStream forward declaration for d3dx8 headers
typedef interface IStream IStream;

#ifdef __cplusplus
}
#endif

#endif // OBJBASE_H
