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
- STLport 4.5.3 (patched)
- 3DSMax 4 SDK
- NVASM
- BYTEmark
- Miles Sound System & "Asimp3"
- Bink
- SafeDisk API
- UniSpySDK (open source replacement for GameSpy)
- ZLib 1.1.4
- LZH-Light 1.0

These components appear in `Code/Libraries` and in various engine devices.  Many of them rely on proprietary technology and Windows specific APIs.  They must be stubbed out or replaced with open source equivalents during the port.

## Tasks

1. **Audit Windows specific code** – files such as `WinMain.cpp` and the `Win32Device` tree contain heavy Win32 dependencies. Replace usage of `HWND`, `HINSTANCE`, and DirectInput with platform independent abstractions.
2. **Renderer Abstraction** – implement a wrapper that maps the existing DirectX style interfaces to OpenGL ES 1.1 calls.  Remove reliance on the DirectX headers under `Libraries/DirectX`.
3. **Window and Input Layer** – integrate LVGL. For desktop use LVGL's SDL or X11 port; on bare metal target the framebuffer driver.  All input events should funnel through LVGL.
4. **Audio** – strip the Miles Sound System implementation in `GameEngineDevice/Source/MilesAudioDevice` and write a new `MiniaudioDevice` backend using miniaudio.
5. **Video Playback** – the code under `VideoDevice/Bink` uses the proprietary Bink SDK. Replace with an open source video player (e.g. ffmpeg) or provide a stub that displays placeholder frames.
6. **Networking** – replace the retired GameSpy SDK with the open source [UniSpySDK](https://github.com/GameProgressive/UniSpySDK) or implement a lightweight cross platform lobby/proxy layer.
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
The networking subsystem under `GameEngine/Source/GameNetwork` has been moved
into `src/GameEngine/GameNetwork` with headers in `include/GameEngine/GameNetwork`.
The GameSpy-based files remain for now and will be replaced with UniSpySDK code.
Legacy code still depends on the Visual Studio projects and will not compile without extensive work.
The engine relies on several third party libraries stored under `lib/`.
Most of them are tracked as git submodules. After cloning run
`git submodule update --init --recursive` to fetch their sources.
The LVGL UI framework is an exception: version 9.3 is bundled directly in
the repository to allow local patching.
The key dependencies are:

- **lvgl** – provides the window and canvas system for new ports (bundled v9.3).
- **miniaudio** – simple cross‑platform audio backend.
   The README in `lib/miniaudio` highlights built-in MP3 support which will
    simplify shimming the Miles Sound System calls.
- **uGLES** – a lightweight OpenGL ES 1.1 wrapper.
- **UniSpySDK** – open source networking toolkit used in place of GameSpy.
- **STLport** – patched C++ standard library used by the original code.
- **zlib** – compression library used by the original asset archives.
- **liblzhl** – reference implementation of EA's LZH-Light algorithm.

LVGL is compiled as a static library with its sources globbed from
`lib/lvgl/src` and `lib/lvgl/demos`. The build defaults to the X11 backend
with optional support for SDL, Wayland, DRM, fbdev and NuttX via
`-DLVGL_USE_SDL`, `-DLVGL_USE_WAYLAND`, `-DLVGL_USE_LINUX_DRM`,
`-DLVGL_USE_LINUX_FBDEV` and `-DLVGL_USE_NUTTX`.

Miniaudio is also compiled as a small static library so the engine can
link it directly.
A Miles SDK stub under `lib/miles-sdk-stub` now links against miniaudio to provide the legacy `mss32.dll` interface.
The stub has been expanded with additional `AIL_*` helpers so more of the
original audio subsystem compiles against the new backend. It now stores
per-sample user data and exposes a minimal 3D provider interface so older
audio managers work without modification.

On macOS the build previously checked for Apple Clang before enabling the
`-fblocks` option required by the CoreAudio headers. When GCC was selected the
option was omitted and compilation failed. The root `CMakeLists.txt` now forces
`clang` and `clang++` to be used on Apple platforms so the correct Blocks
support is available without manual configuration.

A dedicated `lib/CMakeLists.txt` pulls in these libraries so they can
be linked from other modules during the port.  `zlib` and `liblzhl`
remain built as static targets rather than header-only stubs.

The prototype executable selects the LVGL backend at runtime. The window
defaults to the X11 driver but passing `sdl`, `wayland`, `drm`, `fbdev`
or `nuttx` as the first command line argument (or via the
`LV_BACKEND` environment variable) switches to the respective backend.
Each backend opens a window whose size defaults to 800x600. The size can be
overridden with `--width` and `--height` command line options. A timer in the
stub application demonstrates runtime resizing of the LVGL display so all
backends handle resolution changes consistently.

LVGL setup code has been moved into a small `LvglPlatform` module under `src/`.
This wrapper exposes `create_window()` and `poll_events()` so future
components can initialise the UI without including the driver headers.

The stub application now builds from a portable `src/main.cpp` which
creates the LVGL window through `LvglPlatform` and enters a simple
polling loop.  The legacy `WinMain.cpp` remains under `src/Main` but is
only compiled on Windows targets.

A new `lvglDevice` directory mirrors the legacy Win32 layout. It currently holds empty source and header files ready for the LVGL-based implementation.
The first implemented piece is `LvglOSDisplay.cpp` which provides OSDisplayWarningBox() via lv_msgbox.
`LvglBIGFile.cpp` and `LvglBIGFileSystem.cpp` now port the original BIG archive loader using only standard C++ headers.

LvglLocalFileSystem now replaces Win32 directory calls with std::filesystem for file access.
