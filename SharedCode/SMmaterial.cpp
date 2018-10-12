#include "SMmaterial.h"

//----- SMmaterial -----------------------------------------------------------------------------------------------------------------

SMmaterial::SMmaterial()
{
	init("");
}

void SMmaterial::init(std::string name)
{
	colAmbient = vec4f(0.3f,0.3f,0.3f,1.0f);
	colDiffuse = vec4f(1.0f,1.0f,1.0f,1.0f);
	colSpecular = vec4f(0.3f,0.3f,0.3f,1.0f);
	colEmissive = vec4f(1.0f,1.0f,1.0f,1.0f);
	shininess = 0.2f;
	alpha = 1.0f;
	alphaTestValue = 0.f; //no test
	texName = ""; texRef = -1; cullmode = 0; illum = 2; texID = -1;
	texBumpName = ""; texBumpRef = -1;
	animvec = vec2f(0.f,0.f); animoffset = vec2f(0.f,0.f);
	animsize = vec2f(1.f, 1.f);
	this->name = name;
}

void SMmaterial::SetColAmbient(uint32_t colour)
{
	colAmbient = vec4f((GLfloat)(colour & 255) / 255.f, (GLfloat)((colour >> 8) & 255) / 255.f,(GLfloat)((colour >> 16) & 255) / 255.f, (GLfloat)((colour >> 24) & 255) / 255.f);
}

void SMmaterial::SetColAmbient(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	colAmbient = vec4f((GLfloat)r / 255.f, (GLfloat)g / 255.f,(GLfloat)b / 255.f, (GLfloat)a / 255.f);
}

void SMmaterial::SetColEmissive(uint32_t colour)
{
	colEmissive = vec4f((GLfloat)(colour & 255) / 255.f, (GLfloat)((colour >> 8) & 255) / 255.f,(GLfloat)((colour >> 16) & 255) / 255.f, (GLfloat)((colour >> 24) & 255) / 255.f);
}

void SMmaterial::SetColEmissive(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	colEmissive = vec4f((GLfloat)r / 255.f, (GLfloat)g / 255.f,(GLfloat)b / 255.f, (GLfloat)a / 255.f);
}

void SMmaterial::SetColDiffuse(uint32_t colour)
{
	colDiffuse = vec4f((GLfloat)(colour & 255) / 255.f, (GLfloat)((colour >> 8) & 255) / 255.f,(GLfloat)((colour >> 16) & 255) / 255.f, (GLfloat)((colour >> 24) & 255) / 255.f);
}

void SMmaterial::SetColDiffuse(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	colDiffuse = vec4f((GLfloat)r / 255.f, (GLfloat)g / 255.f,(GLfloat)b / 255.f, (GLfloat)a / 255.f);
}

void SMmaterial::SetColSpecular(uint32_t colour)
{
	colSpecular = vec4f((GLfloat)(colour & 255) / 255.f, (GLfloat)((colour >> 8) & 255) / 255.f,(GLfloat)((colour >> 16) & 255) / 255.f, (GLfloat)((colour >> 24) & 255) / 255.f);
}

void SMmaterial::SetColSpecular(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	colSpecular = vec4f((GLfloat)r / 255.f, (GLfloat)g / 255.f,(GLfloat)b / 255.f, (GLfloat)a / 255.f);
}
