// d3d8_math_impl.h - Single location for D3DX math function implementations
#ifndef D3D8_MATH_IMPL_H
#define D3D8_MATH_IMPL_H

// Only include the actual math implementations in ONE source file
// All other files should only get declarations

#ifdef D3D8_MATH_IMPLEMENTATION

// Include the inline file to get implementations
#include "../lib/min-dx8-sdk/d3dx8math.inl"

#endif // D3D8_MATH_IMPLEMENTATION

#endif // D3D8_MATH_IMPL_H