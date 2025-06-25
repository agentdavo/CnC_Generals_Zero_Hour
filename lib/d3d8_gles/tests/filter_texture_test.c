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
    IDirect3DDevice8 *device = NULL;
    HRESULT hr = d3d->lpVtbl->CreateDevice(d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                           pp.hDeviceWindow, 0, &pp, &device);
    assert(hr == D3D_OK);

    IDirect3DTexture8 *tex = NULL;
    hr = D3DXCreateTexture(device, 4, 4, 2, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex);
    assert(hr == D3D_OK && tex);

    hr = D3DXFilterTexture((LPDIRECT3DBASETEXTURE8)tex, NULL, 0, D3DX_FILTER_BOX);
    assert(hr == D3D_OK);

    tex->lpVtbl->Release(tex);
    device->lpVtbl->Release(device);
    d3d->lpVtbl->Release(d3d);
    return 0;
}
