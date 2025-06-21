#pragma once

#include "GameClient/Keyboard.h"

class Win32DIKeyboard : public Keyboard
{
public:
    Win32DIKeyboard() = default;
    virtual ~Win32DIKeyboard() = default;

    virtual void init() override {}
    virtual void reset() override {}
    virtual void update() override {}
    virtual Bool getCapsState() override { return FALSE; }

protected:
    virtual void getKey(KeyboardIO* key) override { (void)key; }
};
