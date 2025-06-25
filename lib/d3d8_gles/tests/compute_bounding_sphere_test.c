#include <assert.h>
#include <d3d8_to_gles.h>
#include <math.h>

int main(void) {
    struct { float x, y, z; } verts[8] = {
        {-1.f,-1.f,-1.f}, {1.f,-1.f,-1.f}, {1.f,1.f,-1.f}, {-1.f,1.f,-1.f},
        {-1.f,-1.f, 1.f}, {1.f,-1.f, 1.f}, {1.f,1.f, 1.f}, {-1.f,1.f, 1.f}
    };
    D3DXVECTOR3 center;
    float radius;
    HRESULT hr = D3DXComputeBoundingSphere(verts, 8, D3DFVF_XYZ, &center, &radius);
    assert(hr == D3D_OK);
    assert(fabsf(center.x) < 1e-6f);
    assert(fabsf(center.y) < 1e-6f);
    assert(fabsf(center.z) < 1e-6f);
    assert(fabsf(radius - sqrtf(3.0f)) < 1e-5f);
    return 0;
}
