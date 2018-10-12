#ifndef __SMFILEOBJ
#define __SMFILEOBJ

#include "SMmeshPool.h"

namespace SMfileOBJ 
{
	void open(std::string path, std::string name, SMmeshPool *meshes, bool asCollider, std::string &error);
	void getMatLib(std::string path, std::string name, SMmeshPool* meshes);
	void createDefaultMaterial(SMmeshPool* meshes);
}

#endif
