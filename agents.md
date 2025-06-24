# Repository Guidelines

This project contains the preserved source code for **Command & Conquer Generals** and **Zero Hour**. Development focuses on modernising the engine while retaining compatibility with the original assets. All contributions should follow the guidelines below.

## Coding

- Use `-std=gnu++11` and write portable C++.
- Target cross platform builds. New code must avoid Win32 specific APIs and instead rely on portable wrappers.
- The repository is being refactored around a `src/`, `include/` and `lib/` layout.
- The long term goal is to use **LVGL** for the window/canvas layer with automatic fallback to X11 where required.
- Rendering should use **OpenGL ES 1.1**. A translation layer from the original DirectX interfaces is expected.
- Audio functionality must use **miniaudio**.
- External libraries are provided as regular directories under `lib/`. A
  `lib/CMakeLists.txt` exposes these libraries as interface targets.

## Documentation

 - Update `migration.md` with any significant porting progress.
- Keep the root `README.md` intact except for clarifications about the new build system when available.

## Pull Requests

 - Each pull request should include a summary of the changes and reference any relevant sections of `migration.md`.
- Build the project with CMake before submitting:
  `cmake -S . -B build && cmake --build build`.
- Save the full build output to `log/build.log` for troubleshooting. Example:
  `cmake -S . -B build > log/build.log 2>&1 && cmake --build build >> log/build.log 2>&1`.

The CMake setup includes stub modules mirroring the original Visual Studio projects.
`src/Main/WinMain.cpp` is the legacy entry point and is not yet part of
the default build.

## Migration Plan

See [migration.md](migration.md) for the current strategy to move the codebase from its legacy Win32/DirectX roots to a portable C++11 engine using LVGL, OpenGL ES 1.1 and miniaudio.
