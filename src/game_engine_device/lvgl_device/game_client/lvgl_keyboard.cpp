#include "lvgl_device/game_client/lvglkeyboard.h"
#include "game_engine/common/debug.h"
#include "common/logger.h"
#include "game_client/keydefs.h"
#include <cstring>

LvglKeyboard::LvglKeyboard()
    : m_indev(nullptr)
{
}

LvglKeyboard::~LvglKeyboard()
{
}

static UnsignedByte convert_lv_key(uint32_t k)
{
    switch(k) {
        case LV_KEY_UP:        return KEY_UP;
        case LV_KEY_DOWN:      return KEY_DOWN;
        case LV_KEY_LEFT:      return KEY_LEFT;
        case LV_KEY_RIGHT:     return KEY_RIGHT;
        case LV_KEY_ESC:       return KEY_ESC;
        case LV_KEY_DEL:       return KEY_DEL;
        case LV_KEY_BACKSPACE: return KEY_BACKSPACE;
        case LV_KEY_ENTER:     return KEY_ENTER;
        case LV_KEY_NEXT:      return KEY_TAB;
        case LV_KEY_PREV:      return KEY_TAB;
        case LV_KEY_HOME:      return KEY_HOME;
        case LV_KEY_END:       return KEY_END;
        default: break;
    }

    /* Windows virtual key codes for function and keypad keys */
    switch(k) {
        case 0x70: return KEY_F1;  /* VK_F1  */
        case 0x71: return KEY_F2;
        case 0x72: return KEY_F3;
        case 0x73: return KEY_F4;
        case 0x74: return KEY_F5;
        case 0x75: return KEY_F6;
        case 0x76: return KEY_F7;
        case 0x77: return KEY_F8;
        case 0x78: return KEY_F9;
        case 0x79: return KEY_F10;
        case 0x7A: return KEY_F11;
        case 0x7B: return KEY_F12;
        case 0x60: return KEY_KP0;  /* VK_NUMPAD0 */
        case 0x61: return KEY_KP1;
        case 0x62: return KEY_KP2;
        case 0x63: return KEY_KP3;
        case 0x64: return KEY_KP4;
        case 0x65: return KEY_KP5;
        case 0x66: return KEY_KP6;
        case 0x67: return KEY_KP7;
        case 0x68: return KEY_KP8;
        case 0x69: return KEY_KP9;
        case 0x6E: return KEY_KPDEL;   /* VK_DECIMAL */
        case 0x6A: return KEY_KPSTAR;  /* VK_MULTIPLY */
        case 0x6D: return KEY_KPMINUS; /* VK_SUBTRACT */
        case 0x6B: return KEY_KPPLUS;  /* VK_ADD */
        case 0x6F: return KEY_KPSLASH; /* VK_DIVIDE */
        case 0x0D: return KEY_KPENTER; /* VK_RETURN with extended flag */
        /* Modifier keys */
        case 0xA0: return KEY_LSHIFT;  /* VK_LSHIFT */
        case 0xA1: return KEY_RSHIFT;  /* VK_RSHIFT */
        case 0xA2: return KEY_LCTRL;   /* VK_LCONTROL */
        case 0xA3: return KEY_RCTRL;   /* VK_RCONTROL */
        case 0xA4: return KEY_LALT;    /* VK_LMENU */
        case 0xA5: return KEY_RALT;    /* VK_RMENU */
        case 0x14: return KEY_CAPS;    /* VK_CAPITAL */
        case 0x90: return KEY_NUM;     /* VK_NUMLOCK */
        case 0x91: return KEY_SCROLL;  /* VK_SCROLL */
        case 0x2C: return KEY_SYSREQ;  /* VK_SNAPSHOT */
        default: break;
    }

    if(k >= 'a' && k <= 'z') k = std::toupper((int)k);

    if(k >= 'A' && k <= 'Z') return KEY_A + (k - 'A');
    if(k >= '0' && k <= '9') return KEY_0 + (k - '0');

    switch(k) {
        case ' ':  return KEY_SPACE;
        case '-':  return KEY_MINUS;
        case '=':  return KEY_EQUAL;
        case '[':  return KEY_LBRACKET;
        case ']':  return KEY_RBRACKET;
        case ';':  return KEY_SEMICOLON;
        case '\'': return KEY_APOSTROPHE;
        case '`':  return KEY_TICK;
        case '\\': return KEY_BACKSLASH;
        case ',':  return KEY_COMMA;
        case '.':  return KEY_PERIOD;
        case '/':  return KEY_SLASH;
        default:   return KEY_NONE;
    }
}

void LvglKeyboard::init()
{
    LOG_INFO("LvglKeyboard::init");
    Keyboard::init();

    m_indev = lv_indev_get_next(nullptr);
    while(m_indev && lv_indev_get_type(m_indev) != LV_INDEV_TYPE_KEYPAD)
        m_indev = lv_indev_get_next(m_indev);
}

void LvglKeyboard::reset()
{
    Keyboard::reset();
}

void LvglKeyboard::update()
{
    Keyboard::update();
}

Bool LvglKeyboard::getCapsState()
{
    return FALSE;
}

void LvglKeyboard::getKey(KeyboardIO *key)
{
    key->key = KEY_NONE;
    key->status = KeyboardIO::STATUS_UNUSED;
    key->state = KEY_STATE_NONE;
    key->sequence = 0;

    if(!m_indev) {
        key->key = KEY_LOST;
        return;
    }

    lv_indev_read_cb_t cb = lv_indev_get_read_cb(m_indev);
    if(!cb) {
        key->key = KEY_LOST;
        return;
    }

    lv_indev_data_t data;
    std::memset(&data, 0, sizeof(data));
    cb(m_indev, &data);

    if(data.key == 0)
        return;

    key->key = convert_lv_key(data.key);
    key->state = (data.state == LV_INDEV_STATE_PRESSED) ? KEY_STATE_DOWN : KEY_STATE_UP;
    key->sequence = lv_tick_get();

    if(data.state == LV_INDEV_STATE_PRESSED) {
        switch(key->key) {
            case KEY_LSHIFT: key->state |= KEY_STATE_LSHIFT; break;
            case KEY_RSHIFT: key->state |= KEY_STATE_RSHIFT; break;
            case KEY_LCTRL:  key->state |= KEY_STATE_LCONTROL; break;
            case KEY_RCTRL:  key->state |= KEY_STATE_RCONTROL; break;
            case KEY_LALT:   key->state |= KEY_STATE_LALT; break;
            case KEY_RALT:   key->state |= KEY_STATE_RALT; break;
            default: break;
        }
    }
}

