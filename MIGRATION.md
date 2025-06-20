# Migration Plan

This document outlines the high level approach to porting Command & Conquer Generals and Zero Hour to a modern cross platform engine. The original codebase is primarily Win32 and DirectX 9 oriented. All new work targets `-std=gnu++11` with portable libraries.

## Goals

1. **Cross Platform C++11** – compile on Windows, Linux and macOS using a modern toolchain.
2. **LVGL window/canvas** – use LVGL to create the main game window and render surface. On desktop builds fallback to the X11 backend when LVGL cannot directly access the framebuffer.
3. **OpenGL ES 1.1** – replace all DirectX calls with an OpenGL ES 1.1 renderer. A translation layer will be written to emulate the original fixed function API.
4. **Input** – capture keyboard and mouse events via LVGL (falling back to the host window system when necessary).
5. **Audio** – use [miniaudio](https://github.com/mackron/miniaudio) in place of Miles Sound System.
6. **Build System** – convert the original Visual Studio projects to CMake to simplify multi-platform builds.
7. **Directory Layout** – reorganise the repository around `src/`, `include/` and `lib/` directories.  Code under `Generals/Code` will gradually move into this structure.

## Existing Dependencies

The README lists a number of obsolete dependencies that require replacement or removal:

- DirectX SDK
- STLport 4.5.3
- 3DSMax 4 SDK
- NVASM
- BYTEmark
- Miles Sound System & "Asimp3"
- Bink
- SafeDisk API
- GameSpy SDK
- ZLib 1.1.4
- LZH-Light 1.0

These components appear in `Code/Libraries` and in various engine devices.  Many of them rely on proprietary technology and Windows specific APIs.  They must be stubbed out or replaced with open source equivalents during the port.

## Tasks

1. **Audit Windows specific code** – files such as `WinMain.cpp` and the `Win32Device` tree contain heavy Win32 dependencies. Replace usage of `HWND`, `HINSTANCE`, and DirectInput with platform independent abstractions.
2. **Renderer Abstraction** – implement a wrapper that maps the existing DirectX style interfaces to OpenGL ES 1.1 calls.  Remove reliance on the DirectX headers under `Libraries/DirectX`.
3. **Window and Input Layer** – integrate LVGL. For desktop use LVGL's SDL or X11 port; on bare metal target the framebuffer driver.  All input events should funnel through LVGL.
4. **Audio** – strip the Miles Sound System implementation in `GameEngineDevice/Source/MilesAudioDevice` and write a new `MiniaudioDevice` backend using miniaudio.
5. **Video Playback** – the code under `VideoDevice/Bink` uses the proprietary Bink SDK. Replace with an open source video player (e.g. ffmpeg) or provide a stub that displays placeholder frames.
6. **Networking** – remove GameSpy and investigate modern alternatives or implement a lightweight cross platform lobby/proxy layer.
7. **Compression and Archives** – update the old ZLib and LZH-Light usage with current zlib and liblzh implementations, ensuring compatibility with existing archive formats.
8. **Build System Migration** – replace the old Makefiles with CMake. Provide a
   root `CMakeLists.txt` and per-module CMake files, placing source code under
   `src/` and headers under `include/`.
9. **Continuous Integration** – add automated builds on all supported platforms to ensure the port stays compilable.
10. **Documentation** – update the README with new build instructions and maintain `MIGRATION.md` as progress continues.

## Status

Initial `CMakeLists.txt` files and a placeholder `src/` directory have been added. Some code from `Generals/Code` has been relocated into `src/` and `include/`.
Stub CMake files exist under `Generals/Code` for `Main`, `GameEngine`, and `GameEngineDevice` based on the old DSP projects.
The original Windows entry point now lives in `src/Main/WinMain.cpp` after initial restructuring.
Partial GameEngine and GameEngineDevice sources have been moved to `src/GameEngine` and `src/GameEngineDevice`.
The audio subsystem under `GameEngine/Source/Common` has been migrated into
`src/GameEngine/Common/Audio` with corresponding headers now located in
`include/GameEngine/Common`.
Several UI related classes from `GameEngine/Source/GameClient` have been moved
into `src/GameEngine/GameClient` with headers under `include/GameClient`. The
precompiled header `PreRTS.h` now lives in `include/Precompiled` and is added to
the build include paths.
Bezier math helpers (`BezFwdIterator`, `BezierSegment`) and the `CRCDebug`
logging utilities have been migrated into `src/GameEngine/Common` with headers
under `include/GameEngine/Common`. A small stub of `D3DX8Math.h` now lives in
`include/` to keep these sources compiling on non-Windows hosts.
The INI configuration system has been relocated under `src/GameEngine/Common/INI`
with accompanying headers in `include/GameEngine/Common`.
Fast trigonometry helpers (`QuickTrig`) and general game constants (`GameCommon`)
are now located in `src/GameEngine/Common/System` with their headers available
in `include/GameEngine/Common`.
String handling utilities (`AsciiString`, `UnicodeString`, and the legacy
`WSYS_String` wrapper) along with thread primitives (`CriticalSection` and
`ScopedMutex`) have been migrated into the same directory. `PerfTimer.cpp` now
lives directly under `src/GameEngine/Common`.
Legacy code still depends on the Visual Studio projects and will not compile without extensive work.

Additional open source libraries are now tracked as submodules under `lib/`.
After cloning the repository, run `git submodule update --init --recursive`
to fetch the following dependencies:

- **lvgl** – provides the window and canvas system for new ports.
- **miniaudio** – simple cross‑platform audio backend.
-   The README in `lib/miniaudio` highlights built-in MP3 support which will
    simplify shimming the Miles Sound System calls.
- **uGLES** – a lightweight OpenGL ES 1.1 wrapper.
- **zlib** – compression library used by the original asset archives.
- **liblzhl** – reference implementation of EA's LZH-Light algorithm.

A dedicated `lib/CMakeLists.txt` pulls in these libraries so they can
be linked from other modules during the port.  `zlib` and `liblzhl`
are now built as static targets rather than header-only stubs.
