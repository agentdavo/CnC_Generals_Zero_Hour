#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "LvglPlatform/LvglPlatform.h"
#include "LvglInput/LvglKeyboard.h"
#include "LvglInput/LvglMouse.h"
#include "LvglGameEngine/LvglGameEngine.h"

static uint32_t window_width = 800;
static uint32_t window_height = 600;

int main(int argc, char **argv)
{
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

    lv_display_t *disp = LvglPlatform::create_window(window_width, window_height, backend);
    if(!disp) {
        std::cerr << "Failed to initialize LVGL backend" << std::endl;
        return 1;
    }

    LvglKeyboard keyboard;
    LvglMouse mouse;
    LvglGameEngine engine(&keyboard, &mouse);

    while(true) {
        engine.update();
        LvglPlatform::poll_events();
    }

    return 0;
}
