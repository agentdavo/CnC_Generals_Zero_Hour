#include <assert.h>
#include <d3d8_to_gles.h>

int main(void) {
    HRESULT hr = D3DXComputeNormals(NULL, NULL);
    assert(hr == D3DERR_INVALIDCALL);
    return 0;
}
