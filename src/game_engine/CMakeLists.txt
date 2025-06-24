add_library(gameengine EXCLUDE_FROM_ALL
    common/BitFlags.cpp
    common/Audio/AudioEventRTS.cpp
    common/Audio/AudioRequest.cpp
    common/Audio/GameAudio.cpp
    common/Audio/GameMusic.cpp
    common/Audio/GameSounds.cpp
    common/Audio/GameSpeech.cpp
    common/Audio/simpleplayer.cpp
    common/Audio/urllaunch.cpp
    common/CRCDebug.cpp
    common/Bezier/BezFwdIterator.cpp
    common/Bezier/BezierSegment.cpp
    common/System/GameCommon.cpp
    common/System/QuickTrig.cpp
    common/System/AsciiString.cpp
    common/System/UnicodeString.cpp
    common/System/String.cpp
    common/System/CriticalSection.cpp
    common/System/LocalFile.cpp
    common/PerfTimer.cpp
    common/StatsCollector.cpp
    ../common/Logger.cpp
    ../common/File.cpp
    ../common/registry.cpp
    Precompiled/PreRTS.cpp
    common/INI/INI.cpp
    common/INI/INIAiData.cpp
    common/INI/INIAnimation.cpp
    common/INI/INIAudioEventInfo.cpp
    common/INI/INICommandButton.cpp
    common/INI/INICommandSet.cpp
    common/INI/INIControlBarScheme.cpp
    common/INI/INICrate.cpp
    common/INI/INIDamageFX.cpp
    common/INI/INIDrawGroupInfo.cpp
    common/INI/INIGameData.cpp
    common/INI/INIMapCache.cpp
    common/INI/INIMapData.cpp
    common/INI/INIMappedImage.cpp
    common/INI/INIMiscAudio.cpp
    common/INI/INIModel.cpp
    common/INI/INIMultiplayer.cpp
    common/INI/INIObject.cpp
    common/INI/INIParticleSys.cpp
    common/INI/INISpecialPower.cpp
    common/INI/INITerrain.cpp
    common/INI/INITerrainBridge.cpp
    common/INI/INITerrainRoad.cpp
    common/INI/INIUpgrade.cpp
    common/INI/INIVideo.cpp
    common/INI/INIWater.cpp
    common/INI/INIWeapon.cpp
    common/INI/INIWebpageURL.cpp
common/CommandLine.cpp
common/DamageFX.cpp
common/Dict.cpp
common/DiscreteCircle.cpp
common/GameEngine.cpp
common/GameLOD.cpp
common/GameMain.cpp
common/GlobalData.cpp
common/Language.cpp
common/MessageStream.cpp
common/MiniLog.cpp
common/MultiplayerSettings.cpp
common/NameKeyGenerator.cpp
common/PartitionSolver.cpp
common/RTS/ActionManager.cpp
common/RTS/Energy.cpp
common/RTS/Handicap.cpp
common/RTS/MissionStats.cpp
common/RTS/Money.cpp
common/RTS/ProductionPrerequisite.cpp
common/RTS/ResourceGatheringManager.cpp
common/RTS/Science.cpp
common/RTS/ScoreKeeper.cpp
common/RTS/SpecialPower.cpp
common/RTS/Team.cpp
common/RTS/TunnelTracker.cpp
common/RandomValue.cpp
common/Recorder.cpp
common/SkirmishBattleHonors.cpp
common/StateMachine.cpp
common/System/ArchiveFile.cpp
common/System/ArchiveFileSystem.cpp
common/System/BuildAssistant.cpp
common/System/CDManager.cpp
common/System/Compression.cpp
common/System/CopyProtection.cpp
common/System/DataChunk.cpp
common/System/Debug.cpp
common/System/Directory.cpp
common/System/DisabledTypes.cpp
common/System/File.cpp
common/System/FileSystem.cpp
common/system/function_lexicon.cpp
common/System/GameMemory.cpp
common/System/GameType.cpp
common/System/Geometry.cpp
common/System/KindOf.cpp
common/System/List.cpp
common/System/LocalFileSystem.cpp
common/System/MemoryInit.cpp
common/System/QuotedPrintable.cpp
common/System/RAMFile.cpp
common/System/Radar.cpp
common/System/SaveGame/GameState.cpp
common/System/SaveGame/GameStateMap.cpp
common/System/Snapshot.cpp
common/System/StackDump.cpp
common/System/StreamingArchiveFile.cpp
common/System/Trig.cpp
common/System/Upgrade.cpp
common/System/Xfer.cpp
common/System/XferCRC.cpp
common/System/XferLoad.cpp
common/System/XferSave.cpp
common/System/encrypt.cpp
common/System/registry.cpp
common/TerrainTypes.cpp
common/Thing/DrawModule.cpp
common/Thing/Module.cpp
common/Thing/ModuleFactory.cpp
common/Thing/Thing.cpp
common/Thing/ThingFactory.cpp
common/Thing/ThingTemplate.cpp
common/UserPreferences.cpp
common/crc.cpp
common/version.cpp
)

file(GLOB_RECURSE GAME_CLIENT_SRCS CONFIGURE_DEPENDS
    GameClient/*.cpp
)

file(GLOB GAME_NETWORK_SRCS
    GameNetwork/*.cpp
    GameNetwork/GameSpy/*.cpp
    GameNetwork/GameSpy/Thread/*.cpp
    GameNetwork/WOLBrowser/*.cpp
)

file(GLOB_RECURSE GAME_LOGIC_SRCS CONFIGURE_DEPENDS
    GameLogic/*.cpp
)

target_sources(gameengine PRIVATE ${GAME_NETWORK_SRCS})
target_sources(gameengine PRIVATE ${GAME_CLIENT_SRCS})
target_sources(gameengine PRIVATE ${GAME_LOGIC_SRCS})

target_include_directories(gameengine PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/GameEngine
    ${PROJECT_SOURCE_DIR}/include/GameEngine/GameLogic
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)

if(COMMAND target_precompile_headers)
    target_precompile_headers(gameengine PRIVATE
        ${PROJECT_SOURCE_DIR}/include/Precompiled/PreRTS.h)
endif()

# Link against core libraries. WPAudio is disabled pending porting
target_link_libraries(gameengine PUBLIC UniSpySDK compression wwutil wwsaveload wwaudio wwdebug wwdownload wwmath wwlib ww3d2)
if(TARGET wwshade)
    target_link_libraries(gameengine PUBLIC wwshade)
endif()
# Partial migration of GameEngine sources
