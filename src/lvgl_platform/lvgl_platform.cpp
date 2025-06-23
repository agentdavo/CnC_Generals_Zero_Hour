#include "lvgl_platform.h"
#include "common/logger.h"

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

namespace LvglPlatform
{

    static bool lvgl_initialized = false;

    lv_display_t *create_window(uint32_t width, uint32_t height, const char *backend)
    {
        LOG_INFO("create_window %ux%u backend=%s", width, height, backend ? backend : "default");
        if (!lvgl_initialized)
        {
            lv_init();
            lvgl_initialized = true;
            LOG_INFO("LVGL initialized");
        }

        lv_display_t *disp = nullptr;
#if LV_USE_SDL
        if (!disp && backend && std::strcmp(backend, "sdl") == 0)
        {
            LOG_INFO("Using SDL backend");
            disp = lv_sdl_window_create(width, height);
        }
#endif

#if LV_USE_WAYLAND
        if (!disp && backend && std::strcmp(backend, "wayland") == 0)
        {
            LOG_INFO("Using Wayland backend");
            disp = lv_wayland_window_create(width, height, (char *)"LVGL Simulator", NULL);
        }
#endif

#if LV_USE_LINUX_DRM
        if (!disp && backend && std::strcmp(backend, "drm") == 0)
        {
            LOG_INFO("Using DRM backend");
            disp = lv_linux_drm_create();
            if (disp)
                lv_linux_drm_set_file(disp, "/dev/dri/card0", -1);
        }
#endif

#if LV_USE_LINUX_FBDEV
        if (!disp && backend && std::strcmp(backend, "fbdev") == 0)
        {
            LOG_INFO("Using FBDEV backend");
            disp = lv_linux_fbdev_create();
            if (disp)
                lv_linux_fbdev_set_file(disp, "/dev/fb0");
        }
#endif

#if LV_USE_NUTTX
        if (!disp && backend && std::strcmp(backend, "nuttx") == 0)
        {
            LOG_INFO("Using NuttX backend");
            lv_nuttx_dsc_t dsc;
            lv_nuttx_result_t res;
            lv_nuttx_dsc_init(&dsc);
            lv_nuttx_init(&dsc, &res);
            disp = res.disp;
        }
#endif

#if LV_USE_X11
        if (!disp)
        {
            LOG_INFO("Using X11 backend");
            disp = lv_x11_window_create("LVGL", width, height);
            if (disp)
                lv_x11_inputs_create(disp, nullptr);
        }
#else
        (void)width;
        (void)height;
        (void)backend;
#endif

        LOG_INFO("window created: %p", (void *)disp);
        return disp;
    }

    void poll_events()
    {
        uint32_t wait = lv_timer_handler();
        usleep(wait * 1000);
    }

} // namespace LvglPlatform
