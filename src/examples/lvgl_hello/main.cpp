#include "lvgl_platform/lvgl_platform.h"
#include <lvgl.h>

int main() {
    lv_display_t *disp = LvglPlatform::create_window(800, 600, nullptr);
    if(!disp) return 1;
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello world");
    lv_obj_center(label);
    while(true) {
        LvglPlatform::poll_events();
    }
    return 0;
}
