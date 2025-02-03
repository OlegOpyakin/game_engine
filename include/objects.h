#include <vector>
#pragma once

struct vec3d
{
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1; 
};


struct triangle
{
    vec3d points[3];
    float r, g, b;
};


struct mesh
{
    std::vector<triangle> tris;
};


struct matrix4x4
{
    float elem[4][4] = { 0 };
};


vec3d MatrixMultiplyVector(matrix4x4 &m, vec3d &i);

matrix4x4 Matrix_MakeIdentity();

matrix4x4 Matrix_MakeRotationX(float fAngleRad);

matrix4x4 Matrix_MakeRotationY(float fAngleRad);

matrix4x4 Matrix_MakeRotationZ(float fAngleRad);

matrix4x4 Matrix_MakeTranslation(float x, float y, float z);

matrix4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);

matrix4x4 Matrix_MultiplyMatrix(matrix4x4 &m1, matrix4x4 &m2);

matrix4x4 Matrix_PointAt(vec3d &pos, vec3d &target, vec3d &up);

matrix4x4 Matrix_QuickInverse(matrix4x4 &m);

vec3d VectorAdd(vec3d &v1, vec3d &v2);

vec3d VectorSub(vec3d &v1, vec3d &v2);

vec3d VectorMul(vec3d &v1, float m);

vec3d VectorDiv(vec3d &v1, float m);

float DotProduct(vec3d &v1, vec3d &v2);

float VectorLength(vec3d &v1);

vec3d VectorNormalise(vec3d &v1);

vec3d Vector_CrossProduct(vec3d &v1, vec3d &v2);

vec3d Vector_IntersectPlane(vec3d &plane_p, vec3d &plane_n, vec3d &lineStart, vec3d &lineEnd);

int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2);