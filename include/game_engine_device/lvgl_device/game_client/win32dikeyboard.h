#pragma once

#include "game_client/keyboard.h"

class Win32DIKeyboard : public Keyboard
{
public:
    Win32DIKeyboard();
    virtual ~Win32DIKeyboard();

    virtual void init() override;
    virtual void reset() override;
    virtual void update() override;
    virtual Bool getCapsState() override;

protected:
    virtual void getKey(KeyboardIO *key) override;
};
