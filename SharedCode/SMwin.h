#ifndef __SDLWINDOW
#define __SDLWINDOW

#include <stdint.h>
#include <string>
#include "SMGL.h"
#include "SMvector.h"

class SDLwindow {
public:
	SDLwindow();
	SDLwindow(const char * title, int width, int height, uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN, uint8_t majvsn = 1, uint8_t minvsn = 2);

	bool init(const char * title, int width, int height, uint32_t flags, uint8_t majvsn, uint8_t minvsn);
	void setCaption(std::string caption);
	void destroy();
	int event();
	void SwapBuffers() { SDL_GL_SwapWindow(handle()); }
	void setClearColour(float red, float green, float blue) { clearColour = vec3f(red,green,blue); glClearColor(red,green,blue, 1.f); }
	void clear();
	void setAlpha(bool v);
	
	int getWidth();
	int getHeight();
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	void forceMouseUp();
	bool isMinimized();
	bool hasquit();
	void setquit(bool q) { quit = q; }
	bool keydown(SDL_Scancode key) { return (key == keyPress); }
	SDL_Scancode getKey() { return keyPress; }
	
	std::string getSystemInfo();
	void showErrorMessage(std::string heading, std::string error);

#ifdef __WINDOWS__
	//windows specific functions
	HWND getHwnd();
	void createWindowsMenu();
#endif

	SDL_Window* handle() { return mWindow; }

	SDL_Renderer* graphics;
	SDL_Event ev;

	int mousex;
	int mousey;
	int mousewheel;
	bool mousedown;
	bool mousedrag;
	bool mouseup;
	bool wasDragging;
	vec2f mouseDeltaXY;
	vec2f mouseXY;
	vec2f oldMouseXY;

	//Window dimensions
	int mWidth;
	int mHeight;
	
private:
	SDL_Window* mWindow;
	SDL_GLContext GLcontext;
	SDL_Scancode keyPress;

	vec3f clearColour;
	
	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;

	bool quit;
};

#endif
