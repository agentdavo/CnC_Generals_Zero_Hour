#ifndef LVGL_PRESENT_H
#define LVGL_PRESENT_H

#ifdef __cplusplus
extern "C" {
#endif

struct Framebuffer;

void lvgl_present_framebuffer(const struct Framebuffer *fb);

#ifdef __cplusplus
}
class LvglGameEngine;
void lvgl_set_game_engine(LvglGameEngine *engine);
#endif

#endif // LVGL_PRESENT_H
