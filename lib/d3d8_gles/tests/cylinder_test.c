#include <assert.h>
#include <d3d8_to_gles.h>

/* Prototype is not declared in the public header */
HRESULT WINAPI D3DXCreateCylinder(LPDIRECT3DDEVICE8 pDevice, FLOAT Radius1,
                                  FLOAT Radius2, FLOAT Length, UINT Slices,
                                  UINT Stacks, LPD3DXMESH *ppMesh,
                                  LPD3DXBUFFER *ppAdjacency);

int main(void) {
  IDirect3D8 *d3d = Direct3DCreate8(D3D_SDK_VERSION);
  assert(d3d && "Failed to create D3D8 interface");

  D3DPRESENT_PARAMETERS pp = {0};
  pp.BackBufferWidth = 8;
  pp.BackBufferHeight = 8;
  pp.BackBufferFormat = D3DFMT_X8R8G8B8;
  pp.BackBufferCount = 1;
  pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  pp.hDeviceWindow = 0;
  pp.Windowed = TRUE;
  pp.EnableAutoDepthStencil = FALSE;
  pp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

  IDirect3DDevice8 *device = NULL;
  HRESULT hr =
      d3d->lpVtbl->CreateDevice(d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                pp.hDeviceWindow, 0, &pp, &device);
  assert(hr == D3D_OK);

  ID3DXMesh *mesh = NULL;
  hr = D3DXCreateCylinder(device, 1.0f, 1.0f, 1.0f, 8, 1, &mesh, NULL);
  assert(hr == D3D_OK && mesh);

  mesh->pVtbl->Release(mesh);
  device->lpVtbl->Release(device);
  d3d->lpVtbl->Release(d3d);
  return 0;
}
