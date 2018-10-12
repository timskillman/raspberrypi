#ifndef __SMSCENE
#define __SMSCENE

#include "SMmeshPool.h"
#include "SMmodel.h"
//#include "SMlight.h"
//#include "SMcamera.h"
//#include "SMavatar.h"
#include "SMshader.h"
#include "SMvector.h"

#include <vector>

/* 
 * Class containing a scene of models, avatars, cameras, lights etc..
 */

class SMscene {
public:
	SMscene() {}
	~SMscene() {}
	
	std::vector<SMmodel> models;
	std::vector<SMmodel> colliders;
	std::vector<SMmodel> models2D;
	std::vector<SMmodel> skyboxes;
	//std::vector<SMcamera> cameras;
	//std::vector<SMlight> lights;
	
	//std::vector<SMavatar> avatar;	//several avatars
	uint32_t playarAvatarRef;		//selects which avatar is the current players avatar

	void render(SMmeshPool &meshes, SMmatrix &parent_matrix, SMshader &shader);
	void render2D(SMmeshPool &meshes, SMmatrix &parent_matrix, SMshader &shader);
	void renderSkybox(SMmeshPool &meshes, SMmatrix &parent_matrix, SMshader &shader, const vec3f &pos, int32_t skyboxref = 0);
	void renderOffscreen(SDL_RWops *f, int width, int height, SMmeshPool &meshes, SMmatrix &parent_matrix, SMshader &shader);
	
	int32_t loadModelOBJ(SMmeshPool &meshes, std::string path, std::string file);
	int32_t loadColliderOBJ(SMmeshPool &meshes, std::string path, std::string file);
	int32_t loadSkybox(SMmeshPool &meshes, std::string path, std::string file, float scale = 1.f);
	
	bool collide(SMmeshPool &meshes, vec3f &pos, vec3f &dir, float radius);
	float collideFloor(SMmeshPool &meshes, vec3f &pos);
	int32_t appendModel(SMmodel &model);
	int32_t append2D(SMmeshPool &meshes, SMmodel model, std::string txfile);
	void appendCollider(SMmodel &model);
	//void appendLight(SMlight &light) { lights.push_back(light); }
	//void appendCamera(SMcamera &camera) { cameras.push_back(camera); }
};

#endif
