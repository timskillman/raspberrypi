#include "SDL.h"

#include "../../../SharedCode/SMGL.h"
#include "../../../SharedCode/SMwin.h"
#include "../../../SharedCode/SMmeshPool.h"
#include "../../../SharedCode/SMscene.h"
#include "../../../SharedCode/SMshapes.h"
#include "../../../SharedCode/SMshader.h"
#include "../../../SharedCode/SMtexture.h"
#include "../../../SharedCode/SMmatrix.h"
#include "../../../SharedCode/SMfileOBJ.h"
#include "../../../SharedCode/SMbbox3d.h"
#include "../../../SharedCode/SMloadOptions.h"

#include <fstream>
#include <sstream>
#include <vector>

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

SMscene loadScreen(const char * file, SMmeshPool &meshes, uint32_t scx, uint32_t scy)
{
	char txtfile[255];
	float vpx,vpy,stepx,stepy;
	std::string line = "";
	std::ifstream opts(file);
	SMscene scene;
	SDL_Log("Loading screen '%s'", file);
	while (std::getline(opts, line)) {
		if (line != ""  && line[0] != '#') {
			std::sscanf(line.c_str(),"%s %f %f %f %f", &txtfile, &vpx, &vpy, &stepx, &stepy);
			std::string fname= &txtfile[0];
			scene.append2D(meshes, SMmodel(&meshes, SMshapes::sprite(vec2f(0, 0), vec2f(scx,scy), vec2f(vpx, vpy)), 0xffffffff), fname.c_str());
			scene.models2D[scene.models2D.size() - 1].material.animvec.x = stepx;
			scene.models2D[scene.models2D.size() - 1].material.animvec.y = stepy;
		}
	}
	return scene;
}

int main(int argc, char *argv[])
{
	// Work out console arguments ...
	std::string optfile = "options.txt";
	for (int i = 0; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "-screens" && (i+1)<argc) optfile = argv[i + 1];
	}

	// Set options ...
	loadOptions opts(optfile.c_str());
	
	vec3f pos;
	vec3f rot;

	SDL_Joystick* gGameController = NULL;

	SDL_Log("Resolution: %d x %d, Perspective: %d", opts.screenx, opts.screeny, opts.PerspValue);
		
    // Create and initialize a window
    uint32_t wflags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (opts.fullscreen) wflags = wflags | SDL_WINDOW_FULLSCREEN;
    static SDLwindow window(opts.title.c_str(), opts.screenx, opts.screeny, wflags, 2,0);
	SMGL::showGLinfo();
	
	if (SDL_NumJoysticks() < 1)
	{
		SDL_Log("Warning: No joysticks connected!\n");
	}
	else
	{
		//Load joystick
		gGameController = SDL_JoystickOpen(0);
		if (gGameController == NULL)
		{
			SDL_Log("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
		}
	}

	// Setup mesh pool ...
	static SMmeshPool meshes;
	std::string error;

	// Setup scene object for assembling scene ...
	std::vector<SMscene> scenes;

	// Load screen scenes ...
	scenes.push_back(loadScreen("screens/circs.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/face.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/hexes.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/doggy.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/eleclines.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/galaxy.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/waterhole.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/spindiamond.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/spiraldna.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/planetshake.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/spiradot.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/wobbleblob.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/hextunnel.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/trees.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/stars.scrn", meshes, opts.screenx, opts.screeny));
	scenes.push_back(loadScreen("screens/warphole.scrn", meshes, opts.screenx, opts.screeny));
	
	uint32_t screensel = 0;
	
	//int32_t ball = scene.loadModelOBJ(meshes, "models", "sphere.obj");
	meshes.uploadMeshesToGPU();


	//Setup shader ..
	SMshader shader("Shaders/vs1.glsl", "Shaders/fs1.glsl", "a_Position a_Normal a_UV", error);
	if (error != "") {
		window.showErrorMessage("Shader Error!", error);
		return 1;
	}
	shader.Use();
	shader.fogColour = opts.fogColour;
	shader.fogMaxDist = opts.fogFar;
	shader.fogMinDist = opts.fogNear;
	shader.setupBasicShader();
	
	// Setup matrices for shader ..
	SMmatrix scenemtx;
	SMmatrix pspmtx; 
	pspmtx.SetPerspective(opts.PerspValue, opts.nfzp.x, opts.nfzp.y);
	
	SMmatrix scene2D;
	SMmatrix orthoMtx;
	orthoMtx.SetOrtho(0, opts.screenx, opts.screeny, 0, 0.1f, 20.f);

	scenemtx.SetSceneMatrix(pos,rot);
	
	//Enable transparency ...
	window.setAlpha(true);
	window.setClearColour(0,0,0.2f);
	
	uint32_t fps = 0;
	uint32_t timer = SDL_GetTicks();
	uint32_t frames = 0;
	
	float floorHt = 0.f;
	float walkspeed = 0.f;

	vec3f ballpos(0,-15.f,0);
	vec3f balldir(0.8f,0,0.2f);
	vec3f vmove;

	uint32_t starttime = SDL_GetTicks();

	while (!window.hasquit())
	{

		//process key presses ...
		const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		vec3f nextpos = pos;
		if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_S]) {

			//if (keystate[SDL_SCANCODE_S])
		}
		
		if (keystate[SDL_SCANCODE_A]) {

		}
		if (keystate[SDL_SCANCODE_D]) {

		}
		if (keystate[SDL_SCANCODE_R]) {

		}
		if (keystate[SDL_SCANCODE_F]) {

		}
		if (keystate[SDL_SCANCODE_ESCAPE]) {
			window.setquit(true);
		}


		//process window events ...
		while (window.event()) {
			switch (window.ev.type)
			{
			case SDL_MOUSEBUTTONUP:
				screensel = (screensel + 1) % scenes.size();
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEWHEEL:
				break;
			case SDL_WINDOWEVENT:
				switch (window.ev.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					pspmtx.SetPerspective(opts.PerspValue, opts.nfzp.x, opts.nfzp.y);
					shader.setProjectionMatrix(pspmtx);
					break;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				SDL_Log("Button = %d", window.ev.jbutton.button);
				screensel=(screensel+1) % scenes.size();
				break;
			}
		}
		

		//clear background ...
		window.clear();
	
		//Render scene ...
		//scenemtx.SetSceneMatrix(pos, rot);
		//shader.setProjectionMatrix(pspmtx);
		//scene.renderSkybox(meshes, scenemtx, shader, -pos);
		//scene.render(meshes, scenemtx, shader);
		
		shader.setProjectionMatrix(orthoMtx);
		glClear(GL_DEPTH_BUFFER_BIT);
		//scene2.render2D(meshes, scene2D, shader);

		scenes[screensel].render2D(meshes, scene2D, shader);

		window.SwapBuffers();
		frames++;
		SDL_Delay(10);

	}

	float totalTime = (float)(SDL_GetTicks() - starttime) / 1000.f;

	SDL_JoystickClose(gGameController);
	gGameController = NULL;

	window.destroy();
	SDL_Log("FPS = %f", (float)frames / totalTime);
	return 0;
}

