// d3d8_device.c - IDirect3DDevice8 implementation
#include "d3d8_to_gles.h"
#include "d3d8_internal.h"

// Common helpers
extern HRESULT D3DAPI common_query_interface(void *This, REFIID riid, void **ppvObj);
extern ULONG D3DAPI common_add_ref(void *This);
extern ULONG D3DAPI common_release(void *This);

// IUnknown-style wrappers for IDirect3DDevice8
static HRESULT D3DAPI d3d8_device_query_interface(IDirect3DDevice8 *This,
                                                  REFIID riid, void **ppv) {
  return common_query_interface(This, riid, ppv);
}

static ULONG D3DAPI d3d8_device_add_ref(IDirect3DDevice8 *This) {
  DeviceImpl *impl = (DeviceImpl*)This;
  return ++impl->ref_count;
}

static ULONG D3DAPI d3d8_device_release(IDirect3DDevice8 *This) {
  DeviceImpl *impl = (DeviceImpl*)This;
  ULONG ref = --impl->ref_count;
  if (ref == 0) {
    // TODO: Cleanup GLES resources
    if (impl->gles) {
      eglDestroyContext(impl->gles->display, impl->gles->context);
      eglDestroySurface(impl->gles->display, impl->gles->surface);
      eglTerminate(impl->gles->display);
      free(impl->gles);
    }
    free(impl);
  }
  return ref;
}

// IDirect3DDevice8 methods - stubs for now
static HRESULT D3DAPI d3d8_test_cooperative_level(IDirect3DDevice8 *This) {
  (void)This;
  return D3D_OK;
}

static UINT D3DAPI d3d8_get_available_texture_mem(IDirect3DDevice8 *This) {
  (void)This;
  return 1024 * 1024 * 256; // 256MB
}

static HRESULT D3DAPI d3d8_resource_manager_discard_bytes(IDirect3DDevice8 *This, DWORD Bytes) {
  (void)This;
  (void)Bytes;
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_get_direct3d(IDirect3DDevice8 *This, IDirect3D8 **ppD3D8) {
  DeviceImpl *impl = (DeviceImpl*)This;
  *ppD3D8 = impl->d3d8;
  return D3D_OK;
}

static HRESULT D3DAPI device_get_device_caps(IDirect3DDevice8 *This, D3DCAPS8 *pCaps) {
  (void)This;
  fill_d3d_caps(pCaps, D3DDEVTYPE_HAL);
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_get_display_mode(IDirect3DDevice8 *This, D3DDISPLAYMODE *pMode) {
  DeviceImpl *impl = (DeviceImpl*)This;
  *pMode = impl->gles->display_mode;
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_get_creation_parameters(IDirect3DDevice8 *This, 
                                                  D3DDEVICE_CREATION_PARAMETERS *pParameters) {
  (void)This;
  (void)pParameters;
  return D3DERR_NOTAVAILABLE; // TODO: Implement
}

static HRESULT D3DAPI d3d8_set_cursor_properties(IDirect3DDevice8 *This, UINT XHotSpot, 
                                                 UINT YHotSpot, IDirect3DSurface8 *pCursorBitmap) {
  (void)This;
  (void)XHotSpot;
  (void)YHotSpot;
  (void)pCursorBitmap;
  return D3DERR_NOTAVAILABLE;
}

static void D3DAPI d3d8_set_cursor_position(IDirect3DDevice8 *This, UINT XScreenSpace,
                                            UINT YScreenSpace, DWORD Flags) {
  (void)This;
  (void)XScreenSpace;
  (void)YScreenSpace;
  (void)Flags;
}

static BOOL D3DAPI d3d8_show_cursor(IDirect3DDevice8 *This, BOOL bShow) {
  (void)This;
  (void)bShow;
  return FALSE;
}

static HRESULT D3DAPI d3d8_create_additional_swap_chain(IDirect3DDevice8 *This,
                                                        D3DPRESENT_PARAMETERS *pPresentationParameters,
                                                        IDirect3DSwapChain8 **pSwapChain) {
  (void)This;
  (void)pPresentationParameters;
  (void)pSwapChain;
  return D3DERR_NOTAVAILABLE;
}

static HRESULT D3DAPI d3d8_reset(IDirect3DDevice8 *This, D3DPRESENT_PARAMETERS *pPresentationParameters) {
  (void)This;
  (void)pPresentationParameters;
  return D3DERR_NOTAVAILABLE; // TODO: Implement
}

static HRESULT D3DAPI d3d8_present(IDirect3DDevice8 *This, CONST RECT *pSourceRect,
                                   CONST RECT *pDestRect, HWND hDestWindowOverride,
                                   CONST RGNDATA *pDirtyRegion) {
  DeviceImpl *impl = (DeviceImpl*)This;
  (void)pSourceRect;
  (void)pDestRect;
  (void)hDestWindowOverride;
  (void)pDirtyRegion;
  
  eglSwapBuffers(impl->gles->display, impl->gles->surface);
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_get_back_buffer(IDirect3DDevice8 *This, UINT BackBuffer,
                                           D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 **ppBackBuffer) {
  (void)This;
  (void)BackBuffer;
  (void)Type;
  (void)ppBackBuffer;
  return D3DERR_NOTAVAILABLE; // TODO: Implement
}

static HRESULT D3DAPI d3d8_get_raster_status(IDirect3DDevice8 *This, D3DRASTER_STATUS *pRasterStatus) {
  (void)This;
  (void)pRasterStatus;
  return D3DERR_NOTAVAILABLE;
}

static void D3DAPI d3d8_set_gamma_ramp(IDirect3DDevice8 *This, DWORD Flags, CONST D3DGAMMARAMP *pRamp) {
  (void)This;
  (void)Flags;
  (void)pRamp;
}

static void D3DAPI d3d8_get_gamma_ramp(IDirect3DDevice8 *This, D3DGAMMARAMP *pRamp) {
  (void)This;
  (void)pRamp;
}

static HRESULT D3DAPI device_create_vertex_buffer(IDirect3DDevice8 *This, UINT Length, DWORD Usage,
                                                  DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8 **ppVertexBuffer) {
  return d3d8_create_vertex_buffer(This, Length, Usage, FVF, Pool, ppVertexBuffer);
}

static HRESULT D3DAPI device_create_index_buffer(IDirect3DDevice8 *This, UINT Length, DWORD Usage,
                                                 D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8 **ppIndexBuffer) {
  return d3d8_create_index_buffer(This, Length, Usage, Format, Pool, ppIndexBuffer);
}

static HRESULT D3DAPI d3d8_create_texture(IDirect3DDevice8 *This, UINT Width, UINT Height, UINT Levels,
                                          DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
                                          IDirect3DTexture8 **ppTexture) {
  (void)This;
  (void)Width;
  (void)Height;
  (void)Levels;
  (void)Usage;
  (void)Format;
  (void)Pool;
  (void)ppTexture;
  return D3DERR_NOTAVAILABLE; // TODO: Implement
}

static HRESULT D3DAPI d3d8_set_texture(IDirect3DDevice8 *This, DWORD Stage, IDirect3DBaseTexture8 *pTexture) {
  (void)This;
  (void)Stage;
  (void)pTexture;
  return D3D_OK; // TODO: Implement
}

static HRESULT D3DAPI d3d8_set_texture_stage_state(IDirect3DDevice8 *This, DWORD Stage,
                                                   D3DTEXTURESTAGESTATETYPE Type, DWORD Value) {
  (void)This;
  (void)Stage;
  (void)Type;
  (void)Value;
  return D3D_OK; // TODO: Implement
}

static HRESULT D3DAPI d3d8_set_render_state(IDirect3DDevice8 *This, D3DRENDERSTATETYPE state, DWORD value) {
  DeviceImpl *impl = (DeviceImpl*)This;
  set_render_state(impl->gles, state, value);
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_begin_scene(IDirect3DDevice8 *This) {
  (void)This;
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_end_scene(IDirect3DDevice8 *This) {
  (void)This;
  return D3D_OK;
}

static HRESULT D3DAPI device_set_stream_source(IDirect3DDevice8 *This, UINT StreamNumber,
                                               IDirect3DVertexBuffer8 *pStreamData, UINT Stride) {
  return d3d8_set_stream_source(This, StreamNumber, pStreamData, Stride);
}

static HRESULT D3DAPI device_set_indices(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 *pIndexData,
                                         UINT BaseVertexIndex) {
  return d3d8_set_indices(This, pIndexData, BaseVertexIndex);
}

static HRESULT D3DAPI d3d8_set_viewport(IDirect3DDevice8 *This, CONST D3DVIEWPORT8 *pViewport) {
  DeviceImpl *impl = (DeviceImpl*)This;
  impl->gles->viewport = *pViewport;
  glViewport(pViewport->X, pViewport->Y, pViewport->Width, pViewport->Height);
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_set_transform(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State,
                                         CONST D3DMATRIX *pMatrix) {
  DeviceImpl *impl = (DeviceImpl*)This;
  
  switch (State) {
  case D3DTS_WORLD:
    impl->gles->world_matrix = *pMatrix;
    break;
  case D3DTS_VIEW:
    impl->gles->view_matrix = *pMatrix;
    break;
  case D3DTS_PROJECTION:
    impl->gles->projection_matrix = *pMatrix;
    break;
  default:
    return D3DERR_INVALIDCALL;
  }
  
  // TODO: Apply matrices to OpenGL
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_draw_indexed_primitive(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType,
                                                  UINT MinVertexIndex, UINT NumVertices, UINT StartIndex,
                                                  UINT PrimitiveCount) {
  (void)This;
  (void)PrimitiveType;
  (void)MinVertexIndex;
  (void)NumVertices;
  (void)StartIndex;
  (void)PrimitiveCount;
  return D3D_OK; // TODO: Implement
}

static HRESULT D3DAPI d3d8_set_vertex_shader_constant(IDirect3DDevice8 *This, DWORD Register,
                                                      CONST void *pConstantData, DWORD ConstantCount) {
  DeviceImpl *impl = (DeviceImpl*)This;
  if (Register + ConstantCount > 256)
    return D3DERR_INVALIDCALL;
  memcpy(&impl->gles->vs_const[Register], pConstantData, ConstantCount * sizeof(D3DXVECTOR4));
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_set_pixel_shader_constant(IDirect3DDevice8 *This, DWORD Register,
                                                     CONST void *pConstantData, DWORD ConstantCount) {
  DeviceImpl *impl = (DeviceImpl*)This;
  if (Register + ConstantCount > 32)
    return D3DERR_INVALIDCALL;
  memcpy(&impl->gles->ps_const[Register], pConstantData, ConstantCount * sizeof(D3DXVECTOR4));
  return D3D_OK;
}

// IDirect3DDevice8 vtable
const IDirect3DDevice8Vtbl device_vtbl = {
    D3D8_CAST(d3d8_device_query_interface),
    D3D8_CAST(d3d8_device_add_ref),
    D3D8_CAST(d3d8_device_release),
    D3D8_CAST(d3d8_test_cooperative_level),
    D3D8_CAST(d3d8_get_available_texture_mem),
    D3D8_CAST(d3d8_resource_manager_discard_bytes),
    D3D8_CAST(d3d8_get_direct3d),
    D3D8_CAST(device_get_device_caps),
    D3D8_CAST(d3d8_get_display_mode),
    D3D8_CAST(d3d8_get_creation_parameters),
    D3D8_CAST(d3d8_set_cursor_properties),
    D3D8_CAST(d3d8_set_cursor_position),
    D3D8_CAST(d3d8_show_cursor),
    D3D8_CAST(d3d8_create_additional_swap_chain),
    D3D8_CAST(d3d8_reset),
    D3D8_CAST(d3d8_present),
    D3D8_CAST(d3d8_get_back_buffer),
    D3D8_CAST(d3d8_get_raster_status),
    D3D8_CAST(d3d8_set_gamma_ramp),
    D3D8_CAST(d3d8_get_gamma_ramp),
    D3D8_CAST(device_create_vertex_buffer),
    D3D8_CAST(device_create_index_buffer),
    D3D8_CAST(d3d8_create_texture),
    D3D8_CAST(d3d8_set_texture),
    D3D8_CAST(d3d8_set_texture_stage_state),
    D3D8_CAST(d3d8_set_render_state),
    D3D8_CAST(d3d8_begin_scene),
    D3D8_CAST(d3d8_end_scene),
    D3D8_CAST(device_set_stream_source),
    D3D8_CAST(device_set_indices),
    D3D8_CAST(d3d8_set_viewport),
    D3D8_CAST(d3d8_set_transform),
    D3D8_CAST(d3d8_draw_indexed_primitive),
    D3D8_CAST(d3d8_set_vertex_shader_constant),
    D3D8_CAST(d3d8_set_pixel_shader_constant)
};