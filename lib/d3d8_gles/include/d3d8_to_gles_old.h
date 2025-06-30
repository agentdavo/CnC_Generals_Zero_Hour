// include/d3d8_to_gles.h
#ifndef D3D8_TO_GLES_H
#define D3D8_TO_GLES_H

#include "GLES/gl.h"
#include "GLES/glext.h"
#include "EGL/egl.h"

#include <limits.h>
#include <float.h>

#include "windows.h"
#define COM_NO_WINDOWS_H
#include "objbase.h"
// Skip d3d8_defs.h to avoid conflicts with min-dx8-sdk

// Define WINAPI for cross-platform compatibility
#ifndef WINAPI
#define WINAPI
#endif

// Define D3DAPI as WINAPI for D3DX functions
#ifndef D3DAPI
#define D3DAPI WINAPI
#endif

// Include D3D8 types first 
#include "../lib/min-dx8-sdk/d3d8types.h"
#include "../lib/min-dx8-sdk/d3d8caps.h"
#include "../lib/min-dx8-sdk/d3dx8core.h"
#include "../lib/min-dx8-sdk/d3dx8mesh.h"
#include "../lib/min-dx8-sdk/d3dx8math.h"

// Forward declarations for GLES types
typedef struct GLES_Device GLES_Device;
typedef struct GLES_Buffer GLES_Buffer;
typedef struct GLES_Texture GLES_Texture;

// D3DX vector and quaternion types (defined early for GLES_Device)
typedef struct _D3DXVECTOR4 {
    float x, y, z, w;
} D3DXVECTOR4;

// GLES Device structure
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

// GLES Buffer structure  
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

// GLES Texture structure
struct GLES_Texture {
    GLuint tex_id;
    UINT width;
    UINT height;
    UINT levels;
    D3DFORMAT format;
    void *temp_buffer;
};

// D3DX type aliases (map to D3D standard types)
typedef D3DVECTOR D3DXVECTOR3;
typedef D3DMATRIX D3DXMATRIX;

// D3DX quaternion types
typedef struct _D3DXQUATERNION {
    float x, y, z, w;
} D3DXQUATERNION;

// D3DX mesh and buffer interfaces
typedef struct ID3DXMesh ID3DXMesh;
typedef struct ID3DXBuffer ID3DXBuffer;
typedef struct ID3DXMatrixStack ID3DXMatrixStack;
typedef struct ID3DXFont ID3DXFont;
typedef struct ID3DXSprite ID3DXSprite;
typedef struct ID3DXRenderToSurface ID3DXRenderToSurface;
typedef struct ID3DXRenderToEnvMap ID3DXRenderToEnvMap;
typedef struct ID3DXEffect ID3DXEffect;
typedef struct ID3DXSkinMesh ID3DXSkinMesh;
typedef struct ID3DXBaseMesh ID3DXBaseMesh;
typedef struct ID3DXPMesh ID3DXPMesh;
typedef struct ID3DXSPMesh ID3DXSPMesh;

// D3DX type aliases  
typedef ID3DXMesh* LPD3DXMESH;
typedef ID3DXBuffer* LPD3DXBUFFER;
typedef ID3DXMatrixStack* LPD3DXMATRIXSTACK;
typedef ID3DXFont* LPD3DXFONT;
typedef ID3DXSprite* LPD3DXSPRITE;
typedef ID3DXRenderToSurface* LPD3DXRENDERTOSURFACE;
typedef ID3DXRenderToEnvMap* LPD3DXRenderToEnvMap;
typedef ID3DXEffect* LPD3DXEFFECT;
typedef ID3DXSkinMesh* LPD3DXSKINMESH;
typedef ID3DXBaseMesh* LPD3DXBASEMESH;
typedef ID3DXPMesh* LPD3DXPMESH;
typedef ID3DXSPMesh* LPD3DXSPMESH;

// D3DX structures
typedef struct _D3DXATTRIBUTERANGE {
    DWORD AttribId;
    DWORD FaceStart;
    DWORD FaceCount;
    DWORD VertexStart;
    DWORD VertexCount;
} D3DXATTRIBUTERANGE;

typedef struct _D3DXATTRIBUTEWEIGHTS {
    FLOAT Position;
    FLOAT Boundary;
    FLOAT Normal;
    FLOAT Diffuse;
    FLOAT Specular;
    FLOAT Texcoord[8];
    FLOAT Tangent; 
    FLOAT Binormal;
} D3DXATTRIBUTEWEIGHTS;
typedef D3DXATTRIBUTEWEIGHTS* LPD3DXATTRIBUTEWEIGHTS;

typedef struct _D3DXIMAGE_INFO {
    UINT Width;
    UINT Height;
    UINT Depth;
    UINT MipLevels;
    D3DFORMAT Format;
    D3DRESOURCETYPE ResourceType;
    D3DFORMAT ImageFileFormat;
} D3DXIMAGE_INFO;

// D3DX constants
#define D3DX_DEFAULT ((UINT)-1)
#define D3DXMESH_MANAGED 0x400
#define MAX_FVF_DECL_SIZE 20

// D3DX error codes
#define D3DXERR_NOTAVAILABLE        MAKE_HRESULT(1, 0x876, 2154)
#define D3DXERR_INVALIDDATA         MAKE_HRESULT(1, 0x876, 2155)
#define D3DXERR_INVALIDMESH         MAKE_HRESULT(1, 0x876, 2156)
#define D3DXERR_SKINNINGNOTSUPPORTED MAKE_HRESULT(1, 0x876, 2157)

// Forward declarations for all interfaces
typedef struct IDirect3D8 IDirect3D8;
typedef struct IDirect3DDevice8 IDirect3DDevice8;
typedef struct IDirect3DVertexBuffer8 IDirect3DVertexBuffer8;
typedef struct IDirect3DIndexBuffer8 IDirect3DIndexBuffer8;
typedef struct IDirect3DTexture8 IDirect3DTexture8;
typedef struct IDirect3DBaseTexture8 IDirect3DBaseTexture8;
typedef struct IDirect3DSurface8 IDirect3DSurface8;
typedef struct IDirect3DSwapChain8 IDirect3DSwapChain8;
typedef struct IDirect3DVolumeTexture8 IDirect3DVolumeTexture8;
typedef struct IDirect3DCubeTexture8 IDirect3DCubeTexture8;

// Convenience typedefs (define early so vtables can use them)
typedef IDirect3D8 *LPDIRECT3D8;
typedef IDirect3DDevice8 *LPDIRECT3DDEVICE8;
typedef IDirect3DVertexBuffer8 *LPDIRECT3DVERTEXBUFFER8;
typedef IDirect3DIndexBuffer8 *LPDIRECT3DINDEXBUFFER8;
typedef IDirect3DTexture8 *LPDIRECT3DTEXTURE8;
typedef IDirect3DBaseTexture8 *LPDIRECT3DBASETEXTURE8;
typedef IDirect3DSurface8 *LPDIRECT3DSURFACE8;

// Forward declarations for vtables
typedef struct IDirect3D8Vtbl IDirect3D8Vtbl;
typedef struct IDirect3DDevice8Vtbl IDirect3DDevice8Vtbl;
typedef struct IDirect3DVertexBuffer8Vtbl IDirect3DVertexBuffer8Vtbl;
typedef struct IDirect3DIndexBuffer8Vtbl IDirect3DIndexBuffer8Vtbl;
typedef struct IDirect3DTexture8Vtbl IDirect3DTexture8Vtbl;

// Vtable definitions
struct IDirect3D8Vtbl {
    HRESULT (WINAPI *QueryInterface)(IDirect3D8 *This, REFIID riid, void **ppvObj);
    ULONG (WINAPI *AddRef)(IDirect3D8 *This);
    ULONG (WINAPI *Release)(IDirect3D8 *This);
    HRESULT (WINAPI *RegisterSoftwareDevice)(IDirect3D8 *This, void *pInitializeFunction);
    UINT (WINAPI *GetAdapterCount)(IDirect3D8 *This);
    HRESULT (WINAPI *GetAdapterIdentifier)(IDirect3D8 *This, UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8 *pIdentifier);
    UINT (WINAPI *GetAdapterModeCount)(IDirect3D8 *This, UINT Adapter);
    HRESULT (WINAPI *EnumAdapterModes)(IDirect3D8 *This, UINT Adapter, UINT Mode, D3DDISPLAYMODE *pMode);
    HRESULT (WINAPI *GetAdapterDisplayMode)(IDirect3D8 *This, UINT Adapter, D3DDISPLAYMODE *pMode);
    HRESULT (WINAPI *CheckDeviceType)(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed);
    HRESULT (WINAPI *CheckDeviceFormat)(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
    HRESULT (WINAPI *CheckDeviceMultiSampleType)(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType);
    HRESULT (WINAPI *CheckDepthStencilMatch)(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
    HRESULT (WINAPI *GetDeviceCaps)(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8 *pCaps);
    HMONITOR (WINAPI *GetAdapterMonitor)(IDirect3D8 *This, UINT Adapter);
    HRESULT (WINAPI *CreateDevice)(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice8 **ppReturnedDeviceInterface);
};

struct IDirect3DDevice8Vtbl {
    HRESULT (WINAPI *QueryInterface)(IDirect3DDevice8 *This, REFIID riid, void **ppvObj);
    ULONG (WINAPI *AddRef)(IDirect3DDevice8 *This);
    ULONG (WINAPI *Release)(IDirect3DDevice8 *This);
    HRESULT (WINAPI *TestCooperativeLevel)(IDirect3DDevice8 *This);
    UINT (WINAPI *GetAvailableTextureMem)(IDirect3DDevice8 *This);
    HRESULT (WINAPI *ResourceManagerDiscardBytes)(IDirect3DDevice8 *This, DWORD Bytes);
    HRESULT (WINAPI *GetDirect3D)(IDirect3DDevice8 *This, IDirect3D8 **ppD3D8);
    HRESULT (WINAPI *GetDeviceCaps)(IDirect3DDevice8 *This, D3DCAPS8 *pCaps);
    HRESULT (WINAPI *GetDisplayMode)(IDirect3DDevice8 *This, D3DDISPLAYMODE *pMode);
    HRESULT (WINAPI *GetCreationParameters)(IDirect3DDevice8 *This, D3DDEVICE_CREATION_PARAMETERS *pParameters);
    HRESULT (WINAPI *SetCursorProperties)(IDirect3DDevice8 *This, UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8 *pCursorBitmap);
    void (WINAPI *SetCursorPosition)(IDirect3DDevice8 *This, UINT XScreenSpace, UINT YScreenSpace, DWORD Flags);
    BOOL (WINAPI *ShowCursor)(IDirect3DDevice8 *This, BOOL bShow);
    HRESULT (WINAPI *CreateAdditionalSwapChain)(IDirect3DDevice8 *This, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DSwapChain8 **pSwapChain);
    HRESULT (WINAPI *Reset)(IDirect3DDevice8 *This, D3DPRESENT_PARAMETERS *pPresentationParameters);
    HRESULT (WINAPI *Present)(IDirect3DDevice8 *This, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
    HRESULT (WINAPI *GetBackBuffer)(IDirect3DDevice8 *This, UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 **ppBackBuffer);
    HRESULT (WINAPI *GetRasterStatus)(IDirect3DDevice8 *This, D3DRASTER_STATUS *pRasterStatus);
    void (WINAPI *SetGammaRamp)(IDirect3DDevice8 *This, DWORD Flags, const D3DGAMMARAMP *pRamp);
    void (WINAPI *GetGammaRamp)(IDirect3DDevice8 *This, D3DGAMMARAMP *pRamp);
    HRESULT (WINAPI *CreateTexture)(IDirect3DDevice8 *This, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8 **ppTexture);
    HRESULT (WINAPI *CreateVolumeTexture)(IDirect3DDevice8 *This, UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8 **ppVolumeTexture);
    HRESULT (WINAPI *CreateCubeTexture)(IDirect3DDevice8 *This, UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8 **ppCubeTexture);
    HRESULT (WINAPI *CreateVertexBuffer)(IDirect3DDevice8 *This, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8 **ppVertexBuffer);
    HRESULT (WINAPI *CreateIndexBuffer)(IDirect3DDevice8 *This, UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8 **ppIndexBuffer);
    HRESULT (WINAPI *CreateRenderTarget)(IDirect3DDevice8 *This, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8 **ppSurface);
    HRESULT (WINAPI *CreateDepthStencilSurface)(IDirect3DDevice8 *This, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8 **ppSurface);
    HRESULT (WINAPI *CreateImageSurface)(IDirect3DDevice8 *This, UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8 **ppSurface);
    HRESULT (WINAPI *CopyRects)(IDirect3DDevice8 *This, IDirect3DSurface8 *pSourceSurface, const RECT *pSourceRectsArray, UINT cRects, IDirect3DSurface8 *pDestinationSurface, const POINT *pDestPointsArray);
    HRESULT (WINAPI *UpdateTexture)(IDirect3DDevice8 *This, IDirect3DBaseTexture8 *pSourceTexture, IDirect3DBaseTexture8 *pDestinationTexture);
    HRESULT (WINAPI *GetFrontBuffer)(IDirect3DDevice8 *This, IDirect3DSurface8 *pDestSurface);
    HRESULT (WINAPI *SetRenderTarget)(IDirect3DDevice8 *This, IDirect3DSurface8 *pRenderTarget, IDirect3DSurface8 *pNewZStencil);
    HRESULT (WINAPI *GetRenderTarget)(IDirect3DDevice8 *This, IDirect3DSurface8 **ppRenderTarget);
    HRESULT (WINAPI *GetDepthStencilSurface)(IDirect3DDevice8 *This, IDirect3DSurface8 **ppZStencilSurface);
    HRESULT (WINAPI *BeginScene)(IDirect3DDevice8 *This);
    HRESULT (WINAPI *EndScene)(IDirect3DDevice8 *This);
    HRESULT (WINAPI *Clear)(IDirect3DDevice8 *This, DWORD Count, const D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
    HRESULT (WINAPI *SetTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix);
    HRESULT (WINAPI *GetTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix);
    HRESULT (WINAPI *MultiplyTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix);
    HRESULT (WINAPI *SetViewport)(IDirect3DDevice8 *This, const D3DVIEWPORT8 *pViewport);
    HRESULT (WINAPI *GetViewport)(IDirect3DDevice8 *This, D3DVIEWPORT8 *pViewport);
    HRESULT (WINAPI *SetMaterial)(IDirect3DDevice8 *This, const D3DMATERIAL8 *pMaterial);
    HRESULT (WINAPI *GetMaterial)(IDirect3DDevice8 *This, D3DMATERIAL8 *pMaterial);
    HRESULT (WINAPI *SetLight)(IDirect3DDevice8 *This, DWORD Index, const D3DLIGHT8 *pLight);
    HRESULT (WINAPI *GetLight)(IDirect3DDevice8 *This, DWORD Index, D3DLIGHT8 *pLight);
    HRESULT (WINAPI *LightEnable)(IDirect3DDevice8 *This, DWORD Index, BOOL Enable);
    HRESULT (WINAPI *GetLightEnable)(IDirect3DDevice8 *This, DWORD Index, BOOL *pEnable);
    HRESULT (WINAPI *SetClipPlane)(IDirect3DDevice8 *This, DWORD Index, const float *pPlane);
    HRESULT (WINAPI *GetClipPlane)(IDirect3DDevice8 *This, DWORD Index, float *pPlane);
    HRESULT (WINAPI *SetRenderState)(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, DWORD Value);
    HRESULT (WINAPI *GetRenderState)(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, DWORD *pValue);
    HRESULT (WINAPI *BeginStateBlock)(IDirect3DDevice8 *This);
    HRESULT (WINAPI *EndStateBlock)(IDirect3DDevice8 *This, DWORD *pToken);
    HRESULT (WINAPI *ApplyStateBlock)(IDirect3DDevice8 *This, DWORD Token);
    HRESULT (WINAPI *CaptureStateBlock)(IDirect3DDevice8 *This, DWORD Token);
    HRESULT (WINAPI *DeleteStateBlock)(IDirect3DDevice8 *This, DWORD Token);
    HRESULT (WINAPI *CreateStateBlock)(IDirect3DDevice8 *This, D3DSTATEBLOCKTYPE Type, DWORD *pToken);
    HRESULT (WINAPI *SetClipStatus)(IDirect3DDevice8 *This, const D3DCLIPSTATUS8 *pClipStatus);
    HRESULT (WINAPI *GetClipStatus)(IDirect3DDevice8 *This, D3DCLIPSTATUS8 *pClipStatus);
    HRESULT (WINAPI *GetTexture)(IDirect3DDevice8 *This, DWORD Stage, IDirect3DBaseTexture8 **ppTexture);
    HRESULT (WINAPI *SetTexture)(IDirect3DDevice8 *This, DWORD Stage, IDirect3DBaseTexture8 *pTexture);
    HRESULT (WINAPI *GetTextureStageState)(IDirect3DDevice8 *This, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue);
    HRESULT (WINAPI *SetTextureStageState)(IDirect3DDevice8 *This, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
    HRESULT (WINAPI *ValidateDevice)(IDirect3DDevice8 *This, DWORD *pNumPasses);
    HRESULT (WINAPI *GetInfo)(IDirect3DDevice8 *This, DWORD DevInfoID, void *pDevInfoStruct, DWORD DevInfoStructSize);
    HRESULT (WINAPI *SetPaletteEntries)(IDirect3DDevice8 *This, UINT PaletteNumber, const PALETTEENTRY *pEntries);
    HRESULT (WINAPI *GetPaletteEntries)(IDirect3DDevice8 *This, UINT PaletteNumber, PALETTEENTRY *pEntries);
    HRESULT (WINAPI *SetCurrentTexturePalette)(IDirect3DDevice8 *This, UINT PaletteNumber);
    HRESULT (WINAPI *GetCurrentTexturePalette)(IDirect3DDevice8 *This, UINT *PaletteNumber);
    HRESULT (WINAPI *DrawPrimitive)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
    HRESULT (WINAPI *DrawIndexedPrimitive)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, UINT minIndex, UINT NumVertices, UINT startIndex, UINT primCount);
    HRESULT (WINAPI *DrawPrimitiveUP)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void *pVertexStreamZeroData, UINT VertexStreamZeroStride);
    HRESULT (WINAPI *DrawIndexedPrimitiveUP)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertexIndices, UINT PrimitiveCount, const void *pIndexData, D3DFORMAT IndexDataFormat, const void *pVertexStreamZeroData, UINT VertexStreamZeroStride);
    HRESULT (WINAPI *ProcessVertices)(IDirect3DDevice8 *This, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8 *pDestBuffer, DWORD Flags);
    HRESULT (WINAPI *CreateVertexShader)(IDirect3DDevice8 *This, const DWORD *pDeclaration, const DWORD *pFunction, DWORD *pHandle, DWORD Usage);
    HRESULT (WINAPI *SetVertexShader)(IDirect3DDevice8 *This, DWORD Handle);
    HRESULT (WINAPI *GetVertexShader)(IDirect3DDevice8 *This, DWORD *pHandle);
    HRESULT (WINAPI *DeleteVertexShader)(IDirect3DDevice8 *This, DWORD Handle);
    HRESULT (WINAPI *SetVertexShaderConstant)(IDirect3DDevice8 *This, DWORD Register, const void *pConstantData, DWORD ConstantCount);
    HRESULT (WINAPI *GetVertexShaderConstant)(IDirect3DDevice8 *This, DWORD Register, void *pConstantData, DWORD ConstantCount);
    HRESULT (WINAPI *GetVertexShaderDeclaration)(IDirect3DDevice8 *This, DWORD Handle, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *GetVertexShaderFunction)(IDirect3DDevice8 *This, DWORD Handle, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *SetStreamSource)(IDirect3DDevice8 *This, UINT StreamNumber, IDirect3DVertexBuffer8 *pStreamData, UINT Stride);
    HRESULT (WINAPI *GetStreamSource)(IDirect3DDevice8 *This, UINT StreamNumber, IDirect3DVertexBuffer8 **ppStreamData, UINT *pStride);
    HRESULT (WINAPI *SetIndices)(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 *pIndexData, UINT BaseVertexIndex);
    HRESULT (WINAPI *GetIndices)(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 **ppIndexData, UINT *pBaseVertexIndex);
    HRESULT (WINAPI *CreatePixelShader)(IDirect3DDevice8 *This, const DWORD *pFunction, DWORD *pHandle);
    HRESULT (WINAPI *SetPixelShader)(IDirect3DDevice8 *This, DWORD Handle);
    HRESULT (WINAPI *GetPixelShader)(IDirect3DDevice8 *This, DWORD *pHandle);
    HRESULT (WINAPI *DeletePixelShader)(IDirect3DDevice8 *This, DWORD Handle);
    HRESULT (WINAPI *SetPixelShaderConstant)(IDirect3DDevice8 *This, DWORD Register, const void *pConstantData, DWORD ConstantCount);
    HRESULT (WINAPI *GetPixelShaderConstant)(IDirect3DDevice8 *This, DWORD Register, void *pConstantData, DWORD ConstantCount);
    HRESULT (WINAPI *GetPixelShaderFunction)(IDirect3DDevice8 *This, DWORD Handle, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *DrawRectPatch)(IDirect3DDevice8 *This, UINT Handle, const float *pNumSegs, const D3DRECTPATCH_INFO *pRectPatchInfo);
    HRESULT (WINAPI *DrawTriPatch)(IDirect3DDevice8 *This, UINT Handle, const float *pNumSegs, const D3DTRIPATCH_INFO *pTriPatchInfo);
    HRESULT (WINAPI *DeletePatch)(IDirect3DDevice8 *This, UINT Handle);
};

struct IDirect3DVertexBuffer8Vtbl {
    HRESULT (WINAPI *QueryInterface)(IDirect3DVertexBuffer8 *This, REFIID riid, void **ppvObj);
    ULONG (WINAPI *AddRef)(IDirect3DVertexBuffer8 *This);
    ULONG (WINAPI *Release)(IDirect3DVertexBuffer8 *This);
    HRESULT (WINAPI *GetDevice)(IDirect3DVertexBuffer8 *This, IDirect3DDevice8 **ppDevice);
    HRESULT (WINAPI *SetPrivateData)(IDirect3DVertexBuffer8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags);
    HRESULT (WINAPI *GetPrivateData)(IDirect3DVertexBuffer8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *FreePrivateData)(IDirect3DVertexBuffer8 *This, REFGUID refguid);
    DWORD (WINAPI *SetPriority)(IDirect3DVertexBuffer8 *This, DWORD PriorityNew);
    DWORD (WINAPI *GetPriority)(IDirect3DVertexBuffer8 *This);
    void (WINAPI *PreLoad)(IDirect3DVertexBuffer8 *This);
    D3DRESOURCETYPE (WINAPI *GetType)(IDirect3DVertexBuffer8 *This);
    HRESULT (WINAPI *Lock)(IDirect3DVertexBuffer8 *This, UINT OffsetToLock, UINT SizeToLock, BYTE **ppbData, DWORD Flags);
    HRESULT (WINAPI *Unlock)(IDirect3DVertexBuffer8 *This);
    HRESULT (WINAPI *GetDesc)(IDirect3DVertexBuffer8 *This, D3DVERTEXBUFFER_DESC *pDesc);
};

struct IDirect3DIndexBuffer8Vtbl {
    HRESULT (WINAPI *QueryInterface)(IDirect3DIndexBuffer8 *This, REFIID riid, void **ppvObj);
    ULONG (WINAPI *AddRef)(IDirect3DIndexBuffer8 *This);
    ULONG (WINAPI *Release)(IDirect3DIndexBuffer8 *This);
    HRESULT (WINAPI *GetDevice)(IDirect3DIndexBuffer8 *This, IDirect3DDevice8 **ppDevice);
    HRESULT (WINAPI *SetPrivateData)(IDirect3DIndexBuffer8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags);
    HRESULT (WINAPI *GetPrivateData)(IDirect3DIndexBuffer8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *FreePrivateData)(IDirect3DIndexBuffer8 *This, REFGUID refguid);
    DWORD (WINAPI *SetPriority)(IDirect3DIndexBuffer8 *This, DWORD PriorityNew);
    DWORD (WINAPI *GetPriority)(IDirect3DIndexBuffer8 *This);
    void (WINAPI *PreLoad)(IDirect3DIndexBuffer8 *This);
    D3DRESOURCETYPE (WINAPI *GetType)(IDirect3DIndexBuffer8 *This);
    HRESULT (WINAPI *Lock)(IDirect3DIndexBuffer8 *This, UINT OffsetToLock, UINT SizeToLock, BYTE **ppbData, DWORD Flags);
    HRESULT (WINAPI *Unlock)(IDirect3DIndexBuffer8 *This);
    HRESULT (WINAPI *GetDesc)(IDirect3DIndexBuffer8 *This, D3DINDEXBUFFER_DESC *pDesc);
};

struct IDirect3DTexture8Vtbl {
    HRESULT (WINAPI *QueryInterface)(IDirect3DTexture8 *This, REFIID riid, void **ppvObj);
    ULONG (WINAPI *AddRef)(IDirect3DTexture8 *This);
    ULONG (WINAPI *Release)(IDirect3DTexture8 *This);
    HRESULT (WINAPI *GetDevice)(IDirect3DTexture8 *This, IDirect3DDevice8 **ppDevice);
    HRESULT (WINAPI *SetPrivateData)(IDirect3DTexture8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags);
    HRESULT (WINAPI *GetPrivateData)(IDirect3DTexture8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *FreePrivateData)(IDirect3DTexture8 *This, REFGUID refguid);
    DWORD (WINAPI *SetPriority)(IDirect3DTexture8 *This, DWORD PriorityNew);
    DWORD (WINAPI *GetPriority)(IDirect3DTexture8 *This);
    void (WINAPI *PreLoad)(IDirect3DTexture8 *This);
    D3DRESOURCETYPE (WINAPI *GetType)(IDirect3DTexture8 *This);
    DWORD (WINAPI *SetLOD)(IDirect3DTexture8 *This, DWORD LODNew);
    DWORD (WINAPI *GetLOD)(IDirect3DTexture8 *This);
    DWORD (WINAPI *GetLevelCount)(IDirect3DTexture8 *This);
    HRESULT (WINAPI *GetLevelDesc)(IDirect3DTexture8 *This, UINT Level, D3DSURFACE_DESC *pDesc);
    HRESULT (WINAPI *GetSurfaceLevel)(IDirect3DTexture8 *This, UINT Level, IDirect3DSurface8 **ppSurfaceLevel);
    HRESULT (WINAPI *LockRect)(IDirect3DTexture8 *This, UINT Level, D3DLOCKED_RECT *pLockedRect, const RECT *pRect, DWORD Flags);
    HRESULT (WINAPI *UnlockRect)(IDirect3DTexture8 *This, UINT Level);
    HRESULT (WINAPI *AddDirtyRect)(IDirect3DTexture8 *This, const RECT *pDirtyRect);
};

// Define actual structures with vtable pointers
struct IDirect3D8 {
    const IDirect3D8Vtbl *lpVtbl;
};

struct IDirect3DDevice8 {
    const IDirect3DDevice8Vtbl *lpVtbl;
    ULONG ref_count;
    GLES_Device *gles;
    IDirect3D8 *d3d8;
};

struct IDirect3DVertexBuffer8 {
    const IDirect3DVertexBuffer8Vtbl *lpVtbl;
    ULONG ref_count;
    GLES_Buffer *buffer;
    LPDIRECT3DDEVICE8 device;
};

struct IDirect3DIndexBuffer8 {
    const IDirect3DIndexBuffer8Vtbl *lpVtbl;
    ULONG ref_count;
    GLES_Buffer *buffer;
    LPDIRECT3DDEVICE8 device;
};

struct IDirect3DTexture8 {
    const IDirect3DTexture8Vtbl *lpVtbl;
    ULONG ref_count;
    GLES_Texture *texture;
    LPDIRECT3DDEVICE8 device;
};

// D3DX interface vtables
typedef struct ID3DXMeshVtbl ID3DXMeshVtbl;
typedef struct ID3DXBufferVtbl ID3DXBufferVtbl;
typedef struct ID3DXMatrixStackVtbl ID3DXMatrixStackVtbl;

struct ID3DXBufferVtbl {
    HRESULT (WINAPI *QueryInterface)(ID3DXBuffer *This, REFIID iid, void **ppvObj);
    ULONG (WINAPI *AddRef)(ID3DXBuffer *This);
    ULONG (WINAPI *Release)(ID3DXBuffer *This);
    LPVOID (WINAPI *GetBufferPointer)(ID3DXBuffer *This);
    DWORD (WINAPI *GetBufferSize)(ID3DXBuffer *This);
};

struct ID3DXMatrixStackVtbl {
    HRESULT (WINAPI *QueryInterface)(ID3DXMatrixStack *This, REFIID iid, void **ppvObj);
    ULONG (WINAPI *AddRef)(ID3DXMatrixStack *This);
    ULONG (WINAPI *Release)(ID3DXMatrixStack *This);
    HRESULT (WINAPI *Pop)(ID3DXMatrixStack *This);
    HRESULT (WINAPI *Push)(ID3DXMatrixStack *This);
    HRESULT (WINAPI *LoadIdentity)(ID3DXMatrixStack *This);
    HRESULT (WINAPI *LoadMatrix)(ID3DXMatrixStack *This, CONST D3DXMATRIX *pM);
    HRESULT (WINAPI *MultMatrix)(ID3DXMatrixStack *This, CONST D3DXMATRIX *pM);
    HRESULT (WINAPI *MultMatrixLocal)(ID3DXMatrixStack *This, CONST D3DXMATRIX *pM);
    HRESULT (WINAPI *RotateAxis)(ID3DXMatrixStack *This, CONST D3DXVECTOR3 *pV, FLOAT Angle);
    HRESULT (WINAPI *RotateAxisLocal)(ID3DXMatrixStack *This, CONST D3DXVECTOR3 *pV, FLOAT Angle);
    HRESULT (WINAPI *RotateYawPitchRoll)(ID3DXMatrixStack *This, FLOAT Yaw, FLOAT Pitch, FLOAT Roll);
    HRESULT (WINAPI *RotateYawPitchRollLocal)(ID3DXMatrixStack *This, FLOAT Yaw, FLOAT Pitch, FLOAT Roll);
    HRESULT (WINAPI *Scale)(ID3DXMatrixStack *This, FLOAT x, FLOAT y, FLOAT z);
    HRESULT (WINAPI *ScaleLocal)(ID3DXMatrixStack *This, FLOAT x, FLOAT y, FLOAT z);
    HRESULT (WINAPI *Translate)(ID3DXMatrixStack *This, FLOAT x, FLOAT y, FLOAT z);
    HRESULT (WINAPI *TranslateLocal)(ID3DXMatrixStack *This, FLOAT x, FLOAT y, FLOAT z);
    D3DXMATRIX *(WINAPI *GetTop)(ID3DXMatrixStack *This);
};

struct ID3DXMeshVtbl {
    HRESULT (WINAPI *QueryInterface)(ID3DXMesh *This, REFIID iid, void **ppvObj);
    ULONG (WINAPI *AddRef)(ID3DXMesh *This);
    ULONG (WINAPI *Release)(ID3DXMesh *This);
    HRESULT (WINAPI *DrawSubset)(ID3DXMesh *This, DWORD AttribId);
    DWORD (WINAPI *GetNumFaces)(ID3DXMesh *This);
    DWORD (WINAPI *GetNumVertices)(ID3DXMesh *This);
    DWORD (WINAPI *GetFVF)(ID3DXMesh *This);
    HRESULT (WINAPI *GetDeclaration)(ID3DXMesh *This, DWORD Declaration[MAX_FVF_DECL_SIZE]);
    DWORD (WINAPI *GetOptions)(ID3DXMesh *This);
    HRESULT (WINAPI *GetDevice)(ID3DXMesh *This, LPDIRECT3DDEVICE8 *ppDevice);
    HRESULT (WINAPI *CloneMeshFVF)(ID3DXMesh *This, DWORD Options, DWORD FVF, LPDIRECT3DDEVICE8 pD3DDevice, LPD3DXMESH *ppCloneMesh);
    HRESULT (WINAPI *CloneMesh)(ID3DXMesh *This, DWORD Options, CONST DWORD *pDeclaration, LPDIRECT3DDEVICE8 pD3DDevice, LPD3DXMESH *ppCloneMesh);
    HRESULT (WINAPI *GetVertexBuffer)(ID3DXMesh *This, LPDIRECT3DVERTEXBUFFER8 *ppVB);
    HRESULT (WINAPI *GetIndexBuffer)(ID3DXMesh *This, LPDIRECT3DINDEXBUFFER8 *ppIB);
    HRESULT (WINAPI *LockVertexBuffer)(ID3DXMesh *This, DWORD Flags, LPVOID *ppData);
    HRESULT (WINAPI *UnlockVertexBuffer)(ID3DXMesh *This);
    HRESULT (WINAPI *LockIndexBuffer)(ID3DXMesh *This, DWORD Flags, LPVOID *ppData);
    HRESULT (WINAPI *UnlockIndexBuffer)(ID3DXMesh *This);
    HRESULT (WINAPI *GetAttributeTable)(ID3DXMesh *This, D3DXATTRIBUTERANGE *pAttribTable, DWORD *pAttribTableSize);
    HRESULT (WINAPI *ConvertPointRepsToAdjacency)(ID3DXMesh *This, CONST DWORD *pPRep, DWORD *pAdjacency);
    HRESULT (WINAPI *ConvertAdjacencyToPointReps)(ID3DXMesh *This, CONST DWORD *pAdjacency, DWORD *pPRep);
    HRESULT (WINAPI *GenerateAdjacency)(ID3DXMesh *This, FLOAT Epsilon, DWORD *pAdjacency);
    HRESULT (WINAPI *UpdateSemantics)(ID3DXMesh *This, DWORD Declaration[MAX_FVF_DECL_SIZE]);
    HRESULT (WINAPI *LockAttributeBuffer)(ID3DXMesh *This, DWORD Flags, DWORD **ppData);
    HRESULT (WINAPI *UnlockAttributeBuffer)(ID3DXMesh *This);
    HRESULT (WINAPI *Optimize)(ID3DXMesh *This, DWORD Flags, CONST DWORD *pAdjacencyIn, DWORD *pAdjacencyOut, DWORD *pFaceRemap, LPD3DXBUFFER *ppVertexRemap, LPD3DXMESH *ppOptMesh);
    HRESULT (WINAPI *OptimizeInplace)(ID3DXMesh *This, DWORD Flags, CONST DWORD *pAdjacencyIn, DWORD *pAdjacencyOut, DWORD *pFaceRemap, LPD3DXBUFFER *ppVertexRemap);
};

// D3DX interface structures
struct ID3DXBuffer {
    const ID3DXBufferVtbl *pVtbl; // Use pVtbl to match code expectations
    ULONG ref_count;
    DWORD size;
    void *data;
};

struct ID3DXMatrixStack {
    const ID3DXMatrixStackVtbl *lpVtbl;
    ULONG ref_count;
    D3DXMATRIX *stack;
    DWORD capacity;
    DWORD top;
};

struct ID3DXMesh {
    const ID3DXMeshVtbl *pVtbl; // Use pVtbl to match the code expectations
    ULONG ref_count;
    LPDIRECT3DDEVICE8 device;   // D3D device pointer
    DWORD num_faces;
    DWORD num_vertices;
    DWORD fvf;
    DWORD options;
    LPDIRECT3DVERTEXBUFFER8 vb; // vertex buffer pointer
    LPDIRECT3DINDEXBUFFER8 ib;  // index buffer pointer
    D3DXATTRIBUTERANGE *attrib_table;
    DWORD attrib_table_size;
    DWORD *attrib_buffer;
};

// Additional convenience typedefs
typedef IDirect3DSwapChain8 *LPDIRECT3DSWAPCHAIN8;

// Additional definitions needed by our implementation
#ifndef D3DADAPTER_DEFAULT
#define D3DADAPTER_DEFAULT 0
#endif

#define D3DAPI WINAPI
#define D3D_SDK_VERSION 220
#define D3DX_DEFAULT_FLOAT FLT_MAX
#define D3DX_FILTER_NONE    (1 << 0)
#define D3DX_FILTER_POINT   (2 << 0)
#define D3DX_FILTER_LINEAR  (3 << 0)
#define D3DX_FILTER_TRIANGLE (4 << 0)
#define D3DX_FILTER_BOX     (5 << 0)

// Error codes
#define D3D_OK 0
#define D3DERR_INVALIDCALL -1
#define MAKE_D3DHRESULT(code) (0x88760000 | (code))
#define D3DERR_OUTOFVIDEOMEMORY MAKE_D3DHRESULT(380)
#define D3DERR_NOTAVAILABLE MAKE_D3DHRESULT(2154)
#ifndef D3DXERR_INVALIDMESH
#define D3DXERR_INVALIDMESH MAKE_DDHRESULT(2901)
#endif
#ifndef D3DXERR_SKINNINGNOTSUPPORTED
#define D3DXERR_SKINNINGNOTSUPPORTED MAKE_DDHRESULT(2903)
#endif
#ifndef D3DXERR_INVALIDDATA
#define D3DXERR_INVALIDDATA MAKE_DDHRESULT(2905)
#endif

#ifndef D3DADAPTER_DEFAULT
#define D3DADAPTER_DEFAULT 0
#endif

#ifndef D3DXMESH_MANAGED
#define D3DXMESH_MANAGED 0x220
#endif

#ifndef D3DXMESHOPT_COMPACT
#define D3DXMESHOPT_COMPACT 0x01000000
#endif
#ifndef D3DXMESHOPT_ATTRSORT
#define D3DXMESHOPT_ATTRSORT 0x02000000
#endif
#ifndef D3DXMESHOPT_VERTEXCACHE
#define D3DXMESHOPT_VERTEXCACHE 0x04000000
#endif
#ifndef D3DXMESHOPT_STRIPREORDER
#define D3DXMESHOPT_STRIPREORDER 0x08000000
#endif
#ifndef D3DXMESHOPT_IGNOREVERTS
#define D3DXMESHOPT_IGNOREVERTS 0x10000000
#endif
#ifndef D3DXMESHOPT_SHAREVB
#define D3DXMESHOPT_SHAREVB 0x1000
#endif

// Additional missing types and structures
typedef struct {
    float x, y, z;
    float nx, ny, nz;
} VertexPN;

typedef struct _D3DVERTEXELEMENT8 {
    WORD Stream;
    WORD Offset;
    BYTE Type;
    BYTE Method;
    BYTE Usage;
    BYTE UsageIndex;
} D3DVERTEXELEMENT8;


// Function declarations for D3DX utility functions
HRESULT WINAPI D3DXCreateBuffer(DWORD NumBytes, LPD3DXBUFFER *ppBuffer);
UINT WINAPI D3DXGetFVFVertexSize(DWORD FVF);
HRESULT WINAPI D3DXDeclaratorFromFVF(DWORD FVF, DWORD Declaration[MAX_FVF_DECL_SIZE]);
HRESULT WINAPI D3DXCreateMeshFVF(DWORD NumFaces, DWORD NumVertices, DWORD Options, DWORD FVF, LPDIRECT3DDEVICE8 pD3D, LPD3DXMESH *ppMesh);
HRESULT WINAPI D3DXCreateBox(LPDIRECT3DDEVICE8 pDevice, FLOAT Width, FLOAT Height, FLOAT Depth, LPD3DXMESH *ppMesh, LPD3DXBUFFER *ppAdjacency);
HRESULT WINAPI D3DXCreateSphere(LPDIRECT3DDEVICE8 pDevice, FLOAT Radius, UINT Slices, UINT Stacks, LPD3DXMESH *ppMesh, LPD3DXBUFFER *ppAdjacency);
HRESULT WINAPI D3DXCreateCylinder(LPDIRECT3DDEVICE8 pDevice, FLOAT Radius1, FLOAT Radius2, FLOAT Length, UINT Slices, UINT Stacks, LPD3DXMESH *ppMesh, LPD3DXBUFFER *ppAdjacency);
HRESULT WINAPI D3DXComputeNormals(LPD3DXBASEMESH pMesh, CONST DWORD *pAdjacency);
HRESULT WINAPI D3DXComputeBoundingSphere(PVOID pPointsFVF, DWORD NumVertices, DWORD FVF, D3DVECTOR *pCenter, FLOAT *pRadius);
HRESULT WINAPI D3DXComputeBoundingBox(PVOID pPointsFVF, DWORD NumVertices, DWORD FVF, D3DVECTOR *pMin, D3DVECTOR *pMax);
HRESULT WINAPI D3DXGetErrorStringA(HRESULT hr, LPSTR pBuffer, UINT BufferLen);
HRESULT WINAPI D3DXCreateMatrixStack(DWORD Flags, LPD3DXMATRIXSTACK *ppStack);
HRESULT WINAPI D3DXCreateTexture(LPDIRECT3DDEVICE8 pDevice, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DTEXTURE8 *ppTexture);
HRESULT WINAPI D3DXCreateTextureFromFileExA(LPDIRECT3DDEVICE8 pDevice, LPCSTR pSrcFile, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE8 *ppTexture);
HRESULT WINAPI D3DXLoadSurfaceFromSurface(LPDIRECT3DSURFACE8 pDestSurface, CONST PALETTEENTRY *pDestPalette, CONST RECT *pDestRect, LPDIRECT3DSURFACE8 pSrcSurface, CONST PALETTEENTRY *pSrcPalette, CONST RECT *pSrcRect, DWORD Filter, D3DCOLOR ColorKey);
HRESULT WINAPI D3DXFilterTexture(LPDIRECT3DBASETEXTURE8 pBaseTexture, CONST PALETTEENTRY *pPalette, UINT SrcLevel, DWORD Filter);
HRESULT WINAPI D3DXLoadMeshFromX(LPSTR pFilename, DWORD Options, LPDIRECT3DDEVICE8 pD3D, LPD3DXBUFFER *ppAdjacency, LPD3DXBUFFER *ppMaterials, DWORD *pNumMaterials, LPD3DXMESH *ppMesh);

// Math functions  
D3DXMATRIX* WINAPI D3DXMatrixIdentity(D3DXMATRIX *pOut);
D3DXMATRIX* WINAPI D3DXMatrixMultiply(D3DXMATRIX *pOut, CONST D3DXMATRIX *pM1, CONST D3DXMATRIX *pM2);
D3DXMATRIX* WINAPI D3DXMatrixLookAtLH(D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt, CONST D3DXVECTOR3 *pUp);
D3DXMATRIX* WINAPI D3DXMatrixLookAtRH(D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt, CONST D3DXVECTOR3 *pUp);
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovLH(D3DXMATRIX *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf);
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovRH(D3DXMATRIX *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf);
D3DXMATRIX* WINAPI D3DXMatrixTranspose(D3DXMATRIX *pOut, CONST D3DXMATRIX *pM);
D3DXMATRIX* WINAPI D3DXMatrixInverse(D3DXMATRIX *pOut, FLOAT *pDeterminant, CONST D3DXMATRIX *pM);
D3DXMATRIX* WINAPI D3DXMatrixScaling(D3DXMATRIX *pOut, FLOAT sx, FLOAT sy, FLOAT sz);
D3DXMATRIX* WINAPI D3DXMatrixTranslation(D3DXMATRIX *pOut, FLOAT x, FLOAT y, FLOAT z);
D3DXMATRIX* WINAPI D3DXMatrixRotationX(D3DXMATRIX *pOut, FLOAT Angle);
D3DXMATRIX* WINAPI D3DXMatrixRotationY(D3DXMATRIX *pOut, FLOAT Angle);
D3DXMATRIX* WINAPI D3DXMatrixRotationZ(D3DXMATRIX *pOut, FLOAT Angle);
D3DXMATRIX* WINAPI D3DXMatrixRotationAxis(D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pV, FLOAT Angle);
D3DXMATRIX* WINAPI D3DXMatrixRotationYawPitchRoll(D3DXMATRIX *pOut, FLOAT Yaw, FLOAT Pitch, FLOAT Roll);
D3DXMATRIX* WINAPI D3DXMatrixOrthoLH(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);
D3DXMATRIX* WINAPI D3DXMatrixOrthoRH(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);
D3DXVECTOR3* WINAPI D3DXVec3Normalize(D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV);
D3DXVECTOR3* WINAPI D3DXVec3TransformCoord(D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV, CONST D3DXMATRIX *pM);
D3DXVECTOR4* WINAPI D3DXVec3Transform(D3DXVECTOR4 *pOut, CONST D3DXVECTOR3 *pV, CONST D3DXMATRIX *pM);
D3DXVECTOR3* WINAPI D3DXVec3Subtract(D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2);
D3DXVECTOR3* WINAPI D3DXVec3Cross(D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2);
FLOAT WINAPI D3DXVec3Dot(CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2);
FLOAT WINAPI D3DXVec4Dot(CONST D3DXVECTOR4 *pV1, CONST D3DXVECTOR4 *pV2);
D3DXVECTOR4* WINAPI D3DXVec4Transform(D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV, CONST D3DXMATRIX *pM);

// Entry point
IDirect3D8 *D3DAPI Direct3DCreate8(UINT SDKVersion);
void fill_d3d_caps(D3DCAPS8 *pCaps, D3DDEVTYPE DeviceType);

#ifdef D3D8_GLES_LOGGING
void d3d8_gles_log(const char *format, ...);
#else
#define d3d8_gles_log(...)
#endif

#endif // D3D8_TO_GLES_H
