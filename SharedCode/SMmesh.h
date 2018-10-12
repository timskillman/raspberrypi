#ifndef __SMMESH
#define __SMMESH

#include "SMGL.h"
#include <string>
#include <stdint.h>
#include <vector>
#include "SMvector.h"
#include "SMbbox3d.h"

#define defaultStride 8

class SMmesh {
public:

	//SMmesh() { init("",defaultStride); }
	SMmesh(std::string _name = "", uint32_t stride = defaultStride);
	~SMmesh() {}

/// FUNCTIONS ...

	void addPackedVert(std::vector<float> &verts, uint32_t &vc, vec3f position, vec3f normal, vec2f uv, GLfloat *cols);

	void addPackedVert2(vec3f position, vec3f normal, vec2f uv, GLfloat *cols);

	void offset(vec3f pos, std::vector<float> &verts, uint32_t start, uint32_t size, uint32_t stride = defaultStride);

	void reset();

	void render();
	
	bool collideVector(bool bounce, vec3f &pos, vec3f &dir);	//returns hit and modified pos, dir vectors
	float collideFloor(vec3f pos, float &prevHeight); 			//returns height above the floor

/// VARIABLES ...

	std::string name;
	SMbbox3d bbox;
	uint16_t mode;
	std::vector<float> verts;
	GLint vertOffset;
	uint32_t vertSize;
	uint32_t stride;
	int32_t materialRef;
	size_t vc;
	GLint bufRef;
	bool collisionMesh;

private:
	void init(std::string _name, uint32_t stride) {
		reset();
		name = _name;
		this->stride = stride;
	}
};

#endif
