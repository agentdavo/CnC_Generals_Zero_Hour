set(LVGL_PLATFORM_SRC
    lvgl_platform/lvgl_platform.cpp
)

add_library(logger STATIC common/logger.cpp)
target_include_directories(logger PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(lvgl_platform STATIC ${LVGL_PLATFORM_SRC})
target_include_directories(lvgl_platform PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/precompiled
)
target_link_libraries(lvgl_platform PUBLIC lvgl logger)

option(BUILD_ENGINE "Build Generals engine executable" ON)

# WPAudio headers are not yet migrated
#add_subdirectory(Libraries/WPAudio)
add_subdirectory(libraries/compression)
add_subdirectory(libraries/ww_vegas)

add_subdirectory(game_engine_device/lvgl_device)

if(BUILD_ENGINE)
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
endif()

add_subdirectory(examples)
