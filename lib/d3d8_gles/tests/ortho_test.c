#include <assert.h>
#include <d3d8_to_gles.h>
#include <math.h>

int main(void) {
    D3DXMATRIX ml, mr;
    D3DXMatrixOrthoLH(&ml, 2.f, 2.f, 0.f, 1.f);
    D3DXMatrixOrthoRH(&mr, 2.f, 2.f, 0.f, 1.f);
    assert(fabsf(ml._11 - 1.f) < 1e-6f);
    assert(fabsf(ml._33 - 1.f) < 1e-6f);
    assert(fabsf(mr._11 - 1.f) < 1e-6f);
    assert(fabsf(mr._33 + 1.f) < 1e-6f);
    return 0;
}
