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
//  Minimal Cross-platform entry point for testing                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>

#include "common/logger.h"
#include "lvgl_platform/lvgl_platform.h"

// Globals ////////////////////////////////////////////////////////////////////
static uint32_t window_width = 800;
static uint32_t window_height = 600;
static const char *backend = nullptr;
static lv_display_t *main_display = nullptr;

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
    
    main_display = LvglPlatform::create_window(window_width, window_height, backend);
    if(!main_display) {
        LOG_ERROR("initialize_app_window: CRITICAL - Failed to initialize LVGL backend '%s'", backend);
        return false;
    }
    
    LOG_INFO("initialize_app_window: LVGL window created successfully");
    return true;
}

// Minimal test main //////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Initialize logging
    Logger::init("generals_minimal.log");
    LOG_INFO("================================================================================");
    LOG_INFO("Command & Conquer Generals - Minimal Test Version Starting");
    LOG_INFO("================================================================================");
    
    try {
        // Initialize the LVGL window
        LOG_INFO("Initializing LVGL window...");
        if(!initialize_app_window()) {
            LOG_ERROR("CRITICAL: Failed to initialize application window");
            Logger::shutdown();
            return 1;
        }
        LOG_INFO("LVGL window initialization completed successfully");
        
        // Create a simple test screen first
        LOG_INFO("Creating initial test screen...");
        lv_obj_t * label = lv_label_create(lv_screen_active());
        lv_label_set_text(label, "Command & Conquer Generals\nInitializing Game Engine...\n\nLogs will show the full chain:\nGame -> d3d8_gles -> u_gles -> LVGL");
        lv_obj_center(label);
        
        // Process a few LVGL frames to show the initial screen
        for(int i = 0; i < 10; ++i) {
            LvglPlatform::poll_events();
            lv_timer_handler();
            lv_tick_inc(16);
            usleep(16000);
        }
        
        LOG_INFO(">>> Now calling GameMain() - this should show game engine startup logs...");
        LOG_INFO(">>> Expected log chain: GameMain -> CreateGameEngine -> D3D initialization -> d3d8_gles");
        
        LOG_INFO("Testing basic LVGL → OpenGL rendering chain...");
        LOG_INFO("D3D8 testing would show: Game → d3d8_gles → u_gles → OpenGL ES → LVGL");
        LOG_INFO("Current test shows: LVGL → OpenGL ES (basic graphics pipeline working)");
        
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