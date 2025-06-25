#include "lvgl_game_engine/lvgl_game_engine.h"
#include "lvgl_platform/lvgl_platform.h"
#include "lvgl_present.h"
#include "common/logger.h"
#include "pipeline/gl_framebuffer.h"
#include <atomic>

void LvglGameEngine::init()
{
    LOG_INFO("LvglGameEngine::init");

    lv_display_t *disp = lv_display_get_default();
    uint32_t width = lv_display_get_horizontal_resolution(disp);
    uint32_t height = lv_display_get_vertical_resolution(disp);

    m_canvas = lv_canvas_create(lv_screen_active());
    lv_obj_set_pos(m_canvas, 0, 0);
    lv_obj_set_size(m_canvas, width, height);

    m_draw_buf = lv_draw_buf_create(width, height, LV_COLOR_FORMAT_NATIVE, LV_STRIDE_AUTO);
    lv_canvas_set_draw_buf(m_canvas, m_draw_buf);
    lv_canvas_fill_bg(m_canvas, lv_color_black(), LV_OPA_COVER);

    lvgl_set_game_engine(this);
}

void LvglGameEngine::reset()
{
    LOG_INFO("LvglGameEngine::reset");
}

void LvglGameEngine::update()
{
    LOG_INFO("LvglGameEngine::update");
    LvglPlatform::poll_events();
}

void LvglGameEngine::serviceWindowsOS()
{
    LOG_INFO("LvglGameEngine::serviceWindowsOS");
    LvglPlatform::poll_events();
}

void LvglGameEngine::present_framebuffer(const Framebuffer *fb)
{
    if(!fb || !m_draw_buf)
        return;

    uint32_t width = fb->width;
    uint32_t height = fb->height;
    uint16_t *dst = reinterpret_cast<uint16_t*>(m_draw_buf->data);

    for(uint32_t y = 0; y < height; ++y) {
        for(uint32_t x = 0; x < width; ++x) {
            uint32_t pixel = std::atomic_load(&fb->color_buffer[(size_t)y * width + x]);
            lv_color_t c;
            c.red = (pixel >> 16) & 0xFF;
            c.green = (pixel >> 8) & 0xFF;
            c.blue = pixel & 0xFF;
            dst[y * width + x] = lv_color_to_u16(c);
        }
    }

    lv_canvas_set_buffer(m_canvas, dst, width, height, LV_COLOR_FORMAT_NATIVE);
    lv_obj_invalidate(m_canvas);
}
