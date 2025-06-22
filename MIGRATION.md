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
All remaining modules from `GameEngine/Source/GameClient` have now been
relocated in the same manner. `src/GameEngine/CMakeLists.txt` uses `file(GLOB)`
to compile the new source tree.
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
These libraries are committed directly in the repository, including
**miniaudio**, **uGLES**, **UniSpySDK** and **STLport**.
The LVGL UI framework (version 9.3) is also bundled so it can be patched
locally when needed.
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
`LvglOSDisplay.cpp` has been expanded to handle all button and icon combinations. Modal flags now create a clickable overlay on `lv_layer_top` to block background input and the returned value matches the pressed button.
`LvglMouse.cpp` now registers a pointer device with LVGL and translates its button states and coordinates into the engine's `MouseIO` structure.
The implementation has been expanded with an internal event buffer mirroring
the old Win32 behaviour. Right and middle button states as well as scroll wheel
movement are now tracked when supported by the underlying LVGL backend.  The
temporary `src/LvglInput` stubs have been removed with the application directly
including the new device headers.

LvglLocalFileSystem now replaces Win32 directory calls with std::filesystem for file access.
`FileUtil` in `src/Common/File.cpp` further wraps std::filesystem to provide portable helpers like `createDirectory`.  Win32 `CreateDirectory` calls in `PeerDefs.cpp` now use this API.
The CMake build now globs the `lvglDevice` sources into a static library and links it to the `Generals` stub by default on non-Windows hosts. Pass `-DUSE_LVGL_DEVICE=ON` to force this implementation or `OFF` to fall back to the unported `Win32Device` tree on Windows.
Input handling is beginning to move over to LVGL as well. A small `LvglGameEngine` class now owns new `LvglKeyboard` and `LvglMouse` objects.  `src/main.cpp` creates these and drives their `update()` methods each frame alongside `LvglPlatform::poll_events()`.

The old input classes under `GameEngineDevice/Source/Win32*` will be phased out
in favour of the LVGL implementations.  `Generals/Code/GameEngineDevice/Source/lvglDevice/GameClient/LvglKeyboard.cpp`
and `Generals/Code/GameEngineDevice/Source/lvglDevice/GameClient/LvglMouse.cpp`
previously provided only a basic keyboard and pointer interface.  Modifier keys
are now detected and `convert_lv_key()` translates every key listed in
`KeyDefs.h`, matching the behaviour of `Win32DIKeyboard`.  The new
`lvglDevice` library now lives under `src/GameEngineDevice/lvglDevice` and is
linked unconditionally from `src/CMakeLists.txt`.

Stub headers for `Common/File.h` and `lib/basetype.h` were added to fix case-sensitive buil
- The W3D device common files (radar, convert and factory helpers) now live under `src/GameEngineDevice/W3DDevice/Common` with their headers available in `include/GameEngineDevice/W3DDevice`. Basic player management classes have been moved to `src/GameEngine/Common/RTS` and corresponding headers under `include/GameEngine/Common`.
- The GameLogic subsystem has been relocated from `Generals/Code/GameEngine/Source/GameLogic` and `Generals/Code/GameEngine/Include/GameLogic` into `src/GameEngine/GameLogic` and `include/GameEngine/GameLogic`. `src/GameEngine/CMakeLists.txt` now globs these files into the `gameengine` library.
- The precompiled source `PreRTS.cpp` moved to `src/GameEngine/Precompiled` and the header now excludes Windows-only includes. CMake builds use `target_precompile_headers` when available.
- A lightweight `Logger` utility now lives in `src/Common` and `include/Common`. `Logger::init()` writes to `std::clog` and optionally a file. Macros `LOG_INFO`, `LOG_WARN` and `LOG_ERROR` replace direct console output in `src/main.cpp`.

  Example of logging progress during start-up:

  ```cpp
  Logger::init("generals.log");
  LOG_INFO("Initializing engine");
  LOG_INFO("Loading assets");
  LOG_INFO("Displaying main menu");
  ```
- Additional logging has been added across the startup code path so that
  initialization of major systems like `Shell` and input devices is visible
  in the log output.
- Miles audio management header moved to `include/GameEngineDevice/MilesAudioDevice`. CMake no longer references `Generals/Code/GameEngineDevice/ A lightweight `Logger` utility now lives in `src/Common` and `include/Common`. `Logger::init()` writes to `std::clog` and an optional file. Macros `LOG_INFO`, `LOG_WARN` and `LOG_E
- MilesAudioManager now builds against a miniaudio-based shim which replaces DirectSound. The gameenginedevice module links with the `milesstub` library and audio headers have moved to `include/GameEngineDevice`.
- `gameenginedevice` now links against `milesstub` from `lib/miles-sdk-stub`.
- `miniaudio` builds as its own static library. When compiling on macOS the file
  is built with Clang and the `-fblocks` option so the CoreAudio backend works.
  The `gameenginedevice` module links directly against this library.
- lvglDevice headers relocated to `include/GameEngineDevice/lvglDevice` and old references updated.
- Removed the legacy `Generals/Code` copies of the W3D device common files.
  `src/GameEngineDevice/CMakeLists.txt` now globs these sources from
  `src/GameEngineDevice/W3DDevice/Common`.

- Networking now links against the full UniSpySDK library. GameEngine sources include headers from `lib/UniSpySDK` and no longer rely on GameSpy stubs.
- GitHub Actions now installs `build-essential`, X11 and Mesa development packages so Linux builds compile on CI.

- LocalFile compilation issues fixed on case-sensitive builds by disabling memory pool macros and using standard allocation during migration.
- A portable LocalFile implementation now lives under `src/GameEngine/Common/System` with its header in `include/Common`.  The class wraps the C stdio API instead of Win32 handles.
- Header paths updated to prefer `Common/File.h` so the new implementation is used on case-sensitive systems.
- LocalFile now overrides `operator new` and `delete` to sidestep the old memory pool hooks.
- Added `GameNetwork/transport.h` shim header to resolve case sensitive include errors on Linux.
- SubsystemInterface has been relocated to src/GameEngine/Common/System with its header in include/GameEngine/Common/subsystem_interface.h. Includes have been updated to use this lowercase path.
- Legacy references to `Generals/Code/GameEngine/Include` have been dropped from
  `src/GameEngineDevice` CMake files. All headers are expected under the new
  `include/` hierarchy.
- `StatsCollector` moved to `src/GameEngine/Common` with its header now under
  `include/GameEngine/Common`.

- The legacy `GameNetwork` directories under `Generals/Code` and
  `GeneralsMD/Code` were removed after verifying all files exist in the new
  `src` and `include` hierarchy.

- Remaining headers from `Generals/Code/GameEngine/Include/Common` migrated into `include/GameEngine/Common` and originals removed.
- Source files under `Generals/Code/GameEngine/Source/Common` migrated into `src/GameEngine/Common`. The CMake build lists these new paths and the old copies were deleted.
- Remaining `GameEngineDevice` modules have been relocated. W3D device game
  client headers now live in `include/GameEngineDevice/W3DDevice/GameClient`
  with sources under `src/GameEngineDevice/W3DDevice/GameClient`. The Win32
  device and Miles audio components moved into matching directories in
  `src/GameEngineDevice` with corresponding headers in `include/GameEngineDevice`.
  `src/GameEngineDevice/CMakeLists.txt` globs these directories and the old
  `Generals/Code/GameEngineDevice` tree was removed.

- Additional W3DDevice modules such as `BaseHeightMap` and `camerashakesystem` have been
  moved into the new hierarchy. Shader assets now live under
  `src/GameEngineDevice/W3DDevice/GameClient/Shaders` and the old
  `GeneralsMD/Code/GameEngineDevice` directory has been removed.
- MapCacheBuilder tool moved to `src/Tools/MapCacheBuilder` with a minimal stub executable. The original directory under `Generals/Code/Tools` was removed.
- buildVersionUpdate, versionUpdate and Compress tools moved to `src/Tools` with basic stubs and new CMake targets.
- The remaining tools from `Generals/Code/Tools` are now located under `src/Tools`. Their original directories were removed. The migrated tools are: Autorun, Babylon, CRCDiff, DebugWindow, GUIEdit, ImagePacker, Launcher, NVASM, PATCHGET, ParticleEditor, WW3D, WorldBuilder, mangler, matchbot, textureCompress, timingTest and wolSetup.
- Audio library sources under `Generals/Code/Libraries/Source/WPAudio` were moved to `src/Libraries/WPAudio` with a new static library `wp_audio`. The duplicate files in `GeneralsMD/Code/Libraries/Source/WPAudio` were removed and `gameengine` now links against this library.
- Compression utilities under `Generals/Code/Libraries/Source/Compression` were relocated to `src/Libraries/Compression` with headers in `include/Libraries/Compression`. A new static library `compression` is linked by `gameengine` and the duplicate `GeneralsMD` directory was removed.
- The obsolete `EABrowserDispatch` COM library has been removed. `BrowserDispatch.idl` was archived and the web browser interface will be reimplemented using a cross-platform HTTP layer.
