#include <assert.h>
#include <d3d8_to_gles.h>

int main(void) {
  IDirect3D8 *d3d = Direct3DCreate8(D3D_SDK_VERSION);
  assert(d3d);

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

  IDirect3DDevice8 *dev = NULL;
  HRESULT hr = d3d->lpVtbl->CreateDevice(
      d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pp.hDeviceWindow, 0, &pp, &dev);
  assert(hr == D3D_OK && dev);

  ID3DXMesh *mesh = NULL;
  hr = D3DXCreateBox(dev, 1.0f, 1.0f, 1.0f, &mesh, NULL);
  assert(hr == D3D_OK && mesh);

  DWORD fvf = mesh->pVtbl->GetFVF(mesh);
  ID3DXMesh *clone1 = NULL;
  hr = mesh->pVtbl->CloneMeshFVF(mesh, mesh->pVtbl->GetOptions(mesh), fvf, dev,
                                 &clone1);
  assert(hr == D3D_OK && clone1);

  DWORD decl[MAX_FVF_DECL_SIZE];
  hr = D3DXDeclaratorFromFVF(fvf, decl);
  assert(hr == D3D_OK);
  ID3DXMesh *clone2 = NULL;
  hr = mesh->pVtbl->CloneMesh(mesh, mesh->pVtbl->GetOptions(mesh), decl, dev,
                              &clone2);
  assert(hr == D3D_OK && clone2);

  clone2->pVtbl->Release(clone2);
  clone1->pVtbl->Release(clone1);
  mesh->pVtbl->Release(mesh);
  dev->lpVtbl->Release(dev);
  d3d->lpVtbl->Release(d3d);
  return 0;
}
