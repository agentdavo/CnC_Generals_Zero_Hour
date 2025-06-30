#pragma once

#include "game_client/gameclient.h"

/**
 * LVGL-specific GameClient implementation
 * This is a stub implementation for cross-platform development
 */
class LvglGameClient : public GameClient
{
public:
    LvglGameClient();
    virtual ~LvglGameClient();

    // GameClient interface implementation
    virtual void init() override;
    virtual void reset() override;
    virtual void update() override;
};