#ifndef D3DX_INLINE_FIX_H
#define D3DX_INLINE_FIX_H

// Workaround for min-dx8-sdk inline functions being multiply defined
// We'll compile with D3DX_MATH_INLINE_HEADER to prevent the definitions
#define D3DX_MATH_INLINE_HEADER

// For C compilation, ensure inline functions are properly marked
#ifdef __cplusplus
  #define D3DX_INLINE inline
#else
  #define D3DX_INLINE static inline
#endif

#endif // D3DX_INLINE_FIX_H