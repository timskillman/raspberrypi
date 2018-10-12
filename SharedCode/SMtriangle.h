#ifndef __SMTRIANGLE
#define __SMTRIANGLE

#include"SMvector.h"

class SMtriangle {

public:
	SMtriangle();
	SMtriangle(vec3f v1, vec3f v2, vec3f v3) { this->v1 = v1; this->v2 = v2; this->v3 = v3; }
	SMtriangle(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z, float v3x, float v3y, float v3z)
		{ v1=vec3f(v1x,v1y,v1z); v2=vec3f(v2x,v2y,v2z); v3=vec3f(v3x,v3y,v3z); }
		
	vec3f v1,v2,v3;

	bool pointInBox(vec3f p);
	bool pointInBoxXZ(vec3f p);
	float rayIntersect(vec3f pos, vec3f dir);
	vec3f faceNormal();
	vec3f reflection(vec3f dir);
};

#endif
