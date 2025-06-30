// d3d8_internal.h - Internal definitions shared between d3d8_gles source files
#ifndef D3D8_INTERNAL_H
#define D3D8_INTERNAL_H

#include "d3d8_to_gles.h"
#include <EGL/eglext.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_MICROGLES
#include "../../u_gles/src/gl_init.h"
#endif

#include "../../lodepng/lodepng.h"

#include <math.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef D3D8_GLES_LOGGING
#include <stdarg.h>
#include <stdio.h>
void d3d8_gles_log(const char *format, ...);
#endif

// Internal function declarations
void fill_d3d_caps(D3DCAPS8 *pCaps, D3DDEVTYPE DeviceType);

// Helper functions from gles_helpers.c
float dword_to_float(DWORD v);
GLenum blend_to_gl(D3DBLEND blend);
GLenum cmp_to_gl(D3DCMPFUNC func);
GLenum stencil_op_to_gl(D3DSTENCILOP op);
GLenum fog_mode_to_gl(D3DFOGMODE mode);
void set_render_state(GLES_Device *gles, D3DRENDERSTATETYPE state, DWORD value);
EGLConfig choose_egl_config(EGLDisplay display, D3DPRESENT_PARAMETERS *params, BOOL want_window);
void d3d_to_gl_matrix(GLfloat *restrict gl_matrix, const D3DXMATRIX *restrict d3d_matrix);
void setup_vertex_attributes(GLES_Device *gles, DWORD fvf, BYTE *data, UINT stride);

// Thread-local matrix storage
extern _Thread_local alignas(64) D3DXMATRIX temp_matrix;

// Global variables
extern D3DDISPLAYMODE g_current_display_mode;

// Interface compatibility fix - min-dx8-sdk uses void* for first parameter
#define D3D8_CAST(func) ((void*)func)

// D3DX math functions not in min-dx8-sdk
D3DXMATRIX* WINAPI D3DXMatrixMultiply(D3DXMATRIX* pOut, CONST D3DXMATRIX* pM1, CONST D3DXMATRIX* pM2);
D3DXMATRIX* WINAPI D3DXMatrixLookAtLH(D3DXMATRIX* pOut, CONST D3DXVECTOR3* pEye, CONST D3DXVECTOR3* pAt, CONST D3DXVECTOR3* pUp);
D3DXMATRIX* WINAPI D3DXMatrixLookAtRH(D3DXMATRIX* pOut, CONST D3DXVECTOR3* pEye, CONST D3DXVECTOR3* pAt, CONST D3DXVECTOR3* pUp);
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovLH(D3DXMATRIX* pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf);
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovRH(D3DXMATRIX* pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf);
D3DXMATRIX* WINAPI D3DXMatrixTranspose(D3DXMATRIX* pOut, CONST D3DXMATRIX* pM);
D3DXMATRIX* WINAPI D3DXMatrixInverse(D3DXMATRIX* pOut, FLOAT* pDeterminant, CONST D3DXMATRIX* pM);
D3DXVECTOR3* WINAPI D3DXVec3Normalize(D3DXVECTOR3* pOut, CONST D3DXVECTOR3* pV);
D3DXVECTOR3* WINAPI D3DXVec3TransformCoord(D3DXVECTOR3* pOut, CONST D3DXVECTOR3* pV, CONST D3DXMATRIX* pM);

// D3DXMatrixStack creation
HRESULT WINAPI D3DXCreateMatrixStack(DWORD Flags, LPD3DXMATRIXSTACK* ppStack);

// Vertex and index buffer creation functions
HRESULT WINAPI d3d8_create_vertex_buffer(IDirect3DDevice8 *This, UINT Length, DWORD Usage, DWORD FVF,
                                         D3DPOOL Pool, IDirect3DVertexBuffer8 **ppVertexBuffer);
HRESULT WINAPI d3d8_create_index_buffer(IDirect3DDevice8 *This, UINT Length, DWORD Usage, D3DFORMAT Format,
                                        D3DPOOL Pool, IDirect3DIndexBuffer8 **ppIndexBuffer);
HRESULT WINAPI d3d8_set_stream_source(IDirect3DDevice8 *This, UINT StreamNumber,
                                      IDirect3DVertexBuffer8 *pStreamData, UINT Stride);
HRESULT WINAPI d3d8_set_indices(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 *pIndexData,
                                UINT BaseVertexIndex);

#endif // D3D8_INTERNAL_H