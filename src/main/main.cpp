#include <cstdlib>
#include <cstring>
#include "common/logger.h"
#include <unistd.h>

#include "lvgl_platform/lvgl_platform.h"
#include "lvgl_device/game_client/lvglkeyboard.h"
#include "lvgl_device/game_client/lvglmouse.h"
#include "lvgl_game_engine/lvgl_game_engine.h"
#include "game_engine_device/lvgl_device/common/lvgllocalfilesystem.h"
#include "game_engine_device/lvgl_device/common/win32_big_file_system.h"
#include "game_engine/common/filesystem.h"
#include "game_engine/common/localfilesystem.h"
#include "game_engine/common/archivefilesystem.h"

extern "C" {
#include "gl_init.h"
#include "gl_thread.h"
#include "pipeline/gl_framebuffer.h"
}

static uint32_t window_width = 800;
static uint32_t window_height = 600;

int main(int argc, char **argv)
{
    Logger::init();
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

    LOG_INFO("Creating window %ux%u using backend %s", window_width, window_height, backend ? backend : "default");
    lv_display_t *disp = LvglPlatform::create_window(window_width, window_height, backend);
    if(!disp) {
        LOG_ERROR("Failed to initialize LVGL backend");
        Logger::shutdown();
        return 1;
    }
    LOG_INFO("Window created");

    static LvglLocalFileSystem local_fs;
    static Win32BIGFileSystem archive_fs;
    static FileSystem fs;
    TheLocalFileSystem = &local_fs;
    TheArchiveFileSystem = &archive_fs;
    TheFileSystem = &fs;
    TheFileSystem->init();

    LvglKeyboard keyboard;
    LvglMouse mouse;
    LvglGameEngine engine(&keyboard, &mouse);

    LV_DRAW_BUF_DEFINE_STATIC(frame_buf, window_width, window_height,
                              LV_COLOR_FORMAT_ARGB8888);
    LV_DRAW_BUF_INIT_STATIC(frame_buf);
    lv_obj_t *canvas = lv_canvas_create(lv_screen_active());
    lv_canvas_set_draw_buf(canvas, &frame_buf);
    lv_obj_center(canvas);

    LOG_INFO("Entering main loop");
    while(true) {
        engine.update();
        thread_pool_wait();
        Framebuffer *fb = GL_get_default_framebuffer();
        if(fb) {
            lv_color32_t *dst = (lv_color32_t *)frame_buf.data;
            for(uint32_t y = 0; y < window_height; ++y) {
                for(uint32_t x = 0; x < window_width; ++x) {
                    uint32_t p = framebuffer_get_pixel(fb, x, y);
                    lv_color32_t c;
                    c.red   = (p >> 16) & 0xFF;
                    c.green = (p >> 8) & 0xFF;
                    c.blue  = p & 0xFF;
                    c.alpha = (p >> 24) & 0xFF;
                    dst[y * window_width + x] = c;
                }
            }
            lv_obj_invalidate(canvas);
        }
        LvglPlatform::poll_events();
    }

    Logger::shutdown();

    return 0;
}
