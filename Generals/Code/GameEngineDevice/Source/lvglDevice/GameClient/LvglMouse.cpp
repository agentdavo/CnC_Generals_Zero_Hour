#include "lvglDevice/GameClient/LvglMouse.h"
#include "LvglPlatform/LvglPlatform.h"
#include <lvgl.h>

LvglMouse::LvglMouse()
    : m_indev(nullptr), m_native(nullptr), m_cursor_obj(nullptr)
{
    m_last_point.x = 0;
    m_last_point.y = 0;
    m_last_state = LV_INDEV_STATE_RELEASED;
}

LvglMouse::~LvglMouse() {}

void LvglMouse::init()
{
    Mouse::init();

    m_native = lv_indev_get_next(nullptr);
    while(m_native && lv_indev_get_type(m_native) != LV_INDEV_TYPE_POINTER)
        m_native = lv_indev_get_next(m_native);

    m_indev = lv_indev_create();
    if(m_indev) {
        lv_indev_set_type(m_indev, LV_INDEV_TYPE_POINTER);
        lv_indev_set_read_cb(m_indev, LvglMouse::read_cb);
        lv_indev_set_user_data(m_indev, this);
    }

    if(m_native)
        m_cursor_obj = lv_indev_get_cursor(m_native);
}

void LvglMouse::reset()
{
    Mouse::reset();
}

void LvglMouse::update()
{
    Mouse::update();

    if(m_native) {
        lv_point_t p;
        lv_indev_get_point(m_native, &p);
        m_currMouse.deltaPos.x = p.x - m_last_point.x;
        m_currMouse.deltaPos.y = p.y - m_last_point.y;
        m_currMouse.pos.x = p.x;
        m_currMouse.pos.y = p.y;

        lv_indev_state_t state = lv_indev_get_state(m_native);
        m_currMouse.leftState = (state == LV_INDEV_STATE_PRESSED) ? MBS_Down : MBS_Up;
        m_currMouse.time = lv_tick_get();
        m_last_point = p;
        m_last_state = state;
    }
}

UnsignedByte LvglMouse::getMouseEvent(MouseIO *result, Bool flush)
{
    (void)flush;
    if(!result) return MOUSE_NONE;

    lv_point_t p = m_last_point;
    lv_indev_state_t state = m_last_state;
    if(m_native) {
        lv_indev_get_point(m_native, &p);
        state = lv_indev_get_state(m_native);
    }

    result->pos.x = p.x;
    result->pos.y = p.y;
    result->deltaPos.x = p.x - m_last_point.x;
    result->deltaPos.y = p.y - m_last_point.y;
    result->wheelPos = 0;
    result->leftState = (state == LV_INDEV_STATE_PRESSED) ? MBS_Down : MBS_Up;
    result->leftEvent = MOUSE_EVENT_NONE;
    result->leftFrame = m_inputFrame;
    result->rightState = MBS_Up;
    result->rightEvent = MOUSE_EVENT_NONE;
    result->rightFrame = 0;
    result->middleState = MBS_Up;
    result->middleEvent = MOUSE_EVENT_NONE;
    result->middleFrame = 0;
    result->time = lv_tick_get();

    m_last_point = p;
    m_last_state = state;
    return MOUSE_OK;
}

void LvglMouse::setCursor(MouseCursor cursor)
{
    (void)cursor;
}

void LvglMouse::capture() {}

void LvglMouse::releaseCapture() {}

void LvglMouse::setVisibility(Bool visible)
{
    Mouse::setVisibility(visible);
    if(m_cursor_obj) {
        if(visible)
            lv_obj_clear_flag(m_cursor_obj, LV_OBJ_FLAG_HIDDEN);
        else
            lv_obj_add_flag(m_cursor_obj, LV_OBJ_FLAG_HIDDEN);
    }
}

void LvglMouse::initCursorResources() {}

void LvglMouse::read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
    LvglMouse *self = static_cast<LvglMouse*>(lv_indev_get_user_data(indev));
    if(!self) return;

    lv_point_t p;
    lv_indev_state_t state = LV_INDEV_STATE_RELEASED;
    if(self->m_native) {
        lv_indev_get_point(self->m_native, &p);
        state = lv_indev_get_state(self->m_native);
    } else {
        p.x = 0;
        p.y = 0;
    }

    data->point.x = p.x;
    data->point.y = p.y;
    data->state = state;

    self->m_currMouse.pos.x = p.x;
    self->m_currMouse.pos.y = p.y;
    self->m_currMouse.deltaPos.x = p.x - self->m_last_point.x;
    self->m_currMouse.deltaPos.y = p.y - self->m_last_point.y;
    self->m_currMouse.leftState = (state == LV_INDEV_STATE_PRESSED) ? MBS_Down : MBS_Up;
    self->m_currMouse.leftEvent = MOUSE_EVENT_NONE;
    self->m_currMouse.time = lv_tick_get();

    self->m_last_point = p;
    self->m_last_state = state;
}

