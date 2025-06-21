#pragma once
class LvglKeyboard;
class LvglMouse;
class LvglGameEngine {
public:
    LvglGameEngine(LvglKeyboard* keyboard, LvglMouse* mouse);
    void update();
private:
    LvglKeyboard* m_keyboard;
    LvglMouse* m_mouse;
};
