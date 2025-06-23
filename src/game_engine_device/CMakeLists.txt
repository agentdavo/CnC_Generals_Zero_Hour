add_library(gameenginedevice EXCLUDE_FROM_ALL)

# lvglDevice sources migrated from Generals/Code
file(GLOB_RECURSE LVGLDEVICE_SRCS CONFIGURE_DEPENDS
    lvgl_device/**/*.cpp
)
list(FILTER LVGLDEVICE_SRCS EXCLUDE REGEX "Win32.*\\.cpp$")
list(FILTER LVGLDEVICE_SRCS EXCLUDE REGEX "Win32BIGFile\\.cpp$")
list(FILTER LVGLDEVICE_SRCS EXCLUDE REGEX "Win32BIGFileSystem\\.cpp$")
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

# win32_device sources migrated from Generals/Code
file(GLOB_RECURSE WIN32DEVICE_COMMON_SRCS CONFIGURE_DEPENDS
    win32_device/common/*.cpp
)
file(GLOB_RECURSE WIN32DEVICE_GAMECLIENT_SRCS CONFIGURE_DEPENDS
    win32_device/game_client/*.cpp
)
target_sources(gameenginedevice PRIVATE
    ${WIN32DEVICE_COMMON_SRCS}
    ${WIN32DEVICE_GAMECLIENT_SRCS}
)

# miles_audio_device sources
file(GLOB MILESAUDIODEVICE_SRCS
    miles_audio_device/*.cpp
)
target_sources(gameenginedevice PRIVATE ${MILESAUDIODEVICE_SRCS})

# VideoDevice sources
file(GLOB_RECURSE VIDEODEVICE_SRCS CONFIGURE_DEPENDS
    video_device/*.cpp
)
target_sources(gameenginedevice PRIVATE ${VIDEODEVICE_SRCS})

target_include_directories(gameenginedevice PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/game_engine
    ${PROJECT_SOURCE_DIR}/include/game_engine_device
    ${PROJECT_SOURCE_DIR}/include/pre_compiled
)

# Link against the Miles Sound System stub which in turn
# bridges legacy audio calls to miniaudio. This removes the
# remaining DirectSound dependency from the GameEngineDevice
# module.
target_link_libraries(gameenginedevice PUBLIC milesstub miniaudio LvglPlatform)
# Partial migration of GameEngineDevice sources
