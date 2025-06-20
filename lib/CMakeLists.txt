# Build third party libraries used by the port
# Currently they are treated as header-only interface targets

add_library(lvgl INTERFACE)
target_include_directories(lvgl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lvgl)

add_library(miniaudio INTERFACE)
target_include_directories(miniaudio INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/miniaudio)

add_library(uGLES INTERFACE)
target_include_directories(uGLES INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/uGLES)

add_subdirectory(zlib)
add_subdirectory(liblzhl)

# Provide a simple alias matching the previous interface target name
add_library(liblzhl ALIAS lzhl)
