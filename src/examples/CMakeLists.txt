add_subdirectory(lvgl_hello)
add_subdirectory(d3d8_triangle)
if(NOT BUILD_ENGINE)
    add_subdirectory(lvgl_ugles_demo)
endif()
