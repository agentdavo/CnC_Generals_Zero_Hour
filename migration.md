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
10. **Documentation** – update the README with new build instructions and maintain `migration.md` as progress continues.
11. **GameEngineDevice Snake Case** – rename the `GameEngineDevice` tree to `game_engine_device` and convert headers/sources to snake case. Update all include paths and create temporary stubs in `include/common` so the build remains green during the transition.

## Status

Initial `CMakeLists.txt` files and a placeholder `src/` directory have been added. Some code from `Generals/Code` has been relocated into `src/` and `include/`.
Stub CMake files exist under `Generals/Code` for `Main`, `GameEngine`, and `GameEngineDevice` based on the old DSP projects.
The original Windows entry point now lives in `src/Main/WinMain.cpp` after initial restructuring.
Partial GameEngine and GameEngineDevice sources have been moved to `src/GameEngine` and `src/GameEngineDevice`.
The audio subsystem under `GameEngine/Source/Common` has been migrated into
`src/game_engine/common/Audio` with corresponding headers now located in
`include/GameEngine/Common`.
Several UI related classes from `GameEngine/Source/game_client` have been moved
into `src/GameEngine/game_client` with headers under `include/game_client`. The
precompiled header `PreRTS.h` now lives in `include/Precompiled` and is added to
the build include paths.
All remaining modules from `GameEngine/Source/game_client` have now been
relocated in the same manner. `src/GameEngine/CMakeLists.txt` uses `file(GLOB)`
to compile the new source tree.
Bezier math helpers (`BezFwdIterator`, `BezierSegment`) and the `CRCDebug`
logging utilities have been migrated into `src/GameEngine/Common` with headers
under `include/GameEngine/Common`. A small stub of `d3dx8_math.h` now lives in
`include/` to keep these sources compiling on non-Windows hosts.
The INI configuration system has been relocated under `src/game_engine/common/INI`
with accompanying headers in `include/GameEngine/Common`.
Fast trigonometry helpers (`QuickTrig`) and general game constants (`GameCommon`)
are now located in `src/game_engine/common/System` with their headers available
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
- **d3d8_gles** – custom shim translating DirectX 8 to OpenGL ES 1.1.
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
A Miles SDK stub under `lib/miles_sdk_stub` now links against miniaudio to provide the legacy `mss32.dll` interface.
The stub has been expanded with additional `AIL_*` helpers so more of the
original audio subsystem compiles against the new backend. It now stores
per-sample user data and exposes a minimal 3D provider interface so older
audio managers work without modification.
- The `wwaudio` module now includes the stub's headers so Miles functions
  compile without the original SDK.

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

Rendering is now handled by the `d3d8_gles` library which implements the Direct3D 8 runtime on top of OpenGL ES 1.1. SDL windows returned from `LvglPlatform::create_window()` are passed straight into `Direct3DCreate8` so the game continues to use familiar DirectX entry points while the shim draws via EGL.

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
Input handling is beginning to move over to LVGL as well. A small `LvglGameEngine` class now owns new `LvglKeyboard` and `LvglMouse` objects.  `src/main/main.cpp` creates these and drives their `update()` methods each frame alongside `LvglPlatform::poll_events()`.

The old input classes under `GameEngineDevice/Source/Win32*` will be phased out
in favour of the LVGL implementations.  `Generals/Code/GameEngineDevice/Source/lvglDevice/game_client/LvglKeyboard.cpp`
and `Generals/Code/GameEngineDevice/Source/lvglDevice/game_client/LvglMouse.cpp`
previously provided only a basic keyboard and pointer interface.  Modifier keys
are now detected and `convert_lv_key()` translates every key listed in
`KeyDefs.h`, matching the behaviour of `Win32DIKeyboard`.  The new
`lvglDevice` library now lives under `src/game_engine_device/lvgl_device` and is
linked unconditionally from `src/CMakeLists.txt`.
Caps Lock state is now tracked inside `LvglKeyboard` using `lv_indev_get_key()`
so text entry mirrors the Win32 behaviour.

Stub headers for `common/File.h` and `lib/basetype.h` were added to fix case-sensitive buil
- The W3D device common files (radar, convert and factory helpers) now live under `src/game_engine_device/w3d_device/Common` with their headers available in `include/game_engine_device/w3d_device`. Basic player management classes have been moved to `src/game_engine/common/RTS` and corresponding headers under `include/GameEngine/Common`.
- The GameLogic subsystem has been relocated from `Generals/Code/GameEngine/Source/GameLogic` and `Generals/Code/GameEngine/Include/GameLogic` into `src/GameEngine/GameLogic` and `include/GameEngine/GameLogic`. `src/GameEngine/CMakeLists.txt` now globs these files into the `gameengine` library.
- The precompiled source `PreRTS.cpp` moved to `src/GameEngine/Precompiled` and the header now excludes Windows-only includes. CMake builds use `target_precompile_headers` when available.
- A lightweight `Logger` utility now lives in `src/Common` and `include/common`. `Logger::init()` writes to `std::clog` and optionally a file. Macros `LOG_INFO`, `LOG_WARN` and `LOG_ERROR` replace direct console output in `src/main.cpp`.

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
- Miles audio management header moved to `include/GameEngineDevice/MilesAudioDevice`. CMake no longer references `Generals/Code/GameEngineDevice/ A lightweight `Logger` utility now lives in `src/Common` and `include/common`. `Logger::init()` writes to `std::clog` and an optional file. Macros `LOG_INFO`, `LOG_WARN` and `LOG_E
- MilesAudioManager now builds against a miniaudio-based shim which replaces DirectSound. The gameenginedevice module links with the `milesstub` library and audio headers have moved to `include/GameEngineDevice`.
- `gameenginedevice` now links against `milesstub` from `lib/miles_sdk_stub`.
- `miniaudio` builds as its own static library. When compiling on macOS the file
  is built with Clang and the `-fblocks` option so the CoreAudio backend works.
  The `gameenginedevice` module links directly against this library.
- lvglDevice headers relocated to `include/GameEngineDevice/lvglDevice` and old references updated.
- Removed the legacy `Generals/Code` copies of the W3D device common files.
  `src/GameEngineDevice/CMakeLists.txt` now globs these sources from
  `src/game_engine_device/w3d_device/Common`.

- Networking now links against the full UniSpySDK library. GameEngine sources include headers from `lib/uni_spy_sdk` and no longer rely on GameSpy stubs.
- Third-party library directories `UniSpySDK` and `STLport` were renamed to `uni_spy_sdk` and `stlport`. CMake include paths and link targets updated accordingly.
- GitHub Actions now installs `build-essential`, X11 and Mesa development packages so Linux builds compile on CI.

- LocalFile compilation issues fixed on case-sensitive builds by disabling memory pool macros and using standard allocation during migration.
- A portable LocalFile implementation now lives under `src/game_engine/common/System` with its header in `include/common`.  The class wraps the C stdio API instead of Win32 handles.
- Header paths updated to prefer `common/File.h` so the new implementation is used on case-sensitive systems.
- LocalFile now overrides `operator new` and `delete` to sidestep the old memory pool hooks.
- Added `GameNetwork/transport.h` shim header to resolve case sensitive include errors on Linux.
- SubsystemInterface has been relocated to src/game_engine/common/System with its header in include/game_engine/common/subsystem_interface.h. Includes have been updated to use this lowercase path.
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
  client headers now live in `include/game_engine_device/w3d_device/game_client`
  with sources under `src/game_engine_device/w3d_device/game_client`. The Win32
  device and Miles audio components moved into matching directories in
  `src/GameEngineDevice` with corresponding headers in `include/GameEngineDevice`.
  `src/GameEngineDevice/CMakeLists.txt` globs these directories and the old
  `Generals/Code/GameEngineDevice` tree was removed.

- Additional w3d_device modules such as `BaseHeightMap` and `camerashakesystem` have been
  moved into the new hierarchy. Shader assets now live under
  `src/game_engine_device/w3d_device/game_client/Shaders` and the old
  `GeneralsMD/Code/GameEngineDevice` directory has been removed.
- MapCacheBuilder tool moved to `src/tools/map_cache_builder` with a minimal stub executable. The original directory under `Generals/Code/Tools` was removed.
- buildVersionUpdate, versionUpdate and Compress tools moved to `src/tools` with basic stubs and new CMake targets.
- The remaining tools from `Generals/Code/Tools` are now located under `src/tools`. Their original directories were removed. The migrated tools are: Autorun, Babylon, CRCDiff, DebugWindow, GUIEdit, ImagePacker, Launcher, NVASM, PATCHGET, ParticleEditor, WW3D, WorldBuilder, mangler, matchbot, textureCompress, timingTest and wolSetup.
- Audio library sources under `Generals/Code/Libraries/Source/WPAudio` were moved to `src/Libraries/WPAudio` with a new static library `wp_audio`. The duplicate files in `GeneralsMD/Code/Libraries/Source/WPAudio` were removed and `gameengine` now links against this library.
- Building of `wp_audio` is temporarily disabled until its headers are fully migrated.
- Compression utilities under `Generals/Code/Libraries/Source/Compression` were relocated to `src/Libraries/Compression` with headers in `include/Libraries/Compression`. A new static library `compression` is linked by `gameengine` and the duplicate `GeneralsMD` directory was removed.
- Fixed case-sensitive include path to `zlib/zlib.h` in `CompressionManager.cpp`.
- `compression` library now links against the bundled `zlib` so headers resolve on non-Windows builds.
- WW3D2 now includes headers from `src/tools/WW3D/pluglib` for `always.h` and related utilities.
- WWVegas helper libraries `Wwutil` and `WWSaveLoad` were moved into `src/Libraries/WWVegas` and now build the `wwutil` and `wwsaveload` static libraries. The duplicate `GeneralsMD` directories were removed and `gameengine` links against these targets.
- Additional WWVegas libraries `WWMath` and `WWLib` now live under `src/Libraries/WWVegas` as the `wwmath` and `wwlib` static libraries. The old copies under `Generals` and `GeneralsMD` were removed and `gameengine` links to them.
- The WW3D2 rendering library was migrated to `src/Libraries/WWVegas/WW3D2` and builds the `ww3d2` static library. The original directory under `Generals/Code` has been removed.
- A stub `Miles6` directory remains in `src/Libraries/WWVegas` with only a `.gitignore` as the legacy Miles Sound System is replaced by miniaudio. The duplicate `GeneralsMD` folder was removed.
- A placeholder `wwshade` directory has been introduced under `src/Libraries/WWVegas` exposing an empty `wwshade` interface target for future shader code migration.
- The `wwshade` shader library was moved to `src/Libraries/WWVegas/wwshade` and now builds the `wwshade` static library. The original directory under `GeneralsMD` was removed.
- Certain `WW3D2` files were restored from the temp backup.
- The obsolete `EABrowserDispatch` COM library has been removed. `BrowserDispatch.idl` was archived and the web browser interface will be reimplemented using a cross-platform HTTP layer.
- macOS build now appends `-no_warning_for_no_symbols` using `CMAKE_C_ARCHIVE_FINISH` and `CMAKE_CXX_ARCHIVE_FINISH` so empty LVGL object files no longer break the archive step while still allowing CMake to find the correct `ranlib` executable.
- Added a portable stub for `windows.h` under `include/common` to ease non-Windows builds.
- Updated source and header includes to reference this stub via `common/windows.h`.
- Fixed case sensitive includes for the WWVegas libraries. `wwlib`, `wwmath` and
  `ww3d2` now add the global `include/` directory to their CMake
  `target_include_directories` so macOS builds resolve headers such as
  `common/windows.h` and `game_engine/common/Debug.h`.
- Renamed `Vector.H` to `vector.h` and updated all includes to use the lowercase
  path to compile on case sensitive filesystems.
- Corrected `WWDownload` headers to include `ftp.h` directly and match file
  casing.
- Moved `include/Common` to `include/common` and updated header includes.
- Renamed Win32Device, VideoDevice and MilesAudioDevice directories to snake_case along with associated sources and headers.
- Consolidated 'Tools' directory casing; duplicate 'src/Tools' removed and lowercase 'src/tools' kept.
- Standardised D3DX8Math stub as 'd3dx8_math.h' across the include tree.
- MapCacheBuilder tool relocated to `src/tools/map_cache_builder` and the entry
  point renamed to `win_main.cpp`.
- Win32BIGFile.cpp and Win32BIGFileSystem.cpp stubs removed; lvgl_big_file.cpp and lvgl_big_file_system.cpp now supply the BIG archive loader.
- Engine startup creates LvglLocalFileSystem and Win32BIGFileSystem so .big archives load automatically.
- BIG file loaders now allocate ArchiveFile objects via std::unique_ptr to avoid leaks on early exit.
- ArchivedFileInfo entries in ArchiveFile are now managed by std::unique_ptr and
  addFile takes a const reference. This removes manual delete logic and improves
  memory safety.
- zlib is now built with `Z_PREFIX` to avoid type clashes with the project's
  `Byte` alias. `CompressionManager` calls updated to use the prefixed
  `z_compress2` and `z_uncompress` APIs.
- Added compatibility wrappers for refcount.h and STLTypedefs.h.
- Modernized `wwlib`'s `bittype.h` to use C++11 `using` aliases guarded
  against redefinition with the win32_compat header. This resolves
  duplicate typedef errors on macOS and clarifies the intent of the
  size-specific types.
- Introduced cross-platform fram_grab stubs and renamed files to snake_case.
- Replaced Windows allocation macros with standard new in aabtreebuilder.cpp.
- nox_compress now calls LZHLDecompress with size_t sizes.
- Fixed return type of lib/zlib/maketree.c main for standards compliance.
- Renamed tool directories versionUpdate, buildVersionUpdate and Compress to snake_case and updated CMake references.
- Building with `cmake --build build -- -j1` avoids the 'No child processes' error seen on some hosts when using parallel Makefiles.
- Unified `src/game_engine/common` with snake_case source files and removed legacy `Common` duplicate.
- Renamed radar implementation to snake_case under w3d_device/common/system.
- Introduced a portable `tchar.h` wrapper and added case-correct alias headers for cross-platform builds.
- Added lvgl_hello example under src/examples. The program opens an 800x600 LVGL window and demonstrates widgets with a 'Hello world' label and a centred button that reacts to clicks. Building with -DBUILD_ENGINE=OFF skips the main Generals engine.
- lvgl_hello now selects the SDL backend by default, improving rendering on macOS.
- Added d3d8_triangle example showing the DirectX 8 shim. It opens an SDL window via LvglPlatform and renders a rotating coloured triangle using D3D8 calls.
- Updated CPU detection code for non-Windows builds and replaced Windows headers with portable stubs.
- Standardised Vegas WW list headers to snake case.
- LVGL's X11 backend is now optional. The cmake option `LVGL_USE_X11` defaults to OFF
  and `lv_conf.h` honours compile definitions so backends can be toggled without
  warnings.
- WW3D2 links against the `d3d8_gles` shim and `gameenginedevice` now links this
  library so DirectX 8 calls route through the OpenGL ES translation layer.
- The microGLES renderer under `lib/u_gles` is built by default. Its `renderer_lib`
  target is aliased as `uGLES` and the Generals executable links this library.
- DX8 wrapper now enumerates a stub 'microGLES Software Renderer' adapter and exposes a fixed 800x600 display mode so the engine can start without querying real hardware.
- Introduced a portable `tchar.h` wrapper and added case-correct alias headers for cross-platform builds.
- Added lvgl_hello example under src/examples. The program opens an 800x600 LVGL window and demonstrates widgets with a 'Hello world' label and a centred button that reacts to clicks. Building with -DBUILD_ENGINE=OFF skips the main Generals engine.
- lvgl_hello now selects the SDL backend by default, improving rendering on macOS.
- Added d3d8_triangle example showing the DirectX 8 shim. It opens an SDL window via LvglPlatform and renders a rotating coloured triangle using D3D8 calls.
- Updated CPU detection code for non-Windows builds and replaced Windows headers with portable stubs.
- Standardised Vegas WW list headers to snake case.
- LVGL's X11 backend is now optional. The cmake option `LVGL_USE_X11` defaults to OFF
  and `lv_conf.h` honours compile definitions so backends can be toggled without
  warnings.
- WW3D2 links against the `d3d8_gles` shim and `gameenginedevice` now links this
  library so DirectX 8 calls route through the OpenGL ES translation layer.
- Began implementing D3DX utility functions. D3DXComputeBoundingSphere and D3DXComputeNormals now have working shims.
- Implemented additional D3DX helpers used by WW3D2 including D3DXCreateTexture,
  D3DXCreateTextureFromFileExA, D3DXLoadSurfaceFromSurface and D3DXFilterTexture.
- Added D3DXVec3Transform and D3DXComputeBoundingBox implementations in the
  d3d8_gles shim.
- Implemented a minimal D3DXLoadMeshFromX loader capable of parsing text and
  binary .x meshes for positions and normals only.
- Removed unused Win32 headers from `lvgl_device/common` and deleted the old
  `Win32BIGFile*.cpp.disabled` sources. `LvglLocalFileSystem` now calls
  `std::filesystem` directly for directory creation and existence checks.
- CMake now exports SDL2 include directories for LVGL so SDL.h resolves on macOS.
- macOS builds now link the LVGL library against the SDL2 framework
  (`-framework SDL2`) when the SDL backend is enabled.
- Added BMP and PNG texture loading in the `d3d8_gles` layer using lodepng.
- lv_conf.h now defines LV_SDL_INCLUDE_PATH ensuring SDL2 headers are found. README explains installing SDL2 development packages.
- Implemented additional D3DX matrix helpers: `D3DXMatrixLookAtRH`,
  `D3DXMatrixPerspectiveFovRH`, `D3DXMatrixOrthoLH`, `D3DXMatrixOrthoRH` and
  `D3DXMatrixRotationQuaternion` with accompanying regression tests.
- Added `SetVertexShaderConstant` and `SetPixelShaderConstant` implementations
  in the Direct3D 8 shim with basic constant storage.
- A new LVGL display driver links the microGLES software renderer directly with
  LVGL so the engine can run without GPU acceleration. Build the project with
  `-DUSE_MICROGLES=ON` to enable this path:

  ```bash
  cmake -S . -B build -DUSE_MICROGLES=ON
  cmake --build build
  ```

  When enabled the d3d8_gles layer drops its EGL dependency and all rendering
  occurs through microGLES via the LVGL display API.
- Remaining directories converted to snake_case (e.g. src/tools/nvasm and src/game_engine_device/w3d_device/game_client/gui/gadget) to eliminate case sensitivity issues.
- SDL2 detection now falls back to pkg-config when the CMake configuration is missing, improving compatibility with minimal distributions.
- Added portable `strupr` and `strlwr` implementations in `windows.h` and renamed
  `INI.H` to `ini.h` to continue the snake_case cleanup.
