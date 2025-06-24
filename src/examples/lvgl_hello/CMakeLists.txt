add_executable(lvgl_hello main.cpp)
target_include_directories(lvgl_hello PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(lvgl_hello PRIVATE lvgl_platform)
