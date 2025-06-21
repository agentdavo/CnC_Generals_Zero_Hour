# Placeholder lvglDevice module
add_library(lvglDevice STATIC EXCLUDE_FROM_ALL)

target_include_directories(lvglDevice PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/GameEngineDevice
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)

target_sources(lvglDevice PRIVATE
    Common/LvglOSDisplay.cpp
    Common/LvglBIGFile.cpp
    Common/LvglBIGFileSystem.cpp
)
