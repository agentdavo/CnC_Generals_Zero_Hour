#pragma once

#include "game_engine/common/modulefactory.h"

/**
 * LVGL-specific ModuleFactory implementation
 * This is a stub implementation for cross-platform development
 */
class LvglModuleFactory : public ModuleFactory
{
public:
    LvglModuleFactory();
    virtual ~LvglModuleFactory();

    // ModuleFactory interface implementation
    virtual void init() override;
    virtual void reset() override;
    virtual void update() override;
};