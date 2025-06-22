#include <cstdlib>
#include <cstring>
#include "Common/Logger.h"
#include <unistd.h>

#include "LvglPlatform/LvglPlatform.h"
#include "lvglDevice/GameClient/LvglKeyboard.h"
#include "lvglDevice/GameClient/LvglMouse.h"
#include "LvglGameEngine/LvglGameEngine.h"

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
