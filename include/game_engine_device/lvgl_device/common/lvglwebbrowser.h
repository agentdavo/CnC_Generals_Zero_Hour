#pragma once

#include "game_network/WOLBrowser/WebBrowser.h"

/**
 * LVGL-specific WebBrowser implementation
 * This is a stub implementation for cross-platform development
 */
class LvglWebBrowser : public WebBrowser
{
public:
    LvglWebBrowser();
    virtual ~LvglWebBrowser();

    // WebBrowser interface implementation
    virtual void init() override;
    virtual void reset() override;
    virtual void update() override;
};