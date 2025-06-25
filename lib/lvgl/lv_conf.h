#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_CONF_INCLUDE_SIMPLE

/* Use SDL by default for development */
#ifndef LV_USE_SDL
#define LV_USE_SDL 1
#endif
#if LV_USE_SDL
#ifndef LV_SDL_INCLUDE_PATH
#define LV_SDL_INCLUDE_PATH <SDL2/SDL.h>
#endif
#endif
#ifndef LV_USE_X11
#define LV_USE_X11 0
#endif
#ifndef LV_USE_WAYLAND
#define LV_USE_WAYLAND 0
#endif
#ifndef LV_USE_LINUX_FBDEV
#define LV_USE_LINUX_FBDEV 0
#endif
#ifndef LV_USE_NUTTX
#define LV_USE_NUTTX 0
#endif
#ifndef LV_USE_LINUX_DRM
#define LV_USE_LINUX_DRM 0
#endif
#define LV_LINUX_DRM_GBM_BUFFERS 1

#include "src/lv_conf_internal.h"

#endif /* LV_CONF_H */
