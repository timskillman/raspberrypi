#include "SMfileOBJ.h"
#include "SMmesh.h"
#include <vector>
#include <fstream>

#define MAXVALS 10000	//vertice block size (increases by this amount if more to read)

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

namespace SMfileOBJ {

	vec3f crossProduct(vec3f b, vec3f c) {
		return vec3f(b.y*c.z - c.y*b.z, b.z*c.x - c.z*b.x, b.x*c.y - c.x*b.y);
	}

	vec3f unitNormal(vec3f v0, vec3f v1, vec3f v2) {
		vec3f n = crossProduct(v1 - v0, v2 - v0);
		float dist = sqrtf(n.x*n.x + n.y*n.y + n.z*n.z);
		if (dist > 0.0f) n = n / dist;
		return n;
	}

	void addMesh(SMmeshPool *meshes, SMmesh &mesh, bool asCollider)
	{
		SDL_Log("MeshPool %d,%d,%d,%d,%d",meshes->meshes.size(),meshes->materials.size(),meshes->textures.size(),meshes->vertBuffer.size(),meshes->currentBuffer);
		mesh.mode = GL_TRIANGLES;
		mesh.bbox.bboxFromVerts(mesh.verts, 0, mesh.vc, mesh.stride);
		meshes->addMesh(mesh, !asCollider);
		SDL_Log("Mesh %d: verts = %d,%d, material = %d", mesh.materialRef, mesh.vertOffset, mesh.vertSize, mesh.materialRef);
	}
	
	///// Load OBJ with Material support //////////////////////////////////////////////////////////////////

	std::string getLine(std::string &filestr, uint32_t &ptr)
	{
		return filestr.substr(ptr, filestr.find('\n', ptr));
		uint32_t i = filestr.find('\n', ptr);
		std::string line = filestr.substr(ptr,i);
		ptr += i;
		return line;
	}

	void open(std::string path, std::string filename, SMmeshPool *meshes, bool asCollider, std::string &error)
	{
		int v1, v2, v3, uv1, uv2, uv3, n1, n2, n3;

		std::vector<vec3f> temp_vertices;
		temp_vertices.resize(MAXVALS);
		std::vector<vec3f> temp_normals;
		temp_normals.resize(MAXVALS);
		std::vector<vec2f> temp_uvs;
		temp_uvs.resize(MAXVALS);
		
		//uint32_t &vc = meshes->vertCount;
		uint32_t meshStart = meshes->meshCount();

		if (meshes->materials.size()==0) createDefaultMaterial(meshes);
		uint32_t materialStart = meshes->materials.size();
				
		bool readIndx = false;
		int readType = 0;

		char ch = *(path.end()-1);
		if (path.size()>0 && ch!='/') path += "/";
		std::string filepath = path + filename;


		std::string line;

		vec3f vertex;
		vec2f texcoords;
		vec3f normal;
		uint32_t tv = 0; uint32_t tn = 0; uint32_t tu = 0;
		uint32_t otv = 0; uint32_t otn = 0; uint32_t otu = 0;
		//uint32_t lineno = 0;
		uint32_t meshc = 0;
		std::string materialName = "";
		std::string vals = "";
		std::string s = "";
		std::string com = "";
		std::string matlibname = "";
		
		std::vector<SMmesh> meshModel;	//models are setup by material count
		
		int32_t materialfound = -1;
		GLfloat * cols = NULL; //[4] = { 0.2f, 0.2f, 1.f, 1.f };
		bool readLine = true;
		uint32_t tri = 0;

		uint32_t tm = SDL_GetTicks();

		// Open OBJ file as string ...
		std::string filestr = "";
		SDL_RWops *rw = SDL_RWFromFile(filepath.c_str(), "r");
		if (rw == NULL) return;
		uint64_t fsize = SDL_RWsize(rw);
		filestr.resize(fsize);
		SDL_RWread(rw, &filestr[0], fsize, 1);
		SDL_RWclose(rw);
		
		uint32_t ptr = 0;
		while (ptr<filestr.size())
		{
			if (readLine) {
				line = filestr.substr(ptr, filestr.find_first_of('\n', ptr) - ptr);
				ptr += line.size()+1;
			}
			else 
				readLine = true;
				
			com = line.substr(0, line.find(" "));
			if (com == "") com = line;
			
			//lineno++;

			if (com != "" && com !="#" && line.length()>1)
			{
				if (com.size() < line.size()) 
					vals = line.substr(com.size() + 1, line.size() - com.size() - 1); else vals = "";
					vals.erase(vals.find_last_not_of(" \n\r\t") + 1);

				//SDL_Log("%s(%d): %s",com.c_str(),com.size(), vals.c_str());
				
				if (com == "v") {
					if (tv+1 > temp_vertices.size()) temp_vertices.resize(temp_vertices.size()+MAXVALS);
					sscanf(vals.c_str(), "%f %f %f", &temp_vertices[tv].x, &temp_vertices[tv].y, &temp_vertices[tv].z);
					tv++;
				}
				else if (com == "vt") {
					if (tu+1 > temp_uvs.size()) temp_uvs.resize(temp_uvs.size()+MAXVALS);
					sscanf(vals.c_str(), "%f %f", &temp_uvs[tu].x, &temp_uvs[tu].y);
					tu++;
				}
				else if (com == "vn") {
					if (tn+1 > temp_normals.size()) temp_normals.resize(temp_normals.size()+MAXVALS);
					sscanf(vals.c_str(), "%f %f %f", &temp_normals[tn].x, &temp_normals[tn].y, &temp_normals[tn].z);
					tn++;
				}
				else if (com == "f")
				{
					if (!readIndx) {
						if (tv > otv && tu > otu && tn > otn)
							readType = 0;
						else if (tv > otv && tu == otu && tn > otn)
							readType = 1;
						else if (tv > otv && tu == otu && tn == otn)
							readType = 2;
						else if (tv > 0 && tu > otu && tn == otn)
							readType = 3;
						readIndx = true;
						otv = tv; otu = tu; otn = tn;
					}
					
					if (materialfound<0) {
						SDL_Log("No material found - using default one");
						materialfound = 0;
						meshModel.resize(1);
						meshModel[0].materialRef = 0;
					}
					
					switch (readType) {
					case 0:
						sscanf(vals.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &uv1, &n1, &v2, &uv2, &n2, &v3, &uv3, &n3);
						meshModel[materialfound].addPackedVert2(temp_vertices[v3 - 1], temp_normals[n3 - 1], temp_uvs[uv3 - 1], cols);
						meshModel[materialfound].addPackedVert2(temp_vertices[v2 - 1], temp_normals[n2 - 1], temp_uvs[uv2 - 1], cols);
						meshModel[materialfound].addPackedVert2(temp_vertices[v1 - 1], temp_normals[n1 - 1], temp_uvs[uv1 - 1], cols);
						tri++;
						break;
					case 1:
						sscanf(vals.c_str(), "%d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
						meshModel[materialfound].addPackedVert2(temp_vertices[v3 - 1], temp_normals[n3 - 1], vec2f(0, 0), cols);
						meshModel[materialfound].addPackedVert2(temp_vertices[v2 - 1], temp_normals[n2 - 1], vec2f(1.f, 0), cols);
						meshModel[materialfound].addPackedVert2(temp_vertices[v1 - 1], temp_normals[n1 - 1], vec2f(0, 1.f), cols);
						tri++;
						break;
					case 2:
						sscanf(vals.c_str(), "%d %d %d", &v1, &v2, &v3); //contains vertices only
						normal = unitNormal(temp_vertices[v1 - 1], temp_vertices[v2 - 1], temp_vertices[v3 - 1]); //create a surface normal
						meshModel[materialfound].addPackedVert2(temp_vertices[v3 - 1], normal, vec2f(0, 0), cols);
						meshModel[materialfound].addPackedVert2(temp_vertices[v2 - 1], normal, vec2f(1.f, 0), cols);
						meshModel[materialfound].addPackedVert2(temp_vertices[v1 - 1], normal, vec2f(0, 1.f), cols);
						tri++;
						break;
					case 3: //verts,uv's
						sscanf(vals.c_str(), "%d/%d %d/%d %d/%d", &v1, &uv1, &v2, &uv2, &v3, &uv3);
						normal = unitNormal(temp_vertices[v1 - 1], temp_vertices[v2 - 1], temp_vertices[v3 - 1]); //create a surface normal
						meshModel[materialfound].addPackedVert2(temp_vertices[v3 - 1], normal, temp_uvs[uv3 - 1], cols);
						meshModel[materialfound].addPackedVert2(temp_vertices[v2 - 1], normal, temp_uvs[uv2 - 1], cols);
						meshModel[materialfound].addPackedVert2(temp_vertices[v1 - 1], normal, temp_uvs[uv1 - 1], cols);
						tri++;
						break;
					}
				}
				else if (com == "mtllib")
				{
					matlibname = vals;
					getMatLib(path, matlibname, meshes);
					
					//setup models that will load by material ..
					meshModel.resize(meshes->materials.size());
					
				}
				else if ((com == "usemtl") || (com == "usemtl(null)"))
				{

					//Find and set material ...
					materialName = matlibname+"_"+vals;
					materialfound = -1;
					
					//SDL_Log("Use material: %s, %d", vals.c_str(),materials.size());
					
					for (size_t i = 0; i < meshes->materials.size() && (materialfound < 0); i++) {
						if (materialName == meshes->materials[i].name) materialfound = i;
					}

					if (materialfound >= 0) {
						SDL_Log("Material found %d, %s", materialfound, materialName.c_str());
						meshModel[materialfound].materialRef = materialfound; //passed back to model
						if (meshModel[materialfound].name == "") {
							SDL_Log("Add new mesh");
							meshModel[materialfound].name = "mesh_"+vals; //Set new mesh name
						}
					} else
					{
						SDL_Log("ERROR! - Material '%s' not found", materialName.c_str());
						return;
					}
					
					readIndx = false;

				}

			}
		}

		SDL_Log("Creating meshes ...");
		
		//Add all models with no alpha materials
		for (size_t i=0; i<meshModel.size(); i++) {
			if (meshModel[i].verts.size()>0 && meshes->materials[meshModel[i].materialRef].alpha >= 1.0f) {
				addMesh(meshes, meshModel[i], asCollider);
			}
		}
		
		//Lastly, add all models with alpha materials (else z depth of alpha will obscure non-alpha objects)
		//However, overlapping alpha's will obscure each other unless z-depth is turned off
		for (size_t i=0; i<meshModel.size(); i++) {
			if (meshModel[i].verts.size() > 0 && meshes->materials[meshModel[i].materialRef].alpha < 1.0f) {
				addMesh(meshes, meshModel[i], asCollider);
			}
		}
		
		SDL_Log("Total time = %d, triangles = %d, mesh parts = %d, total = %d", (SDL_GetTicks() - tm) / 1000, tri, meshes->meshCount() - meshStart, meshes->meshCount()); //, bbox.width(),bbox.height(),bbox.depth());

	}

	void getMatLib(std::string path, std::string name, SMmeshPool* meshes)
	{
		std::string line;
		SMmaterial objmat;
		SDL_Log("Loading material '%s'", (path+name).c_str());
		std::ifstream matfile(path + name, std::ifstream::in);
		if (!matfile.is_open()) {
			SDL_Log("*** Material '%s' not found! ***",name.c_str());
			matfile.close();
			return;
		}

		while (!matfile.eof())
		{
			std::getline(matfile, line);
			//SDL_Log("%s",line.c_str());
			int lo = line.find(" ");
			if (lo > 0) {
				std::string com = line.substr(0, lo);

				if (com != "" && com != "#" && line.size() > 1)
				{
					std::string vals = line.substr(com.size() + 1, line.size() - com.size() - 1);

					if (com == "newmtl") {
						if (objmat.name != "") meshes->materials.push_back(objmat);
						objmat.init(name+"_"+vals);
					}
					else if (com == "Ka")
						sscanf(vals.c_str(), "%f %f %f", &objmat.colAmbient.x, &objmat.colAmbient.y, &objmat.colAmbient.z);
					else if (com == "Kd")
						sscanf(vals.c_str(), "%f %f %f", &objmat.colDiffuse.x, &objmat.colDiffuse.y, &objmat.colDiffuse.z);
					else if (com == "Ks")
						sscanf(vals.c_str(), "%f %f %f", &objmat.colSpecular.x, &objmat.colSpecular.y, &objmat.colSpecular.z);
					else if (com == "Ke")
						sscanf(vals.c_str(), "%f %f %f", &objmat.colEmissive.x, &objmat.colEmissive.y, &objmat.colEmissive.z);
					else if (com == "Tr" || com == "d")
						sscanf(vals.c_str(), "%f", &objmat.alpha);
					else if (com == "illum")
						sscanf(vals.c_str(), "%d", &objmat.illum);
					else if (com == "anim")
						sscanf(vals.c_str(), "%f %f", &objmat.animvec.x, &objmat.animvec.y);
					else if (com == "map_Kd") {
						objmat.texID = -1;
						objmat.texRef = meshes->loadTexture(path, vals, objmat.texID);
						if (objmat.texRef>=0) {
							objmat.texName = vals;
							if (meshes->textures[objmat.texID]->GetDepth() == 4) 
								objmat.alpha = 0.999f; //if 32bit, then mark as alpha channel image
						}
					}
					else if (com == "map_Bump") {
						objmat.texID = -1;
						objmat.texBumpRef = meshes->loadTexture(path, vals, objmat.texID);
						if (objmat.texBumpRef >=0)
							objmat.texBumpName = vals;
					}
				}
			}
		}
		if (objmat.texRef<0) {
			//if no texture, then apply default texture
			objmat.texRef = meshes->materials[0].texRef;
			objmat.texID = meshes->materials[0].texID;
		} else
		if (objmat.texID >= 0) {
			int32_t bpp = meshes->textures[objmat.texID]->GetDepth();
			if (bpp == 4) 
				objmat.alpha = 0.999f;
		}
		if (objmat.name != "") meshes->materials.push_back(objmat);
		matfile.close();
		SDL_Log("Finished loading material");
	}

	void createDefaultMaterial(SMmeshPool* meshes)
	{
		SMmaterial objmat;
		objmat.init("default");
		objmat.texID = -1;
		objmat.texRef = meshes->loadTexture("models/maps/", "white.png", objmat.texID);
		if (objmat.texRef >= 0)
			objmat.texName = "white.png";
		meshes->materials.push_back(objmat);
	}
}

