#ifndef X_MESH_PARSER_H
#define X_MESH_PARSER_H
#include "d3d8_to_gles.h"
#include <stddef.h>

HRESULT parse_text_mesh(const char *text, VertexPN **pp_vertices,
                        DWORD *p_vcount, WORD **pp_indices, DWORD *p_fcount);
HRESULT parse_binary_mesh(const BYTE *data, size_t size, VertexPN **pp_vertices,
                          DWORD *p_vcount, WORD **pp_indices, DWORD *p_fcount);

#endif
