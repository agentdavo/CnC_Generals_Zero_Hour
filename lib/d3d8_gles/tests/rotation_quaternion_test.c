#include <assert.h>
#include <math.h>
#include <d3d8_to_gles.h>

int main(void) {
    float angle = (float)(M_PI_2);
    D3DXMATRIX ry, rq;
    D3DXMatrixRotationY(&ry, angle);
    D3DXQUATERNION q = {0.f, sinf(angle*0.5f), 0.f, cosf(angle*0.5f)};
    D3DXMatrixRotationQuaternion(&rq, &q);
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            assert(fabsf(rq.m[r][c] - ry.m[r][c]) < 1e-5f);
    return 0;
}
