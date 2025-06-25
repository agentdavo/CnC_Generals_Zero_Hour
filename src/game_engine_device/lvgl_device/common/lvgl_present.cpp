#include "lvgl_present.h"
#include "game_engine_device/lvgl_device/common/lvglgameengine.h"
#include "pipeline/gl_framebuffer.h"

extern "C" Framebuffer *GL_get_default_framebuffer(void);

static LvglGameEngine *g_engine = nullptr;

extern "C" void lvgl_present_framebuffer(const Framebuffer *fb)
{
    if(!fb)
        fb = GL_get_default_framebuffer();
    if(g_engine && fb)
        g_engine->present_framebuffer(fb);
}

void lvgl_set_game_engine(LvglGameEngine *engine)
{
    g_engine = engine;
}
