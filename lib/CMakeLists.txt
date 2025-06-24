# Build third party libraries used by the port
# Currently LVGL is built as a static library so its sources can be
# selectively compiled with optional backends.

option(LVGL_USE_SDL "Enable LVGL SDL backend" OFF)
option(LVGL_USE_X11 "Enable LVGL X11 backend" ON)
option(LVGL_USE_WAYLAND "Enable LVGL Wayland backend" OFF)
option(LVGL_USE_LINUX_DRM "Enable LVGL DRM backend" OFF)
option(LVGL_USE_LINUX_FBDEV "Enable LVGL FBDev backend" OFF)
option(LVGL_USE_NUTTX "Enable LVGL NuttX backend" OFF)

file(GLOB_RECURSE LVGL_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/demos/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/demos/*.cpp)

if(LVGL_USE_SDL)
    list(APPEND LVGL_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/drivers/sdl/lv_sdl_keyboard.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/drivers/sdl/lv_sdl_mouse.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/drivers/sdl/lv_sdl_mousewheel.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/drivers/sdl/lv_sdl_window.c)
    find_package(SDL2 REQUIRED)
endif()

if(LVGL_USE_X11)
    list(APPEND LVGL_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/drivers/x11/lv_x11_display.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/drivers/x11/lv_x11_input.c)
    find_package(X11 REQUIRED)
endif()

if(LVGL_USE_WAYLAND)
    file(GLOB WAYLAND_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/drivers/wayland/*.c)
    list(APPEND LVGL_SOURCES ${WAYLAND_SOURCES})
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WAYLAND REQUIRED wayland-client wayland-cursor xkbcommon)
endif()

if(LVGL_USE_LINUX_DRM)
    list(APPEND LVGL_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/drivers/display/drm/lv_linux_drm.c)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(DRM REQUIRED gbm)
endif()

if(LVGL_USE_LINUX_FBDEV)
    list(APPEND LVGL_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/drivers/display/fb/lv_linux_fbdev.c)
endif()

if(LVGL_USE_NUTTX)
    file(GLOB NUTTX_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/drivers/nuttx/*.c)
    list(APPEND LVGL_SOURCES ${NUTTX_SOURCES})
endif()

add_library(lvgl STATIC ${LVGL_SOURCES})
target_include_directories(lvgl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/lvgl
)

if(LVGL_USE_SDL)
    target_link_libraries(lvgl PUBLIC SDL2::SDL2)
    target_compile_definitions(lvgl PUBLIC LV_USE_SDL=1)
endif()

if(LVGL_USE_X11)
    target_include_directories(lvgl PUBLIC ${X11_INCLUDE_DIR})
    target_link_libraries(lvgl PUBLIC ${X11_LIBRARIES})
    target_compile_definitions(lvgl PUBLIC LV_USE_X11=1)
endif()

if(LVGL_USE_WAYLAND)
    target_include_directories(lvgl PUBLIC ${WAYLAND_INCLUDE_DIRS})
    target_link_libraries(lvgl PUBLIC ${WAYLAND_LIBRARIES})
    target_compile_definitions(lvgl PUBLIC LV_USE_WAYLAND=1)
endif()

if(LVGL_USE_LINUX_DRM)
    target_link_libraries(lvgl PUBLIC ${DRM_LIBRARIES})
    target_include_directories(lvgl PUBLIC ${DRM_INCLUDE_DIRS})
    target_compile_definitions(lvgl PUBLIC LV_USE_LINUX_DRM=1)
endif()

if(LVGL_USE_LINUX_FBDEV)
    target_compile_definitions(lvgl PUBLIC LV_USE_LINUX_FBDEV=1)
endif()

if(LVGL_USE_NUTTX)
    target_compile_definitions(lvgl PUBLIC LV_USE_NUTTX=1)
endif()

set(MINIAUDIO_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/miniaudio/miniaudio.c)
add_library(miniaudio STATIC ${MINIAUDIO_SOURCE})
target_include_directories(miniaudio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/miniaudio)
if(APPLE)
    # CoreAudio headers rely on the Clang Blocks extension.  When building on
    # macOS ensure the flag is enabled only when Clang is actually used,
    # otherwise warn that audio may fail to compile.
    target_compile_options(miniaudio PRIVATE
        $<$<C_COMPILER_ID:Clang>:-fblocks>
    )
    if(NOT CMAKE_C_COMPILER_ID MATCHES "Clang")
        message(WARNING "The miniaudio target expects Clang on macOS. Blocks support is disabled")
    endif()
endif()

add_library(uGLES INTERFACE)
target_include_directories(uGLES INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/uGLES)

# The UniSpySDK sources use a lowercase directory name
add_subdirectory(uni_spy_sdk)
target_include_directories(UniSpySDK PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/uni_spy_sdk)

add_library(STLport INTERFACE)
target_include_directories(STLport INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/STLport)

add_subdirectory(miles-sdk-stub)
add_subdirectory(zlib)
add_subdirectory(liblzhl)

# Provide a simple alias matching the previous interface target name
add_library(liblzhl ALIAS lzhl)

