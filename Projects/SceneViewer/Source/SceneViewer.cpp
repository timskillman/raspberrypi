
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


int main(int argc, char *argv[])
{
	// Work out console arguments ...
	std::string optfile = "options.txt";
	for (int i = 0; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "-scene" && (i+1)<argc) optfile = argv[i + 1];
	}

	// Set options ...
	loadOptions opts(optfile.c_str());
	
	bool fly = opts.fly;
	float avatarHeight = opts.avatarHeight;
	float avatarFallSpeed = opts.avatarFallSpeed;
	float avatarWalkSpeed = opts.avatarWalkSpeed;
	float avatarRunSpeed = opts.avatarRunSpeed;
	vec3f pos = opts.pos;
	vec3f rot = opts.rot;

	SDL_Log("Resolution: %d x %d, Perspective: %d", opts.screenx, opts.screeny, opts.PerspValue);
		
    // Create and initialize a window
    uint32_t wflags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (opts.fullscreen) wflags = wflags | SDL_WINDOW_FULLSCREEN;
    static SDLwindow window("Scene Viewer OpenGL ES 2.0", opts.screenx, opts.screeny, wflags, 2,0); //| SDL_WINDOW_FULLSCREEN
	SMGL::showGLinfo();
	
	// Setup mesh pool ...
	static SMmeshPool meshes;
	std::string error;

	// Load models from modelFiles and upload vertex buffer object to GPU ..
	SMscene scene;
	int32_t skybox = scene.loadModelOBJ(meshes,opts.skyboxPath, opts.skybox);
	//int32_t cargoHold = scene.loadModelOBJ(meshes, "models", "CargoHoldBaked2.obj");
	//int32_t cargoHoldCollider = scene.loadColliderOBJ(meshes, "models", "CargoHold_CollisionBig.obj");

	//scene.appendModel(SMmodel(&meshes, SMshapes::cuboid(vec3f(0, 0, 0), vec3f(20, 20, 20)), 0xff0000ff));
	if (opts.logo != "") {
		SDL_Log("Loading logo");
		scene.append2D(meshes, SMmodel(&meshes, SMshapes::sprite(vec2f(50, 50), vec2f(120, 80)), 0xffffffff), opts.modelPath + "/" + opts.logo);
	}
	//scene.appendModel(SMmodel(&meshes, SMshapes::torus(vec3f(0, 0, 0), 50.f, 10.f), 0xff00ffff));
	//scene.appendModel(SMmodel(&meshes, SMshapes::sphere(vec3f(0, 30, 0), 10.f), 0xffffff00));

	//Create as colliders ...
	//scene.appendCollider(SMmodel(&meshes, SMshapes::cuboid(vec3f(0, 0, 0), vec3f(20, 20, 20)), 0xff0000ff, true));
	//scene.appendCollider(SMmodel(&meshes, SMshapes::plane(vec3f(0, 0, 0), vec3f(40, 40, 40)), 0xff00ff00, true));
	//scene.appendCollider(SMmodel(&meshes, SMshapes::torus(vec3f(0, 0, 0), 50.f, 10.f), 0xff00ffff, true));
	//scene.appendCollider(SMmodel(&meshes, SMshapes::sphere(vec3f(0, 30, 0), 10.f, 0, 12, 12), 0xffffff00, true));

	//SMmodel cube;
	//cube.appendMesh(&meshes, SMshapes::cuboid(vec3f(0, 0, 0), vec3f(20, 20, 20), 0), false);
	//scene.appendModel(cube);

	int32_t model = scene.loadModelOBJ(meshes, opts.modelPath, opts.model);
	int32_t collider = scene.loadColliderOBJ(meshes, opts.modelPath, opts.collider);

	//int32_t ball = scene.loadModelOBJ(meshes, "models", "sphere.obj");
	meshes.uploadMeshesToGPU();

	scene.models[skybox].matrix.SetScale(opts.skyboxScale);


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
	//shader.setProjectionMatrix(pspmtx);
	
	SMmatrix scene2D;
	SMmatrix orthoMtx;
	orthoMtx.SetOrtho(0, 800, 600, 0, 0.1f, 20.f);

	scenemtx.SetSceneMatrix(pos,rot);
	
	//Enable transparency ...
	window.setAlpha(true);
	window.setClearColour(0,0,0.2f);
	
	uint32_t fps = 0;
	uint32_t timer = SDL_GetTicks();
	uint32_t lastfps = 0;
	
	float floorHt = 0.f;
	float walkspeed = 0.f;

	vec3f ballpos(0,-15.f,0);
	vec3f balldir(0.8f,0,0.2f);
	vec3f vmove;



	while (!window.hasquit())
	{
		//process key presses ...

		const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		vec3f nextpos = pos;
		if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_S]) {
			if (fly) {
				vmove = vec3f(sinf(rot.y)*-cosf(rot.x), -sinf(rot.x), cosf(rot.x)*-cosf(rot.y));
			}
			else {
				vmove = vec3f(-sinf(rot.y), 0, -cosf(rot.y));
			}
			if (keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]) {
				if (walkspeed < avatarRunSpeed) walkspeed += 0.1f;
			}
			else {
				if (walkspeed < avatarWalkSpeed) walkspeed += 0.1f;
			}
			if (keystate[SDL_SCANCODE_S]) nextpos -= vmove * walkspeed; else nextpos += vmove * walkspeed;
		}
		else 
		//if (walkspeed>0) walkspeed -= 0.005f;
		
		if (keystate[SDL_SCANCODE_A]) {
			nextpos.x -= cosf(rot.y)*walkspeed;
			nextpos.z += sinf(rot.y)*walkspeed;
		}
		if (keystate[SDL_SCANCODE_D]) {
			nextpos.x += cosf(rot.y)*walkspeed;
			nextpos.z -= sinf(rot.y)*walkspeed;
		}
		if (keystate[SDL_SCANCODE_R]) {
			nextpos.y -= 0.5f;
		}
		if (keystate[SDL_SCANCODE_F]) {
			nextpos.y += 0.5f;
		}
		if (keystate[SDL_SCANCODE_ESCAPE]) {
			window.setquit(true);
		}
		
		vec3f bdir = -(nextpos - pos)*1.8f;
		vec3f negpos = -nextpos;
		vec3f footpos = negpos + vec3f(0, -5.0f, 0);
		vec3f headpos = negpos + vec3f(0, 0.5f, 0);
		if (fly || (!scene.collide(meshes, footpos , bdir, 8.f) && !scene.collide(meshes, headpos, bdir, 8.f))) pos=nextpos;

		scene.models[skybox].matrix.Move(-pos);

		//process window events ...

		while (window.event()) {
			switch (window.ev.type)
			{
			case SDL_MOUSEMOTION:
				if (window.mousedown) {
					rot.y -= window.mouseDeltaXY.x / 300.f;
					rot.x -= window.mouseDeltaXY.y / 300.f;
					//scenemtx.SetSceneMatrix(pos,rot);
				}
				break;
			case SDL_MOUSEWHEEL:
				//modmtx.Translate(vec3f(0,0,-((float)window.mousewheel)*1.f));
				//shader.SetMatrix4f(modmtxh, modmtx);
				break;
			case SDL_WINDOWEVENT:
				switch (window.ev.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					pspmtx.SetPerspective(opts.PerspValue, opts.nfzp.x, opts.nfzp.y);
					shader.setProjectionMatrix(pspmtx);
					break;
				}
				break;
			}
		}

		

		//clear background to dark blue ...
		window.clear();
	
		//Render scene ...
		scenemtx.SetSceneMatrix(pos, rot);
		shader.setProjectionMatrix(pspmtx);
		//glEnable(GL_DEPTH_TEST);
		scene.render(meshes, scenemtx, shader);
		
		//scenemtx.Init();
		
		shader.setProjectionMatrix(orthoMtx);
		//glDisable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
		scene.render2D(meshes, scene2D, shader);

		//if (scene.models[cargoHoldCollider].collideSphere(meshes,ballpos,balldir,10.f)) {
			//SDL_Log("Hit! = %f,%f,%f, %f,%f,%f", ballpos.x, ballpos.y, ballpos.z, balldir.x, balldir.y, balldir.z);
		//}
		
		if (!fly) {
			floorHt = scene.collideFloor(meshes, pos);
			if (floorHt < avatarHeight) {
				pos.y -= (avatarHeight - floorHt);
				scenemtx.SetSceneMatrix(pos, rot);
			}
			else if (floorHt > avatarHeight) {
				if (floorHt - avatarFallSpeed < avatarHeight) pos.y -= (avatarHeight - floorHt); else pos.y += avatarFallSpeed;
				scenemtx.SetSceneMatrix(pos, rot);
			}
		}
				
		//ballpos+=balldir;
		//scene.models[ball].matrix.Move(ballpos);
		
	
		fps++;
		if (SDL_GetTicks() > timer) {
			timer = SDL_GetTicks() + 1000;
			//swindow.setCaption("Cargoship Demo (FPS:"+std::to_string(fps)+")");
			//SDL_Log("FPS=%d", fps);
			lastfps= fps;
			fps = 0;
		}

		window.SwapBuffers();
		
	}

	window.destroy();
	SDL_Log("FPS=%d", lastfps);
	return 0;
}

