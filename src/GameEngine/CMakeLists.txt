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
    Common/System/LocalFile.cpp
    Common/PerfTimer.cpp
    Common/StatsCollector.cpp
    ../Common/Logger.cpp
    ../Common/File.cpp
    ../Common/registry.cpp
    Precompiled/PreRTS.cpp
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
Common/CommandLine.cpp
Common/DamageFX.cpp
Common/Dict.cpp
Common/DiscreteCircle.cpp
Common/GameEngine.cpp
Common/GameLOD.cpp
Common/GameMain.cpp
Common/GlobalData.cpp
Common/Language.cpp
Common/MessageStream.cpp
Common/MiniLog.cpp
Common/MultiplayerSettings.cpp
Common/NameKeyGenerator.cpp
Common/PartitionSolver.cpp
Common/RTS/ActionManager.cpp
Common/RTS/Energy.cpp
Common/RTS/Handicap.cpp
Common/RTS/MissionStats.cpp
Common/RTS/Money.cpp
Common/RTS/ProductionPrerequisite.cpp
Common/RTS/ResourceGatheringManager.cpp
Common/RTS/Science.cpp
Common/RTS/ScoreKeeper.cpp
Common/RTS/SpecialPower.cpp
Common/RTS/Team.cpp
Common/RTS/TunnelTracker.cpp
Common/RandomValue.cpp
Common/Recorder.cpp
Common/SkirmishBattleHonors.cpp
Common/StateMachine.cpp
Common/System/ArchiveFile.cpp
Common/System/ArchiveFileSystem.cpp
Common/System/BuildAssistant.cpp
Common/System/CDManager.cpp
Common/System/Compression.cpp
Common/System/CopyProtection.cpp
Common/System/DataChunk.cpp
Common/System/Debug.cpp
Common/System/Directory.cpp
Common/System/DisabledTypes.cpp
Common/System/File.cpp
Common/System/FileSystem.cpp
Common/System/FunctionLexicon.cpp
Common/System/GameMemory.cpp
Common/System/GameType.cpp
Common/System/Geometry.cpp
Common/System/KindOf.cpp
Common/System/List.cpp
Common/System/LocalFileSystem.cpp
Common/System/MemoryInit.cpp
Common/System/QuotedPrintable.cpp
Common/System/RAMFile.cpp
Common/System/Radar.cpp
Common/System/SaveGame/GameState.cpp
Common/System/SaveGame/GameStateMap.cpp
Common/System/Snapshot.cpp
Common/System/StackDump.cpp
Common/System/StreamingArchiveFile.cpp
Common/System/Trig.cpp
Common/System/Upgrade.cpp
Common/System/Xfer.cpp
Common/System/XferCRC.cpp
Common/System/XferLoad.cpp
Common/System/XferSave.cpp
Common/System/encrypt.cpp
Common/System/registry.cpp
Common/TerrainTypes.cpp
Common/Thing/DrawModule.cpp
Common/Thing/Module.cpp
Common/Thing/ModuleFactory.cpp
Common/Thing/Thing.cpp
Common/Thing/ThingFactory.cpp
Common/Thing/ThingTemplate.cpp
Common/UserPreferences.cpp
Common/crc.cpp
Common/version.cpp
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

# Link against UniSpy networking library and WPAudio
target_link_libraries(gameengine PUBLIC UniSpySDK wp_audio compression)
# Partial migration of GameEngine sources
