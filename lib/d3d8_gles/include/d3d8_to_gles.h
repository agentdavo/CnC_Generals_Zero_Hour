// include/d3d8_to_gles.h
#ifndef D3D8_TO_GLES_H
#define D3D8_TO_GLES_H

#include "GLES/gl.h"
#include "GLES/glext.h"
#include "EGL/egl.h"

#include <limits.h>
#include <float.h>

// Include our windows compatibility header but avoid conflicts
#include "windows.h"

// Undefine conflicting macros before including DirectX headers
#ifdef STDAPI
#undef STDAPI
#endif
#ifdef MAKE_DDHRESULT
#undef MAKE_DDHRESULT
#endif

#define COM_NO_WINDOWS_H
#include "objbase.h"

// Define WINAPI for cross-platform compatibility
#ifndef WINAPI
#define WINAPI
#endif

// Define D3DAPI as WINAPI for D3DX functions
#ifndef D3DAPI
#define D3DAPI WINAPI
#endif

// Define STDAPI for DirectX file functions
#ifndef STDAPI
#define STDAPI HRESULT
#endif

// Include D3D8 and D3DX8 headers from min-dx8-sdk (the authoritative source)
#include "../lib/min-dx8-sdk/d3d8types.h"
#include "../lib/min-dx8-sdk/d3d8caps.h"
#include "../lib/min-dx8-sdk/d3d8.h"
#include "../lib/min-dx8-sdk/d3dx8core.h"
#include "../lib/min-dx8-sdk/d3dx8mesh.h"

// Include d3dx8math.h but prevent inline inclusion to avoid multiple definitions
#define __D3DX8MATH_INL__  // This prevents d3dx8math.inl from being included
#include "../lib/min-dx8-sdk/d3dx8math.h"
#undef __D3DX8MATH_INL__   // Clean up the define

#include "../lib/min-dx8-sdk/d3dx8tex.h"

// Forward declarations for GLES implementation types only
typedef struct GLES_Device GLES_Device;
typedef struct GLES_Buffer GLES_Buffer;
typedef struct GLES_Texture GLES_Texture;

// GLES Device structure - our implementation only
struct GLES_Device {
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    D3DDISPLAYMODE display_mode;
    D3DPRESENT_PARAMETERS present_params;
    D3DVIEWPORT8 viewport;
    D3DMATRIX world_matrix;
    D3DMATRIX view_matrix;
    D3DMATRIX projection_matrix;
    DWORD fvf;
    DWORD attrib_id;
    GLuint current_vbo;
    GLuint current_ibo;
    DWORD texcoord_index0;
    D3DXVECTOR4 vs_const[256];
    D3DXVECTOR4 ps_const[32];
    // OpenGL state tracking
    GLboolean depth_test;
    GLboolean cull_face;
    GLenum cull_mode;
    GLboolean blend;
    GLenum src_blend;
    GLenum dest_blend;
    GLenum depth_func;
    GLenum alpha_func;
    GLclampf alpha_ref;
    GLenum fog_mode;
    GLboolean stencil_test;
    GLenum stencil_func;
    GLint stencil_ref;
    GLuint stencil_mask;
    GLenum stencil_fail;
    GLenum stencil_zfail;
    GLenum stencil_pass;
    GLfloat ambient[4];
};

// GLES Buffer structure - our implementation only
struct GLES_Buffer {
    GLuint vbo_id;
    DWORD size;
    DWORD usage;
    DWORD length;
    DWORD fvf;
    void *data;
    void *temp_buffer;
    DWORD lock_offset;
    DWORD lock_size;
    D3DFORMAT format;
    D3DPOOL pool;
};

// GLES Texture structure - our implementation only
struct GLES_Texture {
    GLuint tex_id;
    UINT width;
    UINT height;
    UINT levels;
    D3DFORMAT format;
    void *temp_buffer;
};

// Simple vertex structure for our implementation
typedef struct {
    float x, y, z;
    float nx, ny, nz;
} VertexPN;

// Implementation structures that wrap D3D interfaces with our data
// These contain the interface as first member plus our implementation fields

typedef struct {
    // Must be compatible with ID3DXBuffer interface layout
    void *lpVtbl; // Interface vtable pointer
    ULONG ref_count;
    DWORD size;
    void *data;
} BufferImpl;

typedef struct {
    // Must be compatible with ID3DXMatrixStack interface layout  
    void *lpVtbl; // Interface vtable pointer
    ULONG ref_count;
    D3DXMATRIX *stack;
    DWORD capacity;
    DWORD top;
} MatrixStackImpl;

typedef struct {
    // Must be compatible with ID3DXMesh interface layout
    void *lpVtbl; // Interface vtable pointer
    ULONG ref_count;
    LPDIRECT3DDEVICE8 device;
    DWORD num_faces;
    DWORD num_vertices;
    DWORD fvf;
    DWORD options;
    LPDIRECT3DVERTEXBUFFER8 vb;
    LPDIRECT3DINDEXBUFFER8 ib;
    D3DXATTRIBUTERANGE *attrib_table;
    DWORD attrib_table_size;
    DWORD *attrib_buffer;
} MeshImpl;

// Implementation structures for D3D8 interfaces with our GLES backend
typedef struct {
    // Must be compatible with IDirect3D8 interface layout
    void *lpVtbl;
    // No additional data needed for D3D8 interface
} D3D8Impl;

typedef struct {
    // Must be compatible with IDirect3DDevice8 interface layout
    void *lpVtbl;
    ULONG ref_count;
    GLES_Device *gles;
    IDirect3D8 *d3d8;
} DeviceImpl;

typedef struct {
    // Must be compatible with IDirect3DVertexBuffer8 interface layout
    void *lpVtbl;
    ULONG ref_count;
    GLES_Buffer *buffer;
    LPDIRECT3DDEVICE8 device;
} VertexBufferImpl;

typedef struct {
    // Must be compatible with IDirect3DIndexBuffer8 interface layout
    void *lpVtbl;
    ULONG ref_count;
    GLES_Buffer *buffer;
    LPDIRECT3DDEVICE8 device;
} IndexBufferImpl;

typedef struct {
    // Must be compatible with IDirect3DTexture8 interface layout
    void *lpVtbl;
    ULONG ref_count;
    GLES_Texture *texture;
    LPDIRECT3DDEVICE8 device;
} TextureImpl;

// Only declare functions that are NOT already declared in min-dx8-sdk
// These are our custom implementation functions

// Main D3D8 creation function
IDirect3D8* WINAPI Direct3DCreate8(UINT SDKVersion);

// Custom texture loading (if we need variants not in min-dx8-sdk)
// Note: Most D3DX functions are already declared in min-dx8-sdk headers

#endif // D3D8_TO_GLES_H