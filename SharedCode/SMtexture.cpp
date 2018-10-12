#include "SMtexture.h"


SMtexture::SMtexture()
{
	init();
}

SMtexture::SMtexture(const char* file, bool upload)
{
	init();
	LoadFromFile(file);
	if (upload) Upload();
}

SMtexture::~SMtexture()
{
	if (textureID != 0) glDeleteTextures(1, &textureID);
	if (pixels) delete pixels;
}

void SMtexture::init()
{
	textureID = 0;
	pixels = nullptr;
	width = 0;
	height = 0;
	bpp = 0;
	format = 0;
}

void SMtexture::createColour(uint32_t col)
{
	textureID = 0;
	width = 1;
	height = 1;
	pitch = 4;
	bpp = 4;
	size = 4;
	pixels = new uint8_t[size];
	memcpy(pixels, &col, size);
	format = GL_RGBA;

}

void SMtexture::LoadFromFile(const char* file)
{
	textureID = 0;

	SDL_Surface* Surface = IMG_Load(file);
	if (Surface == NULL)
		SDL_ShowSimpleMessageBox(0, "Load image error", IMG_GetError(), NULL);
		
	if (Surface) {
		width = Surface->w;
		height = Surface->h;
		pitch = Surface->pitch;
		bpp = Surface->format->BytesPerPixel;
		size = height * pitch;
		
		pixels = new uint8_t[size];  //new GLubyte[width*height]()
		if (pixels) {
			memcpy(pixels, Surface->pixels, size);
			if (bpp==3) format = GL_RGB;
			else if (bpp==4)
#ifdef __WINDOWS__
				format = GL_RGBA;
#elif	__LINUX__
				format = GL_RGBA;
#elif  __APPLE__
				format = GL_BGRA;
#endif
		}
		SDL_FreeSurface(Surface);
	}
}

void SMtexture::Upload()
{
	if (textureID == 0 && pixels) {
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void SMtexture::ChangeTexels(uint8_t * texels, GLenum format) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, texels);
}
