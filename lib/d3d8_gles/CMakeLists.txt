# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(D3D8ToGLES LANGUAGES C)

# Set C standard to C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define header-only or single-translation-unit build
option(HEADER_ONLY "Build as header-only library" OFF)

# Find OpenGL ES 1.1 and EGL
find_library(GLESv1_CM_LIBRARY NAMES GLESv1_CM OpenGLES)
find_library(EGL_LIBRARY NAMES EGL)

if(NOT GLESv1_CM_LIBRARY OR NOT EGL_LIBRARY)
    message(WARNING "OpenGL ES 1.1 or EGL not found. Falling back to desktop OpenGL for testing.")
    find_package(OpenGL REQUIRED)
    set(GLESv1_CM_LIBRARY ${OPENGL_gl_LIBRARY})
    set(EGL_LIBRARY "")
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files - organized by functional areas
set(SOURCES 
    # Core D3D8 interfaces
    src/d3d8_core.c
    src/d3d8_device.c
    src/d3d8_resources.c
    src/d3d8_state.c
    
    # D3DX utilities
    src/d3dx_buffer.c
    src/d3dx_math.c
    src/d3dx_mesh.c
    src/d3dx_mesh_shapes.c
    src/d3dx_mesh_loader.c
    src/d3dx_texture.c
    src/d3dx_stubs.c
    
    # OpenGL ES backend
    src/gles_backend.c
    src/gles_helpers.c
    
    # Parsers and utilities
    src/x_mesh_parser.c
)

if(HEADER_ONLY)
    add_library(d3d8_to_gles INTERFACE)
    target_include_directories(d3d8_to_gles INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include)
else()
add_library(d3d8_to_gles STATIC ${SOURCES})
    target_include_directories(d3d8_to_gles PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(d3d8_to_gles PUBLIC
        ${GLESv1_CM_LIBRARY}
        ${EGL_LIBRARY}
        m
        lodepng)
endif()

# Example command line tool for choosing EGL configs
add_executable(egl_config_cli tools/egl_config_cli.c)
target_link_libraries(egl_config_cli PRIVATE d3d8_to_gles)

# Enable error logging
option(ENABLE_LOGGING "Enable internal logging" ON)
if(ENABLE_LOGGING)
    target_compile_definitions(d3d8_to_gles PUBLIC D3D8_GLES_LOGGING)
endif()

# Install targets
install(TARGETS d3d8_to_gles DESTINATION lib)
install(FILES include/d3d8_to_gles.h DESTINATION include)

# Enable testing and build sample test executable
enable_testing()
add_subdirectory(tests)
