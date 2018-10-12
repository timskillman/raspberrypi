#include "SMmesh.h"
#include "SMtriangle.h"

SMmesh::SMmesh(std::string _name, uint32_t stride) {
	reset();
	name = _name;
	this->stride = stride;
}

void SMmesh::reset()
{
	mode = GL_TRIANGLES; 
	//name = ""; 
	vertOffset = 0; 
	vertSize = 0;
	verts.resize(0);
	bbox.init();
	materialRef = -1;
	stride = defaultStride;
	vc = 0;
}
	
void SMmesh::addPackedVert(std::vector<float> &verts, uint32_t &vc, vec3f position, vec3f normal, vec2f uv, GLfloat *cols)
{
	if (vc + stride > verts.size()) verts.resize(vc + 1000);
	verts[vc++] = position.x;
	verts[vc++] = position.y;
	verts[vc++] = position.z;
	verts[vc++] = normal.x;
	verts[vc++] = normal.y;
	verts[vc++] = normal.z;
	verts[vc++] = uv.x;
	verts[vc++] = uv.y;
	if (cols && stride==12) {
		verts[vc++] = cols[0];
		verts[vc++] = cols[1];
		verts[vc++] = cols[2];
		verts[vc++] = cols[3];
	}
}
	
void SMmesh::addPackedVert2(vec3f position, vec3f normal, vec2f uv, GLfloat *cols)
{
	if (vc + stride > verts.size()) verts.resize(vc + 1000);
	verts[vc++] = position.x;
	verts[vc++] = position.y;
	verts[vc++] = position.z;
	verts[vc++] = normal.x;
	verts[vc++] = normal.y;
	verts[vc++] = normal.z;
	verts[vc++] = uv.x;
	verts[vc++] = uv.y;
	if (cols && stride==12) {
		verts[vc++] = cols[0];
		verts[vc++] = cols[1];
		verts[vc++] = cols[2];
		verts[vc++] = cols[3];
	}
}

void SMmesh::offset(vec3f pos, std::vector<float> &verts, uint32_t start, uint32_t size, uint32_t stride)
{
	uint32_t st = start*stride;
	uint32_t sz = size*stride;
	
	if (sz > 3) {
		for (std::size_t i = st; i < (st + sz); i += stride) {
			verts[i] += pos.x; 
			verts[i + 1] += pos.y; 
			verts[i + 2] += pos.z;
		}
	}
}
	
void SMmesh::render() {
	glDrawArrays(mode, vertOffset, vertSize);
}

bool SMmesh::collideVector(bool bounce, vec3f &pos, vec3f &dir)
{
	if (verts.size()==0) return false;

	vec3f nextpos = pos + dir;
	uint32_t s1a = stride; uint32_t s1b = stride+1; uint32_t s1c = stride+2;
	uint32_t s2a = stride*2; uint32_t s2b = stride*2+1; uint32_t s2c = stride*2+2;

	for (size_t sp=0; sp<verts.size()- stride * 3; sp+=stride*3) {
		SMtriangle tri(verts[sp],verts[sp+1],verts[sp+2], 
			verts[sp+s1a],verts[sp+s1b],verts[sp+s1c], 
			verts[sp+s2a],verts[sp+s2b],verts[sp+s2c]);

		//if (tri.pointInBox(pos) || tri.pointInBox(nextpos)) {
		   float d1=tri.rayIntersect(pos,dir);
		   //float d2=tri.rayIntersect(nextpos,dir);
		   //if (d1!=d1 || d2!=d2 || (d1==0.f && d2==0.f)) return false;
		   
		   //return true;
		   if ((d1>0.f && d1<1.f)) {
			   pos = pos+(dir * d1);
			   dir = tri.reflection(dir);
			   return true;
		   }
		//}
	}

	return false;
}

float SMmesh::collideFloor(vec3f pos, float &prevHeight)
{
	if (verts.size() == 0) return prevHeight;

	vec3f dir(0, -1.f, 0); //looking down vector
	//vec3f nextpos = pos + dir;
	uint32_t s1a = stride; uint32_t s1b = stride + 1; uint32_t s1c = stride + 2;
	uint32_t s2a = stride * 2; uint32_t s2b = stride * 2 + 1; uint32_t s2c = stride * 2 + 2;

	for (size_t sp = 0; sp<verts.size() - stride * 3; sp += stride * 3) {
		SMtriangle tri(verts[sp], verts[sp + 1], verts[sp + 2],
			verts[sp + s1a], verts[sp + s1b], verts[sp + s1c],
			verts[sp + s2a], verts[sp + s2b], verts[sp + s2c]);

		if (tri.pointInBoxXZ(pos)) {
			float d1 = tri.rayIntersect(pos, dir);
			
			if (d1>0 && d1<prevHeight) {
				prevHeight = d1;
				//return d1;
			}
		}
	}

	return prevHeight;
}
