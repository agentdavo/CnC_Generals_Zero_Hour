add_library(gameenginedevice EXCLUDE_FROM_ALL)

# lvglDevice sources migrated from Generals/Code
file(GLOB_RECURSE LVGLDEVICE_SRCS CONFIGURE_DEPENDS
    lvgl_device/**/*.cpp
)
target_sources(gameenginedevice PRIVATE ${LVGLDEVICE_SRCS})

# Common w3d_device sources migrated from Generals/Code
file(GLOB_RECURSE W3DDEVICE_COMMON_SRCS CONFIGURE_DEPENDS
    w3d_device/common/*.cpp
)
target_sources(gameenginedevice PRIVATE ${W3DDEVICE_COMMON_SRCS})

# GameClient w3d_device sources migrated from Generals/Code
file(GLOB_RECURSE W3DDEVICE_GAMECLIENT_SRCS CONFIGURE_DEPENDS
    w3d_device/game_client/*.cpp
)
target_sources(gameenginedevice PRIVATE ${W3DDEVICE_GAMECLIENT_SRCS})

# game_logic sources migrated from Generals/Code
file(GLOB W3DDEVICE_GAMELOGIC_SRCS
    w3d_device/game_logic/*.cpp
)
target_sources(gameenginedevice PRIVATE ${W3DDEVICE_GAMELOGIC_SRCS})

# Win32Device sources migrated from Generals/Code
file(GLOB_RECURSE WIN32DEVICE_COMMON_SRCS CONFIGURE_DEPENDS
    Win32Device/common/*.cpp
)
file(GLOB_RECURSE WIN32DEVICE_GAMECLIENT_SRCS CONFIGURE_DEPENDS
    Win32Device/GameClient/*.cpp
)
target_sources(gameenginedevice PRIVATE
    ${WIN32DEVICE_COMMON_SRCS}
    ${WIN32DEVICE_GAMECLIENT_SRCS}
)

# MilesAudioDevice sources
file(GLOB MILESAUDIODEVICE_SRCS
    MilesAudioDevice/*.cpp
)
target_sources(gameenginedevice PRIVATE ${MILESAUDIODEVICE_SRCS})

# VideoDevice sources
file(GLOB_RECURSE VIDEODEVICE_SRCS CONFIGURE_DEPENDS
    VideoDevice/*.cpp
)
target_sources(gameenginedevice PRIVATE ${VIDEODEVICE_SRCS})

target_include_directories(gameenginedevice PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/GameEngine
    ${PROJECT_SOURCE_DIR}/include/GameEngineDevice
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)

# Link against the Miles Sound System stub which in turn
# bridges legacy audio calls to miniaudio. This removes the
# remaining DirectSound dependency from the GameEngineDevice
# module.
target_link_libraries(gameenginedevice PUBLIC milesstub miniaudio LvglPlatform)
# Partial migration of GameEngineDevice sources
