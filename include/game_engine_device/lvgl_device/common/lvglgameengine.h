#pragma once

#include "common/GameEngine.h"
#include "game_network/NetworkInterface.h"
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
