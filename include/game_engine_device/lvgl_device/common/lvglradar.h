#pragma once

#include "game_engine/common/radar.h"

/**
 * LVGL-specific Radar implementation
 * This is a stub implementation for cross-platform development
 */
class LvglRadar : public Radar
{
public:
    LvglRadar();
    virtual ~LvglRadar();

    // Radar interface implementation
    virtual void init() override;
    virtual void reset() override;
    virtual void update() override;
};