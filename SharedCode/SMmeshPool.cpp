#include "SMmeshPool.h"

void SMmeshPool::createDefaultMaterial(std::string name)
{
	SMmaterial defmaterial;
	defmaterial.texRef = createDefaultTexture(defmaterial.texID);
	defmaterial.name = name;
	defmaterial.texName = name;
	materials.push_back(defmaterial);
}

int32_t SMmeshPool::createDefaultTexture(int32_t &texID)
{
	/* Creates a 1 pixel white texture
	   Can be recoloured by material colDiffuse */
	std::shared_ptr<SMtexture> Texture;
	Texture.reset(new SMtexture());
	Texture->createColour(0xffffffff);
	Texture->Upload();
	texID = textures.size();
	textures.push_back(Texture); //keep texture - don't destroy it!
	return Texture->textureID; //textures.size();
}

int32_t SMmeshPool::loadTexture(std::string path, std::string texname, int32_t &texID) {

	for (size_t i = 0; i < textures.size(); i++) {
		if (texname == materials[i].texName) {
			texID = materials[i].texID;
			return materials[i].texRef;
		}
	}

	std::shared_ptr<SMtexture> Texture;
	Texture.reset(new SMtexture());

	std::string filename = path + texname;
	SDL_Log("Loading texture '%s'", filename.c_str());

	Texture->LoadFromFile(filename.c_str());
	if (Texture.get() != nullptr) {
		Texture->Upload();
		texID = textures.size();
		textures.push_back(Texture); //keep texture - don't destroy it!
		return Texture->textureID; //textures.size();
	}
	texID = -1;
	return -1;
}

void SMmeshPool::addMesh(SMmesh &mesh, bool deleteVerts)
{
	//SDL_Log("Creating verts buffer %d",currentBuffer);
	if (currentBuffer<0) {
		//buffer not setup yet? Then push back existing mesh verts into the buffer
		if ((mesh.vc / mesh.stride) > 65520) {
			SDL_Log("ERROR - single mesh verts larger than 65535 verts!!"); //needs fixing later on !
		}
		vertBuffer.push_back(mesh.verts);
		currentBuffer++;
		mesh.vertOffset = 0;
		mesh.vertSize = mesh.vc / mesh.stride;
		mesh.bufRef = currentBuffer;
		meshes.push_back(mesh);
	} 
	else {
		//Check if existing buffer plus mesh verts > 65535 (RPi limitation) and create a new buffer if so
		std::vector<float> &mverts = vertBuffer[currentBuffer];
		uint32_t msz = mverts.size();
		uint32_t vs = (msz + mesh.vc) / mesh.stride;
		uint32_t ms = mesh.vc / mesh.stride;
		uint32_t maxverts = 65520; //multiple of 24 vertices (3 xyz, 3 triangle points)
		if (vs > maxverts) {
			if (ms > maxverts) {
				uint32_t bufsReq = ms / maxverts;
				uint32_t memsize = maxverts;
				std::vector<float> newverts;
				newverts.resize(memsize * mesh.stride);
				mesh.vertOffset = 0;
				mesh.vertSize = memsize;
				for (size_t b = 0; b < bufsReq; b++) {
					memcpy(newverts.data(), &mesh.verts[b * memsize], memsize * mesh.stride * sizeof(float));
					vertBuffer.push_back(newverts);
					currentBuffer++;
					mesh.bufRef = currentBuffer;
					meshes.push_back(mesh);
				}
				memsize = ms - bufsReq * maxverts;
				newverts.resize(memsize * mesh.stride);
				memcpy(newverts.data(), &mesh.verts[bufsReq * maxverts], memsize * mesh.stride * sizeof(float));
				vertBuffer.push_back(newverts);
				currentBuffer++;
				mesh.vertSize = memsize;
				mesh.bufRef = currentBuffer;
				meshes.push_back(mesh);

				//SDL_Log("ERROR - single mesh verts larger than 65535 verts!!"); //needs fixing later on !
			}
			else {
				vertBuffer.push_back(mesh.verts);
				currentBuffer++;
				mesh.vertOffset = 0;
				mesh.vertSize = mesh.vc / mesh.stride;
				mesh.bufRef = currentBuffer;
				meshes.push_back(mesh);
			}
		}
		else {
			//else copy mesh verts into existing buffer
			mverts.resize(msz + mesh.vc);
			memcpy(&mverts[msz], &mesh.verts[0], mesh.vc * sizeof(float));
			mesh.vertOffset = msz / mesh.stride;
			mesh.vertSize = mesh.vc / mesh.stride;
			mesh.bufRef = currentBuffer;
			meshes.push_back(mesh);
		}
	}
	if (deleteVerts) mesh.verts.resize(0);
}

void SMmeshPool::renderMesh(int meshRef)
{
	/* RPi cant cope with more than 65k vertices per array.
	 * This function will setup the correct VBO array for the
	 * mesh if different from the last mesh drawn				*/

	int bufRef = meshes[meshRef].bufRef;
	if (lastVBO != bufRef) {
		glBindBuffer(GL_ARRAY_BUFFER, VBOid[bufRef]); //Bind Vertex buffer

		GLuint ac = 0; GLuint pos = 0; GLuint stride4 = stride*4;
		glVertexAttribPointer(ac, 3, GL_FLOAT, GL_FALSE, stride4, (void*)pos);  //verts
		glEnableVertexAttribArray(ac++); pos += 12;

		glVertexAttribPointer(ac, 3, GL_FLOAT, GL_FALSE, stride4, (void*)pos);	//normals
		glEnableVertexAttribArray(ac++); pos += 12;

		glVertexAttribPointer(ac, 2, GL_FLOAT, GL_FALSE, stride4, (void*)pos);	//uvs
		glEnableVertexAttribArray(ac++); pos += 8;
			
		lastVBO = bufRef;
	}
	meshes[meshRef].render();
}

void SMmeshPool::uploadMeshesToGPU()
{	
	/* Once all the meshes are complete for the scene, 
	 * upload the geometry to the GPU */

	uint32_t vs = vertBuffer.size();
	VBOid.resize(vs);
	
	glGenBuffers(vs, &VBOid.front()); //create the buffer
	
	for (uint32_t i=0; i<vs; i++) {
		std::vector<float> &verts = vertBuffer[i];
		GLuint vno = verts.size();
		if (vno > 0) {
			SDL_Log("VBO:%d, verts:%d", VBOid[i], verts.size());
			glBindBuffer(GL_ARRAY_BUFFER, VBOid[i]); //Vertex buffer
			glBufferData(GL_ARRAY_BUFFER, vno * 4, &verts.front(), GL_STATIC_DRAW);
		}
	}
}
