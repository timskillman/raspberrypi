#ifndef __SMGL
#define __SMGL

//Load cross-platform dependencies
#include "SDL_syswm.h"

#ifdef __WINDOWS__
#include "GL/glew.h"
#endif /* __WINDOWS__ (*** must come before SDL.h ***) */ 

#include <SDL.h>
#include <vector>
#include <string>

#ifdef __IPHONEOS__
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#endif /* __IPHONEOS__ */

#ifdef __LINUX__
#include <SDL_opengles.h>
#include <SDL_opengl_glext.h>
#include <SDL_opengles2.h>
#include <SDL_opengles2_gl2ext.h>
#endif /* __LINUX__ */

#ifdef __WINDOWS__
#include <GL/gl.h>
#include <windows.h>
#endif /* __WINDOWS__ */

namespace SMGL
{
	void showGLinfo();
	void log(std::string const &str);
};

#endif
