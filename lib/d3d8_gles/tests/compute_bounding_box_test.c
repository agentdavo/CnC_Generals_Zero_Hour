#include <assert.h>
#include <d3d8_to_gles.h>

int main(void) {
    struct { float x, y, z; } verts[8] = {
        {-1.f,-1.f,-1.f}, {1.f,-1.f,-1.f}, {1.f,1.f,-1.f}, {-1.f,1.f,-1.f},
        {-1.f,-1.f, 1.f}, {1.f,-1.f, 1.f}, {1.f,1.f, 1.f}, {-1.f,1.f, 1.f}
    };
    D3DXVECTOR3 min, max;
    HRESULT hr = D3DXComputeBoundingBox(verts, 8, D3DFVF_XYZ, &min, &max);
    assert(hr == D3D_OK);
    assert(min.x == -1.f && min.y == -1.f && min.z == -1.f);
    assert(max.x == 1.f && max.y == 1.f && max.z == 1.f);
    return 0;
}
