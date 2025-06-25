#include "lvgl_platform/lvgl_platform.h"
#include <GLES/gl.h>
#include <cstdint>
#include <lvgl.h>

extern "C" {
struct Framebuffer;

Framebuffer *GL_init_with_framebuffer(uint32_t width, uint32_t height);
void GL_cleanup_with_framebuffer(Framebuffer *fb);
void thread_pool_init_from_env(void);
void thread_pool_wait(void);
void thread_pool_shutdown(void);
int logger_init(const char *path, int level);
void logger_shutdown(void);
int memory_tracker_init(void);
void memory_tracker_report(void);
uint32_t framebuffer_get_pixel(const Framebuffer *fb, uint32_t x, uint32_t y);
}

int main() {
  const uint32_t width = 256;
  const uint32_t height = 256;

  lv_display_t *disp = LvglPlatform::create_window(width, height, "sdl");
  if (!disp)
    return 1;

  LV_DRAW_BUF_DEFINE_STATIC(draw_buf, width, height, LV_COLOR_FORMAT_ARGB8888);
  LV_DRAW_BUF_INIT_STATIC(draw_buf);
  lv_obj_t *canvas = lv_canvas_create(lv_screen_active());
  lv_canvas_set_draw_buf(canvas, &draw_buf);
  lv_obj_center(canvas);

  logger_init(nullptr, 1); // LOG_LEVEL_INFO
  memory_tracker_init();
  thread_pool_init_from_env();

  Framebuffer *fb = GL_init_with_framebuffer(width, height);
  if (!fb)
    return 1;

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  GLfloat verts[] = {-0.5f, -0.5f, 0.f, 0.5f, -0.5f, 0.f, 0.0f, 0.5f, 0.f};
  GLfloat colors[] = {1.f, 0.f, 0.f, 1.f, 0.f, 1.f,
                      0.f, 1.f, 0.f, 0.f, 1.f, 1.f};

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, verts);
  glColorPointer(4, GL_FLOAT, 0, colors);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  thread_pool_wait();

  lv_color32_t *out = (lv_color32_t *)draw_buf.data;
  for (uint32_t y = 0; y < height; ++y) {
    for (uint32_t x = 0; x < width; ++x) {
      uint32_t p = framebuffer_get_pixel(fb, x, y);
      lv_color32_t c;
      c.red = (p >> 16) & 0xFF;
      c.green = (p >> 8) & 0xFF;
      c.blue = p & 0xFF;
      c.alpha = (p >> 24) & 0xFF;
      out[y * width + x] = c;
    }
  }
  lv_obj_invalidate(canvas);

  while (true)
    LvglPlatform::poll_events();

  GL_cleanup_with_framebuffer(fb);
  thread_pool_shutdown();
  memory_tracker_report();
  logger_shutdown();

  return 0;
}
