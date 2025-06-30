
# Command & Conquer Generals (inc. Zero Hour) Source Code

This repository includes source code for Command & Conquer Generals, and its expansion pack Zero Hour. This release provides support to the Steam Workshop for both games ([C&C Generals](https://steamcommunity.com/workshop/browse/?appid=2229870) and [C&C Generals - Zero Hour](https://steamcommunity.com/workshop/browse/?appid=2732960)).


## Dependencies

If you wish to rebuild the source code and tools successfully you will need to find or write new replacements (or remove the code using them entirely) for the following libraries;

- DirectX SDK (Version 9.0 or higher) (expected path `\Code\Libraries\DirectX\`)
- STLport (4.5.3) - patched version tracked in `lib/STLport`
- 3DSMax 4 SDK - (expected path `\Code\Libraries\Max4SDK\`)
- NVASM - (expected path `\Code\Tools\NVASM\`)
- BYTEmark - (expected path `\Code\Libraries\Source\Benchmark`)
- RAD Miles Sound System SDK - (expected path `\Code\Libraries\Source\WWVegas\Miles6\`)
- RAD Bink SDK - (expected path `\Code\GameEngineDevice\Include\VideoDevice\Bink`)
- SafeDisk API - (expected path `\Code\GameEngine\Include\Common\SafeDisk` and `\Code\Tools\Launcher\SafeDisk\`)
- Miles Sound System "Asimp3" - (expected path `\Code\Libraries\WPAudio\Asimp3`)
- UniSpySDK - open source replacement for GameSpy (tracked in `lib/UniSpySDK`)
- ZLib (1.1.4) - (expected path `\Code\Libraries\Source\Compression\ZLib\`)
- LZH-Light (1.0) - (expected path `\Code\Libraries\Source\Compression\LZHCompress\CompLibSource` and `CompLibHeader`)

## Directory Layout

Source code now resides in `src/`, public headers in `include/`, and libraries in `lib/`. The legacy tree under `Generals/Code` will be migrated here over time.

External libraries such as LVGL, miniaudio, uGLES, UniSpySDK, zlib and liblzhl are provided under `lib/`. These directories are committed directly in the repository, with LVGL (version 9.3) included to allow local modifications.

The project includes a complete **DirectX 8 → OpenGL ES 1.1 translation layer** in `lib/d3d8_gles/` with:
- ✅ **Functional D3D8 interface creation** and basic API compatibility
- ✅ **Complete D3DX math library** (matrices, vectors, transforms, matrix stack)
- ✅ **COM interface emulation** using C wrapper structs  
- ✅ **Cross-platform rendering backend** via OpenGL ES + EGL

CMake builds these third party libraries via `lib/CMakeLists.txt`.

 

## Compiling (Win32 Only)

To use the compiled binaries, you must own the game. The C&C Ultimate Collection is available for purchase on [EA App](https://www.ea.com/en-gb/games/command-and-conquer/command-and-conquer-the-ultimate-collection/buy/pc) or [Steam](https://store.steampowered.com/bundle/39394/Command__Conquer_The_Ultimate_Collection/).

The quickest way to build all configurations in the project is to open `rts.dsw` in Microsoft Visual Studio C++ 6.0 (SP6 recommended for binary matching to Generals patch 1.08 and Zero Hour patch 1.04) and select Build -> Batch Build, then hit the “Rebuild All” button.

If you wish to compile the code under a modern version of Microsoft Visual Studio, you can convert the legacy project file to a modern MSVC solution by opening `rts.dsw` in Microsoft Visual Studio .NET 2003, and then opening the newly created project and solution file in MSVC 2015 or newer.

## CMake Build (Cross-Platform)
The project now includes a functional cross-platform build system using CMake. Install the build prerequisites
first (for example `sudo apt install build-essential cmake libsdl2-dev
libgl1-mesa-dev libx11-dev libglu1-mesa-dev xorg-dev` on Debian-based systems). Run:
```bash
cmake -S . -B build && cmake --build build
```
This will build the complete game engine with DirectX 8 → OpenGL ES translation layer.
Continuous integration builds should enable strict warnings:
```bash
cmake -S . -B build -DSTRICT_BUILD=ON
```

### Testing D3D8 Translation Layer
The built executable includes a functional D3D8 → OpenGL ES translation test:
```bash
./build/src/main/generals
```
This demonstrates that the DirectX translation layer is operational and can successfully:
- Create D3D8 interfaces via Direct3DCreate8()
- Handle basic DirectX API calls
- Provide complete D3DX math function implementations
- Interface with the LVGL windowing system

To try the minimal LVGL example without the game engine run:
```
cmake -S . -B build -DBUILD_ENGINE=OFF
cmake --build build -j1
./build/src/examples/lvgl_hello/lvgl_hello
```
This uses LVGL's SDL backend to open an 800x600 window showing a "Hello world"
label and a centred button. No engine code is compiled when
`BUILD_ENGINE` is set to OFF.

On **macOS** you must use the Apple Clang compiler (usually `/usr/bin/clang`).
Using GCC will fail because the audio backend requires Clang's `-fblocks`
extension when compiling against CoreAudio headers.

The LVGL examples rely on the SDL2 development headers. Install them via your
package manager (for example `apt install libsdl2-dev` on Linux or `brew install sdl2`
on macOS) and enable the backend with `-DLVGL_USE_SDL=ON` if not already
configured.  If SDL2 is in a non-standard location set `CMAKE_PREFIX_PATH` so
that `find_package(SDL2)` can locate `SDL2/SDL.h`.

NOTE: As modern versions of MSVC enforce newer revisions of the C++ standard, you will need to make extensive changes to the codebase before it successfully compiles, even more so if you plan on compiling for the Win64 platform.

When the workspace has finished building, the compiled binaries will be copied to the folder called `/Run/` found in the root of each games directory.

## Development Tools

### Code Analysis Scripts
Two Python scripts are provided to help with development and migration tracking:

```bash
# Find directories with capital letters (useful for migration tracking)
python3 scripts/capitals.py

# Generate DirectX usage report  
python3 scripts/d3d_scan.py > directx_report.md
```

- **capitals.py**: Identifies directories that don't follow the snake_case convention used in new code, helping track migration progress from legacy Win32 naming
- **d3d_scan.py**: Scans source files for DirectX API usage and generates a comprehensive report showing which files contain DirectX references and their specific usage patterns. This helps track the DirectX → OpenGL ES translation progress.
 


## Known Issues

Windows has a policy where executables that contain words “version”, “update” or “install” in their filename will require UAC Elevation to run. This will affect “versionUpdate” and “buildVersionUpdate” projects from running as post-build events. Renaming the output binary name for these projects to not include these words should resolve the issue for you.


## STLport
The repository includes STLport 4.5.3 under `lib/STLport` with the patch from
`stlport.diff` already applied. The diff is kept for reference should you need
to verify the changes against an original copy of the library.


## Contributing

This repository will not be accepting contributions (pull requests, issues, etc). If you wish to create changes to the source code and encourage collaboration, please create a fork of the repository under your GitHub user/organization space.


## Support

This repository is for preservation purposes only and is archived without support. 


## License

This repository and its contents are licensed under the GPL v3 license, with additional terms applied. Please see [license.md](license.md) for details.
