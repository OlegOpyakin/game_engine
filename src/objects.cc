#include "objects.h"

vec3d MatrixMultiplyVector(matrix4x4 &m, vec3d &i){
	vec3d v;
	v.x = i.x * m.elem[0][0] + i.y * m.elem[1][0] + i.z * m.elem[2][0] + i.w * m.elem[3][0];
	v.y = i.x * m.elem[0][1] + i.y * m.elem[1][1] + i.z * m.elem[2][1] + i.w * m.elem[3][1];
	v.z = i.x * m.elem[0][2] + i.y * m.elem[1][2] + i.z * m.elem[2][2] + i.w * m.elem[3][2];
	v.w = i.x * m.elem[0][3] + i.y * m.elem[1][3] + i.z * m.elem[2][3] + i.w * m.elem[3][3];
	return v;
}

matrix4x4 Matrix_MakeIdentity(){
	matrix4x4 matrix;
	matrix.elem[0][0] = 1.0f;
	matrix.elem[1][1] = 1.0f;
	matrix.elem[2][2] = 1.0f;
	matrix.elem[3][3] = 1.0f;
	return matrix;
}

matrix4x4 Matrix_MakeRotationX(float fAngleRad){
	matrix4x4 matrix;
	matrix.elem[0][0] = 1.0f;
	matrix.elem[1][1] = cosf(fAngleRad);
	matrix.elem[1][2] = sinf(fAngleRad);
	matrix.elem[2][1] = -sinf(fAngleRad);
	matrix.elem[2][2] = cosf(fAngleRad);
	matrix.elem[3][3] = 1.0f;
	return matrix;
}

matrix4x4 Matrix_MakeRotationY(float fAngleRad){
	matrix4x4 matrix;
	matrix.elem[0][0] = cosf(fAngleRad);
	matrix.elem[0][2] = sinf(fAngleRad);
	matrix.elem[2][0] = -sinf(fAngleRad);
	matrix.elem[1][1] = 1.0f;
	matrix.elem[2][2] = cosf(fAngleRad);
	matrix.elem[3][3] = 1.0f;
	return matrix;
}

matrix4x4 Matrix_MakeRotationZ(float fAngleRad){
	matrix4x4 matrix;
	matrix.elem[0][0] = cosf(fAngleRad);
	matrix.elem[0][1] = sinf(fAngleRad);
	matrix.elem[1][0] = -sinf(fAngleRad);
	matrix.elem[1][1] = cosf(fAngleRad);
	matrix.elem[2][2] = 1.0f;
	matrix.elem[3][3] = 1.0f;
	return matrix;
}

matrix4x4 Matrix_MakeTranslation(float x, float y, float z){
	matrix4x4 matrix;
	matrix.elem[0][0] = 1.0f;
	matrix.elem[1][1] = 1.0f;
	matrix.elem[2][2] = 1.0f;
	matrix.elem[3][3] = 1.0f;
	matrix.elem[3][0] = x;
	matrix.elem[3][1] = y;
	matrix.elem[3][2] = z;
	return matrix;
}

matrix4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar){
	float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
	matrix4x4 matrix;
	matrix.elem[0][0] = fAspectRatio * fFovRad;
	matrix.elem[1][1] = fFovRad;
	matrix.elem[2][2] = fFar / (fFar - fNear);
	matrix.elem[3][2] = (-fFar * fNear) / (fFar - fNear);
	matrix.elem[2][3] = 1.0f;
	matrix.elem[3][3] = 0.0f;
	return matrix;
}

matrix4x4 Matrix_MultiplyMatrix(matrix4x4 &m1, matrix4x4 &m2){
	matrix4x4 matrix;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.elem[r][c] = m1.elem[r][0] * m2.elem[0][c] + m1.elem[r][1] * m2.elem[1][c] + m1.elem[r][2] * m2.elem[2][c] + m1.elem[r][3] * m2.elem[3][c];
	return matrix;
}

matrix4x4 Matrix_PointAt(vec3d &pos, vec3d &target, vec3d &up){
	vec3d new_forward = VectorSub(target, pos);
	new_forward = VectorNormalise(new_forward);

	vec3d a = VectorMul(new_forward, DotProduct(up, new_forward));
	vec3d new_up = VectorSub(up, a);
	new_up = VectorNormalise(new_up);

	vec3d new_right = Vector_CrossProduct(new_up, new_forward);

	matrix4x4 matrix;
	matrix.elem[0][0] = new_right.x;		matrix.elem[0][1] = new_right.y;		matrix.elem[0][2] = new_right.z;		matrix.elem[0][3] = 0.0f;
	matrix.elem[1][0] = new_up.x;			matrix.elem[1][1] = new_up.y;			matrix.elem[1][2] = new_up.z;			matrix.elem[1][3] = 0.0f;
	matrix.elem[2][0] = new_forward.x;		matrix.elem[2][1] = new_forward.y;		matrix.elem[2][2] = new_forward.z;		matrix.elem[2][3] = 0.0f;
	matrix.elem[3][0] = pos.x;				matrix.elem[3][1] = pos.y;				matrix.elem[3][2] = pos.z;				matrix.elem[3][3] = 1.0f;
	return matrix;
}

matrix4x4 Matrix_QuickInverse(matrix4x4 &m){
	matrix4x4 matrix;
	matrix.elem[0][0] = m.elem[0][0];   matrix.elem[0][1] = m.elem[1][0]; 	matrix.elem[0][2] = m.elem[2][0];   matrix.elem[0][3] = 0.0f;
	matrix.elem[1][0] = m.elem[0][1]; 	matrix.elem[1][1] = m.elem[1][1];   matrix.elem[1][2] = m.elem[2][1];   matrix.elem[1][3] = 0.0f;
	matrix.elem[2][0] = m.elem[0][2]; 	matrix.elem[2][1] = m.elem[1][2];   matrix.elem[2][2] = m.elem[2][2];   matrix.elem[2][3] = 0.0f;
	matrix.elem[3][0] = -(m.elem[3][0] * matrix.elem[0][0] + m.elem[3][1] * matrix.elem[1][0] + m.elem[3][2] * matrix.elem[2][0]);
	matrix.elem[3][1] = -(m.elem[3][0] * matrix.elem[0][1] + m.elem[3][1] * matrix.elem[1][1] + m.elem[3][2] * matrix.elem[2][1]);
	matrix.elem[3][2] = -(m.elem[3][0] * matrix.elem[0][2] + m.elem[3][1] * matrix.elem[1][2] + m.elem[3][2] * matrix.elem[2][2]);
	matrix.elem[3][3] = 1.0f;
	return matrix;
}

vec3d VectorAdd(vec3d &v1, vec3d &v2){
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

vec3d VectorSub(vec3d &v1, vec3d &v2){
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

vec3d VectorMul(vec3d &v1, float m){
	return { v1.x * m, v1.y * m, v1.z * m };
}

vec3d VectorDiv(vec3d &v1, float m){
	if(m != 0) return { v1.x / m, v1.y / m, v1.z / m };
	else exit(1);
}

float DotProduct(vec3d &v1, vec3d &v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float VectorLength(vec3d &v1){
	return sqrtf(DotProduct( v1, v1 ));
}

vec3d VectorNormalise(vec3d &v1){
	float l = VectorLength(v1);
	return VectorDiv(v1, l);
}

vec3d Vector_CrossProduct(vec3d &v1, vec3d &v2){
	vec3d v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}

vec3d Vector_IntersectPlane(vec3d &plane_p, vec3d &plane_n, vec3d &lineStart, vec3d &lineEnd){
	plane_n = VectorNormalise(plane_n);
	float plane_d = -DotProduct(plane_n, plane_p);
	float ad = DotProduct(lineStart, plane_n);
	float bd = DotProduct(lineEnd, plane_n);
	float t = (-plane_d - ad) / (bd - ad);
	vec3d lineStartToEnd = VectorSub(lineEnd, lineStart);
	vec3d lineToIntersect = VectorMul(lineStartToEnd, t);
	return VectorAdd(lineStart, lineToIntersect);
}



int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2)
{
	plane_n = VectorNormalise(plane_n);

	auto dist = [&](vec3d &p)
	{
		vec3d n = VectorNormalise(p);
		return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - DotProduct(plane_n, plane_p));
	};

	vec3d* inside_points[3];  int nInsidePointCount = 0;
	vec3d* outside_points[3]; int nOutsidePointCount = 0;

	float d0 = dist(in_tri.points[0]);
	float d1 = dist(in_tri.points[1]);
	float d2 = dist(in_tri.points[2]);

	if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.points[0]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.points[0]; }
	if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.points[1]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.points[1]; }
	if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.points[2]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.points[2]; }



	if (nInsidePointCount == 0){
		return 0; 
	}

	if (nInsidePointCount == 3){
		out_tri1 = in_tri;
		return 1; 
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2){
		
		out_tri1.r = in_tri.r;
		out_tri1.g = in_tri.g;
		out_tri1.b = in_tri.b;
	
		/*out_tri1.r = 1;
		out_tri1.g = 0;
		out_tri1.b = 0;*/

		out_tri1.points[0] = *inside_points[0];

		out_tri1.points[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
		out_tri1.points[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

		return 1; 
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1){
		
		out_tri1.r = in_tri.r;
		out_tri1.g = in_tri.g;
		out_tri1.b = in_tri.b;

		out_tri2.r = in_tri.r;
		out_tri2.g = in_tri.g;
		out_tri2.b = in_tri.b;

		/*out_tri1.r = 0;
		out_tri1.g = 1;
		out_tri1.b = 0;

		out_tri2.r = 0;
		out_tri2.g = 0;
		out_tri2.b = 1;*/

		out_tri1.points[0] = *inside_points[0];
		out_tri1.points[1] = *inside_points[1];
		out_tri1.points[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

		out_tri2.points[0] = *inside_points[1];
		out_tri2.points[1] = out_tri1.points[2];
		out_tri2.points[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

		return 2;
	}
}