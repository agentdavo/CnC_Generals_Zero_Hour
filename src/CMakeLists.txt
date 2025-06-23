set(LVGL_PLATFORM_SRC
    lvgl_platform/lvgl_platform.cpp
)

add_library(lvgl_platform STATIC ${LVGL_PLATFORM_SRC})
target_include_directories(lvgl_platform PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/precompiled
)
target_link_libraries(lvgl_platform PUBLIC lvgl)

# WPAudio headers are not yet migrated
#add_subdirectory(Libraries/WPAudio)
add_subdirectory(libraries/compression)
add_subdirectory(libraries/ww_vegas)

add_subdirectory(game_engine_device/lvgl_device)

set(GENERALS_SRC
    main/main.cpp
    lvgl_game_engine/lvgl_game_engine.cpp
)

add_executable(Generals ${GENERALS_SRC})
target_include_directories(Generals PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/precompiled
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/lvgl_game_engine
)
target_link_libraries(Generals PRIVATE lvgl_platform lvgl_device)

target_link_libraries(Generals PRIVATE gameenginedevice)
target_link_libraries(Generals PRIVATE gameengine)
target_link_libraries(Generals PRIVATE milesstub)
