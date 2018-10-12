#include "SMGL.h"

namespace SMGL {

	void log(std::string const &str)
	{
		SDL_Log(str.c_str());
	}

	static void PrintGLString(GLenum name)
	{
		const GLubyte *ret = glGetString(name);
		if (ret!=0) SDL_Log("%s\n",ret);
	}

	void showGLinfo()
	{
		PrintGLString(GL_VERSION);
	
		int devices = SDL_GetNumRenderDrivers();
		for (int i=0; i<devices; i++)
		{
			SDL_RendererInfo renderInfo;
			SDL_GetRenderDriverInfo(i, &renderInfo);
			SDL_Log("%d: %s, accelerated = %d",i,renderInfo.name, (renderInfo.flags & SDL_RENDERER_ACCELERATED) != 0);
		}
	}
}
