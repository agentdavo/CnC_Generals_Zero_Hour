// d3dx_buffer.c - ID3DXBuffer utilities and FVF processing
#include "d3d8_to_gles.h"
#include "d3d8_internal.h"

//=============================================================================
// FVF (Flexible Vertex Format) Utilities
//=============================================================================

UINT WINAPI D3DXGetFVFVertexSize(DWORD FVF) {
  if (!(FVF & (D3DFVF_XYZ | D3DFVF_XYZRHW)))
    return 0;

  if (FVF & ~(D3DFVF_POSITION_MASK | D3DFVF_NORMAL | D3DFVF_PSIZE |
              D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEXCOUNT_MASK))
    return 0;

  UINT size = (FVF & D3DFVF_XYZRHW) ? 4 * sizeof(float) : 3 * sizeof(float);

  switch (FVF & D3DFVF_POSITION_MASK) {
  case D3DFVF_XYZB1:
    size += sizeof(float);
    break;
  case D3DFVF_XYZB2:
    size += 2 * sizeof(float);
    break;
  case D3DFVF_XYZB3:
    size += 3 * sizeof(float);
    break;
  case D3DFVF_XYZB4:
    size += 4 * sizeof(float);
    break;
  case D3DFVF_XYZB5:
    size += 5 * sizeof(float);
    break;
  default:
    break;
  }

  if (FVF & D3DFVF_PSIZE)
    size += sizeof(float);
  if (FVF & D3DFVF_NORMAL)
    size += 3 * sizeof(float);
  if (FVF & D3DFVF_DIFFUSE)
    size += sizeof(DWORD);
  if (FVF & D3DFVF_SPECULAR)
    size += sizeof(DWORD);

  UINT tex_count = (FVF & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT;
  if (tex_count > 8)
    return 0;
  size += tex_count * 2 * sizeof(float); // each texcoord set is 2 floats
  return size;
}

HRESULT WINAPI D3DXDeclaratorFromFVF(DWORD FVF,
                                     DWORD Declaration[MAX_FVF_DECL_SIZE]) {
  if (!(FVF & (D3DFVF_XYZ | D3DFVF_XYZRHW)))
    return D3DERR_INVALIDCALL;
  if ((FVF & D3DFVF_XYZ) && (FVF & D3DFVF_XYZRHW))
    return D3DERR_INVALIDCALL;
  if (FVF & ~(D3DFVF_XYZ | D3DFVF_XYZRHW | D3DFVF_NORMAL | D3DFVF_DIFFUSE |
              D3DFVF_SPECULAR | D3DFVF_TEX1 | D3DFVF_TEX2))
    return D3DERR_INVALIDCALL;

  int i = 0;
  Declaration[i++] = D3DVSD_STREAM(0);
  if (FVF & D3DFVF_XYZRHW)
    Declaration[i++] = D3DVSD_REG(D3DVSDE_POSITION, D3DVSDT_FLOAT4);
  else
    Declaration[i++] = D3DVSD_REG(D3DVSDE_POSITION, D3DVSDT_FLOAT3);
  if (FVF & D3DFVF_NORMAL)
    Declaration[i++] = D3DVSD_REG(D3DVSDE_NORMAL, D3DVSDT_FLOAT3);
  if (FVF & D3DFVF_DIFFUSE)
    Declaration[i++] = D3DVSD_REG(D3DVSDE_DIFFUSE, D3DVSDT_D3DCOLOR);
  if (FVF & D3DFVF_SPECULAR)
    Declaration[i++] = D3DVSD_REG(D3DVSDE_SPECULAR, D3DVSDT_D3DCOLOR);

  UINT tex_count = (FVF & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT;
  if (tex_count > 0)
    Declaration[i++] = D3DVSD_REG(D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2);
  if (tex_count > 1)
    Declaration[i++] = D3DVSD_REG(D3DVSDE_TEXCOORD1, D3DVSDT_FLOAT2);
  Declaration[i++] = D3DVSD_END();

  for (; i < MAX_FVF_DECL_SIZE; i++)
    Declaration[i] = D3DVSD_END();
  return D3D_OK;
}

//=============================================================================
// ID3DXBuffer Implementation
//=============================================================================

// TODO: Add ID3DXBuffer interface implementation here
// This file will contain:
// - ID3DXBuffer interface implementation
// - Buffer allocation and management functions