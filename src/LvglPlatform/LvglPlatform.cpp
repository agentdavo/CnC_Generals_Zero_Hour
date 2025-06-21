#include "LvglPlatform.h"

#include <cstring>
#include <unistd.h>

#if LV_USE_SDL
#include "lvgl/src/drivers/sdl/lv_sdl_window.h"
#endif

#if LV_USE_X11
#include "lvgl/src/drivers/x11/lv_x11.h"
#endif

#if LV_USE_WAYLAND
#include "lvgl/src/drivers/wayland/lv_wayland.h"
#include "lvgl/src/drivers/wayland/lv_wl_window.h"
#endif

#if LV_USE_LINUX_DRM
#include "lvgl/src/drivers/display/drm/lv_linux_drm.h"
#endif

#if LV_USE_LINUX_FBDEV
#include "lvgl/src/drivers/display/fb/lv_linux_fbdev.h"
#endif

#if LV_USE_NUTTX
#include "lvgl/src/drivers/nuttx/lv_nuttx_entry.h"
#endif

namespace LvglPlatform {

static bool lvgl_initialized = false;

lv_display_t *create_window(uint32_t width, uint32_t height, const char *backend)
{
    if(!lvgl_initialized) {
        lv_init();
        lvgl_initialized = true;
    }

#if LV_USE_SDL
    if(backend && std::strcmp(backend, "sdl") == 0) {
        return lv_sdl_window_create(width, height);
    }
#endif

#if LV_USE_WAYLAND
    if(backend && std::strcmp(backend, "wayland") == 0) {
        return lv_wayland_window_create(width, height, (char *)"LVGL Simulator", NULL);
    }
#endif

#if LV_USE_LINUX_DRM
    if(backend && std::strcmp(backend, "drm") == 0) {
        lv_display_t *d = lv_linux_drm_create();
        if(d) lv_linux_drm_set_file(d, "/dev/dri/card0", -1);
        return d;
    }
#endif

#if LV_USE_LINUX_FBDEV
    if(backend && std::strcmp(backend, "fbdev") == 0) {
        lv_display_t *d = lv_linux_fbdev_create();
        if(d) lv_linux_fbdev_set_file(d, "/dev/fb0");
        return d;
    }
#endif

#if LV_USE_NUTTX
    if(backend && std::strcmp(backend, "nuttx") == 0) {
        lv_nuttx_dsc_t dsc; lv_nuttx_result_t res;
        lv_nuttx_dsc_init(&dsc);
        lv_nuttx_init(&dsc, &res);
        return res.disp;
    }
#endif

#if LV_USE_X11
    lv_display_t *disp = lv_x11_window_create("LVGL", width, height);
    if(disp) lv_x11_inputs_create(disp, nullptr);
    return disp;
#else
    (void)width; (void)height; (void)backend;
    return nullptr;
#endif
}

void poll_events()
{
    uint32_t wait = lv_timer_handler();
    usleep(wait * 1000);
}

} // namespace LvglPlatform

