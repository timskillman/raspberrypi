#ifndef __SMMATRIX
#define __SMMATRIX

#include "SMGL.h"
#include "SMvector.h"
#include <stdint.h>
#include <vector>

class SMmatrix {
public:
	SMmatrix() { Init(); }
	
	enum { 	m00 =0, m01 = 1, m02 = 2, m03 = 3,
			m10 =4, m11 = 5, m12 = 6, m13 = 7,
			m20 =8 ,m21 = 9, m22 = 10,m23 = 11,
			m30 =12,m31 = 13,m32 = 14,m33 = 15 };
			
	SMmatrix operator * (SMmatrix const &m2) const;
	
	void Init();
	void SetScale(float scale);
	void SetRotateX(float angle);
	void SetRotateY(float angle);
	void SetRotateZ(float angle);
	void SetRotateXbit(float angle);
	void SetRotateYbit(float angle);
	void SetRotateZbit(float angle);
	void SetPerspective(float pspmul, float znear, float zfar);
	void SetOrtho(float left, float right, float top, float bottom, float znear, float zfar);
	void SetSceneMatrix(vec3f const &pos, vec3f const &rot);
	void Translate(vec3f offset);
	void Translate(float x, float y, float z);
	void Move(vec3f offset);
	void SetLookAt(vec3f const &dir, vec3f const &up);
	vec3f transformVec(vec3f const &vec);
	
	float matrix[16];
		
private:

};
#endif
