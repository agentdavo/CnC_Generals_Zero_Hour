add_library(gen_main EXCLUDE_FROM_ALL
    WinMain.cpp
    RTS.RC
)

target_include_directories(gen_main PUBLIC ${PROJECT_SOURCE_DIR}/include)
# Legacy Win32 entry point not built by default
