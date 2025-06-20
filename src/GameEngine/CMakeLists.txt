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
    Common/CRCDebug.cpp
    Common/Bezier/BezFwdIterator.cpp
    Common/Bezier/BezierSegment.cpp
    Common/System/GameCommon.cpp
    Common/System/QuickTrig.cpp
    Common/System/AsciiString.cpp
    Common/System/UnicodeString.cpp
    Common/System/String.cpp
    Common/System/CriticalSection.cpp
    Common/PerfTimer.cpp
    GameClient/Color.cpp
    GameClient/Display.cpp
    GameClient/DisplayString.cpp
    GameClient/DisplayStringManager.cpp
    GameClient/DrawGroupInfo.cpp
    Common/INI/INI.cpp
    Common/INI/INIAiData.cpp
    Common/INI/INIAnimation.cpp
    Common/INI/INIAudioEventInfo.cpp
    Common/INI/INICommandButton.cpp
    Common/INI/INICommandSet.cpp
    Common/INI/INIControlBarScheme.cpp
    Common/INI/INICrate.cpp
    Common/INI/INIDamageFX.cpp
    Common/INI/INIDrawGroupInfo.cpp
    Common/INI/INIGameData.cpp
    Common/INI/INIMapCache.cpp
    Common/INI/INIMapData.cpp
    Common/INI/INIMappedImage.cpp
    Common/INI/INIMiscAudio.cpp
    Common/INI/INIModel.cpp
    Common/INI/INIMultiplayer.cpp
    Common/INI/INIObject.cpp
    Common/INI/INIParticleSys.cpp
    Common/INI/INISpecialPower.cpp
    Common/INI/INITerrain.cpp
    Common/INI/INITerrainBridge.cpp
    Common/INI/INITerrainRoad.cpp
    Common/INI/INIUpgrade.cpp
    Common/INI/INIVideo.cpp
    Common/INI/INIWater.cpp
    Common/INI/INIWeapon.cpp
    Common/INI/INIWebpageURL.cpp
)

file(GLOB GAME_NETWORK_SRCS
    GameNetwork/*.cpp
    GameNetwork/GameSpy/*.cpp
    GameNetwork/GameSpy/Thread/*.cpp
    GameNetwork/WOLBrowser/*.cpp
)

target_sources(gameengine PRIVATE ${GAME_NETWORK_SRCS})

target_include_directories(gameengine PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/GameEngine
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)
# Partial migration of GameEngine sources
