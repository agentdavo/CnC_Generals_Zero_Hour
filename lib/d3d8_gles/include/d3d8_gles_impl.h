// d3d8_gles_impl.h - Implementation structures for D3D8 to OpenGL ES 1.1 translation
#ifndef D3D8_GLES_IMPL_H
#define D3D8_GLES_IMPL_H

#include "d3d8_to_gles.h"

// Internal implementation structures
// These structures extend the D3D8 objects with GLES-specific data

// Implementation data for IDirect3D8
typedef struct {
    ULONG ref_count;
    GLES_Context *gles_context;
} D3D8_GLES_Impl;

// Implementation data for IDirect3DDevice8  
typedef struct {
    ULONG ref_count;
    GLES_Device *gles_device;
    IDirect3D8 *d3d8_parent;
    D3DPRESENT_PARAMETERS present_params;
    D3DDEVICE_CREATION_PARAMETERS creation_params;
} D3DDevice8_GLES_Impl;

// Implementation data for IDirect3DVertexBuffer8
typedef struct {
    ULONG ref_count;
    GLES_Buffer *gles_buffer;
    IDirect3DDevice8 *device_parent;
    D3DVERTEXBUFFER_DESC desc;
    void *locked_data;
} D3DVertexBuffer8_GLES_Impl;

// Implementation data for IDirect3DIndexBuffer8
typedef struct {
    ULONG ref_count;
    GLES_Buffer *gles_buffer;
    IDirect3DDevice8 *device_parent;
    D3DINDEXBUFFER_DESC desc;
    void *locked_data;
} D3DIndexBuffer8_GLES_Impl;

// Implementation data for IDirect3DTexture8
typedef struct {
    ULONG ref_count;
    GLES_Texture *gles_texture;
    IDirect3DDevice8 *device_parent;
    D3DSURFACE_DESC surface_desc;
    void *locked_data;
    UINT levels;
} D3DTexture8_GLES_Impl;

// Helper macros to get implementation data from D3D8 objects
#define D3D8_IMPL(obj) ((D3D8_GLES_Impl*)(obj))
#define D3DDEVICE8_IMPL(obj) ((D3DDevice8_GLES_Impl*)(obj))
#define D3DVERTEXBUFFER8_IMPL(obj) ((D3DVertexBuffer8_GLES_Impl*)(obj))
#define D3DINDEXBUFFER8_IMPL(obj) ((D3DIndexBuffer8_GLES_Impl*)(obj))
#define D3DTEXTURE8_IMPL(obj) ((D3DTexture8_GLES_Impl*)(obj))

// Factory functions for creating D3D8 objects with GLES implementation
IDirect3D8* create_d3d8_gles_object(void);
IDirect3DDevice8* create_d3ddevice8_gles_object(IDirect3D8 *d3d8, GLES_Device *gles_device);
IDirect3DVertexBuffer8* create_d3dvertexbuffer8_gles_object(IDirect3DDevice8 *device, GLES_Buffer *buffer, const D3DVERTEXBUFFER_DESC *desc);
IDirect3DIndexBuffer8* create_d3dindexbuffer8_gles_object(IDirect3DDevice8 *device, GLES_Buffer *buffer, const D3DINDEXBUFFER_DESC *desc);
IDirect3DTexture8* create_d3dtexture8_gles_object(IDirect3DDevice8 *device, GLES_Texture *texture, const D3DSURFACE_DESC *surface_desc, UINT levels);

#endif // D3D8_GLES_IMPL_H