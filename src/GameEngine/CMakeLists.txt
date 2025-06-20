add_library(gameengine EXCLUDE_FROM_ALL
    Common/BitFlags.cpp
    Common/Audio/AudioEventRTS.cpp
    Common/Audio/AudioRequest.cpp
    Common/Audio/GameAudio.cpp
    Common/Audio/GameMusic.cpp
    Common/Audio/GameSounds.cpp
    Common/Audio/GameSpeech.cpp
    Common/Audio/simpleplayer.cpp
    Common/Audio/urllaunch.cpp
)

target_include_directories(gameengine PUBLIC ${PROJECT_SOURCE_DIR}/include)
# Partial migration of GameEngine sources
