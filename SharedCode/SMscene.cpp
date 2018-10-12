#include "SMscene.h"

void SMscene::render(SMmeshPool &meshes, SMmatrix &parent_matrix, SMshader &shader)
{
	for (auto &sceneModel : models)
		sceneModel.render(meshes, parent_matrix, shader);
}

void SMscene::render2D(SMmeshPool &meshes, SMmatrix &parent_matrix, SMshader &shader)
{
	for (auto &model2D : models2D)
		model2D.render(meshes, parent_matrix, shader);
}

void SMscene::renderSkybox(SMmeshPool &meshes, SMmatrix &parent_matrix, SMshader &shader, const vec3f &pos, int32_t skyboxref)
{
	if (skyboxref >= 0 && skyboxes.size() > 0) {
		skyboxes[skyboxref].matrix.Move(pos);
		skyboxes[skyboxref].render(meshes, parent_matrix, shader);
	}
}

int32_t SMscene::loadModelOBJ(SMmeshPool &meshes, std::string path, std::string file)
{
	SMmodel model;
	if (file == "") return -1;
	model.loadOBJfile(&meshes, path, file, false);
	int32_t c = models.size();
	models.push_back(model);
	return c;
}

int32_t SMscene::loadSkybox(SMmeshPool &meshes, std::string path, std::string file, float scale)
{
	SMmodel model;
	if (file == "") return -1;
	model.loadOBJfile(&meshes, path, file, false);
	model.matrix.SetScale(scale);
	int32_t c = skyboxes.size();
	skyboxes.push_back(model);
	return c;
}

int32_t SMscene::loadColliderOBJ(SMmeshPool &meshes, std::string path, std::string file)
{
	SMmodel model;
	if (file == "") return -1;
	model.loadOBJfile(&meshes, path, file, true);
	model.collider = true;
	colliders.push_back(model);
	return models.size()-1;
}

bool SMscene::collide(SMmeshPool &meshes, vec3f &pos, vec3f &dir, float radius)
{
	for (auto &collideModel : colliders) {
		if (collideModel.collideSphere(meshes, pos, dir, radius)) 
			return true;
	}
	return false;
}

float SMscene::collideFloor(SMmeshPool &meshes, vec3f &pos)
{
	float prevHeight = 1e8f;
	float ht;
	for (auto &collideModel : colliders) {
		ht = collideModel.collideFloorSub(meshes, pos, prevHeight);
	}
	return ht;
}

int32_t SMscene::appendModel(SMmodel &model)
{
	int32_t c = models.size();
	models.push_back(model); 
	return c;
}

int32_t SMscene::append2D(SMmeshPool &meshes, SMmodel model, std::string txfile)
{
	if (txfile != "") {
		model.material.texRef =  meshes.loadTexture(txfile, "", model.material.texID);
		if (model.material.texRef >= 0) {
			model.material.texName = txfile;
			meshes.materials.push_back(model.material);
		}
	}
	int32_t c = models2D.size();
	models2D.push_back(model);
	return c;
}

void SMscene::appendCollider(SMmodel &model)
{
	model.collider = true;
	colliders.push_back(model);
}


void SMscene::renderOffscreen(SDL_RWops *f, int width, int height, SMmeshPool &meshes, SMmatrix &parent_matrix, SMshader &shader)
{
#ifdef __LINUX__
	
	/*
	GLuint fbo = 0, frame_buf = 0, colour_buf = 0, depth_buf = 0;


	glGenRenderbuffers(1, &colour_buf);
	glBindRenderbuffer(GL_RENDERBUFFER, colour_buf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);

	glGenRenderbuffers(1, &depth_buf);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

	glGenFramebuffers(1, &frame_buf);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buf);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colour_buf);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buf);


	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		switch (status)
		{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			XDEBUG_PRINTCOLOR(0, __L("GRPBUFFEROPENGLRENDER::Ini Unable to create Framebuffer : GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"));
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			XDEBUG_PRINTCOLOR(0, __L("GRPBUFFEROPENGLRENDER::Ini Unable to create Framebuffer : GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"));
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			XDEBUG_PRINTCOLOR(0, __L("GRPBUFFEROPENGLRENDER::Ini Unable to create Framebuffer : GL_FRAMEBUFFER_UNSUPPORTED"));
			break;
		default:
			XDEBUG_PRINTCOLOR(0, __L("GRPBUFFEROPENGLRENDER::Ini Unable to create Framebuffer : Unknown Reason"));
			break;
		}
	}
	*/
#endif

#ifdef __WINDOWS__
	//initialize frame buffer
	GLuint fbo = 0, render_buf = 0, depth_buf = 0;
	glGenFramebuffers(1, &fbo);
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, render_buf);

	glGenRenderbuffers(1, &render_buf);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);

	glGenRenderbuffers(1, &depth_buf);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, render_buf);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buf);

	//render
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear colour and Z buffer

	//RENDER
	render(meshes, parent_matrix, shader);

	std::vector<std::uint8_t> data(width*height * 4);
	//glFramebufferReadBufferEXT(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);

	//write png
	SDL_Surface *ss = SDL_CreateRGBSurfaceFrom(&data[0], width, height, 32, width * 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	//SDL_RWops *fo = SDL_RWFromFile("thumb.png", "wb");
	//IMG_SavePNG_RW(ss, fo, 0);
	//SDL_RWclose(fo);

	IMG_SavePNG_RW(ss, f, 0);
	SDL_FreeSurface(ss);
	//SDL_RWclose(f);

	//deinit frame buffer:
	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &render_buf);
	glDeleteRenderbuffers(1, &depth_buf);

	// Return to onscreen rendering:
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	//glViewport(vx, vy, vw, vh); //restore viewport to what it was
	glEnable(GL_DEPTH_TEST);
#endif
}
