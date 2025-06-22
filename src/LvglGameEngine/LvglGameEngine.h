#pragma once
class LvglKeyboard;
class LvglMouse;
class LvglGameEngine {
public:
    LvglGameEngine(LvglKeyboard* keyboard, LvglMouse* mouse);
    void init();
    void reset();
    void update();
    void serviceWindowsOS();
private:
    LvglKeyboard* m_keyboard;
    LvglMouse* m_mouse;
};
