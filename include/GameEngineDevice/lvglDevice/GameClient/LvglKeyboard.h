#pragma once

#include "GameClient/Keyboard.h"
#include <lvgl.h>

class LvglKeyboard : public Keyboard
{
public:
    LvglKeyboard();
    virtual ~LvglKeyboard();

    virtual void init(void) override;
    virtual void reset(void) override;
    virtual void update(void) override;
    virtual Bool getCapsState(void) override;

protected:
    virtual void getKey(KeyboardIO *key) override;

private:
    lv_indev_t *m_indev;
};


