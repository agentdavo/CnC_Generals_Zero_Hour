#include "lvgl_game_engine.h"
#include "LvglKeyboard.h"
#include "LvglMouse.h"
#include "common/logger.h"

LvglGameEngine::LvglGameEngine(LvglKeyboard *keyboard, LvglMouse *mouse)
    : m_keyboard(keyboard), m_mouse(mouse)
{
    LOG_INFO("LvglGameEngine created");
}

void LvglGameEngine::update()
{
    if (m_keyboard)
        m_keyboard->update();
    if (m_mouse)
        m_mouse->update();
}
