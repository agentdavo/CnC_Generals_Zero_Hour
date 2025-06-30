// d3dx_math.c - D3DX math functions not provided by min-dx8-sdk
#include "d3d8_to_gles.h"
#include "d3d8_internal.h"
#include <math.h>

// Matrix operations

// Initialize matrix to identity
D3DXMATRIX *WINAPI D3DXMatrixIdentity(D3DXMATRIX *pOut) {
    if (!pOut) return NULL;
    
    // Set all elements to 0 first
    memset(pOut, 0, sizeof(D3DXMATRIX));
    
    // Set diagonal elements to 1
    pOut->m[0][0] = 1.0f;
    pOut->m[1][1] = 1.0f;
    pOut->m[2][2] = 1.0f;
    pOut->m[3][3] = 1.0f;
    
    return pOut;
}
D3DXMATRIX *WINAPI D3DXMatrixMultiply(D3DXMATRIX *restrict pOut,
                                      CONST D3DXMATRIX *restrict pM1,
                                      CONST D3DXMATRIX *restrict pM2) {
  D3DXMATRIX *restrict result = &temp_matrix;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result->m[i][j] =
          pM1->m[i][0] * pM2->m[0][j] + pM1->m[i][1] * pM2->m[1][j] +
          pM1->m[i][2] * pM2->m[2][j] + pM1->m[i][3] * pM2->m[3][j];
    }
  }
  *pOut = *result;
  return pOut;
}

D3DXMATRIX *WINAPI D3DXMatrixLookAtLH(D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye,
                                      CONST D3DXVECTOR3 *pAt,
                                      CONST D3DXVECTOR3 *pUp) {
  D3DXVECTOR3 zaxis, xaxis, yaxis;
  D3DXVec3Subtract(&zaxis, pAt, pEye);
  D3DXVec3Normalize(&zaxis, &zaxis);
  D3DXVec3Cross(&xaxis, pUp, &zaxis);
  D3DXVec3Normalize(&xaxis, &xaxis);
  D3DXVec3Cross(&yaxis, &zaxis, &xaxis);

  D3DXMatrixIdentity(pOut);
  pOut->_11 = xaxis.x;
  pOut->_12 = yaxis.x;
  pOut->_13 = zaxis.x;
  pOut->_21 = xaxis.y;
  pOut->_22 = yaxis.y;
  pOut->_23 = zaxis.y;
  pOut->_31 = xaxis.z;
  pOut->_32 = yaxis.z;
  pOut->_33 = zaxis.z;
  pOut->_41 = -D3DXVec3Dot(&xaxis, pEye);
  pOut->_42 = -D3DXVec3Dot(&yaxis, pEye);
  pOut->_43 = -D3DXVec3Dot(&zaxis, pEye);
  return pOut;
}

D3DXMATRIX *WINAPI D3DXMatrixLookAtRH(D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye,
                                      CONST D3DXVECTOR3 *pAt,
                                      CONST D3DXVECTOR3 *pUp) {
  D3DXVECTOR3 zaxis, xaxis, yaxis;
  D3DXVec3Subtract(&zaxis, pEye, pAt);
  D3DXVec3Normalize(&zaxis, &zaxis);
  D3DXVec3Cross(&xaxis, pUp, &zaxis);
  D3DXVec3Normalize(&xaxis, &xaxis);
  D3DXVec3Cross(&yaxis, &zaxis, &xaxis);

  D3DXMatrixIdentity(pOut);
  pOut->_11 = xaxis.x;
  pOut->_12 = yaxis.x;
  pOut->_13 = zaxis.x;
  pOut->_21 = xaxis.y;
  pOut->_22 = yaxis.y;
  pOut->_23 = zaxis.y;
  pOut->_31 = xaxis.z;
  pOut->_32 = yaxis.z;
  pOut->_33 = zaxis.z;
  pOut->_41 = -D3DXVec3Dot(&xaxis, pEye);
  pOut->_42 = -D3DXVec3Dot(&yaxis, pEye);
  pOut->_43 = -D3DXVec3Dot(&zaxis, pEye);
  return pOut;
}

D3DXMATRIX *WINAPI D3DXMatrixPerspectiveFovLH(D3DXMATRIX *pOut, FLOAT fovy,
                                              FLOAT Aspect, FLOAT zn,
                                              FLOAT zf) {
  float yscale = 1.0f / tanf(fovy / 2.0f);
  float xscale = yscale / Aspect;
  D3DXMatrixIdentity(pOut);
  pOut->_11 = xscale;
  pOut->_22 = yscale;
  pOut->_33 = zf / (zf - zn);
  pOut->_34 = 1.0f;
  pOut->_43 = -zn * zf / (zf - zn);
  pOut->_44 = 0.0f;
  return pOut;
}

D3DXMATRIX *WINAPI D3DXMatrixPerspectiveFovRH(D3DXMATRIX *pOut, FLOAT fovy,
                                              FLOAT Aspect, FLOAT zn,
                                              FLOAT zf) {
  float yscale = 1.0f / tanf(fovy / 2.0f);
  float xscale = yscale / Aspect;
  D3DXMatrixIdentity(pOut);
  pOut->_11 = xscale;
  pOut->_22 = yscale;
  pOut->_33 = zn / (zn - zf);
  pOut->_34 = -1.0f;
  pOut->_43 = zn * zf / (zn - zf);
  pOut->_44 = 0.0f;
  return pOut;
}

D3DXMATRIX *WINAPI D3DXMatrixTranspose(D3DXMATRIX *pOut, CONST D3DXMATRIX *pM) {
  const D3DXMATRIX *src = pM;
  D3DXMATRIX temp;
  if (pOut == pM) {
    temp = *pM;
    src = &temp;
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      pOut->m[i][j] = src->m[j][i];
    }
  }
  return pOut;
}

D3DXMATRIX *WINAPI D3DXMatrixInverse(D3DXMATRIX *pOut, FLOAT *pDeterminant,
                                     CONST D3DXMATRIX *pM) {
  float m[16] = {pM->_11, pM->_12, pM->_13, pM->_14, pM->_21, pM->_22,
                 pM->_23, pM->_24, pM->_31, pM->_32, pM->_33, pM->_34,
                 pM->_41, pM->_42, pM->_43, pM->_44};
  float inv[16];

  inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] +
           m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
  inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] -
           m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
  inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] +
           m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
  inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] -
            m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
  inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] -
           m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
  inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] +
           m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
  inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] -
           m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
  inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] +
            m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
  inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] +
           m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
  inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] -
           m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
  inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] +
            m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
  inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] -
            m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
  inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] -
           m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
  inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] +
           m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
  inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
  inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

  float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
  if (pDeterminant)
    *pDeterminant = det;
  if (fabsf(det) < 1e-6f)
    return NULL;

  det = 1.0f / det;
  for (int i = 0; i < 16; i++)
    inv[i] *= det;

  pOut->_11 = inv[0];
  pOut->_12 = inv[1];
  pOut->_13 = inv[2];
  pOut->_14 = inv[3];
  pOut->_21 = inv[4];
  pOut->_22 = inv[5];
  pOut->_23 = inv[6];
  pOut->_24 = inv[7];
  pOut->_31 = inv[8];
  pOut->_32 = inv[9];
  pOut->_33 = inv[10];
  pOut->_34 = inv[11];
  pOut->_41 = inv[12];
  pOut->_42 = inv[13];
  pOut->_43 = inv[14];
  pOut->_44 = inv[15];

  return pOut;
}

// Create translation matrix
D3DXMATRIX *WINAPI D3DXMatrixTranslation(D3DXMATRIX *pOut, FLOAT x, FLOAT y, FLOAT z) {
  if (!pOut) return NULL;
  
  D3DXMatrixIdentity(pOut);
  pOut->_41 = x;
  pOut->_42 = y;
  pOut->_43 = z;
  
  return pOut;
}

// Create scaling matrix
D3DXMATRIX *WINAPI D3DXMatrixScaling(D3DXMATRIX *pOut, FLOAT sx, FLOAT sy, FLOAT sz) {
  if (!pOut) return NULL;
  
  D3DXMatrixIdentity(pOut);
  pOut->_11 = sx;
  pOut->_22 = sy;
  pOut->_33 = sz;
  
  return pOut;
}

// Create rotation matrix from yaw, pitch, roll
D3DXMATRIX *WINAPI D3DXMatrixRotationYawPitchRoll(D3DXMATRIX *pOut, FLOAT Yaw, FLOAT Pitch, FLOAT Roll) {
  if (!pOut) return NULL;
  
  float cy = cosf(Yaw);
  float sy = sinf(Yaw);
  float cp = cosf(Pitch);
  float sp = sinf(Pitch);
  float cr = cosf(Roll);
  float sr = sinf(Roll);
  
  D3DXMatrixIdentity(pOut);
  pOut->_11 = cy * cr + sy * sp * sr;
  pOut->_12 = sr * cp;
  pOut->_13 = -sy * cr + cy * sp * sr;
  pOut->_21 = -cy * sr + sy * sp * cr;
  pOut->_22 = cr * cp;
  pOut->_23 = sy * sr + cy * sp * cr;
  pOut->_31 = sy * cp;
  pOut->_32 = -sp;
  pOut->_33 = cy * cp;
  
  return pOut;
}

// Create rotation matrix around arbitrary axis
D3DXMATRIX *WINAPI D3DXMatrixRotationAxis(D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pV, FLOAT Angle) {
  if (!pOut || !pV) return NULL;
  
  D3DXVECTOR3 v;
  D3DXVec3Normalize(&v, pV);
  
  float c = cosf(Angle);
  float s = sinf(Angle);
  float t = 1.0f - c;
  
  D3DXMatrixIdentity(pOut);
  pOut->_11 = t * v.x * v.x + c;
  pOut->_12 = t * v.x * v.y + s * v.z;
  pOut->_13 = t * v.x * v.z - s * v.y;
  pOut->_21 = t * v.x * v.y - s * v.z;
  pOut->_22 = t * v.y * v.y + c;
  pOut->_23 = t * v.y * v.z + s * v.x;
  pOut->_31 = t * v.x * v.z + s * v.y;
  pOut->_32 = t * v.y * v.z - s * v.x;
  pOut->_33 = t * v.z * v.z + c;
  
  return pOut;
}

// Vector operations
D3DXVECTOR3 *WINAPI D3DXVec3Normalize(D3DXVECTOR3 *pOut,
                                      CONST D3DXVECTOR3 *pV) {
  float length = sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);
  if (length == 0.0f) {
    *pOut = *pV;
    return pOut;
  }
  pOut->x = pV->x / length;
  pOut->y = pV->y / length;
  pOut->z = pV->z / length;
  return pOut;
}

D3DXVECTOR3 *WINAPI D3DXVec3TransformCoord(D3DXVECTOR3 *restrict pOut,
                                           CONST D3DXVECTOR3 *restrict pV,
                                           CONST D3DXMATRIX *restrict pM) {
  D3DXVECTOR4 temp = {pV->x, pV->y, pV->z, 1.0f};
  pOut->x =
      temp.x * pM->_11 + temp.y * pM->_21 + temp.z * pM->_31 + temp.w * pM->_41;
  pOut->y =
      temp.x * pM->_12 + temp.y * pM->_22 + temp.z * pM->_32 + temp.w * pM->_42;
  pOut->z =
      temp.x * pM->_13 + temp.y * pM->_23 + temp.z * pM->_33 + temp.w * pM->_43;
  float w =
      temp.x * pM->_14 + temp.y * pM->_24 + temp.z * pM->_34 + temp.w * pM->_44;
  if (w != 0.0f) {
    pOut->x /= w;
    pOut->y /= w;
    pOut->z /= w;
  }
  return pOut;
}

// Vector subtraction
D3DXVECTOR3 *WINAPI D3DXVec3Subtract(D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2) {
  if (!pOut || !pV1 || !pV2) return NULL;
  
  pOut->x = pV1->x - pV2->x;
  pOut->y = pV1->y - pV2->y;
  pOut->z = pV1->z - pV2->z;
  
  return pOut;
}

// Vector cross product
D3DXVECTOR3 *WINAPI D3DXVec3Cross(D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2) {
  if (!pOut || !pV1 || !pV2) return NULL;
  
  D3DXVECTOR3 temp;
  temp.x = pV1->y * pV2->z - pV1->z * pV2->y;
  temp.y = pV1->z * pV2->x - pV1->x * pV2->z;
  temp.z = pV1->x * pV2->y - pV1->y * pV2->x;
  
  *pOut = temp;
  return pOut;
}

// Vector dot product
FLOAT WINAPI D3DXVec3Dot(CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2) {
  if (!pV1 || !pV2) return 0.0f;
  
  return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

// Vector length
FLOAT WINAPI D3DXVec3Length(CONST D3DXVECTOR3 *pV) {
  if (!pV) return 0.0f;
  
  return sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);
}

// Vector squared length
FLOAT WINAPI D3DXVec3LengthSq(CONST D3DXVECTOR3 *pV) {
  if (!pV) return 0.0f;
  
  return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z;
}

// ID3DXMatrixStack implementation - using MatrixStackImpl from header

static HRESULT D3DAPI d3dx_matrix_stack_query_interface(ID3DXMatrixStack *This,
                                                        REFIID riid,
                                                        LPVOID *ppvObj) {
  (void)This;
  (void)riid;
  (void)ppvObj;
  return D3DERR_INVALIDCALL;
}

static ULONG D3DAPI d3dx_matrix_stack_add_ref(ID3DXMatrixStack *This) {
  (void)This;
  return 1;
}

static ULONG D3DAPI d3dx_matrix_stack_release(ID3DXMatrixStack *This) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  free(impl->stack);
  free(impl);
  return 0;
}

static HRESULT D3DAPI d3dx_matrix_stack_pop(ID3DXMatrixStack *This) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return D3DERR_INVALIDCALL;
  impl->top--;
  return D3D_OK;
}

static HRESULT D3DAPI d3dx_matrix_stack_push(ID3DXMatrixStack *This) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top >= impl->capacity) {
    DWORD new_capacity = impl->capacity ? impl->capacity * 2 : 16;
    D3DXMATRIX *new_stack =
        realloc(impl->stack, new_capacity * sizeof(D3DXMATRIX));
    if (!new_stack)
      return D3DERR_OUTOFVIDEOMEMORY;
    impl->stack = new_stack;
    impl->capacity = new_capacity;
  }
  if (impl->top > 0)
    impl->stack[impl->top] = impl->stack[impl->top - 1];
  else
    D3DXMatrixIdentity(&impl->stack[impl->top]);
  impl->top++;
  return D3D_OK;
}

static HRESULT D3DAPI d3dx_matrix_stack_load_identity(ID3DXMatrixStack *This) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return D3DERR_INVALIDCALL;
  D3DXMatrixIdentity(&impl->stack[impl->top - 1]);
  return D3D_OK;
}

static HRESULT D3DAPI d3dx_matrix_stack_load_matrix(ID3DXMatrixStack *This,
                                                    CONST D3DXMATRIX *pM) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return D3DERR_INVALIDCALL;
  impl->stack[impl->top - 1] = *pM;
  return D3D_OK;
}

static HRESULT D3DAPI d3dx_matrix_stack_mult_matrix(ID3DXMatrixStack *This,
                                                    CONST D3DXMATRIX *pM) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return D3DERR_INVALIDCALL;
  D3DXMatrixMultiply(&impl->stack[impl->top - 1], &impl->stack[impl->top - 1],
                     pM);
  return D3D_OK;
}

static HRESULT D3DAPI d3dx_matrix_stack_mult_matrix_local(
    ID3DXMatrixStack *This, CONST D3DXMATRIX *pM) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return D3DERR_INVALIDCALL;
  D3DXMATRIX temp;
  D3DXMatrixMultiply(&temp, pM, &impl->stack[impl->top - 1]);
  impl->stack[impl->top - 1] = temp;
  return D3D_OK;
}

static HRESULT D3DAPI d3dx_matrix_stack_rotate_axis(ID3DXMatrixStack *This,
                                                    CONST D3DXVECTOR3 *pV,
                                                    FLOAT Angle) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return D3DERR_INVALIDCALL;
  D3DXMATRIX rot;
  D3DXMatrixRotationAxis(&rot, pV, Angle);
  return d3dx_matrix_stack_mult_matrix(This, &rot);
}

static HRESULT D3DAPI d3dx_matrix_stack_rotate_axis_local(
    ID3DXMatrixStack *This, CONST D3DXVECTOR3 *pV, FLOAT Angle) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return D3DERR_INVALIDCALL;
  D3DXMATRIX rot;
  D3DXMatrixRotationAxis(&rot, pV, Angle);
  return d3dx_matrix_stack_mult_matrix_local(This, &rot);
}

static HRESULT D3DAPI d3dx_matrix_stack_rotate_yaw_pitch_roll(
    ID3DXMatrixStack *This, FLOAT Yaw, FLOAT Pitch, FLOAT Roll) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return D3DERR_INVALIDCALL;
  D3DXMATRIX rot;
  D3DXMatrixRotationYawPitchRoll(&rot, Yaw, Pitch, Roll);
  return d3dx_matrix_stack_mult_matrix(This, &rot);
}

static HRESULT D3DAPI d3dx_matrix_stack_rotate_yaw_pitch_roll_local(
    ID3DXMatrixStack *This, FLOAT Yaw, FLOAT Pitch, FLOAT Roll) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return D3DERR_INVALIDCALL;
  D3DXMATRIX rot;
  D3DXMatrixRotationYawPitchRoll(&rot, Yaw, Pitch, Roll);
  return d3dx_matrix_stack_mult_matrix_local(This, &rot);
}

static HRESULT D3DAPI d3dx_matrix_stack_scale(ID3DXMatrixStack *This, FLOAT x,
                                              FLOAT y, FLOAT z) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return D3DERR_INVALIDCALL;
  D3DXMATRIX scale;
  D3DXMatrixScaling(&scale, x, y, z);
  return d3dx_matrix_stack_mult_matrix(This, &scale);
}

static HRESULT D3DAPI d3dx_matrix_stack_scale_local(ID3DXMatrixStack *This,
                                                    FLOAT x, FLOAT y, FLOAT z) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return D3DERR_INVALIDCALL;
  D3DXMATRIX scale;
  D3DXMatrixScaling(&scale, x, y, z);
  return d3dx_matrix_stack_mult_matrix_local(This, &scale);
}

static HRESULT D3DAPI d3dx_matrix_stack_translate(ID3DXMatrixStack *This,
                                                  FLOAT x, FLOAT y, FLOAT z) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return D3DERR_INVALIDCALL;
  D3DXMATRIX trans;
  D3DXMatrixTranslation(&trans, x, y, z);
  return d3dx_matrix_stack_mult_matrix(This, &trans);
}

static HRESULT D3DAPI d3dx_matrix_stack_translate_local(ID3DXMatrixStack *This,
                                                        FLOAT x, FLOAT y,
                                                        FLOAT z) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return D3DERR_INVALIDCALL;
  D3DXMATRIX trans;
  D3DXMatrixTranslation(&trans, x, y, z);
  return d3dx_matrix_stack_mult_matrix_local(This, &trans);
}

static D3DXMATRIX *D3DAPI d3dx_matrix_stack_get_top(ID3DXMatrixStack *This) {
  MatrixStackImpl *impl = (MatrixStackImpl*)This;
  if (impl->top == 0)
    return NULL;
  return &impl->stack[impl->top - 1];
}

// D3DXCreateMatrixStack implementation
HRESULT WINAPI D3DXCreateMatrixStack(DWORD Flags, LPD3DXMATRIXSTACK *ppStack) {
  (void)Flags;
  
  MatrixStackImpl *stack = calloc(1, sizeof(MatrixStackImpl));
  if (!stack)
    return D3DERR_OUTOFVIDEOMEMORY;

  static const ID3DXMatrixStackVtbl matrix_stack_vtbl = {
      D3D8_CAST(d3dx_matrix_stack_query_interface),
      D3D8_CAST(d3dx_matrix_stack_add_ref),
      D3D8_CAST(d3dx_matrix_stack_release),
      D3D8_CAST(d3dx_matrix_stack_pop),
      D3D8_CAST(d3dx_matrix_stack_push),
      D3D8_CAST(d3dx_matrix_stack_load_identity),
      D3D8_CAST(d3dx_matrix_stack_load_matrix),
      D3D8_CAST(d3dx_matrix_stack_mult_matrix),
      D3D8_CAST(d3dx_matrix_stack_mult_matrix_local),
      D3D8_CAST(d3dx_matrix_stack_rotate_axis),
      D3D8_CAST(d3dx_matrix_stack_rotate_axis_local),
      D3D8_CAST(d3dx_matrix_stack_rotate_yaw_pitch_roll),
      D3D8_CAST(d3dx_matrix_stack_rotate_yaw_pitch_roll_local),
      D3D8_CAST(d3dx_matrix_stack_scale),
      D3D8_CAST(d3dx_matrix_stack_scale_local),
      D3D8_CAST(d3dx_matrix_stack_translate),
      D3D8_CAST(d3dx_matrix_stack_translate_local),
      D3D8_CAST(d3dx_matrix_stack_get_top)
  };

  stack->lpVtbl = (void*)&matrix_stack_vtbl;
  stack->capacity = 0;
  stack->top = 0;
  stack->stack = NULL;

  // Initialize with one matrix
  if (FAILED(d3dx_matrix_stack_push((ID3DXMatrixStack*)stack))) {
    free(stack);
    return D3DERR_OUTOFVIDEOMEMORY;
  }

  *ppStack = (ID3DXMatrixStack*)stack;
  return D3D_OK;
}