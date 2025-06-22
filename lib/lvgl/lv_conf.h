#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_CONF_INCLUDE_SIMPLE

/* Use X11 by default for development */
#define LV_USE_SDL 0
#define LV_USE_X11 1
#define LV_USE_WAYLAND 0
#define LV_USE_LINUX_FBDEV 0
#define LV_USE_NUTTX 0
#define LV_USE_LINUX_DRM 0
#define LV_LINUX_DRM_GBM_BUFFERS 1

#include "src/lv_conf_internal.h"

#endif /* LV_CONF_H */
