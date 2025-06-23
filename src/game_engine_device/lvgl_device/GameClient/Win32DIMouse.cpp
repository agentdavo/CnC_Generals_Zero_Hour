#include "lvglDevice/GameClient/Win32DIMouse.h"

Win32DIMouse::Win32DIMouse() {}
Win32DIMouse::~Win32DIMouse() {}

void Win32DIMouse::init() {}
void Win32DIMouse::reset() {}
void Win32DIMouse::update() {}
void Win32DIMouse::initCursorResources() {}
void Win32DIMouse::setCursor(MouseCursor) {}
void Win32DIMouse::capture() {}
void Win32DIMouse::releaseCapture() {}
void Win32DIMouse::setVisibility(Bool) {}
UnsignedByte Win32DIMouse::getMouseEvent(MouseIO *, Bool) { return MOUSE_NONE; }
