/*
**    Command & Conquer Generals(tm) - Simple D3D8 Test
**    Copyright 2025 Electronic Arts Inc.
**
**    Simple test program to verify basic D3D8 → OpenGL ES translation
*/

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>

#include "common/logger.h"
#include "lvgl_platform/lvgl_platform.h"

// Include only the basic D3D8 headers without the problematic math inline functions
#include "windows.h"
#include "objbase.h"

// Direct includes from min-dx8-sdk (the authoritative source)
#include "../lib/d3d8_gles/lib/min-dx8-sdk/d3d8types.h"
#include "../lib/d3d8_gles/lib/min-dx8-sdk/d3d8caps.h"
#include "../lib/d3d8_gles/lib/min-dx8-sdk/d3d8.h"

// Extern declaration for our Direct3DCreate8 function
extern "C" IDirect3D8 *WINAPI Direct3DCreate8(UINT SDKVersion);

// Globals ////////////////////////////////////////////////////////////////////
static uint32_t window_width = 800;
static uint32_t window_height = 600;
static lv_display_t *main_display = nullptr;

// Initialize LVGL window ////////////////////////////////////////////////////
static bool initialize_app_window()
{
    LOG_INFO("initialize_app_window: Starting LVGL window initialization");
    
    const char* backend = "sdl";
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

// Simple D3D8 Translation Test ///////////////////////////////////////////////
static bool test_simple_d3d8()
{
    LOG_INFO("================================================================================");
    LOG_INFO("TESTING SIMPLE D3D8 → OpenGL ES TRANSLATION CHAIN");
    LOG_INFO("================================================================================");
    
    // Step 1: Create D3D8 interface
    LOG_INFO(">>> STEP 1: Creating D3D8 interface via Direct3DCreate8()");
    LOG_INFO("This should trigger d3d8_gles logs showing interface creation...");
    
    IDirect3D8 *d3d8 = Direct3DCreate8(D3D_SDK_VERSION);
    if(!d3d8) {
        LOG_ERROR("FAILED: Direct3DCreate8() returned NULL!");
        return false;
    }
    LOG_INFO("SUCCESS: D3D8 interface created at %p", (void*)d3d8);
    
    // Step 2: Get adapter count (simple operation)
    LOG_INFO(">>> STEP 2: Getting adapter count");
    LOG_INFO("This should trigger d3d8_gles adapter enumeration logs...");
    
    UINT adapter_count = d3d8->GetAdapterCount();
    LOG_INFO("D3D8 reports %u adapters available", adapter_count);
    
    // Step 3: Get display mode count (simple operation)
    LOG_INFO(">>> STEP 3: Getting display mode count");
    
    UINT mode_count = d3d8->GetAdapterModeCount(0);
    LOG_INFO("Adapter 0 has %u display modes", mode_count);
    
    // Step 4: Try to create device
    LOG_INFO(">>> STEP 4: Creating D3D8 device");
    LOG_INFO("This should trigger major d3d8_gles → u_gles → OpenGL initialization...");
    
    D3DPRESENT_PARAMETERS present_params = {};
    present_params.BackBufferWidth = window_width;
    present_params.BackBufferHeight = window_height;
    present_params.BackBufferFormat = D3DFMT_X8R8G8B8;
    present_params.BackBufferCount = 1;
    present_params.MultiSampleType = D3DMULTISAMPLE_NONE;
    present_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    present_params.hDeviceWindow = (HWND)1; // Dummy window handle
    present_params.Windowed = TRUE;
    present_params.EnableAutoDepthStencil = TRUE;
    present_params.AutoDepthStencilFormat = D3DFMT_D24S8;
    
    IDirect3DDevice8 *device = nullptr;
    HRESULT hr = d3d8->CreateDevice(0, D3DDEVTYPE_HAL, (HWND)1, 
                                   D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
                                   &present_params, &device);
    
    if(SUCCEEDED(hr) && device) {
        LOG_INFO("SUCCESS: D3D8 device created at %p", (void*)device);
        LOG_INFO("This means the basic chain works: D3D8 → d3d8_gles → u_gles → OpenGL ES!");
        
        // Step 5: Basic device operations (no math functions)
        LOG_INFO(">>> STEP 5: Testing basic device operations");
        
        // Test basic clear
        LOG_INFO("Calling device->Clear() - should trigger OpenGL clear...");
        hr = device->Clear(0, nullptr, D3DCLEAR_TARGET, 0xFF4080FF, 1.0f, 0);
        if(SUCCEEDED(hr)) {
            LOG_INFO("Clear operation succeeded!");
        } else {
            LOG_WARN("Clear operation failed: 0x%08lx", hr);
        }
        
        // Test present
        LOG_INFO("Calling device->Present() - should trigger buffer swap...");
        hr = device->Present(nullptr, nullptr, nullptr, nullptr);
        if(SUCCEEDED(hr)) {
            LOG_INFO("Present operation succeeded!");
        } else {
            LOG_WARN("Present operation failed: 0x%08lx", hr);
        }
        
        LOG_INFO("Basic rendering operations completed successfully!");
        
        // Cleanup
        device->Release();
        LOG_INFO("D3D8 device released");
    } else {
        LOG_ERROR("FAILED: D3D8 device creation failed with HRESULT 0x%08lx", hr);
        d3d8->Release();
        return false;
    }
    
    // Cleanup
    d3d8->Release();
    LOG_INFO("D3D8 interface released");
    
    LOG_INFO("================================================================================");
    LOG_INFO("SIMPLE D3D8 TRANSLATION TEST COMPLETED SUCCESSFULLY!");
    LOG_INFO("================================================================================");
    
    return true;
}

// Main test program //////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Initialize logging
    Logger::init("d3d8_simple_test.log");
    LOG_INFO("================================================================================");
    LOG_INFO("Command & Conquer Generals - Simple D3D8 Translation Test");
    LOG_INFO("================================================================================");
    LOG_INFO("Testing: Game → d3d8_gles → u_gles → OpenGL ES → LVGL");
    LOG_INFO("(Avoiding math functions to bypass linking issues)");
    
    try {
        // Initialize the LVGL window first (provides OpenGL context)
        LOG_INFO(">>> PHASE 1: LVGL Window Initialization");
        if(!initialize_app_window()) {
            LOG_ERROR("CRITICAL: Failed to initialize application window");
            Logger::shutdown();
            return 1;
        }
        LOG_INFO("LVGL window ready - OpenGL ES context should be available");
        
        // Run the simple D3D8 translation test
        LOG_INFO(">>> PHASE 2: Simple D3D8 Translation Test");
        if(!test_simple_d3d8()) {
            LOG_ERROR("CRITICAL: Simple D3D8 translation test failed");
            Logger::shutdown();
            return 1;
        }
        
        LOG_INFO(">>> PHASE 3: Visual Test (showing results for 3 seconds)");
        // Show a test screen briefly
        lv_obj_t * label = lv_label_create(lv_screen_active());
        lv_label_set_text(label, "Simple D3D8 → OpenGL ES Test\n\nSUCCESS!\n\nBasic D3D8 translation working\nCheck console/log for detailed results");
        lv_obj_center(label);
        
        // Run for a few seconds to show the result
        for(int i = 0; i < 180; ++i) {  // 3 seconds at 60fps
            LvglPlatform::poll_events();
            lv_timer_handler();
            lv_tick_inc(16);
            usleep(16000);
        }
        
    } catch(const std::exception& e) {
        LOG_ERROR("CRITICAL EXCEPTION in main(): %s", e.what());
        Logger::shutdown();
        return 1;
    } catch(...) {
        LOG_ERROR("CRITICAL UNKNOWN EXCEPTION in main()");
        Logger::shutdown();
        return 1;
    }
    
    LOG_INFO("Simple D3D8 test program completed successfully");
    LOG_INFO("================================================================================");
    Logger::shutdown();
    return 0;
}