#pragma once

#include "GameClient/Mouse.h"

class Win32Mouse : public Mouse
{
public:
    Win32Mouse() = default;
    virtual ~Win32Mouse() = default;

    virtual void init() override {}
    virtual void reset() override {}
    virtual void update() override {}
    virtual void initCursorResources() override {}
    virtual void setCursor(MouseCursor) override {}
    virtual void capture() override {}
    virtual void releaseCapture() override {}

protected:
    virtual UnsignedByte getMouseEvent(MouseIO* result, Bool flush) override { (void)result; (void)flush; return 0; }
};
