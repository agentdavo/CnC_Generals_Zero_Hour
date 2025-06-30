// gles_helpers.c - GL state mapping and FVF setup helpers
#include "d3d8_to_gles.h"
#include "d3d8_internal.h"
#include <string.h>

// Common IUnknown implementation for all interfaces
HRESULT common_query_interface(void *This, REFIID riid, void **ppvObject) {
    if (!ppvObject) return 0x80070057; // E_INVALIDARG
    
    // Compare GUIDs - we only support IUnknown for now
    static const GUID IID_IUnknown_local = {0x00000000, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
    
    if (memcmp(riid, &IID_IUnknown_local, sizeof(GUID)) == 0) {
        *ppvObject = This;
        // AddRef would be called here in a real implementation
        return S_OK;
    }
    
    *ppvObject = NULL;
    return E_NOINTERFACE;
}

// Convert DWORD color/float value to float
float dword_to_float(DWORD v) {
  union {
    DWORD d;
    float f;
  } u;
  u.d = v;
  return u.f;
}

// Map D3D blend mode to OpenGL
GLenum blend_to_gl(D3DBLEND blend) {
  switch (blend) {
  case D3DBLEND_ZERO:
    return GL_ZERO;
  case D3DBLEND_ONE:
    return GL_ONE;
  case D3DBLEND_SRCCOLOR:
    return GL_SRC_COLOR;
  case D3DBLEND_INVSRCCOLOR:
    return GL_ONE_MINUS_SRC_COLOR;
  case D3DBLEND_SRCALPHA:
    return GL_SRC_ALPHA;
  case D3DBLEND_INVSRCALPHA:
    return GL_ONE_MINUS_SRC_ALPHA;
  case D3DBLEND_DESTALPHA:
    return GL_DST_ALPHA;
  case D3DBLEND_INVDESTALPHA:
    return GL_ONE_MINUS_DST_ALPHA;
  case D3DBLEND_DESTCOLOR:
    return GL_DST_COLOR;
  case D3DBLEND_INVDESTCOLOR:
    return GL_ONE_MINUS_DST_COLOR;
  case D3DBLEND_SRCALPHASAT:
    return GL_SRC_ALPHA_SATURATE;
  default:
    return GL_ONE;
  }
}

// Map D3D comparison function to OpenGL
GLenum cmp_to_gl(D3DCMPFUNC func) {
  switch (func) {
  case D3DCMP_NEVER:
    return GL_NEVER;
  case D3DCMP_LESS:
    return GL_LESS;
  case D3DCMP_EQUAL:
    return GL_EQUAL;
  case D3DCMP_LESSEQUAL:
    return GL_LEQUAL;
  case D3DCMP_GREATER:
    return GL_GREATER;
  case D3DCMP_NOTEQUAL:
    return GL_NOTEQUAL;
  case D3DCMP_GREATEREQUAL:
    return GL_GEQUAL;
  case D3DCMP_ALWAYS:
    return GL_ALWAYS;
  default:
    return GL_ALWAYS;
  }
}

// Map D3D stencil operation to OpenGL
GLenum stencil_op_to_gl(D3DSTENCILOP op) {
  switch (op) {
  case D3DSTENCILOP_KEEP:
    return GL_KEEP;
  case D3DSTENCILOP_ZERO:
    return GL_ZERO;
  case D3DSTENCILOP_REPLACE:
    return GL_REPLACE;
  case D3DSTENCILOP_INCRSAT:
    return GL_INCR;
  case D3DSTENCILOP_DECRSAT:
    return GL_DECR;
  case D3DSTENCILOP_INVERT:
    return GL_INVERT;
  case D3DSTENCILOP_INCR:
    return GL_INCR;
  case D3DSTENCILOP_DECR:
    return GL_DECR;
  default:
    return GL_KEEP;
  }
}

// Map D3D fog mode to OpenGL
GLenum fog_mode_to_gl(D3DFOGMODE mode) {
  switch (mode) {
  case D3DFOG_EXP:
    return GL_EXP;
  case D3DFOG_EXP2:
    return GL_EXP2;
  case D3DFOG_LINEAR:
    return GL_LINEAR;
  default:
    return GL_EXP;
  }
}

// Helper: Map Direct3D render state to OpenGL ES
void set_render_state(GLES_Device *gles, D3DRENDERSTATETYPE state,
                      DWORD value) {
  switch (state) {
  case D3DRS_ZENABLE:
    gles->depth_test = value;
    if (value)
      glEnable(GL_DEPTH_TEST);
    else
      glDisable(GL_DEPTH_TEST);
    break;
  case D3DRS_ZWRITEENABLE:
    glDepthMask(value ? GL_TRUE : GL_FALSE);
    break;
  case D3DRS_ALPHATESTENABLE:
    if (value)
      glEnable(GL_ALPHA_TEST);
    else
      glDisable(GL_ALPHA_TEST);
    break;
  case D3DRS_SRCBLEND:
    gles->src_blend = blend_to_gl((D3DBLEND)value);
    glBlendFunc(gles->src_blend, gles->dest_blend);
    break;
  case D3DRS_DESTBLEND:
    gles->dest_blend = blend_to_gl((D3DBLEND)value);
    glBlendFunc(gles->src_blend, gles->dest_blend);
    break;
  case D3DRS_CULLMODE:
    gles->cull_face = (value != D3DCULL_NONE);
    if (gles->cull_face) {
      glEnable(GL_CULL_FACE);
      gles->cull_mode = (value == D3DCULL_CW) ? GL_BACK : GL_FRONT;
      glCullFace(gles->cull_mode);
    } else {
      glDisable(GL_CULL_FACE);
    }
    break;
  case D3DRS_ZFUNC:
    gles->depth_func = cmp_to_gl((D3DCMPFUNC)value);
    glDepthFunc(gles->depth_func);
    break;
  case D3DRS_ALPHAREF:
    gles->alpha_ref = dword_to_float(value);
    glAlphaFunc(gles->alpha_func, gles->alpha_ref);
    break;
  case D3DRS_ALPHAFUNC:
    gles->alpha_func = cmp_to_gl((D3DCMPFUNC)value);
    glAlphaFunc(gles->alpha_func, gles->alpha_ref);
    break;
  case D3DRS_DITHERENABLE:
    if (value)
      glEnable(GL_DITHER);
    else
      glDisable(GL_DITHER);
    break;
  case D3DRS_ALPHABLENDENABLE:
    gles->blend = value;
    if (value)
      glEnable(GL_BLEND);
    else
      glDisable(GL_BLEND);
    break;
  case D3DRS_FOGENABLE:
    if (value)
      glEnable(GL_FOG);
    else
      glDisable(GL_FOG);
    break;
  case D3DRS_FOGCOLOR: {
    GLfloat color[4] = {
        (value & 0xFF) / 255.0f,
        ((value >> 8) & 0xFF) / 255.0f,
        ((value >> 16) & 0xFF) / 255.0f,
        ((value >> 24) & 0xFF) / 255.0f,
    };
    glFogfv(GL_FOG_COLOR, color);
    break;
  }
  case D3DRS_FOGTABLEMODE:
    gles->fog_mode = fog_mode_to_gl((D3DFOGMODE)value);
    glFogf(GL_FOG_MODE, (GLfloat)gles->fog_mode);
    break;
  case D3DRS_FOGSTART:
    glFogf(GL_FOG_START, dword_to_float(value));
    break;
  case D3DRS_FOGEND:
    glFogf(GL_FOG_END, dword_to_float(value));
    break;
  case D3DRS_FOGDENSITY:
    glFogf(GL_FOG_DENSITY, dword_to_float(value));
    break;
  case D3DRS_STENCILENABLE:
    gles->stencil_test = value;
    if (value)
      glEnable(GL_STENCIL_TEST);
    else
      glDisable(GL_STENCIL_TEST);
    break;
  case D3DRS_STENCILFUNC:
    gles->stencil_func = cmp_to_gl((D3DCMPFUNC)value);
    glStencilFunc(gles->stencil_func, gles->stencil_ref, gles->stencil_mask);
    break;
  case D3DRS_STENCILREF:
    gles->stencil_ref = (GLint)value;
    glStencilFunc(gles->stencil_func, gles->stencil_ref, gles->stencil_mask);
    break;
  case D3DRS_STENCILMASK:
    gles->stencil_mask = value;
    glStencilFunc(gles->stencil_func, gles->stencil_ref, gles->stencil_mask);
    break;
  case D3DRS_STENCILWRITEMASK:
    glStencilMask(value);
    break;
  case D3DRS_STENCILFAIL:
    gles->stencil_fail = stencil_op_to_gl((D3DSTENCILOP)value);
    glStencilOp(gles->stencil_fail, gles->stencil_zfail, gles->stencil_pass);
    break;
  case D3DRS_STENCILZFAIL:
    gles->stencil_zfail = stencil_op_to_gl((D3DSTENCILOP)value);
    glStencilOp(gles->stencil_fail, gles->stencil_zfail, gles->stencil_pass);
    break;
  case D3DRS_STENCILPASS:
    gles->stencil_pass = stencil_op_to_gl((D3DSTENCILOP)value);
    glStencilOp(gles->stencil_fail, gles->stencil_zfail, gles->stencil_pass);
    break;
  case D3DRS_COLORWRITEENABLE: {
    GLboolean r = (value & D3DCOLORWRITEENABLE_RED) ? GL_TRUE : GL_FALSE;
    GLboolean g = (value & D3DCOLORWRITEENABLE_GREEN) ? GL_TRUE : GL_FALSE;
    GLboolean b = (value & D3DCOLORWRITEENABLE_BLUE) ? GL_TRUE : GL_FALSE;
    GLboolean a = (value & D3DCOLORWRITEENABLE_ALPHA) ? GL_TRUE : GL_FALSE;
    glColorMask(r, g, b, a);
    break;
  }
  case D3DRS_LIGHTING:
    if (value)
      glEnable(GL_LIGHTING);
    else
      glDisable(GL_LIGHTING);
    break;
  case D3DRS_AMBIENT:
    gles->ambient[0] = (value & 0xFF) / 255.0f;
    gles->ambient[1] = ((value >> 8) & 0xFF) / 255.0f;
    gles->ambient[2] = ((value >> 16) & 0xFF) / 255.0f;
    gles->ambient[3] = ((value >> 24) & 0xFF) / 255.0f;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gles->ambient);
    break;
  default:
    d3d8_gles_log("Unsupported render state: %d\n", state);
  }
}

// Helper: Map D3DPRESENT_PARAMETERS to EGL config
EGLConfig choose_egl_config(EGLDisplay display,
                           D3DPRESENT_PARAMETERS *params,
                           BOOL want_window) {
  EGLint config_attributes[] = {
      EGL_RED_SIZE,
      (params->BackBufferFormat == D3DFMT_A8R8G8B8 ||
       params->BackBufferFormat == D3DFMT_X8R8G8B8)
          ? 8
          : 5,
      EGL_GREEN_SIZE,
      (params->BackBufferFormat == D3DFMT_A8R8G8B8 ||
       params->BackBufferFormat == D3DFMT_X8R8G8B8)
          ? 8
          : 6,
      EGL_BLUE_SIZE,
      (params->BackBufferFormat == D3DFMT_A8R8G8B8 ||
       params->BackBufferFormat == D3DFMT_X8R8G8B8)
          ? 8
          : 5,
      EGL_ALPHA_SIZE,
      (params->BackBufferFormat == D3DFMT_A8R8G8B8) ? 8 : 0,
      EGL_DEPTH_SIZE,
      params->EnableAutoDepthStencil ? 16 : 0,
      EGL_STENCIL_SIZE,
      params->EnableAutoDepthStencil ? 8 : 0,
      EGL_SURFACE_TYPE,
      want_window ? EGL_WINDOW_BIT : EGL_PBUFFER_BIT,
      EGL_RENDERABLE_TYPE,
      EGL_OPENGL_ES_BIT,
      EGL_SAMPLES,
      params->MultiSampleType >= D3DMULTISAMPLE_2_SAMPLES
          ? params->MultiSampleType
          : 0,
      EGL_NONE};

  EGLConfig config;
  EGLint num_configs;
  if (!eglChooseConfig(display, config_attributes, &config, 1, &num_configs) ||
      num_configs == 0) {
    d3d8_gles_log("Failed to choose EGL config\n");
    return NULL;
  }
  return config;
}

// Helper: Convert Direct3D matrix to OpenGL (left-handed to right-handed,
// z=[0,1] to [-1,1])
void d3d_to_gl_matrix(GLfloat *restrict gl_matrix,
                     const D3DXMATRIX *restrict d3d_matrix) {
  // Transpose and adjust for coordinate system differences
  gl_matrix[0] = d3d_matrix->_11;
  gl_matrix[4] = d3d_matrix->_21;
  gl_matrix[8] = d3d_matrix->_31;
  gl_matrix[12] = d3d_matrix->_41;
  gl_matrix[1] = d3d_matrix->_12;
  gl_matrix[5] = d3d_matrix->_22;
  gl_matrix[9] = d3d_matrix->_32;
  gl_matrix[13] = d3d_matrix->_42;
  gl_matrix[2] = -d3d_matrix->_13;
  gl_matrix[6] = -d3d_matrix->_23;
  gl_matrix[10] = -d3d_matrix->_33;
  gl_matrix[14] = -d3d_matrix->_43;
  gl_matrix[3] = d3d_matrix->_14;
  gl_matrix[7] = d3d_matrix->_24;
  gl_matrix[11] = d3d_matrix->_34;
  gl_matrix[15] = d3d_matrix->_44;
}

// Helper: Setup vertex attributes based on FVF
void setup_vertex_attributes(GLES_Device *gles, DWORD fvf, BYTE *data,
                            UINT stride) {
  GLint offset = 0;

  DWORD pos = fvf & D3DFVF_POSITION_MASK;
  int weight_count = 0;

  if (pos == D3DFVF_XYZRHW) {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4, GL_FLOAT, stride, data + offset);
    offset += 16;
  } else {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, stride, data + offset);
    offset += 12;

    switch (pos) {
    case D3DFVF_XYZB1:
      weight_count = 1;
      break;
    case D3DFVF_XYZB2:
      weight_count = 2;
      break;
    case D3DFVF_XYZB3:
      weight_count = 3;
      break;
    case D3DFVF_XYZB4:
      weight_count = 4;
      break;
    case D3DFVF_XYZB5:
      weight_count = 5;
      break;
    default:
      break;
    }
  }

  if (weight_count > 0)
    offset += weight_count * 4;

  if (fvf & D3DFVF_PSIZE)
    offset += 4;

  if (fvf & D3DFVF_NORMAL) {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, stride, data + offset);
    offset += 12;
  } else {
    glDisableClientState(GL_NORMAL_ARRAY);
  }

  if (fvf & D3DFVF_DIFFUSE) {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, stride, data + offset);
    offset += 4;
  } else if (fvf & D3DFVF_SPECULAR) {
    /* Use specular color when diffuse color is absent */
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, stride, data + offset);
    offset += 4;
  } else {
    glDisableClientState(GL_COLOR_ARRAY);
  }

  if (fvf & D3DFVF_SPECULAR && (fvf & D3DFVF_DIFFUSE)) {
    /* Skip specular bytes when diffuse was used */
    offset += 4;
  }

  int tex_count = (fvf & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT;
  int limit = tex_count > 2 ? 2 : tex_count;
  /*
   * gles->texcoord_index0 selects which vertex texcoord set feeds
   * texture stage 0. When it equals 1 with TEX2 data present, the
   * second coordinate array is bound to GL_TEXTURE0 and the first
   * array becomes GL_TEXTURE1. This mirrors the D3DTSS_TEXCOORDINDEX
   * behaviour for two stages.
   */
  for (int i = 0; i < tex_count; i++) {
    if (i < limit) {
      int unit = i;
      if (gles->texcoord_index0 < limit) {
        if (i == gles->texcoord_index0) {
          unit = 0;
        } else if (i < gles->texcoord_index0) {
          unit = i + 1;
        }
      }
      glClientActiveTexture(GL_TEXTURE0 + unit);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, stride, data + offset);
    }
    offset += 8;
  }
  for (int i = limit; i < 2; i++) {
    glClientActiveTexture(GL_TEXTURE0 + i);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  glClientActiveTexture(GL_TEXTURE0);
}