#pragma once

#ifndef __LVGLMOUSE_H_
#define __LVGLMOUSE_H_

#include "GameClient/Mouse.h"
#include <lvgl.h>

class LvglMouse : public Mouse
{
public:
    LvglMouse();
    virtual ~LvglMouse();

    virtual void init(void) override;
    virtual void reset(void) override;
    virtual void update(void) override;
    virtual void initCursorResources(void) override;

    virtual void setCursor(MouseCursor cursor) override;
    virtual void capture(void) override;
    virtual void releaseCapture(void) override;
    virtual void setVisibility(Bool visible) override;

protected:
    virtual UnsignedByte getMouseEvent(MouseIO *result, Bool flush) override;

private:
    static void read_cb(lv_indev_t *indev, lv_indev_data_t *data);

    lv_indev_t *m_indev;        // input device registered for LVGL
    lv_indev_t *m_native;       // underlying LVGL pointer device
    lv_obj_t   *m_cursor_obj;   // cursor object if any
    lv_point_t  m_last_point;
    lv_indev_state_t m_last_state;
    lv_indev_state_t m_last_right;
    lv_indev_state_t m_last_middle;
    int16_t m_last_wheel;

    struct EventEntry {
        Bool valid;
        MouseIO io;
    };
    EventEntry m_eventBuffer[NUM_MOUSE_EVENTS];
    UnsignedInt m_nextFreeIndex;
    UnsignedInt m_nextGetIndex;
};

#endif // __LVGLMOUSE_H_
