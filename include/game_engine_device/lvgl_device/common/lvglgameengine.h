#pragma once

#include "common/GameEngine.h"
#include "game_network/NetworkInterface.h"
#include "GameLogic/GameLogic.h"
#include <lvgl.h>
#include "lvgl_present.h"

struct Framebuffer;

class LvglGameEngine : public GameEngine
{
public:
    LvglGameEngine() = default;
    virtual ~LvglGameEngine() = default;

    virtual void init();
    virtual void reset();
    virtual void update();
    virtual void serviceWindowsOS();

    void present_framebuffer(const Framebuffer *fb);

private:
    lv_obj_t *m_canvas = nullptr;
    lv_draw_buf_t *m_draw_buf = nullptr;
};
