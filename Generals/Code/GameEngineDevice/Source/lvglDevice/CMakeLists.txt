# Placeholder lvglDevice module
add_library(lvglDevice STATIC EXCLUDE_FROM_ALL
    GameClient/LvglMouse.cpp
    Common/LvglOSDisplay.cpp
)

target_include_directories(lvglDevice PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/GameEngineDevice
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)
