add_library(gameengine EXCLUDE_FROM_ALL
    common/bit_flags.cpp
    common/audio/audio_event_rts.cpp
    common/audio/audio_request.cpp
    common/audio/game_audio.cpp
    common/audio/game_music.cpp
    common/audio/game_sounds.cpp
    common/audio/game_speech.cpp
    common/audio/simpleplayer.cpp
    common/audio/urllaunch.cpp
    common/crc_debug.cpp
    common/bezier/bez_fwd_iterator.cpp
    common/bezier/bezier_segment.cpp
    common/system/game_common.cpp
    common/system/quick_trig.cpp
    common/system/ascii_string.cpp
    common/system/unicode_string.cpp
    common/system/string.cpp
    common/system/critical_section.cpp
    common/system/local_file.cpp
    common/perf_timer.cpp
    common/stats_collector.cpp
    ../common/logger.cpp
    ../common/file.cpp
    ../common/registry.cpp
    Precompiled/PreRTS.cpp
    common/ini/ini.cpp
    common/ini/ini_ai_data.cpp
    common/ini/ini_animation.cpp
    common/ini/ini_audio_event_info.cpp
    common/ini/ini_command_button.cpp
    common/ini/ini_command_set.cpp
    common/ini/ini_control_bar_scheme.cpp
    common/ini/ini_crate.cpp
    common/ini/ini_damage_fx.cpp
    common/ini/ini_draw_group_info.cpp
    common/ini/ini_game_data.cpp
    common/ini/ini_map_cache.cpp
    common/ini/ini_map_data.cpp
    common/ini/ini_mapped_image.cpp
    common/ini/ini_misc_audio.cpp
    common/ini/ini_model.cpp
    common/ini/ini_multiplayer.cpp
    common/ini/ini_object.cpp
    common/ini/ini_particle_sys.cpp
    common/ini/ini_special_power.cpp
    common/ini/ini_terrain.cpp
    common/ini/ini_terrain_bridge.cpp
    common/ini/ini_terrain_road.cpp
    common/ini/ini_upgrade.cpp
    common/ini/ini_video.cpp
    common/ini/ini_water.cpp
    common/ini/ini_weapon.cpp
    common/ini/ini_webpage_url.cpp
common/command_line.cpp
common/damage_fx.cpp
common/dict.cpp
common/discrete_circle.cpp
common/game_engine.cpp
common/game_lod.cpp
common/game_main.cpp
common/global_data.cpp
common/language.cpp
common/message_stream.cpp
common/mini_log.cpp
common/multiplayer_settings.cpp
common/name_key_generator.cpp
common/partition_solver.cpp
common/rts/action_manager.cpp
common/rts/energy.cpp
common/rts/handicap.cpp
common/rts/mission_stats.cpp
common/rts/money.cpp
common/rts/production_prerequisite.cpp
common/rts/resource_gathering_manager.cpp
common/rts/science.cpp
common/rts/score_keeper.cpp
common/rts/special_power.cpp
common/rts/team.cpp
common/rts/tunnel_tracker.cpp
common/random_value.cpp
common/recorder.cpp
common/skirmish_battle_honors.cpp
common/state_machine.cpp
common/system/archive_file.cpp
common/system/archive_file_system.cpp
common/system/build_assistant.cpp
common/system/cd_manager.cpp
common/system/compression.cpp
common/system/copy_protection.cpp
common/system/data_chunk.cpp
common/system/debug.cpp
common/system/directory.cpp
common/system/disabled_types.cpp
common/system/file.cpp
common/system/file_system.cpp
common/system/function_lexicon.cpp
common/system/game_memory.cpp
common/system/game_type.cpp
common/system/geometry.cpp
common/system/kind_of.cpp
common/system/list.cpp
common/system/local_file_system.cpp
common/system/memory_init.cpp
common/system/quoted_printable.cpp
common/system/ram_file.cpp
common/system/radar.cpp
common/system/SaveGame/game_state.cpp
common/system/SaveGame/game_state_map.cpp
common/system/snapshot.cpp
common/system/stack_dump.cpp
common/system/streaming_archive_file.cpp
common/system/trig.cpp
common/system/upgrade.cpp
common/system/xfer.cpp
common/system/xfer_crc.cpp
common/system/xfer_load.cpp
common/system/xfer_save.cpp
common/system/encrypt.cpp
common/system/registry.cpp
common/terrain_types.cpp
common/thing/draw_module.cpp
common/thing/module.cpp
common/thing/module_factory.cpp
common/thing/thing.cpp
common/thing/thing_factory.cpp
common/thing/thing_template.cpp
common/user_preferences.cpp
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
