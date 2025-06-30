// d3dx_texture.c - D3DX texture functions
#include "d3d8_to_gles.h"
#include "../../lodepng/lodepng.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// D3DX error constants (from original DirectX SDK)
#ifndef D3DXERR_INVALIDDATA
#define D3DXERR_INVALIDDATA MAKE_HRESULT(1, 0x876, 0x87A)
#endif
#ifndef D3DXERR_NOTAVAILABLE  
#define D3DXERR_NOTAVAILABLE MAKE_HRESULT(1, 0x876, 0x87B)
#endif

//=============================================================================
// Image Loading Utilities
//=============================================================================

static int load_png(const char *path, unsigned char **pixels, unsigned *w,
                    unsigned *h) {
  unsigned error = lodepng_decode32_file(pixels, w, h, path);
  return error ? -1 : 0;
}

static int load_bmp(const char *path, unsigned char **pixels, unsigned *w,
                    unsigned *h) {
  FILE *f = fopen(path, "rb");
  if (!f)
    return -1;
  unsigned char header[54];
  if (fread(header, 1, 54, f) != 54) {
    fclose(f);
    return -1;
  }
  if (header[0] != 'B' || header[1] != 'M') {
    fclose(f);
    return -1;
  }

  unsigned data_offset = *(unsigned *)(header + 10);
  unsigned width = *(unsigned *)(header + 18);
  unsigned height = *(unsigned *)(header + 22);
  unsigned bits_per_pixel = *(unsigned short *)(header + 28);
  
  if (bits_per_pixel != 24 && bits_per_pixel != 32) {
    fclose(f);
    return -1;
  }

  unsigned row_padded = (width * (bits_per_pixel / 8) + 3) & (~3);
  unsigned char *data = malloc(row_padded * height);
  if (!data) {
    fclose(f);
    return -1;
  }

  fseek(f, data_offset, SEEK_SET);
  if (fread(data, 1, row_padded * height, f) != row_padded * height) {
    free(data);
    fclose(f);
    return -1;
  }
  fclose(f);

  *pixels = malloc(width * height * 4);
  if (!*pixels) {
    free(data);
    return -1;
  }

  for (unsigned y = 0; y < height; y++) {
    for (unsigned x = 0; x < width; x++) {
      unsigned src_offset = (height - 1 - y) * row_padded + x * (bits_per_pixel / 8);
      unsigned dst_offset = (y * width + x) * 4;
      
      if (bits_per_pixel == 24) {
        (*pixels)[dst_offset + 2] = data[src_offset + 0]; // B
        (*pixels)[dst_offset + 1] = data[src_offset + 1]; // G
        (*pixels)[dst_offset + 0] = data[src_offset + 2]; // R
        (*pixels)[dst_offset + 3] = 255; // A
      } else {
        (*pixels)[dst_offset + 2] = data[src_offset + 0]; // B
        (*pixels)[dst_offset + 1] = data[src_offset + 1]; // G
        (*pixels)[dst_offset + 0] = data[src_offset + 2]; // R
        (*pixels)[dst_offset + 3] = data[src_offset + 3]; // A
      }
    }
  }

  free(data);
  *w = width;
  *h = height;
  return 0;
}

static int str_ends_with(const char *a, const char *b) {
  const char *a_end = a + strlen(a);
  const char *b_end = b + strlen(b);
  while (a_end >= a && b_end >= b && *a_end == *b_end) {
    a_end--;
    b_end--;
  }
  return *a == '\0' && *b == '\0';
}

//=============================================================================
// D3DX Texture Functions
//=============================================================================

HRESULT WINAPI D3DXCreateTexture(LPDIRECT3DDEVICE8 pDevice, UINT Width,
                                 UINT Height, UINT MipLevels, DWORD Usage,
                                 D3DFORMAT Format, D3DPOOL Pool,
                                 LPDIRECT3DTEXTURE8 *ppTexture) {
  if (!pDevice || !ppTexture || Width == 0 || Height == 0)
    return D3DERR_INVALIDCALL;
  // Cast to our wrapper struct and call the device method
  DeviceImpl *device_impl = (DeviceImpl*)pDevice;
  IDirect3DDevice8Vtbl *vtbl = (IDirect3DDevice8Vtbl*)device_impl->lpVtbl;
  return vtbl->CreateTexture(pDevice, Width, Height, MipLevels,
                            Usage, Format, Pool, ppTexture);
}

HRESULT WINAPI D3DXCreateTextureFromFileExA(
    LPDIRECT3DDEVICE8 pDevice, LPCSTR pSrcFile, UINT Width, UINT Height,
    UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter,
    DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo,
    PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE8 *ppTexture) {
  (void)Filter;
  (void)MipFilter;
  (void)pPalette;
  
  if (!pDevice || !pSrcFile || !ppTexture)
    return D3DERR_INVALIDCALL;

  unsigned char *pixels;
  unsigned img_w, img_h;
  int result;
  
  if (str_ends_with(pSrcFile, ".png")) {
    result = load_png(pSrcFile, &pixels, &img_w, &img_h);
  } else if (str_ends_with(pSrcFile, ".bmp")) {
    result = load_bmp(pSrcFile, &pixels, &img_w, &img_h);
  } else {
    return D3DXERR_NOTAVAILABLE;
  }

  if (result != 0) {
    return D3DXERR_INVALIDDATA;
  }

  if (pSrcInfo) {
    pSrcInfo->Width = img_w;
    pSrcInfo->Height = img_h;
    pSrcInfo->Depth = 1;
    pSrcInfo->MipLevels = 1;
    pSrcInfo->Format = D3DFMT_A8R8G8B8;
    pSrcInfo->ResourceType = D3DRTYPE_TEXTURE;
    pSrcInfo->ImageFileFormat = str_ends_with(pSrcFile, ".png") ? D3DXIFF_PNG : D3DXIFF_BMP;
  }

  UINT tex_w = (Width == D3DX_DEFAULT) ? img_w : Width;
  UINT tex_h = (Height == D3DX_DEFAULT) ? img_h : Height;
  if (Format == D3DFMT_UNKNOWN)
    Format = D3DFMT_A8R8G8B8;

  HRESULT hr = D3DXCreateTexture(pDevice, tex_w, tex_h, MipLevels, Usage,
                                 Format, Pool, ppTexture);
  if (hr != D3D_OK) {
    free(pixels);
    return hr;
  }

  if (ColorKey) {
    for (unsigned i = 0; i < img_w * img_h; i++) {
      uint32_t *p = (uint32_t *)(pixels + i * 4);
      if ((*p & 0x00FFFFFF) == (ColorKey & 0x00FFFFFF))
        *p &= 0x00FFFFFF;
    }
  }

  D3DLOCKED_RECT rect;
  TextureImpl *tex_impl = (TextureImpl*)(*ppTexture);
  IDirect3DTexture8Vtbl *vtbl = (IDirect3DTexture8Vtbl*)tex_impl->lpVtbl;
  hr = vtbl->LockRect(*ppTexture, 0, &rect, NULL, 0);
  if (SUCCEEDED(hr)) {
    for (unsigned y = 0; y < img_h && y < tex_h; y++) {
      memcpy((char *)rect.pBits + y * rect.Pitch, pixels + y * img_w * 4,
             img_w * 4);
    }
    vtbl->UnlockRect(*ppTexture, 0);
  }

  free(pixels);
  return hr;
}

HRESULT WINAPI D3DXLoadSurfaceFromSurface(LPDIRECT3DSURFACE8 pDestSurface,
                                          CONST PALETTEENTRY *pDestPalette,
                                          CONST RECT *pDestRect,
                                          LPDIRECT3DSURFACE8 pSrcSurface,
                                          CONST PALETTEENTRY *pSrcPalette,
                                          CONST RECT *pSrcRect, DWORD Filter,
                                          D3DCOLOR ColorKey) {
  (void)pDestSurface;
  (void)pDestPalette;
  (void)pDestRect;
  (void)pSrcSurface;
  (void)pSrcPalette;
  (void)pSrcRect;
  (void)Filter;
  (void)ColorKey;
  return D3DXERR_NOTAVAILABLE;
}

HRESULT WINAPI D3DXFilterTexture(LPDIRECT3DBASETEXTURE8 pBaseTexture,
                                 CONST PALETTEENTRY *pPalette, UINT SrcLevel,
                                 DWORD Filter) {
  (void)pBaseTexture;
  (void)pPalette;
  (void)SrcLevel;
  (void)Filter;
  return D3D_OK;
}