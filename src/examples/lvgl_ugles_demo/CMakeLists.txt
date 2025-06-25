add_executable(lvgl_ugles_demo main.cpp)

# include project headers
 target_include_directories(lvgl_ugles_demo PRIVATE
     ${PROJECT_SOURCE_DIR}/src
     ${PROJECT_SOURCE_DIR}/include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
find_package(Threads REQUIRED)
target_include_directories(lvgl_ugles_demo PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(lvgl_ugles_demo PRIVATE lvgl_platform uGLES Threads::Threads)
target_link_libraries(lvgl_ugles_demo PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(lvgl_ugles_demo PRIVATE ${SDL2_LIBRARIES})
