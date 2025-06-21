#pragma once

#include "Common/GameEngine.h"

class Win32GameEngine : public GameEngine
{
public:
    Win32GameEngine() = default;
    virtual ~Win32GameEngine() = default;

    virtual void init() override {}
    virtual void reset() override {}
    virtual void update() override {}
    virtual void serviceWindowsOS() override {}
};
