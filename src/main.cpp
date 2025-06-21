#include <lvgl.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
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

static uint32_t window_width = 800;
static uint32_t window_height = 600;

static lv_display_t * init_display(const char *backend)
{
#if LV_USE_SDL
    if(backend && std::strcmp(backend, "sdl") == 0) {
        return lv_sdl_window_create(window_width, window_height);
    }
#endif

#if LV_USE_WAYLAND
    if(backend && std::strcmp(backend, "wayland") == 0) {
        return lv_wayland_window_create(window_width, window_height,
                                        (char *)"LVGL Simulator", NULL);
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
    lv_display_t *disp = lv_x11_window_create("LVGL", window_width, window_height);
    if(disp) lv_x11_inputs_create(disp, nullptr);
    return disp;
#else
    (void)backend;
    return nullptr;
#endif
}

int main(int argc, char **argv)
{
    lv_init();

    const char *backend = nullptr;
    for(int i = 1; i < argc; ++i) {
        if(std::strncmp(argv[i], "--width=", 8) == 0) {
            window_width = static_cast<uint32_t>(std::atoi(argv[i] + 8));
        } else if(std::strncmp(argv[i], "--height=", 9) == 0) {
            window_height = static_cast<uint32_t>(std::atoi(argv[i] + 9));
        } else if(std::strncmp(argv[i], "--backend=", 10) == 0) {
            backend = argv[i] + 10;
        } else if(std::strcmp(argv[i], "--backend") == 0 && i + 1 < argc) {
            backend = argv[++i];
        } else if(!backend) {
            backend = argv[i];
        }
    }

    if(!backend)
        backend = std::getenv("LV_BACKEND");

    lv_display_t *disp = init_display(backend);
    if(!disp) {
        std::cerr << "Failed to initialize LVGL backend" << std::endl;
        return 1;
    }

    struct resize_ctx {
        lv_display_t *disp;
        uint32_t base_w;
        uint32_t base_h;
        bool toggled;
    } ctx { disp, window_width, window_height, false };

    auto resize_cb = [](lv_timer_t *t){
        resize_ctx *c = static_cast<resize_ctx *>(lv_timer_get_user_data(t));
        uint32_t new_w = c->toggled ? c->base_w : c->base_w + 100;
        uint32_t new_h = c->toggled ? c->base_h : c->base_h + 100;
        lv_display_set_resolution(c->disp, new_w, new_h);
        c->toggled = !c->toggled;
    };

    lv_timer_t *timer = lv_timer_create(resize_cb, 3000, &ctx);
    (void)timer;

    while(true) {
        uint32_t wait = lv_timer_handler();
        usleep(wait * 1000);
    }

    return 0;
}
