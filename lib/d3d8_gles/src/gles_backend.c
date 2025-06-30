// gles_backend.c - OpenGL ES implementation
#include "d3d8_to_gles.h"
#include "d3d8_internal.h"

// Thread-local matrix storage
_Thread_local alignas(64) D3DXMATRIX temp_matrix;

#ifdef D3D8_GLES_LOGGING
void d3d8_gles_log(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
}
#endif

// Last created device's display mode for adapter queries. Initialise with a
// sane default so adapter queries succeed before any device is created.
D3DDISPLAYMODE g_current_display_mode = {
    800,            /* Width */
    600,            /* Height */
    60,             /* RefreshRate */
    D3DFMT_X8R8G8B8 /* Format */
};

// TODO: Add OpenGL ES backend implementation here
// This file will contain:
// - EGL context management
// - OpenGL ES state management
// - GL resource creation and management
// - D3D to GL state mapping