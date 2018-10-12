#ifndef __SMOPTIONS
#define __SMOPTIONS

#include <string>
#include <vector>
#include "SMvector.h"

class loadOptions {
public:

	loadOptions(const char * file);
	loadOptions() {}
	~loadOptions() {}

	int screenx; 
	int screeny; 
	int PerspValue;

	vec3f lightPos;
	vec3f pos;
	vec3f rot;
	vec2f nfzp;

	vec3f fogColour;
	float fogFar;
	float fogNear;

	bool fullscreen;
	bool fly;

	float avatarWalkSpeed;
	float avatarRunSpeed;
	float avatarHeight;
	float avatarFallSpeed;

	float skyboxScale;

	std::string model;
	std::string modelPath;
	std::string collider;
	std::string skybox;
	std::string skyboxPath;
	std::string logo;
	std::string title;

private:

	void open(const char * file);
	void init();
	void getVec3f(std::string str, vec3f &res);
	void getVec2f(std::string str, vec2f &res);
	void getFloat(std::string str, float &res);
	void get3ints(std::string &str, int &a, int &b, int &c);
};

#endif
