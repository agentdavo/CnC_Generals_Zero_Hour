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

### Platform-Specific Notes
- **macOS**: Must use Clang compiler (set automatically) for Blocks extension support with CoreAudio
- **Linux**: Install development dependencies: `sudo apt install build-essential cmake libsdl2-dev libgl1-mesa-dev`
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
- **d3d8_gles**: DirectX 8 → OpenGL ES 1.1 translation with comprehensive test suite
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