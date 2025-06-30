if(WIN32)
add_library(gen_main EXCLUDE_FROM_ALL
    win_main.cpp
    RTS.RC
)

target_include_directories(gen_main PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)
# Legacy Win32 entry point not built by default
endif()

# Include the new cross-platform executable
include(CMakeLists_new.txt)
