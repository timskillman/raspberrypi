#ifndef __SM3DBOX
#define __SM3DBOX

#include <stdint.h>
#include "SMvector.h"
#include "SMmatrix.h"
#include <vector>

class SMbbox3d {
public:
	SMbbox3d() {
		init();
	}
	SMbbox3d(vec3f origin, vec3f size);
	~SMbbox3d() {}

	void init();
	void update(vec3f point);
	void update(SMbbox3d box, SMmatrix* mtx);
	void update(SMbbox3d box);
	void set(SMbbox3d box, SMmatrix* mtx);
	void bboxFromVerts(std::vector<float> &verts, uint32_t start, uint32_t size, uint32_t stride);
	void bboxFromTVerts(std::vector<float> &verts, SMmatrix* mtx, uint32_t start, uint32_t size, uint32_t stride);
	void radiusFromVerts(std::vector<float> &verts, vec3f centre, uint32_t start, uint32_t size, uint32_t stride);
	float radiusFromTVerts(std::vector<float> &verts, vec3f centre, SMmatrix* mtx, uint32_t start, uint32_t size, uint32_t stride);
	SMbbox3d bboxFromTVerts(SMmatrix* mtx);
	float width() { return max.x - min.x; }
	float depth() { return max.z - min.z; }
	float height() { return max.y - min.y; }
	vec3f center() { return (min+max)/2.0f; }
	float calcradius() { 
		float w = width() / 2, h = height() / 2, d = depth() / 2;
		return sqrtf(w*w + h*h + d*d);
	}
	vec3f size() { return vec3f(max.x - min.x, max.y - min.y, max.z - min.z); }
	void translate(vec3f t);
	void moveto(vec3f p);
	bool valid() { return (!(min.x > max.x)); }
	
	bool pointInside(vec3f p) { 
		return ((p.x > min.x && p.x < max.x) && (p.y > min.y && p.y < max.y) && (p.z > min.z && p.z < max.z)); }
	bool pointInsideXZ(vec3f p) { 
		return ((p.x > min.x && p.x < max.x) && (p.z > min.z && p.z < max.z)); }

	vec3f min;
	vec3f max;
	float radius;
};
#endif
