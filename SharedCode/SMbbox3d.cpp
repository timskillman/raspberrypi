#include "SMbbox3d.h"
#ifdef __WINDOWS__
#include <cmath>
#endif

void SMbbox3d::init()
{
	min = vec3f(1e8, 1e8, 1e8);
	max = vec3f(-1e8, -1e8, -1e8);
	radius = 0;
}

SMbbox3d::SMbbox3d(vec3f _min, vec3f _max)
{
	init();
}

 
void SMbbox3d::update(vec3f point) 
{
	if (point.x < min.x) min.x = point.x;
	if (point.x > max.x) max.x = point.x;
	if (point.y < min.y) min.y = point.y;
	if (point.y > max.y) max.y = point.y;
	if (point.z < min.z) min.z = point.z;
	if (point.z > max.z) max.z = point.z;

	//approximate radius from bbox ..
	vec3f d = max - center();
	float r = sqrtf(d.x*d.x + d.y*d.y + d.z*d.z);
	if (r > radius) radius = r;
}

void SMbbox3d::update(SMbbox3d box, SMmatrix* mtx)
{
	update(mtx->transformVec(vec3f(box.min.x, box.min.y, box.min.z)));
	update(mtx->transformVec(vec3f(box.max.x, box.min.y, box.min.z)));
	update(mtx->transformVec(vec3f(box.max.x, box.max.y, box.min.z)));
	update(mtx->transformVec(vec3f(box.min.x, box.max.y, box.min.z)));
	update(mtx->transformVec(vec3f(box.min.x, box.min.y, box.max.z)));
	update(mtx->transformVec(vec3f(box.max.x, box.min.y, box.max.z)));
	update(mtx->transformVec(vec3f(box.max.x, box.max.y, box.max.z)));
	update(mtx->transformVec(vec3f(box.min.x, box.max.y, box.max.z)));
}

void SMbbox3d::update(SMbbox3d box)
{
	update(vec3f(box.min.x, box.min.y, box.min.z));
	update(vec3f(box.max.x, box.min.y, box.min.z));
	update(vec3f(box.max.x, box.max.y, box.min.z));
	update(vec3f(box.min.x, box.max.y, box.min.z));
	update(vec3f(box.min.x, box.min.y, box.max.z));
	update(vec3f(box.max.x, box.min.y, box.max.z));
	update(vec3f(box.max.x, box.max.y, box.max.z));
	update(vec3f(box.min.x, box.max.y, box.max.z));
}

void SMbbox3d::set(SMbbox3d box, SMmatrix* mtx)
{
	init();
	update(box, mtx);
}

void SMbbox3d::bboxFromVerts(std::vector<float> &verts, uint32_t start, uint32_t vsize, uint32_t stride)
{
	min.x = verts[start]; max.x = min.x;
	min.y = verts[start + 1]; max.y = min.y;
	min.z = verts[start + 2]; max.z = min.z;

	for (uint32_t i = 0; i < vsize; i += stride) {
		if (verts[i] < min.x) min.x = verts[i]; else if (verts[i] > max.x) max.x = verts[i];
		if (verts[i + 1] < min.y) min.y = verts[i + 1]; else if (verts[i + 1] > max.y) max.y = verts[i + 1];
		if (verts[i + 2] < min.z) min.z = verts[i + 2]; else if (verts[i + 2] > max.z) max.z = verts[i + 2];
	}
}

void SMbbox3d::radiusFromVerts(std::vector<float> &verts, vec3f centre, uint32_t start, uint32_t size, uint32_t stride)
{
	uint32_t st = start*stride;
	uint32_t sz = size*stride;
	//Works out the bounding radius of the vertices
	radius = 0;
	//TODO: This should strictly determine bounds by vertices 'used' in the scene according to polygon indices.
	for (std::size_t i = st; i < (st+sz); i=stride) {
		vec3f d = vec3f(verts[i], verts[i+1], verts[i+2]) - centre;
		float r = sqrtf(d.x*d.x + d.y*d.y + d.z*d.z);
		if (r > radius) radius = r;
	}
}

float SMbbox3d::radiusFromTVerts(std::vector<float> &verts, vec3f centre, SMmatrix* mtx, uint32_t start, uint32_t size, uint32_t stride)
{
	uint32_t st = start*stride;
	uint32_t sz = size*stride;
	//Works out the bounding radius of the vertices
	float rad = 0;
	//TODO: This should strictly determine bounds by vertices 'used' in the scene according to polygon indices.
	for (std::size_t i = st; i < (st + sz); i = stride) {
		vec3f d = mtx->transformVec(vec3f(verts[i], verts[i + 1], verts[i + 2])) - centre;
		float r = sqrtf(d.x*d.x + d.y*d.y + d.z*d.z);
		if (r > rad) rad = r;
	}
	return rad;
}

SMbbox3d SMbbox3d::bboxFromTVerts(SMmatrix* mtx)
{
	//Transforms a bounding box with a matrix
	SMbbox3d tbox;
	tbox.update(*this);
	return tbox;
}

void SMbbox3d::bboxFromTVerts(std::vector<float> &verts, SMmatrix* mtx, uint32_t start, uint32_t size, uint32_t stride)
{
	uint32_t st = start*stride;
	uint32_t sz = size*stride;

	//Assumes first three values of the vertex element is position data
	min = mtx->transformVec(vec3f(verts[st], verts[st+1], verts[st+2]));
	max = min;

	//TODO: This should strictly determine bounds by vertices 'used' in the scene according to polygon indices.
	for (std::size_t i = st; i < (st+sz); i += stride) {
		vec3f tvec = mtx->transformVec(vec3f(verts[i], verts[i + 1], verts[i + 2]));
		if (tvec.x < min.x) min.x = tvec.x; else if (tvec.x > max.x) max.x = tvec.x;
		if (tvec.y < min.y) min.y = tvec.y; else if (tvec.y > max.y) max.y = tvec.y;
		if (tvec.z < min.z) min.z = tvec.z; else if (tvec.z > max.z) max.z = tvec.z;
	}

	radius = radiusFromTVerts(verts, center(), mtx, start, size, stride);
}


void SMbbox3d::translate(vec3f t)
{
	min += t; max += t;
}

void SMbbox3d::moveto(vec3f p)
{
	vec3f bc = center();
	min = (min-bc)+p; 
	max = (max-bc)+p;
}