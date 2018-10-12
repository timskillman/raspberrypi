#ifndef __SMTEXTURE
#define __SMTEXTURE

#include "SMGL.h"
#include <SDL_image.h>
#include <stdint.h>

class SMtexture
{
public:
	SMtexture();
	SMtexture(const char* file, bool upload = true);
	~SMtexture();

	void			LoadFromFile(const char* file);
	virtual void    Upload();

	inline uint32_t GetWidth() const { return width; }
	inline uint32_t GetHeight() const { return height; }
	inline uint16_t GetDepth() const { 
		return (uint16_t)bpp; 
	}
	inline uint8_t * GetTexels() { return pixels; };
	inline uint32_t  GetPitch() const { return pitch;  }
	void ChangeTexels(uint8_t * pTexels, GLenum format);
	void createColour(uint32_t col);

	uint32_t textureID;
	
private:
	void init();
	
	uint8_t * pixels;
	int bpp;
	GLenum format;
	int width;
	int height;
	int pitch;
	size_t size;
};

#endif
