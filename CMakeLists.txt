cmake_minimum_required(VERSION 3.10)
project(GeneralsPort LANGUAGES CXX)

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
