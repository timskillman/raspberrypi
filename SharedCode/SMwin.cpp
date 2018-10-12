#include "SMwin.h"
#include <sstream>

SDLwindow::SDLwindow() {
	mWindow = NULL;
	mMouseFocus = false;
	mKeyboardFocus = false;
	mFullScreen = false;
	mMinimized = false;
	mWidth = 0;
	mHeight = 0;
	mousex = 0;
	mousey = 0;
	mousedown = false;
	mousedrag = false;
	mouseup = false;
	mouseXY = vec2f(0.f,0.f);
	oldMouseXY = vec2f(0.f,0.f);
	mouseDeltaXY = vec2f(0.f,0.f);
	clearColour = vec3f(0.f,0.f,0.f);
	wasDragging = false;
	quit = false;
}

SDLwindow::SDLwindow(const char * title, int width, int height, uint32_t flags, uint8_t majvsn, uint8_t minvsn) {
	if (init(title, width, height, flags, majvsn, minvsn)) {
		GLcontext = SDL_GL_CreateContext(mWindow);
		SDL_GL_SetSwapInterval(1);
		SDL_GL_MakeCurrent(mWindow, GLcontext);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

#ifdef __WINDOWS__
		glewExperimental = GL_TRUE;
		GLenum glew_status = glewInit();
#endif

	}
}

bool SDLwindow::init(const char * title, int width, int height, uint32_t flags, uint8_t majvsn, uint8_t minvsn) {
	//Create window
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		SDL_Log("Unable to initialize SDL");
		return false;
	}

	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); //Turn on anti-aliasing (before creating the SDL window)
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	//SDL_SetHint(SDL_HINT_VIDEO_DOUBLE_BUFFER,"1");
	
	mWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
	if (mWindow != NULL)
	{
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = width;
		mHeight = height;
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majvsn);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minvsn);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		//graphics = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	}
	else
	{
		SDL_Log("Window error %d", SDL_GetError());
	}

	return mWindow != NULL;
}

int SDLwindow::event()
{
	int p = SDL_PollEvent(&ev);
	if (p) {
		switch (ev.type) {
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_MOUSEMOTION:
			mousex = ev.motion.x;
			mousey = ev.motion.y;
			mouseXY = vec2f((float)ev.motion.x,(float)ev.motion.y);
			mouseDeltaXY = mouseXY-oldMouseXY;
			oldMouseXY = mouseXY;
			wasDragging = false;
			mouseup = false;
			if (mousedown) mousedrag = true;
			break;
		case SDL_KEYDOWN:
			keyPress = ev.key.keysym.scancode;
			break;
		case SDL_MOUSEWHEEL:
			mousewheel = ev.motion.x;
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (ev.button.button) {
			case SDL_BUTTON_LEFT:
				if (!mousedown) {
					mousex = ev.button.x;
					mousey = ev.button.y;
					mousedown = true;
					mousedrag = false;
					mouseup = false;
					wasDragging = false;
				}
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (ev.button.button) {
			case SDL_BUTTON_LEFT:
				wasDragging = mousedrag;
				mousex = ev.button.x;
				mousey = ev.button.y;
				mousedown = false;
				mousedrag = false;
				mouseup = true;
				break;
			case SDL_BUTTON_RIGHT:
				quit = true;
				break;
			}
			break;
		case SDL_WINDOWEVENT:
			switch (ev.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				//Get new dimensions and repaint on window size change
				mWidth = ev.window.data1;
				mHeight = ev.window.data2;
				glViewport(0, 0, mWidth, mHeight);
				break;

			case SDL_WINDOWEVENT_ENTER:
				mMouseFocus = true;
				break;

				//Mouse left window
			case SDL_WINDOWEVENT_LEAVE:
				mMouseFocus = false;
				break;

				//Window has keyboard focus
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				mKeyboardFocus = true;
				break;

				//Window lost keyboard focus
			case SDL_WINDOWEVENT_FOCUS_LOST:
				mKeyboardFocus = false;
				break;

				//Window minimized
			case SDL_WINDOWEVENT_MINIMIZED:
				mMinimized = true;
				break;

				//Window maxized
			case SDL_WINDOWEVENT_MAXIMIZED:
				mMinimized = false;
				break;

				//Window restored
			case SDL_WINDOWEVENT_RESTORED:
				mMinimized = false;
				break;

			}
		}
	}
	return p;
}

void SDLwindow::destroy()
{
	//SDL_DestroyRenderer(graphics);

	if (mWindow != NULL)
	{
		SDL_DestroyWindow(mWindow);
		SDL_GL_DeleteContext(GLcontext);
	}
	SDL_Quit();
}

int SDLwindow::getWidth()
{
	return mWidth;
}

int SDLwindow::getHeight()
{
	return mHeight;
}

bool SDLwindow::hasMouseFocus()
{
	return mMouseFocus;
}

bool SDLwindow::hasKeyboardFocus()
{
	return mKeyboardFocus;
}

void SDLwindow::forceMouseUp()
{
	mousedown = false;
	mousedrag = false;
}

void SDLwindow::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SDLwindow::setAlpha(bool useAlpha)
{
	if (useAlpha) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
	} 
	else {
		glDisable(GL_BLEND);
	}
}

bool SDLwindow::isMinimized()
{
	return mMinimized;
}

bool SDLwindow::hasquit()
{
	return quit;
}

void SDLwindow::setCaption(std::string caption)
{
	SDL_SetWindowTitle(mWindow, caption.c_str());
}

void SDLwindow::showErrorMessage(std::string heading, std::string error)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, heading.c_str(), error.c_str(), mWindow);	
}

std::string SDLwindow::getSystemInfo()
{
	//Must be called after the creation of the SDL_window

	SDL_SysWMinfo info;
	std::string subsystem = "Unknown";

	SDL_VERSION(&info.version); /* initialize info structure with SDL version info */

	if (SDL_GetWindowWMInfo(mWindow, &info)) { /* the call returns true on success */
											  /* success */
		switch (info.subsystem) {
		case SDL_SYSWM_UNKNOWN:   break;
		case SDL_SYSWM_WINDOWS:   subsystem = "Microsoft Windows(TM)";  break;
		case SDL_SYSWM_X11:       subsystem = "X Window System";        break;
#if SDL_VERSION_ATLEAST(2, 0, 3)
		case SDL_SYSWM_WINRT:     subsystem = "WinRT";                  break;
#endif
		case SDL_SYSWM_DIRECTFB:  subsystem = "DirectFB";               break;
		case SDL_SYSWM_COCOA:     subsystem = "Apple OS X";             break;
		case SDL_SYSWM_UIKIT:     subsystem = "UIKit";                  break;
#if SDL_VERSION_ATLEAST(2, 0, 2)
		case SDL_SYSWM_WAYLAND:   subsystem = "Wayland";                break;
		case SDL_SYSWM_MIR:       subsystem = "Mir";                    break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 4)
		case SDL_SYSWM_ANDROID:   subsystem = "Android";                break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 5)
		case SDL_SYSWM_VIVANTE:   subsystem = "Vivante";                break;
		//case SDL_SYSWM_OS2:		  subsystem = "OS2";					break;
#endif
		}

		SDL_Log("This program is running SDL version %d.%d.%d on %s",
			(int)info.version.major,
			(int)info.version.minor,
			(int)info.version.patch,
			subsystem.c_str());
	}
	else {
		/* call failed */
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't get window information: %s", SDL_GetError());
	}

	return subsystem;
}

#ifdef __WINDOWS__
//windows specific functions
HWND SDLwindow::getHwnd()
{
	struct SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	SDL_GetWindowWMInfo(mWindow, &info);
	HWND hwnd = info.info.win.window;
	return hwnd;
}

void SDLwindow::createWindowsMenu()
{
	HWND myWindow;
	HMENU myMenu;

	myWindow = getHwnd();
	SDL_Log("HWND = %d", myWindow);
	myMenu = LoadMenu(NULL, MAKEINTRESOURCE(102));
	SetMenu(myWindow, myMenu);
}

#endif

