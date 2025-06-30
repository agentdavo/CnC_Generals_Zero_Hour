// d3d8_core.c - IDirect3D8 and IDirect3DDevice8 core functionality
#include "d3d8_to_gles.h"
#include "d3d8_internal.h"

// Map OpenGL ES 1.1 capabilities to D3DCAPS8
void fill_d3d_caps(D3DCAPS8 *pCaps, D3DDEVTYPE DeviceType) {
  memset(pCaps, 0, sizeof(D3DCAPS8));
  pCaps->DeviceType = DeviceType;
  pCaps->AdapterOrdinal = D3DADAPTER_DEFAULT;
  pCaps->Caps = D3DCAPS_READ_SCANLINE;
  pCaps->Caps2 =
      D3DCAPS2_DYNAMICTEXTURES |
      (DeviceType == D3DDEVTYPE_REF ? 0 : D3DCAPS2_CANRENDERWINDOWED);
  pCaps->PresentationIntervals =
      D3DPRESENT_INTERVAL_IMMEDIATE | D3DPRESENT_INTERVAL_ONE;
  pCaps->DevCaps = D3DDEVCAPS_EXECUTESYSTEMMEMORY |
                   D3DDEVCAPS_TLVERTEXSYSTEMMEMORY |
                   D3DDEVCAPS_TEXTURESYSTEMMEMORY |
                   D3DDEVCAPS_DRAWPRIMTLVERTEX | D3DDEVCAPS_HWRASTERIZATION |
                   (DeviceType == D3DDEVTYPE_REF ? 0 : D3DDEVCAPS_PUREDEVICE);
  pCaps->PrimitiveMiscCaps = D3DPMISCCAPS_MASKZ | D3DPMISCCAPS_CULLNONE |
                             D3DPMISCCAPS_CULLCW | D3DPMISCCAPS_CULLCCW |
                             D3DPMISCCAPS_COLORWRITEENABLE;
  pCaps->RasterCaps = D3DPRASTERCAPS_DITHER | D3DPRASTERCAPS_ZTEST |
                      D3DPRASTERCAPS_FOGVERTEX | D3DPRASTERCAPS_MIPMAPLODBIAS;
  pCaps->ZCmpCaps = pCaps->AlphaCmpCaps =
      D3DPCMPCAPS_NEVER | D3DPCMPCAPS_LESS | D3DPCMPCAPS_EQUAL |
      D3DPCMPCAPS_LESSEQUAL | D3DPCMPCAPS_GREATER | D3DPCMPCAPS_NOTEQUAL |
      D3DPCMPCAPS_GREATEREQUAL | D3DPCMPCAPS_ALWAYS;
  pCaps->SrcBlendCaps = pCaps->DestBlendCaps =
      D3DPBLENDCAPS_ZERO | D3DPBLENDCAPS_ONE | D3DPBLENDCAPS_SRCCOLOR |
      D3DPBLENDCAPS_INVSRCCOLOR | D3DPBLENDCAPS_SRCALPHA |
      D3DPBLENDCAPS_INVSRCALPHA | D3DPBLENDCAPS_DESTALPHA |
      D3DPBLENDCAPS_INVDESTALPHA | D3DPBLENDCAPS_SRCALPHASAT;
  pCaps->ShadeCaps = D3DPSHADECAPS_COLORGOURAUDRGB |
                     D3DPSHADECAPS_ALPHAGOURAUDBLEND | D3DPSHADECAPS_FOGGOURAUD;
  GLint max_texture_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
  pCaps->MaxTextureWidth = pCaps->MaxTextureHeight = max_texture_size;
  pCaps->TextureCaps = D3DPTEXTURECAPS_PERSPECTIVE | D3DPTEXTURECAPS_ALPHA |
                       D3DPTEXTURECAPS_MIPMAP | D3DPTEXTURECAPS_CUBEMAP;
  pCaps->TextureFilterCaps = pCaps->CubeTextureFilterCaps =
      D3DPTFILTERCAPS_MINFPOINT | D3DPTFILTERCAPS_MINFLINEAR |
      D3DPTFILTERCAPS_MIPFPOINT | D3DPTFILTERCAPS_MIPFLINEAR |
      D3DPTFILTERCAPS_MAGFPOINT | D3DPTFILTERCAPS_MAGFLINEAR;
  pCaps->TextureAddressCaps = D3DPTADDRESSCAPS_WRAP | D3DPTADDRESSCAPS_MIRROR |
                              D3DPTADDRESSCAPS_CLAMP |
                              D3DPTADDRESSCAPS_INDEPENDENTUV;
  pCaps->StencilCaps = D3DSTENCILCAPS_KEEP | D3DSTENCILCAPS_ZERO |
                       D3DSTENCILCAPS_REPLACE | D3DSTENCILCAPS_INCRSAT |
                       D3DSTENCILCAPS_DECRSAT | D3DSTENCILCAPS_INVERT |
                       D3DSTENCILCAPS_INCR | D3DSTENCILCAPS_DECR;
  pCaps->TextureOpCaps = D3DTEXOPCAPS_DISABLE | D3DTEXOPCAPS_SELECTARG1 |
                         D3DTEXOPCAPS_SELECTARG2 | D3DTEXOPCAPS_MODULATE |
                         D3DTEXOPCAPS_MODULATE2X | D3DTEXOPCAPS_ADD;
  pCaps->FVFCaps = D3DFVFCAPS_TEXCOORDCOUNTMASK & 0x8;
  pCaps->VertexProcessingCaps =
      D3DVTXPCAPS_TEXGEN | D3DVTXPCAPS_MATERIALSOURCE7 |
      D3DVTXPCAPS_DIRECTIONALLIGHTS | D3DVTXPCAPS_LOCALVIEWER;
  pCaps->MaxActiveLights = 8;
  pCaps->MaxUserClipPlanes = 0;
  pCaps->MaxVertexBlendMatrices = 4;
  pCaps->MaxStreams = 1;
  pCaps->MaxStreamStride = 256;
  pCaps->VertexShaderVersion = D3DVS_VERSION(1, 1);
  pCaps->MaxVertexShaderConst = 96;
  pCaps->PixelShaderVersion = 0;
  pCaps->MaxPixelShaderValue = 0.0f;
  pCaps->MaxTextureBlendStages = 2;
  pCaps->MaxSimultaneousTextures = 2;
  pCaps->MaxPrimitiveCount = 65535;
  pCaps->MaxVertexIndex = 65535;
  pCaps->MaxPointSize = 64.0f;
}

// Common helper functions for COM interfaces
static HRESULT D3DAPI common_query_interface(void *This, REFIID riid,
                                             void **ppvObj) {
  (void)This;
  (void)riid;
  (void)ppvObj;
  return D3DERR_INVALIDCALL;
}

static ULONG D3DAPI common_add_ref(void *This) {
  (void)This;
  return 1;
}

static ULONG D3DAPI common_release(void *This) {
  free(This);
  return 0;
}

// Forward declarations for IDirect3DDevice8 vtable
extern const IDirect3DDevice8Vtbl device_vtbl;

// IDirect3D8 interface methods
static HRESULT D3DAPI d3d8_register_software_device(IDirect3D8 *This,
                                                    void *pInitializeFunction) {
  (void)This;
  (void)pInitializeFunction;
  return D3DERR_NOTAVAILABLE;
}

static UINT D3DAPI d3d8_get_adapter_count(IDirect3D8 *This) {
  (void)This;
  return 1;
}

static HRESULT D3DAPI
d3d8_get_adapter_identifier(IDirect3D8 *This, UINT Adapter, DWORD Flags,
                            D3DADAPTER_IDENTIFIER8 *pIdentifier) {
  (void)This;
  (void)Flags;
  
  if (Adapter != D3DADAPTER_DEFAULT || !pIdentifier)
    return D3DERR_INVALIDCALL;

  memset(pIdentifier, 0, sizeof(*pIdentifier));

  const char *renderer = (const char *)glGetString(GL_RENDERER);
  if (!renderer)
    renderer = "d3d8_to_gles";
  strncpy(pIdentifier->Driver, renderer, sizeof(pIdentifier->Driver) - 1);

  const char *vendor = (const char *)glGetString(GL_VENDOR);
  if (!vendor)
    vendor = "Unknown";
  strncpy(pIdentifier->Description, vendor,
          sizeof(pIdentifier->Description) - 1);

  return D3D_OK;
}

static UINT D3DAPI d3d8_get_adapter_mode_count(IDirect3D8 *This, UINT Adapter) {
  (void)This;
  (void)Adapter;
  return 1;
}

static HRESULT D3DAPI d3d8_enum_adapter_modes(IDirect3D8 *This, UINT Adapter,
                                              UINT Mode,
                                              D3DDISPLAYMODE *pMode) {
  (void)This;
  
  if (Adapter != D3DADAPTER_DEFAULT || Mode > 0 || !pMode)
    return D3DERR_INVALIDCALL;

  *pMode = g_current_display_mode;
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_get_adapter_display_mode(IDirect3D8 *This,
                                                    UINT Adapter,
                                                    D3DDISPLAYMODE *pMode) {
  (void)This;
  if (Adapter != D3DADAPTER_DEFAULT || !pMode)
    return D3DERR_INVALIDCALL;

  *pMode = g_current_display_mode;
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_check_device_type(IDirect3D8 *This, UINT Adapter,
                                             D3DDEVTYPE CheckType,
                                             D3DFORMAT DisplayFormat,
                                             D3DFORMAT BackBufferFormat,
                                             BOOL Windowed) {
  (void)This;
  (void)Adapter;
  (void)CheckType;
  (void)DisplayFormat;
  (void)BackBufferFormat;
  (void)Windowed;
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_check_device_format(IDirect3D8 *This, UINT Adapter,
                                               D3DDEVTYPE DeviceType,
                                               D3DFORMAT AdapterFormat,
                                               DWORD Usage,
                                               D3DRESOURCETYPE RType,
                                               D3DFORMAT CheckFormat) {
  (void)This;
  (void)Adapter;
  (void)DeviceType;
  (void)AdapterFormat;
  (void)Usage;
  (void)RType;
  (void)CheckFormat;
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_check_device_multi_sample_type(
    IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType,
    D3DFORMAT SurfaceFormat, BOOL Windowed,
    D3DMULTISAMPLE_TYPE MultiSampleType) {
  (void)This;
  (void)Adapter;
  (void)DeviceType;
  (void)SurfaceFormat;
  (void)Windowed;
  (void)MultiSampleType;
  return D3DERR_NOTAVAILABLE;
}

static HRESULT D3DAPI d3d8_check_depth_stencil_match(
    IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType,
    D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat,
    D3DFORMAT DepthStencilFormat) {
  (void)This;
  (void)Adapter;
  (void)DeviceType;
  (void)AdapterFormat;
  (void)RenderTargetFormat;
  (void)DepthStencilFormat;
  return D3D_OK;
}

static HRESULT D3DAPI d3d8_get_device_caps(IDirect3D8 *This, UINT Adapter,
                                           D3DDEVTYPE DeviceType,
                                           D3DCAPS8 *pCaps) {
  (void)This;
  (void)Adapter;
  fill_d3d_caps(pCaps, DeviceType);
  return D3D_OK;
}

static HMONITOR D3DAPI d3d8_get_adapter_monitor(IDirect3D8 *This,
                                                UINT Adapter) {
  (void)This;
  (void)Adapter;
  return NULL;
}

static HRESULT D3DAPI d3d8_create_device(
    IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow,
    DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters,
    IDirect3DDevice8 **ppReturnedDeviceInterface) {
  d3d8_gles_log("=== CreateDevice: D3D8 Device Creation Started ===\n");
  d3d8_gles_log("CreateDevice: Adapter=%u, DeviceType=%u, hFocusWindow=%p\n", 
                Adapter, DeviceType, (void*)hFocusWindow);
  d3d8_gles_log("CreateDevice: BehaviorFlags=0x%08X\n", BehaviorFlags);
  
  if (pPresentationParameters) {
    d3d8_gles_log("CreateDevice: PresentationParameters:\n");
    d3d8_gles_log("  BackBufferWidth=%u, BackBufferHeight=%u\n", 
                  pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight);
    d3d8_gles_log("  BackBufferFormat=%u, BackBufferCount=%u\n",
                  pPresentationParameters->BackBufferFormat, pPresentationParameters->BackBufferCount);
    d3d8_gles_log("  MultiSampleType=%u, SwapEffect=%u\n",
                  pPresentationParameters->MultiSampleType, pPresentationParameters->SwapEffect);
    d3d8_gles_log("  AutoDepthStencilFormat=%u, FullScreen_RefreshRateInHz=%u\n",
                  pPresentationParameters->AutoDepthStencilFormat, pPresentationParameters->FullScreen_RefreshRateInHz);
  } else {
    d3d8_gles_log("CreateDevice: ERROR - PresentationParameters is NULL\n");
  }
  
  if (Adapter != D3DADAPTER_DEFAULT || !pPresentationParameters ||
      DeviceType == D3DDEVTYPE_SW) {
    d3d8_gles_log("CreateDevice: ERROR - Invalid parameters (Adapter=%u, DeviceType=%u, pPresentationParameters=%p)\n",
                  Adapter, DeviceType, (void*)pPresentationParameters);
    return D3DERR_INVALIDCALL;
  }

  d3d8_gles_log("CreateDevice: Allocating GLES_Device structure...\n");
  GLES_Device *gles = calloc(1, sizeof(GLES_Device));
  if (!gles) {
    d3d8_gles_log("CreateDevice: ERROR - Failed to allocate GLES_Device memory\n");
    return D3DERR_OUTOFVIDEOMEMORY;
  }
  d3d8_gles_log("CreateDevice: GLES_Device allocated at %p\n", (void*)gles);

#ifdef USE_MICROGLES
  d3d8_gles_log("CreateDevice: Using MicroGLES implementation\n");
  d3d8_gles_log("CreateDevice: Initializing MicroGLES with framebuffer %ux%u\n",
                pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight);
  GL_init_with_framebuffer(pPresentationParameters->BackBufferWidth,
                           pPresentationParameters->BackBufferHeight);
  gles->display = EGL_NO_DISPLAY;
  gles->surface = EGL_NO_SURFACE;
  gles->context = EGL_NO_CONTEXT;
  d3d8_gles_log("CreateDevice: MicroGLES initialization completed\n");
#else
  d3d8_gles_log("CreateDevice: Using standard EGL implementation\n");
  d3d8_gles_log("CreateDevice: Getting EGL display...\n");
  gles->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  d3d8_gles_log("CreateDevice: EGL display = %p\n", (void*)gles->display);
  
  d3d8_gles_log("CreateDevice: Initializing EGL...\n");
  if (!eglInitialize(gles->display, NULL, NULL)) {
    d3d8_gles_log("CreateDevice: Standard EGL initialization failed, trying MESA surfaceless platform...\n");
    gles->display =
        eglGetPlatformDisplay(EGL_PLATFORM_SURFACELESS_MESA,
                              (void *)(intptr_t)EGL_DEFAULT_DISPLAY, NULL);
    d3d8_gles_log("CreateDevice: MESA platform display = %p\n", (void*)gles->display);
    
    if (!eglInitialize(gles->display, NULL, NULL)) {
      d3d8_gles_log("CreateDevice: ERROR - Both EGL initialization methods failed\n");
      free(gles);
      return D3DERR_INVALIDCALL;
    }
    d3d8_gles_log("CreateDevice: MESA EGL initialization succeeded\n");
  } else {
    d3d8_gles_log("CreateDevice: Standard EGL initialization succeeded\n");
  }

  BOOL want_window = hFocusWindow != NULL;
  d3d8_gles_log("CreateDevice: Choosing EGL config (want_window=%s)...\n", want_window ? "true" : "false");
  EGLConfig config =
      choose_egl_config(gles->display, pPresentationParameters, want_window);
  if (!config) {
    d3d8_gles_log("CreateDevice: ERROR - Failed to choose suitable EGL config\n");
    eglTerminate(gles->display);
    free(gles);
    return D3DERR_INVALIDCALL;
  }
  d3d8_gles_log("CreateDevice: EGL config chosen successfully\n");

  d3d8_gles_log("CreateDevice: Creating EGL surface...\n");
  if (hFocusWindow) {
    d3d8_gles_log("CreateDevice: Creating window surface for hWnd=%p\n", (void*)hFocusWindow);
    gles->surface = eglCreateWindowSurface(
        gles->display, config, (EGLNativeWindowType)(uintptr_t)hFocusWindow,
        NULL);
  } else {
    d3d8_gles_log("CreateDevice: Creating pbuffer surface %ux%u\n",
                  pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight);
    const EGLint pbuffer_attribs[] = {
        EGL_WIDTH, pPresentationParameters->BackBufferWidth, EGL_HEIGHT,
        pPresentationParameters->BackBufferHeight, EGL_NONE};
    gles->surface =
        eglCreatePbufferSurface(gles->display, config, pbuffer_attribs);
  }
  d3d8_gles_log("CreateDevice: EGL surface = %p\n", (void*)gles->surface);
  
  d3d8_gles_log("CreateDevice: Creating EGL context...\n");
  gles->context = eglCreateContext(gles->display, config, EGL_NO_CONTEXT, NULL);
  d3d8_gles_log("CreateDevice: EGL context = %p\n", (void*)gles->context);
  
  d3d8_gles_log("CreateDevice: Making EGL context current...\n");
  if (!gles->surface || !gles->context ||
      !eglMakeCurrent(gles->display, gles->surface, gles->surface,
                      gles->context)) {
    d3d8_gles_log("CreateDevice: ERROR - Failed to create surface/context or make current\n");
    d3d8_gles_log("CreateDevice: surface=%p, context=%p\n", (void*)gles->surface, (void*)gles->context);
    if (gles->context)
      eglDestroyContext(gles->display, gles->context);
    if (gles->surface)
      eglDestroySurface(gles->display, gles->surface);
    eglTerminate(gles->display);
    free(gles);
    return D3DERR_INVALIDCALL;
  }
  d3d8_gles_log("CreateDevice: EGL context made current successfully\n");
#endif

  d3d8_gles_log("CreateDevice: Setting up OpenGL ES viewport and default state...\n");
  gles->viewport.X = 0;
  gles->viewport.Y = 0;
  gles->viewport.Width = pPresentationParameters->BackBufferWidth;
  gles->viewport.Height = pPresentationParameters->BackBufferHeight;
  gles->viewport.MinZ = 0.0f;
  gles->viewport.MaxZ = 1.0f;
  d3d8_gles_log("CreateDevice: Viewport set to (%d,%d) %ux%u depth %.3f-%.3f\n",
                gles->viewport.X, gles->viewport.Y, gles->viewport.Width, gles->viewport.Height,
                gles->viewport.MinZ, gles->viewport.MaxZ);
  glViewport(0, 0, gles->viewport.Width, gles->viewport.Height);
  D3DXMatrixIdentity(&gles->world_matrix);
  D3DXMatrixIdentity(&gles->view_matrix);
  D3DXMatrixIdentity(&gles->projection_matrix);
  gles->src_blend = GL_ONE;
  gles->dest_blend = GL_ZERO;
  gles->alpha_func = GL_ALWAYS;
  gles->alpha_ref = 0.0f;
  gles->depth_func = GL_LEQUAL;
  gles->fog_mode = GL_EXP;
  gles->stencil_test = GL_FALSE;
  gles->stencil_func = GL_ALWAYS;
  gles->stencil_ref = 0;
  gles->stencil_mask = 0xFFFFFFFFu;
  gles->stencil_fail = GL_KEEP;
  gles->stencil_zfail = GL_KEEP;
  gles->stencil_pass = GL_KEEP;
  gles->texcoord_index0 = 0;
  gles->present_params = *pPresentationParameters;
  gles->display_mode.Width = pPresentationParameters->BackBufferWidth;
  gles->display_mode.Height = pPresentationParameters->BackBufferHeight;
  gles->display_mode.Format = pPresentationParameters->BackBufferFormat;
  gles->display_mode.RefreshRate =
      pPresentationParameters->FullScreen_RefreshRateInHz;

  g_current_display_mode = gles->display_mode;

  d3d8_gles_log("CreateDevice: Allocating IDirect3DDevice8 interface...\n");
  DeviceImpl *device = calloc(1, sizeof(DeviceImpl));
  if (!device) {
    d3d8_gles_log("CreateDevice: ERROR - Failed to allocate IDirect3DDevice8 interface\n");
    eglDestroyContext(gles->display, gles->context);
    eglDestroySurface(gles->display, gles->surface);
    eglTerminate(gles->display);
    free(gles);
    return D3DERR_OUTOFVIDEOMEMORY;
  }
  d3d8_gles_log("CreateDevice: IDirect3DDevice8 allocated at %p\n", (void*)device);

  d3d8_gles_log("CreateDevice: Setting up device vtable and linking structures...\n");
  device->lpVtbl = (void*)&device_vtbl;
  device->ref_count = 1;
  device->gles = gles;
  device->d3d8 = This;

  d3d8_gles_log("CreateDevice: SUCCESS - Device creation completed\n");
  d3d8_gles_log("CreateDevice: Device interface = %p, GLES device = %p\n", (void*)device, (void*)gles);
  d3d8_gles_log("=== CreateDevice: D3D8 Device Creation FINISHED ===\n");
  *ppReturnedDeviceInterface = (IDirect3DDevice8*)device;
  return D3D_OK;
}

// IUnknown-style helpers for IDirect3D8
static HRESULT D3DAPI d3d8_query_interface(IDirect3D8 *This, REFIID riid,
                                           void **ppv) {
  return common_query_interface(This, riid, ppv);
}
static ULONG D3DAPI d3d8_add_ref(IDirect3D8 *This) {
  return common_add_ref(This);
}
static ULONG D3DAPI d3d8_release(IDirect3D8 *This) {
  return common_release(This);
}

// Main entry point to create IDirect3D8 interface
IDirect3D8 *WINAPI Direct3DCreate8(UINT SDKVersion) {
  d3d8_gles_log("=== DirectX D3D8 to OpenGL ES Translation Layer ===\n");
  d3d8_gles_log("Direct3DCreate8: Called with SDK version %u (expected %u)\n", SDKVersion, D3D_SDK_VERSION);
  
  if (SDKVersion != D3D_SDK_VERSION) {
    d3d8_gles_log("Direct3DCreate8: ERROR - Invalid SDK version: %u\n", SDKVersion);
    return NULL;
  }

  d3d8_gles_log("Direct3DCreate8: Allocating D3D8 interface object...\n");
  D3D8Impl *d3d = calloc(1, sizeof(D3D8Impl));
  if (!d3d) {
    d3d8_gles_log("Direct3DCreate8: ERROR - Failed to allocate memory for D3D8 interface\n");
    return NULL;
  }

  static const IDirect3D8Vtbl d3d_vtbl = {
      D3D8_CAST(d3d8_query_interface),
      D3D8_CAST(d3d8_add_ref),
      D3D8_CAST(d3d8_release),
      D3D8_CAST(d3d8_register_software_device),
      D3D8_CAST(d3d8_get_adapter_count),
      D3D8_CAST(d3d8_get_adapter_identifier),
      D3D8_CAST(d3d8_get_adapter_mode_count),
      D3D8_CAST(d3d8_enum_adapter_modes),
      D3D8_CAST(d3d8_get_adapter_display_mode),
      D3D8_CAST(d3d8_check_device_type),
      D3D8_CAST(d3d8_check_device_format),
      D3D8_CAST(d3d8_check_device_multi_sample_type),
      D3D8_CAST(d3d8_check_depth_stencil_match),
      D3D8_CAST(d3d8_get_device_caps),
      D3D8_CAST(d3d8_get_adapter_monitor),
      D3D8_CAST(d3d8_create_device)};
  d3d->lpVtbl = (void*)&d3d_vtbl;

  return (IDirect3D8*)d3d;
}