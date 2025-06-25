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

  DWORD face_count = mesh->pVtbl->GetNumFaces(mesh);
  DWORD vert_count = mesh->pVtbl->GetNumVertices(mesh);
  DWORD *adj = calloc(face_count * 3, sizeof(DWORD));
  DWORD *preps = calloc(vert_count, sizeof(DWORD));
  assert(adj && preps);

  hr = mesh->pVtbl->GenerateAdjacency(mesh, 0.0f, adj);
  assert(hr == D3D_OK);
  for (DWORD i = 0; i < face_count * 3; i++)
    assert(adj[i] == 0xFFFFFFFFu);

  hr = mesh->pVtbl->ConvertAdjacencyToPointReps(mesh, adj, preps);
  assert(hr == D3D_OK);
  for (DWORD i = 0; i < vert_count; i++)
    assert(preps[i] == i);

  hr = mesh->pVtbl->ConvertPointRepsToAdjacency(mesh, preps, adj);
  assert(hr == D3D_OK);

  ID3DXMesh *opt = NULL;
  hr = mesh->pVtbl->Optimize(mesh, 0, adj, adj, NULL, NULL, &opt);
  assert(hr == D3D_OK && opt);
  opt->pVtbl->Release(opt);

  hr = mesh->pVtbl->OptimizeInplace(mesh, 0, adj, adj, NULL, NULL);
  assert(hr == D3D_OK);

  free(preps);
  free(adj);
  mesh->pVtbl->Release(mesh);
  dev->lpVtbl->Release(dev);
  d3d->lpVtbl->Release(d3d);
  return 0;
}
