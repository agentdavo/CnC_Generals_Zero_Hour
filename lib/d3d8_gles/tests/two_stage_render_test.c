#include <assert.h>
#include <d3d8_to_gles.h>
#include <stdint.h>
#include <string.h>

UINT WINAPI D3DXGetFVFVertexSize(DWORD FVF);

typedef struct {
    float x, y, z;
    float u0, v0;
    float u1, v1;
} Vertex;

int main(void) {
    IDirect3D8 *d3d = Direct3DCreate8(D3D_SDK_VERSION);
    assert(d3d && "Failed to create D3D8 interface");

    D3DPRESENT_PARAMETERS pp = {0};
    pp.BackBufferWidth = 4;
    pp.BackBufferHeight = 4;
    pp.BackBufferFormat = D3DFMT_X8R8G8B8;
    pp.BackBufferCount = 1;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.hDeviceWindow = 0;
    pp.Windowed = TRUE;
    pp.EnableAutoDepthStencil = FALSE;
    pp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    IDirect3DDevice8 *device = NULL;
    HRESULT hr = d3d->lpVtbl->CreateDevice(d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                           pp.hDeviceWindow, 0, &pp, &device);
    assert(hr == D3D_OK && "CreateDevice failed");

    /* Create red texture */
    IDirect3DTexture8 *tex0 = NULL;
    hr = device->lpVtbl->CreateTexture(device, 1, 1, 1, 0, D3DFMT_A8R8G8B8,
                                       D3DPOOL_MANAGED, &tex0);
    assert(hr == D3D_OK && tex0);
    D3DLOCKED_RECT lr;
    hr = tex0->lpVtbl->LockRect(tex0, 0, &lr, NULL, 0);
    assert(hr == D3D_OK);
    ((uint32_t*)lr.pBits)[0] = 0xffff0000u; /* ARGB red */
    tex0->lpVtbl->UnlockRect(tex0, 0);

    /* Create blue texture */
    IDirect3DTexture8 *tex1 = NULL;
    hr = device->lpVtbl->CreateTexture(device, 1, 1, 1, 0, D3DFMT_A8R8G8B8,
                                       D3DPOOL_MANAGED, &tex1);
    assert(hr == D3D_OK && tex1);
    hr = tex1->lpVtbl->LockRect(tex1, 0, &lr, NULL, 0);
    assert(hr == D3D_OK);
    ((uint32_t*)lr.pBits)[0] = 0xff0000ffu; /* ARGB blue */
    tex1->lpVtbl->UnlockRect(tex1, 0);

    hr = device->lpVtbl->SetTexture(device, 0, tex0);
    assert(hr == D3D_OK);
    hr = device->lpVtbl->SetTexture(device, 1, tex1);
    assert(hr == D3D_OK);

    hr = device->lpVtbl->SetTextureStageState(device, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    assert(hr == D3D_OK);
    hr = device->lpVtbl->SetTextureStageState(device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    assert(hr == D3D_OK);

    hr = device->lpVtbl->SetTextureStageState(device, 1, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
    assert(hr == D3D_OK);
    hr = device->lpVtbl->SetTextureStageState(device, 1, D3DTSS_COLORARG1, D3DTA_CURRENT);
    assert(hr == D3D_OK);
    hr = device->lpVtbl->SetTextureStageState(device, 1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    assert(hr == D3D_OK);

    DWORD fvf = D3DFVF_XYZ | D3DFVF_TEX2;
    UINT stride = D3DXGetFVFVertexSize(fvf);
    assert(stride == sizeof(Vertex));

    IDirect3DVertexBuffer8 *vb = NULL;
    hr = device->lpVtbl->CreateVertexBuffer(device, 3 * stride, D3DUSAGE_WRITEONLY,
                                            fvf, D3DPOOL_MANAGED, &vb);
    assert(hr == D3D_OK && vb);

    Vertex verts[3] = {
        {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        { 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f},
        { 0.0f,  1.0f, 0.0f, 0.5f, 1.0f, 0.5f, 1.0f}
    };

    BYTE *data;
    hr = vb->lpVtbl->Lock(vb, 0, 0, &data, 0);
    assert(hr == D3D_OK);
    memcpy(data, verts, sizeof(verts));
    vb->lpVtbl->Unlock(vb);

    IDirect3DIndexBuffer8 *ib = NULL;
    hr = device->lpVtbl->CreateIndexBuffer(device, 3 * sizeof(WORD), D3DUSAGE_WRITEONLY,
                                           D3DFMT_INDEX16, D3DPOOL_MANAGED, &ib);
    assert(hr == D3D_OK && ib);
    WORD indices[3] = {0, 1, 2};
    hr = ib->lpVtbl->Lock(ib, 0, 0, &data, 0);
    assert(hr == D3D_OK);
    memcpy(data, indices, sizeof(indices));
    ib->lpVtbl->Unlock(ib);

    device->gles->fvf = fvf;
    hr = device->lpVtbl->SetStreamSource(device, 0, vb, stride);
    assert(hr == D3D_OK);
    hr = device->lpVtbl->SetIndices(device, ib, 0);
    assert(hr == D3D_OK);

    hr = device->lpVtbl->BeginScene(device);
    assert(hr == D3D_OK);
    hr = device->lpVtbl->DrawIndexedPrimitive(device, D3DPT_TRIANGLELIST, 0, 3, 0, 1);
    assert(hr == D3D_OK);
    hr = device->lpVtbl->EndScene(device);
    assert(hr == D3D_OK);

    unsigned char pixel[4] = {0};
    glReadPixels(1, 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    assert(pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 255);

    ib->lpVtbl->Release(ib);
    vb->lpVtbl->Release(vb);
    tex1->lpVtbl->Release(tex1);
    tex0->lpVtbl->Release(tex0);
    device->lpVtbl->Release(device);
    d3d->lpVtbl->Release(d3d);
    return 0;
}
