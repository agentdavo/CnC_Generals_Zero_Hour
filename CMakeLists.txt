cmake_minimum_required(VERSION 3.10)

# On macOS the build relies on Clang to enable the Blocks extension required by
# CoreAudio. When the user configures the project without explicitly selecting
# Clang, CMake may default to GCC which causes compilation failures. Detect the
# Apple platform early and force the compilers to clang/clang++ so the rest of
# the build can assume Blocks support is available.
if(APPLE)
    set(CMAKE_C_COMPILER clang CACHE STRING "C compiler" FORCE)
    set(CMAKE_CXX_COMPILER clang++ CACHE STRING "C++ compiler" FORCE)
endif()

project(GeneralsPort LANGUAGES C CXX)

# Verbose compile commands and color diagnostics
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    # Apple's ranlib treats empty object files as fatal by default. Some
    # optional LVGL sources compile to no symbols depending on build flags
    # which stops the archive step. Use CMAKE_*_ARCHIVE_FINISH to append
    # the -no_warning_for_no_symbols flag instead of overriding CMAKE_RANLIB
    # so that CMake can still locate the ranlib executable.
    foreach(lang C CXX)
        set(CMAKE_${lang}_ARCHIVE_FINISH
            "<CMAKE_RANLIB> -no_warning_for_no_symbols <TARGET>"
            CACHE STRING "Archive finish command" FORCE)
    endforeach()
endif()

# Third party libraries
message(STATUS "Configuring bundled libraries")
add_subdirectory(lib)

# Core application stub
message(STATUS "Configuring core sources")
add_subdirectory(src)

# Migrated modules from Generals/Code
message(STATUS "Configuring migrated engine modules")
add_subdirectory(src/Main)
add_subdirectory(src/GameEngine)
add_subdirectory(src/GameEngineDevice)

# Tools migrated from Generals/Code
add_subdirectory(src/Tools)

