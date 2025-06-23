#include "lvglDevice/GameClient/LvglMouse.h"
#include "LvglPlatform/LvglPlatform.h"
#include "common/Logger.h"
#include <lvgl.h>
#if LV_USE_WAYLAND
#include "lvgl/src/drivers/wayland/lv_wayland_private.h"
#endif

static lv_indev_state_t get_right_state(lv_indev_t *indev)
{
#if LV_USE_WAYLAND
    lv_display_t *disp = lv_indev_get_display(indev);
    window *win = static_cast<window *>(lv_display_get_user_data(disp));
    if(win) return win->body->input.pointer.right_button;
#else
    (void)indev;
#endif
    return LV_INDEV_STATE_RELEASED;
}

static lv_indev_state_t get_middle_state(lv_indev_t *indev)
{
#if LV_USE_WAYLAND
    lv_display_t *disp = lv_indev_get_display(indev);
    window *win = static_cast<window *>(lv_display_get_user_data(disp));
    if(win) return win->body->input.pointer.wheel_button;
#else
    (void)indev;
#endif
    return LV_INDEV_STATE_RELEASED;
}

static int16_t get_wheel_diff(lv_indev_t *indev)
{
#if LV_USE_WAYLAND
    lv_display_t *disp = lv_indev_get_display(indev);
    window *win = static_cast<window *>(lv_display_get_user_data(disp));
    if(win) {
        int16_t d = win->body->input.pointer.wheel_diff;
        win->body->input.pointer.wheel_diff = 0;
        return d;
    }
#else
    (void)indev;
#endif
    return 0;
}

LvglMouse::LvglMouse()
    : m_indev(nullptr), m_native(nullptr), m_cursor_obj(nullptr),
      m_nextFreeIndex(0), m_nextGetIndex(0)
{
    m_last_point.x = 0;
    m_last_point.y = 0;
    m_last_state = LV_INDEV_STATE_RELEASED;
    m_last_right = LV_INDEV_STATE_RELEASED;
    m_last_middle = LV_INDEV_STATE_RELEASED;
    m_last_wheel = 0;
    for(auto &e : m_eventBuffer) e.valid = FALSE;
}

LvglMouse::~LvglMouse() {}

void LvglMouse::init()
{
    LOG_INFO("LvglMouse::init");
    Mouse::init();

    m_inputMovesAbsolute = TRUE;

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

    initCursorResources();
}

void LvglMouse::reset()
{
    Mouse::reset();
}

void LvglMouse::update()
{
    Mouse::update();

    if(!m_native)
        return;

    lv_point_t p;
    lv_indev_get_point(m_native, &p);
    lv_indev_state_t left   = lv_indev_get_state(m_native);
    lv_indev_state_t right  = get_right_state(m_native);
    lv_indev_state_t middle = get_middle_state(m_native);
    int16_t wheel           = get_wheel_diff(m_native);

    if(m_eventBuffer[m_nextFreeIndex].valid)
        return; /* buffer full */

    EventEntry &ev = m_eventBuffer[m_nextFreeIndex];
    ev.io.pos.x = p.x;
    ev.io.pos.y = p.y;
    ev.io.deltaPos.x = p.x - m_last_point.x;
    ev.io.deltaPos.y = p.y - m_last_point.y;
    ev.io.wheelPos = wheel;
    ev.io.leftState   = (left == LV_INDEV_STATE_PRESSED) ? MBS_Down : MBS_Up;
    ev.io.leftFrame   = (left != m_last_state) ? 1 : 0;
    ev.io.rightState  = (right == LV_INDEV_STATE_PRESSED) ? MBS_Down : MBS_Up;
    ev.io.rightFrame  = (right != m_last_right) ? 1 : 0;
    ev.io.middleState = (middle == LV_INDEV_STATE_PRESSED) ? MBS_Down : MBS_Up;
    ev.io.middleFrame = (middle != m_last_middle) ? 1 : 0;
    ev.io.leftEvent = ev.io.rightEvent = ev.io.middleEvent = MOUSE_EVENT_NONE;
    ev.io.time = lv_tick_get();

    ev.valid = TRUE;
    m_nextFreeIndex++;
    if(m_nextFreeIndex >= NUM_MOUSE_EVENTS) m_nextFreeIndex = 0;

    m_last_point = p;
    m_last_state = left;
    m_last_right = right;
    m_last_middle = middle;
    m_last_wheel = wheel;
}

UnsignedByte LvglMouse::getMouseEvent(MouseIO *result, Bool flush)
{
    (void)flush;
    if(!result) return MOUSE_NONE;

    if(!m_eventBuffer[m_nextGetIndex].valid)
        return MOUSE_NONE;

    *result = m_eventBuffer[m_nextGetIndex].io;
    m_eventBuffer[m_nextGetIndex].valid = FALSE;
    m_nextGetIndex++;
    if(m_nextGetIndex >= NUM_MOUSE_EVENTS) m_nextGetIndex = 0;

    return MOUSE_OK;
}

void LvglMouse::setCursor(MouseCursor cursor)
{
    Mouse::setCursor(cursor);
    if(!m_cursor_obj) return;

    const void *sym = LV_SYMBOL_RIGHT;
    lv_image_set_src(m_cursor_obj, sym);
    lv_indev_set_cursor(m_indev, m_cursor_obj);
    if(!m_visible)
        lv_obj_add_flag(m_cursor_obj, LV_OBJ_FLAG_HIDDEN);
    else
        lv_obj_clear_flag(m_cursor_obj, LV_OBJ_FLAG_HIDDEN);
}

void LvglMouse::capture() {
    if(m_indev) lv_indev_wait_release(m_indev);
}

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

void LvglMouse::initCursorResources()
{
    LOG_INFO("LvglMouse::initCursorResources");
    if(!m_indev || m_cursor_obj) return;
    m_cursor_obj = lv_image_create(lv_screen_active());
    lv_image_set_src(m_cursor_obj, LV_SYMBOL_RIGHT);
    lv_obj_add_flag(m_cursor_obj, LV_OBJ_FLAG_IGNORE_LAYOUT);
    if(!m_visible) lv_obj_add_flag(m_cursor_obj, LV_OBJ_FLAG_HIDDEN);
    lv_indev_set_cursor(m_indev, m_cursor_obj);
}

void LvglMouse::read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
    LvglMouse *self = static_cast<LvglMouse*>(lv_indev_get_user_data(indev));
    if(!self) return;

    lv_point_t p;
    lv_indev_state_t state = LV_INDEV_STATE_RELEASED;
    lv_indev_state_t right = LV_INDEV_STATE_RELEASED;
    lv_indev_state_t middle = LV_INDEV_STATE_RELEASED;
    int16_t wheel = 0;
    if(self->m_native) {
        lv_indev_get_point(self->m_native, &p);
        state = lv_indev_get_state(self->m_native);
        right = get_right_state(self->m_native);
        middle = get_middle_state(self->m_native);
        wheel = get_wheel_diff(self->m_native);
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
    self->m_currMouse.rightState = (right == LV_INDEV_STATE_PRESSED) ? MBS_Down : MBS_Up;
    self->m_currMouse.middleState = (middle == LV_INDEV_STATE_PRESSED) ? MBS_Down : MBS_Up;
    self->m_currMouse.wheelPos = wheel;
    self->m_currMouse.leftEvent = MOUSE_EVENT_NONE;
    self->m_currMouse.time = lv_tick_get();

    self->m_last_point = p;
    self->m_last_state = state;
    self->m_last_right = right;
    self->m_last_middle = middle;
}

