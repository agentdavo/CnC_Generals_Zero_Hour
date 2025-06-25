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
    assert(hr == D3D_OK && device);

    D3DXVECTOR4 v[2] = { {1.f,2.f,3.f,4.f}, {5.f,6.f,7.f,8.f} };
    hr = device->lpVtbl->SetVertexShaderConstant(device, 0, v, 2);
    assert(hr == D3D_OK);
    assert(device->gles->vs_const[0].x == 1.f && device->gles->vs_const[1].y == 6.f);

    D3DXVECTOR4 p = {9.f,10.f,11.f,12.f};
    hr = device->lpVtbl->SetPixelShaderConstant(device, 1, &p, 1);
    assert(hr == D3D_OK);
    assert(device->gles->ps_const[1].z == 11.f);

    device->lpVtbl->Release(device);
    d3d->lpVtbl->Release(d3d);
    return 0;
}
