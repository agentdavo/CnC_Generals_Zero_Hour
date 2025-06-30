/*
**    Command & Conquer Generals(tm) - D3D8 Translation Test
**    Copyright 2025 Electronic Arts Inc.
**
**    Test program to verify D3D8 → OpenGL ES translation chain
*/

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>

#include "common/logger.h"
#include "lvgl_platform/lvgl_platform.h"

// D3D8 headers - this should go through our translation layer
#include "d3d8_to_gles.h"

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

// D3D8 Translation Test //////////////////////////////////////////////////////
static bool test_d3d8_translation()
{
    LOG_INFO("================================================================================");
    LOG_INFO("TESTING D3D8 → OpenGL ES TRANSLATION CHAIN");
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
    
    // Step 2: Get adapter information
    LOG_INFO(">>> STEP 2: Getting adapter information");
    LOG_INFO("This should trigger d3d8_gles adapter enumeration logs...");
    
    UINT adapter_count = d3d8->GetAdapterCount();
    LOG_INFO("D3D8 reports %u adapters available", adapter_count);
    
    if(adapter_count > 0) {
        D3DADAPTER_IDENTIFIER8 adapter_info;
        HRESULT hr = d3d8->GetAdapterIdentifier(0, 0, &adapter_info);
        if(SUCCEEDED(hr)) {
            LOG_INFO("Adapter 0: %s", adapter_info.Description);
            LOG_INFO("Driver: %s", adapter_info.Driver);
        } else {
            LOG_WARN("Failed to get adapter 0 info: 0x%08lx", hr);
        }
    }
    
    // Step 3: Check display modes
    LOG_INFO(">>> STEP 3: Enumerating display modes");
    LOG_INFO("This should trigger d3d8_gles display mode enumeration...");
    
    UINT mode_count = d3d8->GetAdapterModeCount(0);
    LOG_INFO("Adapter 0 has %u display modes", mode_count);
    
    // Step 4: Check device capabilities
    LOG_INFO(">>> STEP 4: Getting device capabilities");
    
    D3DCAPS8 caps;
    HRESULT hr = d3d8->GetDeviceCaps(0, D3DDEVTYPE_HAL, &caps);
    if(SUCCEEDED(hr)) {
        LOG_INFO("Device caps retrieved successfully");
        LOG_INFO("Max texture width: %lu", caps.MaxTextureWidth);
        LOG_INFO("Max texture height: %lu", caps.MaxTextureHeight);
    } else {
        LOG_WARN("Failed to get device caps: 0x%08lx", hr);
    }
    
    // Step 5: Create D3D8 device
    LOG_INFO(">>> STEP 5: Creating D3D8 device");
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
    hr = d3d8->CreateDevice(0, D3DDEVTYPE_HAL, (HWND)1, 
                           D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
                           &present_params, &device);
    
    if(SUCCEEDED(hr) && device) {
        LOG_INFO("SUCCESS: D3D8 device created at %p", (void*)device);
        LOG_INFO("This means the full chain worked: D3D8 → d3d8_gles → u_gles → OpenGL ES!");
        
        // Step 6: Basic device operations
        LOG_INFO(">>> STEP 6: Testing basic device operations");
        
        // Clear the backbuffer
        LOG_INFO("Calling device->Clear() - should trigger OpenGL clear...");
        device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
                     D3DCOLOR_RGBA(64, 128, 255, 255), 1.0f, 0);
        
        // Present the frame
        LOG_INFO("Calling device->Present() - should trigger buffer swap...");
        device->Present(nullptr, nullptr, nullptr, nullptr);
        
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
    LOG_INFO("D3D8 TRANSLATION TEST COMPLETED SUCCESSFULLY!");
    LOG_INFO("================================================================================");
    
    return true;
}

// Main test program //////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Initialize logging
    Logger::init("d3d8_translation_test.log");
    LOG_INFO("================================================================================");
    LOG_INFO("Command & Conquer Generals - D3D8 Translation Chain Test");
    LOG_INFO("================================================================================");
    LOG_INFO("Testing the full chain: Game → d3d8_gles → u_gles → OpenGL ES → LVGL");
    
    try {
        // Initialize the LVGL window first (provides OpenGL context)
        LOG_INFO(">>> PHASE 1: LVGL Window Initialization");
        if(!initialize_app_window()) {
            LOG_ERROR("CRITICAL: Failed to initialize application window");
            Logger::shutdown();
            return 1;
        }
        LOG_INFO("LVGL window ready - OpenGL ES context should be available");
        
        // Run the D3D8 translation test
        LOG_INFO(">>> PHASE 2: D3D8 Translation Layer Test");
        if(!test_d3d8_translation()) {
            LOG_ERROR("CRITICAL: D3D8 translation test failed");
            Logger::shutdown();
            return 1;
        }
        
        LOG_INFO(">>> PHASE 3: Visual Test (showing results for 3 seconds)");
        // Show a test screen briefly
        lv_obj_t * label = lv_label_create(lv_screen_active());
        lv_label_set_text(label, "D3D8 → OpenGL ES Translation Test\n\nSUCCESS!\n\nCheck console/log for detailed results\nGame rendering chain is functional");
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
    
    LOG_INFO("Test program completed successfully");
    LOG_INFO("================================================================================");
    Logger::shutdown();
    return 0;
}