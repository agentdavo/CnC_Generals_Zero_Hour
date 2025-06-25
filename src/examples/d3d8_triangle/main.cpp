#include "lvgl_platform/lvgl_platform.h"
#include <lvgl.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <d3d8_to_gles.h>
#include <cmath>
#include <cstring>

struct Vertex {
    float x, y, z;
    unsigned int color;
};

static EGLNativeWindowType get_native_window(lv_display_t *disp)
{
#if LV_USE_SDL
    SDL_Renderer *renderer = static_cast<SDL_Renderer*>(lv_sdl_window_get_renderer(disp));
    SDL_Window *window = SDL_RenderGetWindow(renderer);
    SDL_SysWMinfo wm;
    SDL_VERSION(&wm.version);
    if(!SDL_GetWindowWMInfo(window, &wm))
        return (EGLNativeWindowType)window;
#if defined(SDL_VIDEO_DRIVER_X11)
    return (EGLNativeWindowType)wm.info.x11.window;
#elif defined(SDL_VIDEO_DRIVER_WAYLAND)
    return (EGLNativeWindowType)wm.info.wl.egl_window;
#elif defined(SDL_VIDEO_DRIVER_ANDROID)
    return (EGLNativeWindowType)wm.info.android.window;
#else
    return (EGLNativeWindowType)window;
#endif
#else
    (void)disp;
    return 0;
#endif
}

int main()
{
    const uint32_t width = 640;
    const uint32_t height = 480;
    lv_display_t *disp = LvglPlatform::create_window(width, height, "sdl");
    if(!disp) return 1;

    EGLNativeWindowType native_window = get_native_window(disp);

    IDirect3D8 *d3d = Direct3DCreate8(D3D_SDK_VERSION);
    if(!d3d) return 1;

    D3DPRESENT_PARAMETERS pp = {};
    pp.BackBufferWidth = width;
    pp.BackBufferHeight = height;
    pp.BackBufferFormat = D3DFMT_X8R8G8B8;
    pp.BackBufferCount = 1;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.hDeviceWindow = (HWND)native_window;
    pp.Windowed = TRUE;
    pp.EnableAutoDepthStencil = FALSE;
    pp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    IDirect3DDevice8 *device = nullptr;
    if(d3d->lpVtbl->CreateDevice(d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                 pp.hDeviceWindow, 0, &pp, &device) != D3D_OK)
        return 1;

    Vertex verts[3] = {
        {-1.f, -1.f, 0.f, 0xFFFF0000u},
        { 1.f, -1.f, 0.f, 0xFF00FF00u},
        { 0.f,  1.f, 0.f, 0xFF0000FFu}
    };
    DWORD fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;
    UINT stride = D3DXGetFVFVertexSize(fvf);
    IDirect3DVertexBuffer8 *vb = nullptr;
    if(device->lpVtbl->CreateVertexBuffer(device, sizeof(verts), D3DUSAGE_WRITEONLY,
                                          fvf, D3DPOOL_MANAGED, &vb) != D3D_OK)
        return 1;
    void *data = nullptr;
    vb->lpVtbl->Lock(vb, 0, 0, &data, 0);
    std::memcpy(data, verts, sizeof(verts));
    vb->lpVtbl->Unlock(vb);

    WORD indices[3] = {0,1,2};
    IDirect3DIndexBuffer8 *ib = nullptr;
    if(device->lpVtbl->CreateIndexBuffer(device, sizeof(indices), D3DUSAGE_WRITEONLY,
                                         D3DFMT_INDEX16, D3DPOOL_MANAGED, &ib) != D3D_OK)
        return 1;
    ib->lpVtbl->Lock(ib, 0, 0, &data, 0);
    std::memcpy(data, indices, sizeof(indices));
    ib->lpVtbl->Unlock(ib);

    device->gles->fvf = fvf;
    device->lpVtbl->SetStreamSource(device, 0, vb, stride);
    device->lpVtbl->SetIndices(device, ib, 0);
    device->lpVtbl->SetRenderState(device, D3DRS_LIGHTING, FALSE);
    device->lpVtbl->SetRenderState(device, D3DRS_ZENABLE, FALSE);

    D3DXVECTOR3 eye = {0.f, 0.f, -3.f};
    D3DXVECTOR3 at  = {0.f, 0.f, 0.f};
    D3DXVECTOR3 up  = {0.f, 1.f, 0.f};
    D3DXMATRIX view;
    D3DXMatrixLookAtLH(&view, &eye, &at, &up);
    device->lpVtbl->SetTransform(device, D3DTS_VIEW, &view);

    D3DXMATRIX proj;
    D3DXMatrixPerspectiveFovLH(&proj, 60.0f * (3.14159265f/180.0f),
                               (float)width/(float)height, 0.1f, 10.f);
    device->lpVtbl->SetTransform(device, D3DTS_PROJECTION, &proj);

    float angle = 0.f;
    while(true) {
        LvglPlatform::poll_events();

        D3DXMATRIX world;
        D3DXMatrixRotationY(&world, angle);
        device->lpVtbl->SetTransform(device, D3DTS_WORLD, &world);
        angle += 0.01f;

        device->lpVtbl->BeginScene(device);
        device->lpVtbl->Clear(device, 0, nullptr, D3DCLEAR_TARGET, 0xFF101010, 1.f, 0);
        device->lpVtbl->DrawIndexedPrimitive(device, D3DPT_TRIANGLELIST, 0, 3, 0, 1);
        device->lpVtbl->EndScene(device);
        device->lpVtbl->Present(device, nullptr, nullptr, 0, nullptr);
    }

    return 0;
}

