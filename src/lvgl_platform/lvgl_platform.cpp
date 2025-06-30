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
        LOG_INFO("LvglPlatform::create_window: Creating %ux%u window, backend='%s'", 
                 width, height, backend ? backend : "default");
        
        if (!lvgl_initialized)
        {
            LOG_INFO("LvglPlatform::create_window: LVGL not initialized, calling lv_init()");
            lv_init();
            lvgl_initialized = true;
            LOG_INFO("LvglPlatform::create_window: LVGL core initialization completed");
        } else {
            LOG_INFO("LvglPlatform::create_window: LVGL already initialized, skipping lv_init()");
        }

        lv_display_t *disp = nullptr;
        LOG_INFO("LvglPlatform::create_window: Testing backend options...");
#if LV_USE_SDL
        if (!disp && backend && std::strcmp(backend, "sdl") == 0)
        {
            LOG_INFO("LvglPlatform::create_window: Attempting SDL backend initialization");
            disp = lv_sdl_window_create(width, height);
            if(disp) {
                LOG_INFO("LvglPlatform::create_window: SDL backend successfully created window");
            } else {
                LOG_ERROR("LvglPlatform::create_window: SDL backend failed to create window");
            }
        }
#endif

#if LV_USE_WAYLAND
        if (!disp && backend && std::strcmp(backend, "wayland") == 0)
        {
            LOG_INFO("LvglPlatform::create_window: Attempting Wayland backend initialization");
            disp = lv_wayland_window_create(width, height, (char *)"Command & Conquer Generals", NULL);
            if(disp) {
                LOG_INFO("LvglPlatform::create_window: Wayland backend successfully created window");
            } else {
                LOG_ERROR("LvglPlatform::create_window: Wayland backend failed to create window");
            }
        }
#endif

#if LV_USE_LINUX_DRM
        if (!disp && backend && std::strcmp(backend, "drm") == 0)
        {
            LOG_INFO("LvglPlatform::create_window: Attempting DRM backend initialization");
            disp = lv_linux_drm_create();
            if (disp) {
                lv_linux_drm_set_file(disp, "/dev/dri/card0", -1);
                LOG_INFO("LvglPlatform::create_window: DRM backend successfully created, using /dev/dri/card0");
            } else {
                LOG_ERROR("LvglPlatform::create_window: DRM backend failed to create display");
            }
        }
#endif

#if LV_USE_LINUX_FBDEV
        if (!disp && backend && std::strcmp(backend, "fbdev") == 0)
        {
            LOG_INFO("LvglPlatform::create_window: Attempting FBDEV backend initialization");
            disp = lv_linux_fbdev_create();
            if (disp) {
                lv_linux_fbdev_set_file(disp, "/dev/fb0");
                LOG_INFO("LvglPlatform::create_window: FBDEV backend successfully created, using /dev/fb0");
            } else {
                LOG_ERROR("LvglPlatform::create_window: FBDEV backend failed to create display");
            }
        }
#endif

#if LV_USE_NUTTX
        if (!disp && backend && std::strcmp(backend, "nuttx") == 0)
        {
            LOG_INFO("LvglPlatform::create_window: Attempting NuttX backend initialization");
            lv_nuttx_dsc_t dsc;
            lv_nuttx_result_t res;
            lv_nuttx_dsc_init(&dsc);
            lv_nuttx_init(&dsc, &res);
            disp = res.disp;
            if(disp) {
                LOG_INFO("LvglPlatform::create_window: NuttX backend successfully initialized");
            } else {
                LOG_ERROR("LvglPlatform::create_window: NuttX backend failed to initialize");
            }
        }
#endif

#if LV_USE_X11
        if (!disp)
        {
            LOG_INFO("LvglPlatform::create_window: Attempting X11 backend initialization (fallback)");
            disp = lv_x11_window_create("Command & Conquer Generals", width, height);
            if (disp) {
                lv_x11_inputs_create(disp, nullptr);
                LOG_INFO("LvglPlatform::create_window: X11 backend successfully created window with inputs");
            } else {
                LOG_ERROR("LvglPlatform::create_window: X11 backend failed to create window");
            }
        }
#else
        (void)width;
        (void)height;
        (void)backend;
        LOG_WARN("LvglPlatform::create_window: No fallback backend available");
#endif

        if(disp) {
            LOG_INFO("LvglPlatform::create_window: SUCCESS - Window created with display pointer %p", (void *)disp);
        } else {
            LOG_ERROR("LvglPlatform::create_window: CRITICAL FAILURE - No backend could create a window");
        }
        return disp;
    }

    void poll_events()
    {
        // Note: This function is called very frequently, so we avoid logging here
        // to prevent log spam. Only log if there are issues.
        uint32_t wait = lv_timer_handler();
        if(wait > 100) {
            // Only log if we're going to wait more than 100ms
            LOG_INFO("LvglPlatform::poll_events: Long wait time %u ms", wait);
        }
        usleep(wait * 1000);
    }

} // namespace LvglPlatform
