#ifndef __SMMATERIAL
#define __SMMATERIAL

#include "SMGL.h"
#include "SMvector.h"
#include <string>

class SMmaterial {
public:
	SMmaterial();

	void init(std::string name = "");
	void SetColAmbient(uint32_t colour);
	void SetColAmbient(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void SetColEmissive(uint32_t colour);
	void SetColEmissive(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void SetColDiffuse(uint32_t colour);
	void SetColDiffuse(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void SetColSpecular(uint32_t colour);
	void SetColSpecular(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void SetSpecularPower(uint32_t s) { shininess = (float)s / 255.0f; }
	void SetAlphaMode(GLenum mode) { }
	void SetCullMode(int32_t cm) { cullmode = (cm == 1); }

	enum {
		ALPHA_NONE = 0,
		ALPHA_HALF = 1,
		ALPHA_ADD = 2,
		ALPHA_SUB = 3,
		ALPHA_BLEND = 4,
		ALPHA_DEFAULT = 5
	};

	enum {
		CULL_FRONT,
		CULL_BACK,
		CULL_NONE
	};

	std::string name;
	int illum;
	
	std::string texName;
	int32_t texRef; //GL ptr to texture ref
	int32_t texID; //ref to stored texture in meshpool
	std::string texBumpName;
	int32_t texBumpRef;
	
	vec2f animvec; 			//cheap texture animation incrementals
	vec2f animoffset;  		//cheap texture animation offset
	vec2f animsize;

//private:
	vec4f colAmbient;
	vec4f colDiffuse;
	vec4f colEmissive;
	vec4f colSpecular;
	float shininess;
	float alpha;
	float alphaTestValue;
	bool cullmode;

};

#endif
