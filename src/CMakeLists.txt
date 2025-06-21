set(LVGL_PLATFORM_SRC
    LvglPlatform/LvglPlatform.cpp
)

add_library(LvglPlatform STATIC ${LVGL_PLATFORM_SRC})
target_include_directories(LvglPlatform PUBLIC
    ${PROJECT_SOURCE_DIR}/src/LvglPlatform
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)
target_link_libraries(LvglPlatform PUBLIC lvgl)

add_executable(Generals main.cpp)
target_include_directories(Generals PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/Precompiled
    ${PROJECT_SOURCE_DIR}/src/LvglPlatform
)
target_link_libraries(Generals PRIVATE LvglPlatform)
