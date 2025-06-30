#pragma once

#include "game_engine/common/thingfactory.h"

/**
 * LVGL-specific ThingFactory implementation
 * This is a stub implementation for cross-platform development
 */
class LvglThingFactory : public ThingFactory
{
public:
    LvglThingFactory();
    virtual ~LvglThingFactory();

    // ThingFactory interface implementation
    virtual void init() override;
    virtual void reset() override;
    virtual void update() override;
};