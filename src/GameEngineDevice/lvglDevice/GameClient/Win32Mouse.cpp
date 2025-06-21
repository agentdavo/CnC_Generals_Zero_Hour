#include "lvglDevice/GameClient/Win32Mouse.h"

Win32Mouse::Win32Mouse() {}
Win32Mouse::~Win32Mouse() {}

void Win32Mouse::init() {}
void Win32Mouse::reset() {}
void Win32Mouse::update() {}
void Win32Mouse::initCursorResources() {}
void Win32Mouse::setCursor(MouseCursor) {}
void Win32Mouse::capture() {}
void Win32Mouse::releaseCapture() {}
void Win32Mouse::setVisibility(Bool) {}
UnsignedByte Win32Mouse::getMouseEvent(MouseIO *, Bool) { return MOUSE_NONE; }
