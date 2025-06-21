set(_default_lvgl_device ON)
if(WIN32)
    set(_default_lvgl_device OFF)
endif()

option(USE_LVGL_DEVICE "Use LVGL based device implementation" ${_default_lvgl_device})

set(LVGL_PLATFORM_SRC
    LvglPlatform/LvglPlatform.cpp
)

add_library(LvglPlatform STATIC ${LVGL_PLATFORM_SRC})
target_include_directories(LvglPlatform PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)
target_link_libraries(LvglPlatform PUBLIC lvgl)

file(GLOB_RECURSE LVGL_DEVICE_SRC CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/Generals/Code/GameEngineDevice/Source/lvglDevice/*.cpp)

add_library(lvglDevice STATIC ${LVGL_DEVICE_SRC})
target_include_directories(lvglDevice PUBLIC
    ${PROJECT_SOURCE_DIR}/Generals/Code/GameEngineDevice/Include
    ${PROJECT_SOURCE_DIR}/include/GameEngine
    ${PROJECT_SOURCE_DIR}/Generals/Code/GameEngine/Include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/Precompiled
    ${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(lvglDevice PUBLIC LvglPlatform)

add_executable(Generals main.cpp)
target_include_directories(Generals PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/Precompiled
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(Generals PRIVATE LvglPlatform)

if(USE_LVGL_DEVICE OR NOT WIN32)
    target_link_libraries(Generals PRIVATE lvglDevice)
else()
    # TODO: add Win32Device library when available
endif()
