#include "SMmodel.h"
#include "SMfileOBJ.h"

SMmodel::SMmodel(SMmeshPool *meshes, SMmesh mesh, uint32_t diffuseColour, bool asCollider)
{
	init();
	meshes->addMesh(mesh, !asCollider);
	int32_t i = meshes->meshes.size() - 1;
	meshRef = i;
	//if (meshes->materials.size() > meshes->meshes[i].materialRef) {
		//material = meshes->materials[meshes->meshes[i].materialRef];
		material = meshes->materials[0];  //The default material
		material.SetColDiffuse(diffuseColour);
	//}
	bbox = meshes->meshes[i].bbox;
}

void SMmodel::init()
{
	visible = true;
	deleted = false;
	collider = false;
	materialRef = -1;
	meshRef = -1;
	centreOnAvatar = false;
	alphaOverride = false;
}

void SMmodel::render(SMmeshPool &meshes, SMmatrix &parent_matrix, SMshader &shader)
{
	if (!visible || deleted || collider) return;
	
	SMmatrix newmatrix = matrix * parent_matrix;
	
	if (group.size()>0) 
	{
		for (auto &sg:group) 
			sg.render(meshes, newmatrix, shader);
	}
	else 
		if (meshRef >= 0) {
			shader.SetModelMatrix(newmatrix);
			shader.setMaterial(material);
			meshes.renderMesh(meshRef);
		}
}

void SMmodel::appendMesh(SMmeshPool *meshes, SMmesh mesh, bool asCollider)
{
	meshes->addMesh(mesh);
	int32_t i = meshes->meshes.size() - 1;
	if (group.size() == 0) {
		if (meshRef < 0) {
			meshRef = i;
			bbox = meshes->meshes[i].bbox;
		}
		else {
			SMmodel newGroup = *this;
			group.push_back(newGroup);
			SMmodel newMesh;
			newMesh.meshRef = i;
			bbox.update(meshes->meshes[i].bbox);
			group.push_back(newMesh);
		}
	}
	else {
		SMmodel newMesh;
		newMesh.meshRef = i;
		bbox.update(meshes->meshes[i].bbox);
		group.push_back(newMesh);
	}
}

void SMmodel::loadOBJfile(SMmeshPool *meshes, std::string path, std::string modelfile, bool asCollider)
{	
	std::string error;
	uint32_t meshStart = meshes->meshes.size();
	SMfileOBJ::open(path, modelfile, meshes, asCollider, error);
	uint32_t meshEnd = meshes->meshes.size();
		
	if (meshEnd-meshStart > 0) {
		for (size_t i=meshStart; i<meshEnd; i++) {
			SMmodel newGroupModel;
			newGroupModel.meshRef = i;
			newGroupModel.material = meshes->materials[meshes->meshes[i].materialRef]; 	//copy material into group model so it can change;
			newGroupModel.bbox = meshes->meshes[i].bbox;
			bbox.update(meshes->meshes[i].bbox);
			group.push_back(newGroupModel);
		}
	}
}

bool SMmodel::collideSphere(SMmeshPool &meshes, vec3f &spherePos, vec3f &sphereDir, float radius)
{
	if (group.size()>0) {
		for (auto &sg:group) {
			if (sg.collideSphere(meshes, spherePos, sphereDir, radius)) return true;
		}
	} 
	else 
		if (meshRef>=0) 
			return meshes.meshes[meshRef].collideVector(false,spherePos,sphereDir);
		
	return false;
}

float SMmodel::collideFloor(SMmeshPool &meshes, vec3f &pos)
{
	float prevHeight = 1e8f;
	return collideFloorSub(meshes,pos,prevHeight);
}

float SMmodel::collideFloorSub(SMmeshPool &meshes, vec3f &pos, float &prevHeight)
{
	if (group.size()>0) {
		for (auto &sg:group) {
			float ht= sg.collideFloorSub(meshes, pos, prevHeight);
			if (ht<1e8f) return ht;
		}
	} 
	else 
		if (meshRef>=0) 
			return meshes.meshes[meshRef].collideFloor(-pos, prevHeight);
		
	return 1e8f;
}

