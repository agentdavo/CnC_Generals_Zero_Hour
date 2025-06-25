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

    LOG_INFO("Entering main loop");
    while(true) {
        engine.update();
        LvglPlatform::poll_events();
    }

    Logger::shutdown();

    return 0;
}
