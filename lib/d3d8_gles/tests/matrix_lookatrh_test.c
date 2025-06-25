#include <assert.h>
#include <math.h>
#include <d3d8_to_gles.h>

int main(void) {
    D3DXVECTOR3 eye = {0.f, 0.f, -5.f};
    D3DXVECTOR3 at  = {0.f, 0.f, 0.f};
    D3DXVECTOR3 up  = {0.f, 1.f, 0.f};
    D3DXMATRIX m;
    D3DXMatrixLookAtRH(&m, &eye, &at, &up);
    assert(fabsf(m._11 + 1.f) < 1e-6f);
    assert(fabsf(m._22 - 1.f) < 1e-6f);
    assert(fabsf(m._33 + 1.f) < 1e-6f);
    assert(fabsf(m._43 - 5.f) < 1e-6f);
    return 0;
}
