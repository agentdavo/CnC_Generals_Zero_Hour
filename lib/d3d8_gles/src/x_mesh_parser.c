#include "d3d8_to_gles.h"
#include "x_mesh_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static void skip_sep(const char **p) {
    while (**p && (isspace((unsigned char)**p) || **p == ',' || **p == ';'))
        (*p)++;
}

static int parse_uint(const char **p, DWORD *out) {
    skip_sep(p);
    if (!**p) return 0;
    *out = (DWORD)strtoul(*p, (char **)p, 10);
    skip_sep(p);
    return 1;
}

static int parse_float(const char **p, float *out) {
    skip_sep(p);
    if (!**p) return 0;
    *out = strtof(*p, (char **)p);
    skip_sep(p);
    return 1;
}

HRESULT parse_text_mesh(const char *text, VertexPN **pp_vertices,
                             DWORD *p_vcount, WORD **pp_indices,
                             DWORD *p_fcount) {
    const char *p = strstr(text, "Mesh");
    if (!p) return D3DXERR_INVALIDDATA;
    p = strchr(p, '{');
    if (!p) return D3DXERR_INVALIDDATA;
    p++;

    DWORD vcount;
    if (!parse_uint(&p, &vcount) || vcount == 0) return D3DXERR_INVALIDDATA;
    VertexPN *verts = calloc(vcount, sizeof(VertexPN));
    if (!verts) return D3DERR_OUTOFVIDEOMEMORY;

    for (DWORD i = 0; i < vcount; i++) {
        if (!parse_float(&p, &verts[i].x) ||
            !parse_float(&p, &verts[i].y) ||
            !parse_float(&p, &verts[i].z)) {
            free(verts);
            return D3DXERR_INVALIDDATA;
        }
        if (i + 1 < vcount) {
            const char *c = strchr(p, ',');
            if (c) p = c + 1;
        }
    }
    const char *semi = strchr(p, ';');
    if (semi) p = semi + 1;
    skip_sep(&p);

    DWORD fcount;
    if (!parse_uint(&p, &fcount) || fcount == 0) { free(verts); return D3DXERR_INVALIDDATA; }
    WORD *indices = calloc(fcount * 3, sizeof(WORD));
    if (!indices) { free(verts); return D3DERR_OUTOFVIDEOMEMORY; }

    for (DWORD f = 0; f < fcount; f++) {
        DWORD n;
        if (!parse_uint(&p, &n) || n != 3) { free(verts); free(indices); return D3DXERR_INVALIDDATA; }
        DWORD i0,i1,i2;
        if (!parse_uint(&p,&i0) || !parse_uint(&p,&i1) || !parse_uint(&p,&i2)) {
            free(verts); free(indices); return D3DXERR_INVALIDDATA; }
        indices[f*3+0] = (WORD)i0;
        indices[f*3+1] = (WORD)i1;
        indices[f*3+2] = (WORD)i2;
        const char *s2 = strchr(p, ';');
        if (s2) p = s2 + 1;
        skip_sep(&p);
    }

    // Optional normals
    const char *norm = strstr(p, "MeshNormals");
    if (norm) {
        p = strchr(norm, '{');
        if (p) {
            p++;
            DWORD n_norm;
            if (parse_uint(&p, &n_norm)) {
                for (DWORD i = 0; i < n_norm && i < vcount; i++) {
                    parse_float(&p, &verts[i].nx);
                    parse_float(&p, &verts[i].ny);
                    parse_float(&p, &verts[i].nz);
                    const char *c = strchr(p, ';');
                    if (c) p = c + 1;
                }
            }
        }
    }

    *pp_vertices = verts;
    *p_vcount = vcount;
    *pp_indices = indices;
    *p_fcount = fcount;
    return D3D_OK;
}

static uint32_t read_u32(const BYTE **p) {
    uint32_t v;
    memcpy(&v, *p, 4);
    *p += 4;
    return v;
}

static float read_f32(const BYTE **p) {
    float v;
    memcpy(&v, *p, 4);
    *p += 4;
    return v;
}

HRESULT parse_binary_mesh(const BYTE *data, size_t size,
                                 VertexPN **pp_vertices, DWORD *p_vcount,
                                 WORD **pp_indices, DWORD *p_fcount) {
    const BYTE *p = data;
    if (size < 4) return D3DXERR_INVALIDDATA;
    DWORD vcount = read_u32(&p);
    if (vcount == 0 || size < 4 + vcount * 12) return D3DXERR_INVALIDDATA;
    VertexPN *verts = calloc(vcount, sizeof(VertexPN));
    if (!verts) return D3DERR_OUTOFVIDEOMEMORY;
    for (DWORD i = 0; i < vcount; i++) {
        verts[i].x = read_f32(&p);
        verts[i].y = read_f32(&p);
        verts[i].z = read_f32(&p);
    }
    if (p >= data + size) { free(verts); return D3DXERR_INVALIDDATA; }
    DWORD fcount = read_u32(&p);
    if (fcount == 0 || size < (p - data) + fcount * 4 * 4) {
        free(verts);
        return D3DXERR_INVALIDDATA;
    }
    WORD *indices = calloc(fcount * 3, sizeof(WORD));
    if (!indices) { free(verts); return D3DERR_OUTOFVIDEOMEMORY; }
    for (DWORD f = 0; f < fcount; f++) {
        DWORD n = read_u32(&p);
        if (n != 3) { free(verts); free(indices); return D3DXERR_INVALIDDATA; }
        indices[f*3+0] = (WORD)read_u32(&p);
        indices[f*3+1] = (WORD)read_u32(&p);
        indices[f*3+2] = (WORD)read_u32(&p);
    }
    if (p + 4 <= data + size) {
        DWORD n_norm = read_u32(&p);
        for (DWORD i = 0; i < n_norm && i < vcount; i++) {
            verts[i].nx = read_f32(&p);
            verts[i].ny = read_f32(&p);
            verts[i].nz = read_f32(&p);
        }
    }
    *pp_vertices = verts;
    *p_vcount = vcount;
    *pp_indices = indices;
    *p_fcount = fcount;
    return D3D_OK;
}


