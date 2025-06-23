#pragma once

#include "Common/GameEngine.h"
#include "GameNetwork/NetworkInterface.h"
#include "GameLogic/GameLogic.h"

class LvglGameEngine : public GameEngine
{
public:
    LvglGameEngine() = default;
    virtual ~LvglGameEngine() = default;

    virtual void init();
    virtual void reset();
    virtual void update();
    virtual void serviceWindowsOS();
};
