add_executable(d3d8_triangle main.cpp)
target_include_directories(d3d8_triangle PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(d3d8_triangle PRIVATE lvgl_platform d3d8_to_gles)
