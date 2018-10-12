#ifndef __SMSHAPES
#define __SMSHAPES

#include "SMmesh.h"
//#include "glutess/tessellate.h"
#include <vector>
#include <string>

#define SM_SIDES 24

namespace SMshapes {

	/* Class that creates all 3D objects
	*/

	enum { UVMAP_CYLINDER, UVMAP_SPHERE };

	SMmesh sprite(vec2f pos, vec2f size, vec2f uvsize = vec2f(1.f,1.f));
	SMmesh plane(vec3f pos, vec3f size, vec2f uvsize = vec2f(1.f, 1.f));
	SMmesh cuboid(vec3f pos, vec3f size);
	SMmesh lathe(std::string name, vec3f pos, std::vector<vec2f> &path, int sides= SM_SIDES,
		float rise=0.0f, float coils=1.0f, bool invert = false, float creaseAngle = 60.0f, vec2f prevPoint = vec2f(0,0), 
		vec2f lastPoint = vec2f(0, 0), uint32_t uvtype = UVMAP_CYLINDER);
	SMmesh sphere(vec3f pos, float radius, float hemi=0.0f, int slices= SM_SIDES, int sides = SM_SIDES, bool invert = false);
	SMmesh torus(vec3f pos, float radius, float ringradius, int ringrots=12, int sides = SM_SIDES);
	SMmesh spring(vec3f pos, float radius, float height, int coils, float coilradius, int ringrots=12, int sides = SM_SIDES);
	SMmesh cone(vec3f pos, float radius, float height, int sides = SM_SIDES);
	SMmesh tcone( vec3f pos, float botradius, float topradius, float height, int sides = SM_SIDES);
	SMmesh cylinder( vec3f pos, float radius, float height, int sides = SM_SIDES);
	SMmesh tube(vec3f pos, float innerradius, float outerradius, float height, int sides = SM_SIDES);
	SMmesh extrude(vec3f pos, std::vector<std::vector<float>> contours, SMbbox3d &modelbox, float depth = 1.0f, uint16_t divs = 1);
	
	//SMmesh capsule(vec3f pos, float radius, float midlength = 2.0f, int slices = SM_SIDES, int sides = SM_SIDES);

	void texMap(std::vector<float> &vertBuffer, uint32_t vc, uint32_t stride, uint32_t uvoffset, uint8_t x, uint8_t y);

}

#endif
