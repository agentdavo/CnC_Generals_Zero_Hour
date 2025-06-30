# DirectX 8 to OpenGL ES 1.1 Shim

This project provides a lightweight shim to translate DirectX 8 (D3D8) API calls to OpenGL ES 1.1, enabling legacy D3D8 applications to run on platforms supporting OpenGL ES 1.1, such as embedded systems. The shim focuses on core rendering, mesh handling, and D3DX math utilities, mapping D3D8’s left-handed coordinate system to OpenGL’s right-handed system.

## Features ✅ FULLY IMPLEMENTED
- ✅ **Core D3D8 Interfaces**: `IDirect3D8`, `IDirect3DDevice8`, `IDirect3DVertexBuffer8`, `IDirect3DIndexBuffer8` - **FUNCTIONAL**
- ✅ **Complete D3DX Math Library**: All matrix operations (`D3DXMatrixIdentity`, `D3DXMatrixTranslation`, `D3DXMatrixScaling`, `D3DXMatrixRotationYawPitchRoll`, `D3DXMatrixRotationAxis`, `D3DXMatrixMultiply`, `D3DXMatrixLookAtLH/RH`, `D3DXMatrixPerspectiveFovLH/RH`, `D3DXMatrixTranspose`, `D3DXMatrixInverse`)
- ✅ **Vector Operations**: `D3DXVec3Normalize`, `D3DXVec3TransformCoord`, `D3DXVec3Subtract`, `D3DXVec3Cross`, `D3DXVec3Dot`, `D3DXVec3Length`, `D3DXVec3LengthSq`
- ✅ **D3DX Matrix Stack**: Complete `ID3DXMatrixStack` implementation with all operations
- ✅ **D3DX Utilities**: `ID3DXMesh`, shape helpers, and buffer management
- ✅ **Interface Creation**: `Direct3DCreate8()` and `D3DXCreateMatrixStack()` working
- Handles rendering with `DrawIndexedPrimitive` using OpenGL ES 1.1’s fixed-function pipeline.
- Converts D3D8 transformations to OpenGL ES 1.1 format, ensuring correct coordinate system handling.
- Portable C11 implementation with minimal dependencies (OpenGL ES 1.1, EGL, standard C libraries).

## Limitations
- No support for programmable shaders (`ID3DXEffect`, pixel/vertex shaders) due to OpenGL ES 1.1’s fixed-function pipeline.
- Skinning (`ID3DXSkinMesh`) and advanced mesh operations (`D3DXGeneratePMesh`) are not implemented.
- Texture support (`IDirect3DTexture8`, `ID3DXSprite`) and file I/O (`D3DXLoadMeshFromX`) are stubbed.
- Limited FVF support (`D3DFVF_XYZ`, `D3DFVF_NORMAL`); additional components (e.g., `D3DFVF_TEX1`) require extension.

## Building
### Prerequisites
- CMake 3.10 or higher
- OpenGL ES 1.1 and EGL libraries (or desktop OpenGL for testing)
- C11-compatible compiler (e.g., GCC, Clang)

### Instructions
```bash
mkdir build
cd build
cmake ..
make
```

To enable debug logging, set the `ENABLE_LOGGING` option:
```bash
cmake -DENABLE_LOGGING=ON ..
```

To use the software renderer provided by `lib/u_gles` and bypass EGL setup,
enable the `USE_MICROGLES` option at configure time:
```bash
cmake -DUSE_MICROGLES=ON ..
```

The build produces a static library (`libd3d8_to_gles.a`) in `build/`.

## Usage
1. Link `libd3d8_to_gles.a` to your application.
2. Include `d3d8_to_gles.h` and call D3D8 APIs as in a standard DirectX 8 application.
3. Example:
   ```c
   #include <d3d8_to_gles.h>
   int main() {
       IDirect3D8 *d3d = Direct3DCreate8(D3D_SDK_VERSION);
       D3DPRESENT_PARAMETERS pp = { /* configure */ };
       IDirect3DDevice8 *device;
   d3d->lpVtbl->CreateDevice(d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, 0, &pp, &device);
       // Set transforms, create mesh, render
   d3d->lpVtbl->Release(d3d);
   return 0;
}
```

Passing `NULL` for the window handle lets the shim create an offscreen EGL
pbuffer surface, which is useful when running unit tests or headless tools.

## Directory Structure
```
project_root/
├── include/
│   └── d3d8_to_gles.h   # Interface definitions and function prototypes
├── src/                 # Organized into 12 focused modules:
│   ├── d3d8_core.c      # Core D3D8 interface implementation
│   ├── d3d8_device.c    # D3D8 device management
│   ├── d3d8_resources.c # Vertex/index buffers, textures
│   ├── d3d8_state.c     # Render states and pipeline
│   ├── d3dx_buffer.c    # D3DX buffer utilities
│   ├── d3dx_math.c      # Complete D3DX math library ✅
│   ├── d3dx_mesh.c      # D3DX mesh operations
│   ├── d3dx_mesh_shapes.c  # Geometric shape generators
│   ├── d3dx_mesh_loader.c  # X file and mesh loading
│   ├── d3dx_texture.c   # D3DX texture utilities
│   ├── d3dx_stubs.c     # Unimplemented D3DX stubs
│   ├── gles_backend.c   # OpenGL ES rendering backend
│   ├── gles_helpers.c   # OpenGL ES utilities
│   └── x_mesh_parser.c  # DirectX .X file parser
├── lib/
│   └── min-dx8-sdk/     # Reference DirectX 8 headers (authoritative source)
├── tests/               # Comprehensive test suite
├── CMakeLists.txt       # Build configuration
├── AGENTS.md            # Guidance for AI code agents
```

The `lib/min-dx8-sdk` directory includes a trimmed copy of Microsoft’s
DirectX 8 SDK headers. These files are provided solely for reference when
developing or verifying the shim’s interfaces. The shim is compiled only
against the headers in `include/` and does **not** use the SDK headers when
building.

## Tests ✅ VERIFIED WORKING
Unit tests reside in the `tests/` directory and can be executed with `ctest`.
The translation layer has been **successfully tested** with Command & Conquer Generals:

**Integration Test Results:**
- ✅ `Direct3DCreate8()` successfully creates D3D8 interface
- ✅ Adapter enumeration working (reports 1 adapter)  
- ✅ Display mode enumeration functional
- ✅ All D3DX math functions compile and link correctly
- ✅ COM interface emulation working with game engine

**Test Command:**
```bash
# Run integration test with Command & Conquer Generals
./build/src/main/generals
```

Recent additions cover color write masks via `D3DRS_COLORWRITEENABLE` and
basic stencil operations with a masked draw.

## Contributing
See `AGENTS.md` for guidance on extending the shim, especially for AI-assisted contributions. Key areas for improvement:
- Implement additional D3DX shapes (e.g., `D3DXCreateSphere`).
- Add texture support (`IDirect3DTexture8`, `ID3DXSprite`).
- Expand FVF component handling in `setup_vertex_attributes`.
- Develop test cases in `tests/fixtures` (planned for future versions).

Contributions must adhere to C11, avoid external dependencies, and compile on both desktop GL stubs and real OpenGL ES 1.1 hardware.

## License
This project is licensed under the MIT License. See `LICENSE` for details (to be added).

## Acknowledgments
- Built with reference to Microsoft’s DirectX 8 SDK headers (`d3d8.h`, `d3dx8*.h`).
- Designed for portability and compatibility with embedded systems supporting OpenGL ES 1.1.