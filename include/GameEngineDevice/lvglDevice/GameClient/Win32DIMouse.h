#pragma once

#include "GameClient/Mouse.h"

class Win32DIMouse : public Mouse
{
public:
    Win32DIMouse();
    virtual ~Win32DIMouse();

    virtual void init() override;
    virtual void reset() override;
    virtual void update() override;
    virtual void initCursorResources() override;

    virtual void setCursor(MouseCursor cursor) override;
    virtual void capture() override;
    virtual void releaseCapture() override;
    virtual void setVisibility(Bool visible) override;

protected:
    virtual UnsignedByte getMouseEvent(MouseIO *result, Bool flush) override;
};

