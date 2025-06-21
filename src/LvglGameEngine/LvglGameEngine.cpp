#include "LvglGameEngine.h"
#include "LvglKeyboard.h"
#include "LvglMouse.h"

LvglGameEngine::LvglGameEngine(LvglKeyboard* keyboard, LvglMouse* mouse)
    : m_keyboard(keyboard), m_mouse(mouse) {}

void LvglGameEngine::update() {
    if(m_keyboard) m_keyboard->update();
    if(m_mouse) m_mouse->update();
}
