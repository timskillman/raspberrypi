#include "SMloadOptions.h"

#include <fstream>
#include <sstream>

loadOptions::loadOptions(const char * file)
{
	init();
	open(file);
}

void loadOptions::getFloat(std::string str, float &res)
{
	std::stringstream ss;
	ss.str(str);
	ss >> res;
}

void loadOptions::getVec3f(std::string str, vec3f &res)
{
	std::stringstream ss;
	ss.str(str);
	ss >> res.x >> res.y >> res.z;
}

void loadOptions::getVec2f(std::string str, vec2f &res)
{
	std::stringstream ss;
	ss.str(str);
	ss >> res.x >> res.y;
}

void loadOptions::get3ints(std::string &str, int &a, int &b, int &c)
{
	std::stringstream ss;
	ss.str(str);
	ss >> a >> b >> c;
}

void loadOptions::open(const char * file)
{
	std::string line = "";
	std::ifstream opts(file);
	while (std::getline(opts, line)) {
		if (line != ""  && line[0] != '#') {
			std::string com = line.substr(0, line.find(":"));
			if (com.size() > 0) {
				std::string val = line.substr(com.size() + 1, line.size() - com.size() + 1);
				if (val != "") {
					if (com == "screensize") get3ints(val, screenx, screeny, PerspValue);
					else if (com == "modelPath") modelPath = val;
					else if (com == "model") model = val;
					else if (com == "collider") collider = val;
					else if (com == "skyboxPath") skyboxPath = val;
					else if (com == "skybox") skybox = val;
					else if (com == "logo") logo = val;
					else if (com == "lightpos") getVec3f(val, lightPos);
					else if (com == "startpos") getVec3f(val, pos);
					else if (com == "nearzfarz") getVec2f(val, nfzp);
					else if (com == "fullscreen") fullscreen = (val == "true");
					else if (com == "fly") fly = (val == "true");
					else if (com == "avatarHeight") getFloat(val, avatarHeight);
					else if (com == "avatarWalkSpeed") getFloat(val, avatarWalkSpeed);
					else if (com == "avatarRunSpeed") getFloat(val, avatarRunSpeed);
					else if (com == "avatarFallSpeed") getFloat(val, avatarFallSpeed);
					else if (com == "fogColour") getVec3f(val, fogColour);
					else if (com == "fogFar") getFloat(val, fogFar);
					else if (com == "fogNear") getFloat(val, fogNear);
					else if (com == "skyBoxScale") getFloat(val, skyboxScale);
					else if (com == "title") title = val;
				}
			}
		}
	}

}

void loadOptions::init()
{
	int screenx = 1440;
	int screeny = 900;
	int PerspValue = 300;
	lightPos = vec3f(0.f, 5.f, 0.f);
	pos = vec3f(0.f, 0.f, 0.f);
	rot = vec3f(0.f, 0.f, 0.f);
	nfzp = vec2f(1.f, 1000.f);
	fogColour = vec3f(1.f, 1.f, 1.f);
	fogFar = 800.f;
	fogNear = 100.f;
	avatarWalkSpeed = 0.8f;
	avatarRunSpeed = 1.2;
	avatarHeight = 8.0f;
	avatarFallSpeed = 0.3f;
	skyboxScale = 10.f;

	fullscreen = false;
	skybox = "";
	modelPath = "";
	model = "";
	collider = "";
	skyboxPath = "";
	logo = "";
	title = "Untitled";
}