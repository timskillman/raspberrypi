#ifndef __SMMESHPOOL
#define __SMMESHPOOL

#include "SMmesh.h"
#include "SMtexture.h"
#include "SMmaterial.h"
#include "SMGL.h"
#include <vector>
#include <string>
#include <memory>

class SMmeshPool {
public:
	SMmeshPool(uint32_t stride = 8)
	{ 
		this->stride = stride; 
		currentBuffer = -1;
		lastVBO = -1;
		createDefaultMaterial();
	}
	
	~SMmeshPool()
	{
		for (uint32_t i=0; i<VBOid.size(); i++) {
			if (VBOid[i]>0) glDeleteBuffers(1, &VBOid[i]);
		}
	}
	
	void appendMesh(SMmesh mesh) { meshes.push_back(mesh); }
	size_t meshCount() { return meshes.size(); }

	int32_t loadTexture(std::string path, std::string name, int32_t &texID);  //loads a texture and returns a texture reference to the textures array (else -1 if failed)
	void createDefaultMaterial(std::string name = "default");
	int32_t createDefaultTexture(int32_t &texID);


	SMtexture* getTexture(int32_t ref) {
		if (ref >= 0 && ref <= (int32_t)textures.size()) return textures[ref].get(); else return nullptr;
	}

	void addMesh(SMmesh &mesh, bool deleteVerts = true);
	
	void uploadMeshesToGPU();
	
	void renderMesh(int bufRef);
	
	void clearAll()
	{
		meshes.clear();
		materials.clear();
		textures.clear();
		vertBuffer.resize(0);
		vertCount = 0;
		VBOid.resize(0);
		currentBuffer = -1;
		lastVBO = -1;
	}

	size_t poolSize(uint32_t id) { return vertBuffer[id].size(); }
	
	std::vector<std::vector<float>> vertBuffer;
	uint32_t vertCount;
	int32_t currentBuffer;
	
	std::vector<SMmesh> meshes;
	std::vector<SMmaterial> materials;
	std::vector<std::shared_ptr<SMtexture>> textures;
	
private:
	
	std::vector<GLuint> VBOid;
	uint32_t stride;
	int lastVBO;
};

#endif
