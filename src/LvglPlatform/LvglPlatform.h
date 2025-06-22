#pragma once

#include <lvgl.h>
#include <cstdint>

namespace LvglPlatform {

/**
 * Create an LVGL window using the requested backend. The backend string
 * matches the LVGL driver name ("sdl", "x11", "wayland", "drm", "fbdev",
 * "nuttx"). If nullptr, platform defaults are used.
 *
 * This call performs lv_init() if it hasn't been called yet.
 */
lv_display_t *create_window(uint32_t width, uint32_t height, const char *backend = nullptr);

/**
 * Process LVGL timers and events. Call periodically from the main loop.
 */
void poll_events();

} // namespace LvglPlatform

