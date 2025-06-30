// d3d8_resources.c - Vertex/Index buffers and Textures
#include "d3d8_to_gles.h"
#include "d3d8_internal.h"

// Forward declarations
static HRESULT D3DAPI vb_query_interface(IDirect3DVertexBuffer8 *This, REFIID riid, void **ppvObj);
static ULONG D3DAPI vb_add_ref(IDirect3DVertexBuffer8 *This);
static ULONG D3DAPI vb_release(IDirect3DVertexBuffer8 *This);
static HRESULT D3DAPI ib_query_interface(IDirect3DIndexBuffer8 *This, REFIID riid, void **ppvObj);
static ULONG D3DAPI ib_add_ref(IDirect3DIndexBuffer8 *This);
static ULONG D3DAPI ib_release(IDirect3DIndexBuffer8 *This);
static HRESULT D3DAPI tex_query_interface(IDirect3DTexture8 *This, REFIID riid, void **ppvObj);
static ULONG D3DAPI tex_add_ref(IDirect3DTexture8 *This);
static ULONG D3DAPI tex_release(IDirect3DTexture8 *This);

// Common IUnknown implementations for vertex buffer
static HRESULT D3DAPI vb_query_interface(IDirect3DVertexBuffer8 *This, REFIID riid, void **ppvObj) {
    (void)This;
    (void)riid;
    (void)ppvObj;
    return D3DERR_INVALIDCALL;
}

static ULONG D3DAPI vb_add_ref(IDirect3DVertexBuffer8 *This) {
    VertexBufferImpl *impl = (VertexBufferImpl*)This;
    return ++impl->ref_count;
}

static ULONG D3DAPI vb_release(IDirect3DVertexBuffer8 *This) {
    VertexBufferImpl *impl = (VertexBufferImpl*)This;
    ULONG ref = --impl->ref_count;
    if (ref == 0) {
        if (impl->buffer) {
            if (impl->buffer->vbo_id) {
                glDeleteBuffers(1, &impl->buffer->vbo_id);
            }
            free(impl->buffer->temp_buffer);
            free(impl->buffer);
        }
        free(impl);
    }
    return ref;
}

// Common IUnknown implementations for index buffer
static HRESULT D3DAPI ib_query_interface(IDirect3DIndexBuffer8 *This, REFIID riid, void **ppvObj) {
    (void)This;
    (void)riid;
    (void)ppvObj;
    return D3DERR_INVALIDCALL;
}

static ULONG D3DAPI ib_add_ref(IDirect3DIndexBuffer8 *This) {
    IndexBufferImpl *impl = (IndexBufferImpl*)This;
    return ++impl->ref_count;
}

static ULONG D3DAPI ib_release(IDirect3DIndexBuffer8 *This) {
    IndexBufferImpl *impl = (IndexBufferImpl*)This;
    ULONG ref = --impl->ref_count;
    if (ref == 0) {
        if (impl->buffer) {
            if (impl->buffer->vbo_id) {
                glDeleteBuffers(1, &impl->buffer->vbo_id);
            }
            free(impl->buffer->temp_buffer);
            free(impl->buffer);
        }
        free(impl);
    }
    return ref;
}

// Common IUnknown implementations for texture
static HRESULT D3DAPI tex_query_interface(IDirect3DTexture8 *This, REFIID riid, void **ppvObj) {
    (void)This;
    (void)riid;
    (void)ppvObj;
    return D3DERR_INVALIDCALL;
}

static ULONG D3DAPI tex_add_ref(IDirect3DTexture8 *This) {
    TextureImpl *impl = (TextureImpl*)This;
    return ++impl->ref_count;
}

static ULONG D3DAPI tex_release(IDirect3DTexture8 *This) {
    TextureImpl *impl = (TextureImpl*)This;
    ULONG ref = --impl->ref_count;
    if (ref == 0) {
        if (impl->texture) {
            if (impl->texture->tex_id) {
                glDeleteTextures(1, &impl->texture->tex_id);
            }
            free(impl->texture);
        }
        free(impl);
    }
    return ref;
}

// Vertex Buffer IDirect3DResource8 methods
static HRESULT D3DAPI d3d8_vb_get_device(IDirect3DVertexBuffer8 *This, IDirect3DDevice8 **ppDevice) {
    VertexBufferImpl *impl = (VertexBufferImpl*)This;
    *ppDevice = impl->device;
    return D3D_OK;
}

static HRESULT D3DAPI d3d8_vb_set_private_data(IDirect3DVertexBuffer8 *This, REFGUID refguid,
                                               CONST void *pData, DWORD SizeOfData, DWORD Flags) {
    (void)This; (void)refguid; (void)pData; (void)SizeOfData; (void)Flags;
    return D3DERR_NOTAVAILABLE;
}

static HRESULT D3DAPI d3d8_vb_get_private_data(IDirect3DVertexBuffer8 *This, REFGUID refguid,
                                               void *pData, DWORD *pSizeOfData) {
    (void)This; (void)refguid; (void)pData; (void)pSizeOfData;
    return D3DERR_NOTAVAILABLE;
}

static HRESULT D3DAPI d3d8_vb_free_private_data(IDirect3DVertexBuffer8 *This, REFGUID refguid) {
    (void)This; (void)refguid;
    return D3DERR_NOTAVAILABLE;
}

static DWORD D3DAPI d3d8_vb_set_priority(IDirect3DVertexBuffer8 *This, DWORD PriorityNew) {
    (void)This; (void)PriorityNew;
    return 0;
}

static DWORD D3DAPI d3d8_vb_get_priority(IDirect3DVertexBuffer8 *This) {
    (void)This;
    return 0;
}

static void D3DAPI d3d8_vb_pre_load(IDirect3DVertexBuffer8 *This) {
    (void)This;
}

static D3DRESOURCETYPE D3DAPI d3d8_vb_get_type(IDirect3DVertexBuffer8 *This) {
    (void)This;
    return D3DRTYPE_VERTEXBUFFER;
}

// Vertex Buffer specific methods
static HRESULT D3DAPI d3d8_vb_lock(IDirect3DVertexBuffer8 *This, UINT OffsetToLock, UINT SizeToLock,
                                   BYTE **ppbData, DWORD Flags) {
    VertexBufferImpl *impl = (VertexBufferImpl*)This;
    GLES_Buffer *buffer = impl->buffer;
    if (SizeToLock == 0)
        SizeToLock = buffer->length - OffsetToLock;

    buffer->lock_offset = OffsetToLock;
    buffer->lock_size = SizeToLock;

    buffer->temp_buffer = malloc(buffer->length);
    if (!buffer->temp_buffer)
        return D3DERR_OUTOFVIDEOMEMORY;

    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo_id);
    if (Flags & D3DLOCK_DISCARD) {
        GLenum usage = (buffer->usage & D3DUSAGE_DYNAMIC) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
        glBufferData(GL_ARRAY_BUFFER, buffer->length, NULL, usage);
    }
    *ppbData = buffer->temp_buffer + OffsetToLock;
    return D3D_OK;
}

static HRESULT D3DAPI d3d8_vb_unlock(IDirect3DVertexBuffer8 *This) {
    VertexBufferImpl *impl = (VertexBufferImpl*)This;
    GLES_Buffer *buffer = impl->buffer;
    if (!buffer->temp_buffer)
        return D3DERR_INVALIDCALL;

    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo_id);
    glBufferSubData(GL_ARRAY_BUFFER, buffer->lock_offset, buffer->lock_size,
                    buffer->temp_buffer + buffer->lock_offset);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    free(buffer->temp_buffer);
    buffer->temp_buffer = NULL;
    return D3D_OK;
}

static HRESULT D3DAPI d3d8_vb_get_desc(IDirect3DVertexBuffer8 *This, D3DVERTEXBUFFER_DESC *pDesc) {
    VertexBufferImpl *impl = (VertexBufferImpl*)This;
    pDesc->Format = D3DFMT_VERTEXDATA;
    pDesc->Type = D3DRTYPE_VERTEXBUFFER;
    pDesc->Usage = impl->buffer->usage;
    pDesc->Pool = impl->buffer->pool;
    pDesc->Size = impl->buffer->length;
    pDesc->FVF = impl->buffer->fvf;
    return D3D_OK;
}

// Index Buffer IDirect3DResource8 methods
static HRESULT D3DAPI d3d8_ib_get_device(IDirect3DIndexBuffer8 *This, IDirect3DDevice8 **ppDevice) {
    IndexBufferImpl *impl = (IndexBufferImpl*)This;
    *ppDevice = impl->device;
    return D3D_OK;
}

static HRESULT D3DAPI d3d8_ib_set_private_data(IDirect3DIndexBuffer8 *This, REFGUID refguid,
                                               CONST void *pData, DWORD SizeOfData, DWORD Flags) {
    (void)This; (void)refguid; (void)pData; (void)SizeOfData; (void)Flags;
    return D3DERR_NOTAVAILABLE;
}

static HRESULT D3DAPI d3d8_ib_get_private_data(IDirect3DIndexBuffer8 *This, REFGUID refguid,
                                               void *pData, DWORD *pSizeOfData) {
    (void)This; (void)refguid; (void)pData; (void)pSizeOfData;
    return D3DERR_NOTAVAILABLE;
}

static HRESULT D3DAPI d3d8_ib_free_private_data(IDirect3DIndexBuffer8 *This, REFGUID refguid) {
    (void)This; (void)refguid;
    return D3DERR_NOTAVAILABLE;
}

static DWORD D3DAPI d3d8_ib_set_priority(IDirect3DIndexBuffer8 *This, DWORD PriorityNew) {
    (void)This; (void)PriorityNew;
    return 0;
}

static DWORD D3DAPI d3d8_ib_get_priority(IDirect3DIndexBuffer8 *This) {
    (void)This;
    return 0;
}

static void D3DAPI d3d8_ib_pre_load(IDirect3DIndexBuffer8 *This) {
    (void)This;
}

static D3DRESOURCETYPE D3DAPI d3d8_ib_get_type(IDirect3DIndexBuffer8 *This) {
    (void)This;
    return D3DRTYPE_INDEXBUFFER;
}

// Index Buffer specific methods
static HRESULT D3DAPI d3d8_ib_lock(IDirect3DIndexBuffer8 *This, UINT OffsetToLock, UINT SizeToLock,
                                   BYTE **ppbData, DWORD Flags) {
    IndexBufferImpl *impl = (IndexBufferImpl*)This;
    GLES_Buffer *buffer = impl->buffer;
    if (SizeToLock == 0)
        SizeToLock = buffer->length - OffsetToLock;

    buffer->lock_offset = OffsetToLock;
    buffer->lock_size = SizeToLock;

    buffer->temp_buffer = malloc(buffer->length);
    if (!buffer->temp_buffer)
        return D3DERR_OUTOFVIDEOMEMORY;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->vbo_id);
    if (Flags & D3DLOCK_DISCARD) {
        GLenum usage = (buffer->usage & D3DUSAGE_DYNAMIC) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer->length, NULL, usage);
    }
    *ppbData = buffer->temp_buffer + OffsetToLock;
    return D3D_OK;
}

static HRESULT D3DAPI d3d8_ib_unlock(IDirect3DIndexBuffer8 *This) {
    IndexBufferImpl *impl = (IndexBufferImpl*)This;
    GLES_Buffer *buffer = impl->buffer;
    if (!buffer->temp_buffer)
        return D3DERR_INVALIDCALL;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->vbo_id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, buffer->lock_offset, buffer->lock_size,
                    buffer->temp_buffer + buffer->lock_offset);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    free(buffer->temp_buffer);
    buffer->temp_buffer = NULL;
    return D3D_OK;
}

static HRESULT D3DAPI d3d8_ib_get_desc(IDirect3DIndexBuffer8 *This, D3DINDEXBUFFER_DESC *pDesc) {
    IndexBufferImpl *impl = (IndexBufferImpl*)This;
    pDesc->Format = impl->buffer->format;
    pDesc->Type = D3DRTYPE_INDEXBUFFER;
    pDesc->Usage = impl->buffer->usage;
    pDesc->Pool = impl->buffer->pool;
    pDesc->Size = impl->buffer->length;
    return D3D_OK;
}

// Device method implementations for creating buffers
HRESULT WINAPI d3d8_create_vertex_buffer(IDirect3DDevice8 *This, UINT Length, DWORD Usage, DWORD FVF,
                                         D3DPOOL Pool, IDirect3DVertexBuffer8 **ppVertexBuffer) {
    DeviceImpl *device_impl = (DeviceImpl*)This;
    
    GLES_Buffer *buffer = calloc(1, sizeof(GLES_Buffer));
    if (!buffer)
        return D3DERR_OUTOFVIDEOMEMORY;

    buffer->length = Length;
    buffer->usage = Usage;
    buffer->fvf = FVF;
    buffer->pool = Pool;

    glGenBuffers(1, &buffer->vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo_id);
    GLenum gl_usage = (Usage & D3DUSAGE_DYNAMIC) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    glBufferData(GL_ARRAY_BUFFER, Length, NULL, gl_usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    VertexBufferImpl *vb = calloc(1, sizeof(VertexBufferImpl));
    if (!vb) {
        glDeleteBuffers(1, &buffer->vbo_id);
        free(buffer);
        return D3DERR_OUTOFVIDEOMEMORY;
    }

    static const IDirect3DVertexBuffer8Vtbl vb_vtbl = {
        D3D8_CAST(vb_query_interface),
        D3D8_CAST(vb_add_ref),
        D3D8_CAST(vb_release),
        D3D8_CAST(d3d8_vb_get_device),
        D3D8_CAST(d3d8_vb_set_private_data),
        D3D8_CAST(d3d8_vb_get_private_data),
        D3D8_CAST(d3d8_vb_free_private_data),
        D3D8_CAST(d3d8_vb_set_priority),
        D3D8_CAST(d3d8_vb_get_priority),
        D3D8_CAST(d3d8_vb_pre_load),
        D3D8_CAST(d3d8_vb_get_type),
        D3D8_CAST(d3d8_vb_lock),
        D3D8_CAST(d3d8_vb_unlock),
        D3D8_CAST(d3d8_vb_get_desc)
    };
    
    vb->lpVtbl = (void*)&vb_vtbl;
    vb->ref_count = 1;
    vb->buffer = buffer;
    vb->device = This;

    *ppVertexBuffer = (IDirect3DVertexBuffer8*)vb;
    return D3D_OK;
}

HRESULT WINAPI d3d8_create_index_buffer(IDirect3DDevice8 *This, UINT Length, DWORD Usage, D3DFORMAT Format,
                                        D3DPOOL Pool, IDirect3DIndexBuffer8 **ppIndexBuffer) {
    DeviceImpl *device_impl = (DeviceImpl*)This;
    
    GLES_Buffer *buffer = calloc(1, sizeof(GLES_Buffer));
    if (!buffer)
        return D3DERR_OUTOFVIDEOMEMORY;

    buffer->length = Length;
    buffer->usage = Usage;
    buffer->format = Format;
    buffer->pool = Pool;

    glGenBuffers(1, &buffer->vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->vbo_id);
    GLenum gl_usage = (Usage & D3DUSAGE_DYNAMIC) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Length, NULL, gl_usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    IndexBufferImpl *ib = calloc(1, sizeof(IndexBufferImpl));
    if (!ib) {
        glDeleteBuffers(1, &buffer->vbo_id);
        free(buffer);
        return D3DERR_OUTOFVIDEOMEMORY;
    }

    static const IDirect3DIndexBuffer8Vtbl ib_vtbl = {
        D3D8_CAST(ib_query_interface),
        D3D8_CAST(ib_add_ref),
        D3D8_CAST(ib_release),
        D3D8_CAST(d3d8_ib_get_device),
        D3D8_CAST(d3d8_ib_set_private_data),
        D3D8_CAST(d3d8_ib_get_private_data),
        D3D8_CAST(d3d8_ib_free_private_data),
        D3D8_CAST(d3d8_ib_set_priority),
        D3D8_CAST(d3d8_ib_get_priority),
        D3D8_CAST(d3d8_ib_pre_load),
        D3D8_CAST(d3d8_ib_get_type),
        D3D8_CAST(d3d8_ib_lock),
        D3D8_CAST(d3d8_ib_unlock),
        D3D8_CAST(d3d8_ib_get_desc)
    };
    
    ib->lpVtbl = (void*)&ib_vtbl;
    ib->ref_count = 1;
    ib->buffer = buffer;
    ib->device = This;

    *ppIndexBuffer = (IDirect3DIndexBuffer8*)ib;
    return D3D_OK;
}

// Stream source and indices binding
HRESULT WINAPI d3d8_set_stream_source(IDirect3DDevice8 *This, UINT StreamNumber,
                                      IDirect3DVertexBuffer8 *pStreamData, UINT Stride) {
    DeviceImpl *device_impl = (DeviceImpl*)This;
    VertexBufferImpl *vb_impl = (VertexBufferImpl*)pStreamData;
    
    if (!pStreamData) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        device_impl->gles->current_vbo = 0;
        return D3D_OK;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, vb_impl->buffer->vbo_id);
    device_impl->gles->current_vbo = vb_impl->buffer->vbo_id;
    return D3D_OK;
}

HRESULT WINAPI d3d8_set_indices(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 *pIndexData,
                                UINT BaseVertexIndex) {
    DeviceImpl *device_impl = (DeviceImpl*)This;
    IndexBufferImpl *ib_impl = (IndexBufferImpl*)pIndexData;
    
    if (!pIndexData) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        device_impl->gles->current_ibo = 0;
        return D3D_OK;
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_impl->buffer->vbo_id);
    device_impl->gles->current_ibo = ib_impl->buffer->vbo_id;
    return D3D_OK;
}