file(GLOB LVGLDEVICE_SRC CONFIGURE_DEPENDS
    common/*.cpp
    game_client/*.cpp
)

add_library(lvgl_device STATIC ${LVGLDEVICE_SRC})

target_include_directories(lvgl_device PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/game_engine
    ${PROJECT_SOURCE_DIR}/include/game_engine_device
    ${PROJECT_SOURCE_DIR}/include/pre_compiled
)

target_link_libraries(lvgl_device PUBLIC lvgl_platform)
