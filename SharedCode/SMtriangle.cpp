#include "SMtriangle.h"

bool SMtriangle::pointInBox(vec3f p) {
	//fast way of discarding a point outside the triangle's bbox
	if (v1.x > p.x && v2.x > p.x && v3.x > p.x) return false;
	if (v1.x < p.x && v2.x < p.x && v3.x < p.x) return false;
	if (v1.y > p.y && v2.y > p.y && v3.y > p.y) return false;
	if (v1.y < p.y && v2.y < p.y && v3.y < p.y) return false;
	if (v1.z > p.z && v2.z > p.z && v3.z > p.z) return false;
	if (v1.z < p.z && v2.z < p.z && v3.z < p.z) return false;
	return true;
}

bool SMtriangle::pointInBoxXZ(vec3f p) {
	//fast way of discarding a point outside the triangle's footprint on XZ axis
	if (v1.x > p.x && v2.x > p.x && v3.x > p.x) return false;
	if (v1.x < p.x && v2.x < p.x && v3.x < p.x) return false;
	if (v1.z > p.z && v2.z > p.z && v3.z > p.z) return false;
	if (v1.z < p.z && v2.z < p.z && v3.z < p.z) return false;
	return true;
}

vec3f crossProduct(vec3f &a, vec3f &b)
{
	return vec3f(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
}

float innerProduct(vec3f &a, vec3f &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
	
float SMtriangle::rayIntersect(vec3f pos, vec3f dir)
{
	//Based on www.lighthouse3d.com/tutorials/ray-triangle-intersection/
	
	vec3f e1 = v2 - v1;
	vec3f e2 = v3 - v1;
	vec3f n = crossProduct(dir, e2);
	float a = innerProduct(e1, n);
	
	if (a > -0.00001f && a < 0.00001f) return 0.f; //roughly zero
	
	float f = 1.f / a;
	vec3f s = pos - v1;
	float u = f * innerProduct(s, n);
	
	if (u < 0.f || u > 1.f) return 0.f;
	
	vec3f q = crossProduct(s, e1);
	float v = f * innerProduct(dir, q);
	
	if (v < 0.f || u+v > 1.f) return 0.f;
	
	float i = f * innerProduct(e2, q);	//distance from pos to triangle intersection
	
	return i;
}
	
vec3f SMtriangle::faceNormal()
{
	vec3f e1 = v3 - v2;
	vec3f e2 = v2 - v1;
	vec3f n = crossProduct(e1, e2);
	float sq = sqrtf(n.x*n.x + n.y*n.y + n.z*n.z);
	if (sq > 0.f) return n/sq;
	return n;
}

vec3f SMtriangle::reflection(vec3f dir)
{
	vec3f norm = faceNormal();
	return dir - (norm * innerProduct(dir,norm) * 2.f);
}
