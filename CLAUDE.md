# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

### Basic Build
```bash
cmake -S . -B build && cmake --build build
```

### Development Build with Strict Warnings (for CI/testing)
```bash
cmake -S . -B build -DSTRICT_BUILD=ON
cmake --build build
```

### LVGL-Only Build (without game engine)
```bash
cmake -S . -B build -DBUILD_ENGINE=OFF
cmake --build build -j1
./build/src/examples/lvgl_hello/lvgl_hello
```

### Running Tests
```bash
# Build and run DirectX translation layer tests
cmake -S . -B build && cmake --build build
cd build && ctest

# Run specific test
./build/lib/d3d8_gles/tests/capability_test

# Run OpenGL ES conformance tests
cmake -S . -B build && cmake --build build
./build/lib/u_gles/conformance/tests_main

# Test D3D8 → OpenGL ES translation layer
./build/src/main/generals  # Simple D3D8 interface test
```

### Platform-Specific Notes
- **macOS**: Must use Clang compiler (set automatically) for Blocks extension support with CoreAudio
- **Linux**: Install development dependencies: `sudo apt install build-essential cmake libsdl2-dev libgl1-mesa-dev libx11-dev libglu1-mesa-dev xorg-dev`
- **SDL2 Backend**: Enable with `-DLVGL_USE_SDL=ON` (default ON)

## Architecture Overview

This is a preservation project migrating Command & Conquer Generals from legacy Visual Studio 6.0/DirectX to modern cross-platform C++11. The architecture uses translation layers to maintain compatibility while enabling modern deployment.

### Key Components

#### Game Engine Core (`src/game_engine/`)
- **Common**: Memory management, file I/O, audio system integration
- **Game Logic**: AI, object management, scripting, physics, weapons
- **Game Network**: Multiplayer networking with GameSpy replacement (UniSpySDK)

#### Device Abstraction Layer (`src/game_engine_device/`)
- **LVGL Device**: Cross-platform UI backend replacing Win32 windowing
- **Miles Audio Device**: Audio system abstraction (being replaced by miniaudio)
- **W3D Device**: Westwood 3D rendering system with OpenGL translation
- **Video Device**: Bink video playback support

#### Translation Layers (`lib/`)
- **d3d8_gles**: DirectX 8 → OpenGL ES 1.1 translation with comprehensive test suite ✅ **FUNCTIONAL**
  - Complete D3DX math function implementation (matrices, vectors, transforms)
  - COM interface emulation using C wrapper structs
  - Organized into 12 focused modules (core, device, resources, state, D3DX utilities, OpenGL backend)
  - Successfully creates D3D8 interfaces and handles basic API calls
- **miniaudio**: Replaces Miles Sound System with cross-platform audio
- **UniSpySDK**: Open source GameSpy replacement for networking

#### Legacy Libraries
- **STLport 4.5.3**: Patched legacy C++ standard library (in `lib/stlport/`)
- **WW Vegas**: Westwood's graphics and engine libraries (being modernized)

### Directory Structure
- `src/` - New cross-platform source code
- `include/` - Public headers organized by component
- `lib/` - Third-party dependencies (LVGL, miniaudio, zlib, etc.)
- `generals_md/` - Legacy original source tree (being migrated from)

## Development Patterns

### Naming Conventions
- New code uses snake_case
- Headers in `include/` mirror source organization in `src/`
- Platform-specific code isolated in device modules

### Migration Strategy
- Legacy code gradually moved from `generals_md/` to `src/`
- Translation layers maintain API compatibility
- Device abstraction enables platform portability
- Modern C++11 patterns replace Win32-specific code

### Current Migration Status
- ✅ **D3D8 Translation Layer**: Fully functional with complete math library
- ✅ **LVGL Windowing**: Cross-platform window creation working
- ✅ **Game Engine Core**: Builds successfully with all components
- 🔄 **DirectX → OpenGL Rendering**: Interface layer functional, device creation needs window integration
- 🔄 **Audio System**: Miles SDK stubbed, miniaudio integration in progress

### Key Dependencies
- **LVGL 9.3**: UI framework with SDL2/X11/Wayland backends
- **SDL2**: Required for LVGL backend on most platforms
- **OpenGL/OpenGL ES**: Graphics rendering target
- **CMake 3.10+**: Build system

## Testing

### Test Suites
- `lib/d3d8_gles/tests/`: Comprehensive DirectX translation tests
- `lib/u_gles/conformance/`: OpenGL ES conformance tests
- LVGL examples serve as integration tests
- `src/main/generals`: D3D8 translation layer integration test

### Test Status
- ✅ **D3D8 Interface Creation**: Direct3DCreate8() working
- ✅ **D3DX Math Functions**: All matrix and vector operations implemented
- ✅ **LVGL + OpenGL ES**: Window creation and basic rendering functional
- 🔄 **EGL Surface Creation**: Requires proper window handle integration

### CI Configuration
- GitHub Actions builds on Linux with strict warnings
- Automated dependency installation and testing
- Cross-platform compatibility verification

## Development Notes

### Apple-Specific Requirements
- Must use Clang compiler for Blocks extension (set automatically)
- Special ranlib configuration for empty object files in LVGL

### Library Configuration
- LVGL backends configured via CMake options (`LVGL_USE_SDL`, `LVGL_USE_X11`, etc.)
- Third-party libraries built via `lib/CMakeLists.txt`
- Strict build mode available for development (`-DSTRICT_BUILD=ON`)

### Common Issues
- SDL2 development headers required for LVGL backend
- OpenGL/Mesa development libraries needed for graphics
- Some legacy code may require extensive modification for modern C++ standards

## Examples and Development Testing

### Available Examples
- `src/examples/lvgl_hello/`: Basic LVGL "Hello World" application
- `src/examples/d3d8_triangle/`: DirectX 8 triangle rendering test
- `src/examples/lvgl_ugles_demo/`: LVGL with OpenGL ES integration demo

### Running Examples
```bash
# LVGL Hello World (minimal UI example)
cmake -S . -B build -DBUILD_ENGINE=OFF && cmake --build build
./build/src/examples/lvgl_hello/lvgl_hello

# DirectX 8 Triangle Test
cmake -S . -B build && cmake --build build
./build/src/examples/d3d8_triangle/d3d8_triangle
```

## Development Scripts

### Code Analysis Scripts
```bash
# Find directories with capital letters (useful for migration tracking)
python3 scripts/capitals.py

# Generate DirectX usage report
python3 scripts/d3d_scan.py > directx_report.md
```

The `capitals.py` script identifies directories that don't follow the snake_case convention used in new code, helping track migration progress from legacy Win32 naming.

The `d3d_scan.py` script scans `src/` and `include/` directories for DirectX API usage, generating a report showing which files contain DirectX references and their specific usage patterns. This is valuable for:
- Tracking DirectX → OpenGL ES translation progress
- Identifying files that need attention during the graphics API migration
- Understanding the scope of DirectX dependencies

## Linting and Code Quality

This project does not currently have automated linting configured. When making changes:
- Follow existing code style in each module (legacy code uses different conventions than new code)
- Use strict build mode for development: `cmake -S . -B build -DSTRICT_BUILD=ON`
- Ensure tests pass before committing changes