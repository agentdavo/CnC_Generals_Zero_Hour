# lvgl_ugles_demo

This example uses the microGLES software renderer to draw a simple triangle and display it on an LVGL canvas.

## Building and running

```bash
cmake -S . -B build -DBUILD_ENGINE=OFF
cmake --build build -j1
./build/src/examples/lvgl_ugles_demo/lvgl_ugles_demo
```
