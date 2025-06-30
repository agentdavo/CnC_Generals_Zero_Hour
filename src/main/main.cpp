/*
**    Command & Conquer Generals(tm)
**    Copyright 2025 Electronic Arts Inc.
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Cross-platform entry point for Command & Conquer Generals                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// FILE: main.cpp /////////////////////////////////////////////////////////////
// 
// Entry point for cross-platform game application using LVGL
//
// Based on WinMain.cpp but adapted for LVGL/SDL2
//
///////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>

#include "common/logger.h"
#include "lvgl_platform/lvgl_platform.h"
#include "game_engine/common/gameengine.h"

// GLOBALS ////////////////////////////////////////////////////////////////////
static uint32_t window_width = 800;
static uint32_t window_height = 600;
static const char *backend = nullptr;
static lv_display_t *main_display = nullptr;
static bool application_is_windowed = true;  // always windowed for cross-platform version
static bool application_quitting = false;

// Command line parsing ///////////////////////////////////////////////////////
static bool parse_command_line(int argc, char **argv, int *game_argc, char ***game_argv)
{
    LOG_INFO("parse_command_line: Processing %d arguments", argc);
    
    // Parse LVGL-specific arguments and pass the rest to the game
    static char **remaining_args = nullptr;
    remaining_args = new char*[argc + 1];
    int remaining_count = 0;
    
    // Always pass program name
    remaining_args[remaining_count++] = argv[0];
    LOG_INFO("parse_command_line: Program name = '%s'", argv[0]);
    
    for(int i = 1; i < argc; ++i) {
        if(std::strncmp(argv[i], "--width=", 8) == 0) {
            window_width = static_cast<uint32_t>(std::atoi(argv[i] + 8));
            LOG_INFO("parse_command_line: Window width set to %u", window_width);
        } else if(std::strncmp(argv[i], "--height=", 9) == 0) {
            window_height = static_cast<uint32_t>(std::atoi(argv[i] + 9));
            LOG_INFO("parse_command_line: Window height set to %u", window_height);
        } else if(std::strncmp(argv[i], "--backend=", 10) == 0) {
            backend = argv[i] + 10;
            LOG_INFO("parse_command_line: LVGL backend set to '%s'", backend);
        } else if(std::strcmp(argv[i], "--backend") == 0 && i + 1 < argc) {
            backend = argv[++i];
            LOG_INFO("parse_command_line: LVGL backend set to '%s' (separate arg)", backend);
        } else if(std::strcmp(argv[i], "-win") == 0) {
            application_is_windowed = true;  // redundant for cross-platform, but keep for compatibility
            remaining_args[remaining_count++] = argv[i];
            LOG_INFO("parse_command_line: Windowed mode flag detected (passed to game)");
        } else if(!backend && i == 1) {
            // First non-option argument might be backend
            backend = argv[i];
            LOG_INFO("parse_command_line: First argument '%s' treated as backend", backend);
        } else {
            // Pass unknown arguments to the game
            remaining_args[remaining_count++] = argv[i];
            LOG_INFO("parse_command_line: Unknown arg '%s' passed to game", argv[i]);
        }
    }
    
    remaining_args[remaining_count] = nullptr;
    *game_argc = remaining_count;
    *game_argv = remaining_args;
    
    LOG_INFO("parse_command_line: Final window size = %ux%u, backend = '%s'", 
             window_width, window_height, backend ? backend : "(auto)");
    LOG_INFO("parse_command_line: %d arguments will be passed to game engine", remaining_count);
    
    return true;
}

// Initialize LVGL window ////////////////////////////////////////////////////
static bool initialize_app_window()
{
    LOG_INFO("initialize_app_window: Starting LVGL window initialization");
    
    if(!backend) {
        backend = std::getenv("LV_BACKEND");
        if(backend) {
            LOG_INFO("initialize_app_window: Using backend from LV_BACKEND env var: '%s'", backend);
        }
    }
    
    // Default to SDL backend if available
    if(!backend) {
#if LV_USE_SDL
        backend = "sdl";
        LOG_INFO("initialize_app_window: No backend specified, defaulting to SDL");
#else
        backend = "x11";  // fallback to X11 if SDL not available
        LOG_INFO("initialize_app_window: SDL not available, defaulting to X11");
#endif
    }

    LOG_INFO("initialize_app_window: Creating LVGL window %ux%u using backend '%s'", 
             window_width, window_height, backend);
    
    // Log available backends
    LOG_INFO("initialize_app_window: Available LVGL backends:");
#if LV_USE_SDL
    LOG_INFO("  - SDL2 (LV_USE_SDL=1)");
#else
    LOG_INFO("  - SDL2 (disabled)");
#endif
#if LV_USE_X11
    LOG_INFO("  - X11 (LV_USE_X11=1)");
#else
    LOG_INFO("  - X11 (disabled)");
#endif
#if LV_USE_WAYLAND
    LOG_INFO("  - Wayland (LV_USE_WAYLAND=1)");
#else
    LOG_INFO("  - Wayland (disabled)");
#endif

    main_display = LvglPlatform::create_window(window_width, window_height, backend);
    if(!main_display) {
        LOG_ERROR("initialize_app_window: CRITICAL - Failed to initialize LVGL backend '%s'", backend);
        LOG_ERROR("initialize_app_window: Window creation failed - cannot continue");
        return false;
    }
    
    LOG_INFO("initialize_app_window: LVGL window created successfully");
    LOG_INFO("initialize_app_window: Display pointer = %p", (void*)main_display);
    LOG_INFO("initialize_app_window: Window initialization complete");
    
    return true;
}

// Main application loop //////////////////////////////////////////////////////
static void handle_application_events()
{
    // Process LVGL events
    LvglPlatform::poll_events();
    
    // Check if we should quit (LVGL backend dependent)
    // For now, we'll rely on the game engine's quitting mechanism
    if(TheGameEngine && TheGameEngine->getQuitting()) {
        application_quitting = true;
    }
}

// Cross-platform main entry point ///////////////////////////////////////////
int main(int argc, char **argv)
{
    // Initialize logging with a file for better debugging
    Logger::init("generals_boot.log");
    LOG_INFO("================================================================================");
    LOG_INFO("Command & Conquer Generals - Cross-Platform Version Starting");
    LOG_INFO("================================================================================");
    LOG_INFO("Application entry point reached with %d arguments", argc);
    
    // Log all command line arguments
    for(int i = 0; i < argc; ++i) {
        LOG_INFO("  argv[%d] = '%s'", i, argv[i]);
    }
    
    try {
        LOG_INFO(">>> PHASE 1: Command Line Parsing");
        // Parse command line arguments
        int game_argc;
        char **game_argv;
        if(!parse_command_line(argc, argv, &game_argc, &game_argv)) {
            LOG_ERROR("CRITICAL: Failed to parse command line arguments");
            Logger::shutdown();
            return 1;
        }
        LOG_INFO("Command line parsed successfully - %d args for game engine", game_argc);
        
        // Log game engine arguments
        for(int i = 0; i < game_argc; ++i) {
            LOG_INFO("  game_argv[%d] = '%s'", i, game_argv[i]);
        }
        
        LOG_INFO(">>> PHASE 2: Environment Setup");
        // Set up working directory similar to WinMain
        char buffer[1024];
        if(getcwd(buffer, sizeof(buffer))) {
            LOG_INFO("Working directory: %s", buffer);
        } else {
            LOG_WARN("Could not determine working directory");
        }
        
        LOG_INFO(">>> PHASE 3: LVGL Window Initialization");
        // Initialize the LVGL window
        if(!initialize_app_window()) {
            LOG_ERROR("CRITICAL: Failed to initialize application window");
            Logger::shutdown();
            return 1;
        }
        LOG_INFO("LVGL window initialization completed successfully");
        
        LOG_INFO(">>> PHASE 4: Game Engine Bootstrap");
        LOG_INFO("About to call GameMain() - this is where the real game starts");
        LOG_INFO("From this point forward, logging will switch to game engine systems");
        LOG_INFO("================================================================================");
        
        // Call the main game entry point - this is the cross-platform equivalent of WinMain calling GameMain
        GameMain(game_argc, game_argv);
        
        // The game has exited, cleanup
        LOG_INFO("================================================================================");
        LOG_INFO("GameMain() returned - game execution completed");
        LOG_INFO(">>> PHASE 5: Shutdown and Cleanup");
        
        // Cleanup allocated command line arguments
        delete[] game_argv;
        LOG_INFO("Command line arguments cleaned up");
        
    } catch(const std::exception& e) {
        LOG_ERROR("CRITICAL EXCEPTION in main(): %s", e.what());
        Logger::shutdown();
        return 1;
    } catch(...) {
        LOG_ERROR("CRITICAL UNKNOWN EXCEPTION in main()");
        Logger::shutdown();
        return 1;
    }
    
    LOG_INFO("Application shutdown complete");
    LOG_INFO("================================================================================");
    Logger::shutdown();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Cross-platform game engine factory function //////////////////////////////
// This replaces the Win32-specific CreateGameEngine function ////////////////
////////////////////////////////////////////////////////////////////////////////

#include "game_engine_device/lvgl_device/common/lvglgameengine.h"

/**
 * Create the cross-platform LVGL game engine
 * This is the cross-platform equivalent of the Win32 CreateGameEngine function
 */
GameEngine *CreateGameEngine(void)
{
    LOG_INFO("Creating LVGL Game Engine");
    LvglGameEngine *engine = new LvglGameEngine();
    
    // Store reference to main display for the engine to use
    // The engine will need this for rendering operations
    
    return engine;
}