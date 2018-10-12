#ifndef __SMMODEL
#define __SMMODEL

#include "SMmeshPool.h"
#include "SMmatrix.h"
#include "SMshader.h"
#include "SMmaterial.h"

class SMmodel {
public:
	SMmodel() { init(); }
	SMmodel(SMmeshPool *meshes, SMmesh mesh, uint32_t diffuseColour = 0xffffffff, bool asCollider = false);
	~SMmodel() {}

/// FUNCTIONS

	void init();
	void render(SMmeshPool &meshes, SMmatrix &parent_matrix, SMshader &shader);
	void appendMesh(SMmeshPool *meshes, SMmesh mesh, bool asCollider);
	void loadOBJfile(SMmeshPool *meshes, std::string path, std::string modelfile, bool asCollider);
	bool collideSphere(SMmeshPool &meshes, vec3f &spherePos, vec3f &sphereDir, float radius);
	float collideFloor(SMmeshPool &meshes, vec3f &pos);
	float collideFloorSub(SMmeshPool &meshes, vec3f &pos, float &prevHeight);
	
/// VARIABLES
	int32_t meshRef; // [5];			//mesh reference to mesh in meshPool (-1 if not rendered at all)
	float LODdist[5];			//LOD distances for models (first mesh is highest level of detail)
	int32_t colliderRef[5];		//collider mesh reference (or -1 for none)

	SMmatrix matrix;			//matrix used to transform mesh/group
	SMmaterial material;		//copy of material in the meshPool - used for rendering mesh - can be modified
	int32_t materialRef;		//material reference of original material in the meshPool
	bool visible;				//visible in scene
	bool deleted;				//deleted from scene
	bool centreOnAvatar;		//Used for skyboxes - effectively centred on the avatar position
	bool collider;
	float alphaOverride;		//Used for fading out mesh / group
	SMbbox3d bbox;
	
	std::vector<SMmodel> group;	//Group containing other models (or none at all)

private:


};


#endif
