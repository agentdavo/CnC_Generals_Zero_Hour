#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "LvglPlatform/LvglPlatform.h"

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
        LvglPlatform::poll_events();
    }

    return 0;
}
