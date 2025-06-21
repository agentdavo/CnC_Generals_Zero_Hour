file(GLOB LVGLDEVICE_SRC CONFIGURE_DEPENDS
    Common/*.cpp
    GameClient/*.cpp
)

add_library(lvglDevice STATIC ${LVGLDEVICE_SRC})

target_include_directories(lvglDevice PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/GameEngine
    ${PROJECT_SOURCE_DIR}/include/GameEngineDevice
    ${PROJECT_SOURCE_DIR}/Generals/Code/GameEngine/Include
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)

target_link_libraries(lvglDevice PUBLIC LvglPlatform)
