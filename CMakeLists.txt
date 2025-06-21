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

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Third party libraries
add_subdirectory(lib)

# Core application stub
add_subdirectory(src)

# Migrated modules from Generals/Code
add_subdirectory(src/Main)
add_subdirectory(src/GameEngine)
add_subdirectory(src/GameEngineDevice)

# Legacy tools pending migration
add_subdirectory(Generals/Code/Tools/mangler)
