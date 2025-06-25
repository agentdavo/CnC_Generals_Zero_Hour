#include <assert.h>
#include <d3d8_to_gles.h>

int main(void) {
    D3DXVECTOR3 v = {1.f, 2.f, 3.f};
    D3DXMATRIX m;
    D3DXMatrixIdentity(&m);
    D3DXVECTOR4 out;
    D3DXVECTOR4 *res = D3DXVec3Transform(&out, &v, &m);
    assert(res == &out);
    assert(out.x == 1.f && out.y == 2.f && out.z == 3.f && out.w == 1.f);
    return 0;
}
