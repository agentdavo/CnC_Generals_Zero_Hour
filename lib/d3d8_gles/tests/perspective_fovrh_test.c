#include <assert.h>
#include <math.h>
#include <d3d8_to_gles.h>

int main(void) {
    D3DXMATRIX m;
    D3DXMatrixPerspectiveFovRH(&m, (FLOAT)(M_PI_2), 1.f, 1.f, 10.f);
    assert(fabsf(m._11 - 1.f) < 1e-6f);
    assert(fabsf(m._22 - 1.f) < 1e-6f);
    assert(fabsf(m._33 - (1.f/(1.f - 10.f))) < 1e-6f);
    assert(fabsf(m._34 + 1.f) < 1e-6f);
    assert(fabsf(m._43 - (1.f*10.f/(1.f - 10.f))) < 1e-6f);
    return 0;
}
