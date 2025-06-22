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

add_subdirectory(GameEngineDevice/lvglDevice)

set(GENERALS_SRC
    main.cpp
    LvglGameEngine/LvglGameEngine.cpp
)

add_executable(Generals ${GENERALS_SRC})
target_include_directories(Generals PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/Precompiled
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/LvglGameEngine
)
target_link_libraries(Generals PRIVATE LvglPlatform lvglDevice)

target_link_libraries(Generals PRIVATE gameenginedevice)
target_link_libraries(Generals PRIVATE gameengine)
target_link_libraries(Generals PRIVATE milesstub)
