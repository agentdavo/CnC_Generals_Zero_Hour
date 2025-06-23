#ifndef D3DX8MATH_H
#define D3DX8MATH_H

struct D3DXVECTOR4 {
    float x, y, z, w;
    D3DXVECTOR4() = default;
    D3DXVECTOR4(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w) {}
};

struct D3DXMATRIX {
    float m[4][4];
    D3DXMATRIX() = default;
    D3DXMATRIX(float m00, float m01, float m02, float m03,
               float m10, float m11, float m12, float m13,
               float m20, float m21, float m22, float m23,
               float m30, float m31, float m32, float m33) {
        m[0][0]=m00; m[0][1]=m01; m[0][2]=m02; m[0][3]=m03;
        m[1][0]=m10; m[1][1]=m11; m[1][2]=m12; m[1][3]=m13;
        m[2][0]=m20; m[2][1]=m21; m[2][2]=m22; m[2][3]=m23;
        m[3][0]=m30; m[3][1]=m31; m[3][2]=m32; m[3][3]=m33;
    }
};

inline void D3DXVec4Transform(D3DXVECTOR4* out, const D3DXVECTOR4* v, const D3DXMATRIX* m) {
    D3DXVECTOR4 r;
    r.x = v->x*m->m[0][0] + v->y*m->m[1][0] + v->z*m->m[2][0] + v->w*m->m[3][0];
    r.y = v->x*m->m[0][1] + v->y*m->m[1][1] + v->z*m->m[2][1] + v->w*m->m[3][1];
    r.z = v->x*m->m[0][2] + v->y*m->m[1][2] + v->z*m->m[2][2] + v->w*m->m[3][2];
    r.w = v->x*m->m[0][3] + v->y*m->m[1][3] + v->z*m->m[2][3] + v->w*m->m[3][3];
    *out = r;
}

inline float D3DXVec4Dot(const D3DXVECTOR4* a, const D3DXVECTOR4* b) {
    return a->x*b->x + a->y*b->y + a->z*b->z + a->w*b->w;
}

#endif // D3DX8MATH_H
