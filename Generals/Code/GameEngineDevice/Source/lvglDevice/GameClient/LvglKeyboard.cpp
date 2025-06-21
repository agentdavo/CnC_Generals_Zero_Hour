#include "lvglDevice/GameClient/LvglKeyboard.h"
#include "Common/Debug.h"
#include "GameClient/KeyDefs.h"
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

    if(k >= 'a' && k <= 'z') k = std::toupper((int)k);

    if(k >= 'A' && k <= 'Z') return KEY_A + (k - 'A');
    if(k >= '0' && k <= '9') return KEY_0 + (k - '0');

    switch(k) {
        case ' ': return KEY_SPACE;
        case '-': return KEY_MINUS;
        case '=': return KEY_EQUAL;
        case '[': return KEY_LBRACKET;
        case ']': return KEY_RBRACKET;
        case ';': return KEY_SEMICOLON;
        case '\'': return KEY_APOSTROPHE;
        case '`': return KEY_TICK;
        case '\\': return KEY_BACKSLASH;
        case ',': return KEY_COMMA;
        case '.': return KEY_PERIOD;
        case '/': return KEY_SLASH;
        default: return KEY_NONE;
    }
}

void LvglKeyboard::init()
{
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

    if(!m_indev)
        return;

    lv_indev_read_cb_t cb = lv_indev_get_read_cb(m_indev);
    if(!cb)
        return;

    lv_indev_data_t data;
    std::memset(&data, 0, sizeof(data));
    cb(m_indev, &data);

    if(data.key == 0)
        return;

    key->key = convert_lv_key(data.key);
    key->state = (data.state == LV_INDEV_STATE_PRESSED) ? KEY_STATE_DOWN : KEY_STATE_UP;
}

