#include "SMmatrix.h"
#include <cmath>
#include <cstring>

SMmatrix SMmatrix::operator * (SMmatrix const &m2) const
{
	//multiply only the bits we need (36 mults instead of 48)...
	
	SMmatrix nm;
	nm.matrix[m00] = matrix[m00] * m2.matrix[m00] + matrix[m01] * m2.matrix[m10] + matrix[m02] * m2.matrix[m20]; // + matrix[m03] * m2.matrix[m30];
	nm.matrix[m01] = matrix[m00] * m2.matrix[m01] + matrix[m01] * m2.matrix[m11] + matrix[m02] * m2.matrix[m21]; // + matrix[m03] * m2.matrix[m31];
	nm.matrix[m02] = matrix[m00] * m2.matrix[m02] + matrix[m01] * m2.matrix[m12] + matrix[m02] * m2.matrix[m22]; // + matrix[m03] * m2.matrix[m32];
	//nm.matrix[m03] = matrix[m00] * m2.matrix[m03] + matrix[m01] * m2.matrix[m13] + matrix[m02] * m2.matrix[m23]; // + matrix[m03] * m2.matrix[m33];
	
	nm.matrix[m10] = matrix[m10] * m2.matrix[m00] + matrix[m11] * m2.matrix[m10] + matrix[m12] * m2.matrix[m20]; // + matrix[m13] * m2.matrix[m30];
	nm.matrix[m11] = matrix[m10] * m2.matrix[m01] + matrix[m11] * m2.matrix[m11] + matrix[m12] * m2.matrix[m21]; // + matrix[m13] * m2.matrix[m31];
	nm.matrix[m12] = matrix[m10] * m2.matrix[m02] + matrix[m11] * m2.matrix[m12] + matrix[m12] * m2.matrix[m22]; // + matrix[m13] * m2.matrix[m32];
	//nm.matrix[m13] = matrix[m10] * m2.matrix[m03] + matrix[m11] * m2.matrix[m13] + matrix[m12] * m2.matrix[m23]; // + matrix[m13] * m2.matrix[m33];

	nm.matrix[m20] = matrix[m20] * m2.matrix[m00] + matrix[m21] * m2.matrix[m10] + matrix[m22] * m2.matrix[m20]; // + matrix[m23] * m2.matrix[m30];
	nm.matrix[m21] = matrix[m20] * m2.matrix[m01] + matrix[m21] * m2.matrix[m11] + matrix[m22] * m2.matrix[m21]; // + matrix[m23] * m2.matrix[m31];
	nm.matrix[m22] = matrix[m20] * m2.matrix[m02] + matrix[m21] * m2.matrix[m12] + matrix[m22] * m2.matrix[m22]; // + matrix[m23] * m2.matrix[m32];
	//nm.matrix[m23] = matrix[m20] * m2.matrix[m03] + matrix[m21] * m2.matrix[m13] + matrix[m22] * m2.matrix[m23]; // + matrix[m23] * m2.matrix[m33];

	//nm.matrix[m30] = matrix[m30] * m2.matrix[m00] + matrix[m31] * m2.matrix[m10] + matrix[m32] * m2.matrix[m20]; // + matrix[m33] * m2.matrix[m30];
	//nm.matrix[m31] = matrix[m30] * m2.matrix[m01] + matrix[m31] * m2.matrix[m11] + matrix[m32] * m2.matrix[m21]; // + matrix[m33] * m2.matrix[m31];
	//nm.matrix[m32] = matrix[m30] * m2.matrix[m02] + matrix[m31] * m2.matrix[m12] + matrix[m32] * m2.matrix[m22]; // + matrix[m33] * m2.matrix[m32];
	//nm.matrix[m33] = matrix[m30] * m2.matrix[m03] + matrix[m31] * m2.matrix[m13] + matrix[m32] * m2.matrix[m23]; // + matrix[m33] * m2.matrix[m33];
	
	nm.matrix[m30] = m2.matrix[m00] * matrix[m30] + m2.matrix[m10] * matrix[m31] + m2.matrix[m20] * matrix[m32] + m2.matrix[m30]; //X
	nm.matrix[m31] = m2.matrix[m01] * matrix[m30] + m2.matrix[m11] * matrix[m31] + m2.matrix[m21] * matrix[m32] + m2.matrix[m31]; //Y
	nm.matrix[m32] = m2.matrix[m02] * matrix[m30] + m2.matrix[m12] * matrix[m31] + m2.matrix[m22] * matrix[m32] + m2.matrix[m32]; //Z
	return nm;
}

void SMmatrix::SetScale(float scale)
{
	matrix[m00] = scale; matrix[m11] = scale; matrix[m22] = scale;
}

void SMmatrix::SetRotateX(float angle)
{
	matrix[0] = 1.f; matrix[1] = 0; matrix[2] = 0; matrix[3] = 0;
	matrix[4] = 0; matrix[5] = cosf(angle); matrix[6] = sinf(angle); matrix[7] = 0;
	matrix[8] = 0; matrix[9] = -sinf(angle); matrix[10] = cosf(angle); matrix[11] = 0.f;
	matrix[12] = 0; matrix[13] = 0; matrix[14] = 0; matrix[15] = 1.f;
}

void SMmatrix::SetRotateXbit(float angle)
{
	matrix[m11] = matrix[m22] = cosf(angle);
	matrix[m21] = -(matrix[m12] = sinf(angle));
}

void SMmatrix::SetRotateY(float angle)
{
	matrix[0] = cosf(angle); matrix[1] = 0; matrix[2] = sinf(angle); matrix[3] = 0;
	matrix[4] = 0; matrix[5] = 1.f; matrix[6] = 0; matrix[7] = 0;
	matrix[8] = -sinf(angle); matrix[9] = 0; matrix[10] = cosf(angle); matrix[11] = 0.f;
	matrix[12] = 0; matrix[13] = 0; matrix[14] = 0; matrix[15] = 1.f;
}

void SMmatrix::SetRotateYbit(float angle)
{
	matrix[m00] = matrix[m22] = cosf(angle);
	matrix[m20] = -(matrix[m02] = sinf(angle));
}

void SMmatrix::SetRotateZ(float angle)
{
	matrix[m00] = cosf(angle); matrix[m01] = sinf(angle); matrix[m02] = 0; matrix[3] = 0;
	matrix[4] = -sinf(angle); matrix[5] = cosf(angle); matrix[6] = 0; matrix[7] = 0;
	matrix[8] = 0; matrix[9] = 0; matrix[10] = 1.f; matrix[11] = 0.f;
	matrix[12] = 0; matrix[13] = 0; matrix[14] = 0; matrix[15] = 1.f;
}

void SMmatrix::SetRotateZbit(float angle)
{
	matrix[m00] = matrix[m11] = cosf(angle);
	matrix[m10] = -(matrix[m01] = sinf(angle));
}

void SMmatrix::SetPerspective(float pspmul, float znear, float zfar)
{
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	GLfloat aspectRatio = (float)vp[2] / (float)vp[3];
	float fov = ((float)vp[3] / 2.f) / pspmul;
	float w = -cosf(0.5f*fov) / sinf(0.5f*fov);
	float h = -w * aspectRatio;
	float a = -(znear + zfar) / (znear - zfar);
	float b = -((2.f * zfar*znear) / (zfar - znear));

	matrix[m00] = w; matrix[m01] = 0; matrix[m02] = 0; matrix[m03] = 0;
	matrix[m10] = 0; matrix[m11] = h; matrix[m12] = 0; matrix[m13] = 0;
	matrix[m20] = 0; matrix[m21] = 0; matrix[m22] = a; matrix[m23] = 1.f;
	matrix[m30] = 0; matrix[m31] = 0; matrix[m32] = b; matrix[m33] = 0.f;
}

void SMmatrix::SetOrtho(float left, float right, float top, float bottom, float znear, float zfar)
{
	float w = 2.f / (right - left);
	float h = 2.f / (top - bottom);
	float zn = -2.f / (zfar - znear);
	matrix[m00] = w; matrix[m01] = 0; matrix[m02] = 0; matrix[m03] = 0;
	matrix[m10] = 0; matrix[m11] = h; matrix[m12] = 0; matrix[m13] = 0;
	matrix[m20] = 0; matrix[m21] = 0; matrix[m22] = zn; matrix[m23] = 0;
	matrix[m30] = -(right + left) / (right - left); 
	matrix[m31] = -(top + bottom) / (top - bottom); 
	matrix[m32] = -(znear + zfar) / (zfar - znear); 
	matrix[m33] = 1.f;
}

void SMmatrix::SetSceneMatrix(vec3f const &pos, vec3f const &rot)
{
	SMmatrix rotxmtx, rotymtx, xyzmtx;
	rotxmtx.SetRotateYbit(rot.y);
	rotymtx.SetRotateXbit(rot.x);
	xyzmtx.Translate(pos);
	*this = xyzmtx * rotxmtx * rotymtx;
}

vec3f crossProduct(vec3f const a, vec3f const b)
{
	return vec3f(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
}

void SMmatrix::SetLookAt(vec3f const &dir, vec3f const &up)
{
	vec3f xax = crossProduct(up, dir).unit();
	vec3f yax = crossProduct(dir, xax).unit();
	matrix[m00] = xax.x; matrix[m10] = yax.x; matrix[m20] = dir.x;
	matrix[m01] = xax.y; matrix[m11] = yax.y; matrix[m21] = dir.y;
	matrix[m02] = xax.z; matrix[m12] = yax.z; matrix[m22] = dir.z;
}

void SMmatrix::Translate(vec3f vec)
{
	matrix[12] += vec.x;
	matrix[13] += vec.y;
	matrix[14] += vec.z;
}

void SMmatrix::Translate(float x, float y, float z)
{
	matrix[12] += x;
	matrix[13] += y;
	matrix[14] += z;
}

void SMmatrix::Move(vec3f offset) {
	matrix[12] = offset.x; 
	matrix[13] = offset.y; 
	matrix[14] = offset.z;
}

vec3f SMmatrix::transformVec(vec3f const &vec)
{
	return vec3f(
		matrix[m00] * vec.x + matrix[m10] * vec.y + matrix[m20] * vec.z + matrix[m30],
		matrix[m01] * vec.x + matrix[m11] * vec.y + matrix[m21] * vec.z + matrix[m31],
		matrix[m02] * vec.x + matrix[m12] * vec.y + matrix[m22] * vec.z + matrix[m32]);
}

void SMmatrix::Init() {
	memset(&matrix,0,60);
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.f;
}


