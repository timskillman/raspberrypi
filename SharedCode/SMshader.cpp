#include "SMshader.h"
#include <fstream>
#include <sstream>

bool SMshader::CreateShaderProgram(GLuint &vertexShader, GLuint &fragmentShader, std::string &attribStr, std::string &error)
{
	program = glCreateProgram();
	if (program == 0) {
		error = "Could not create shader program";
		return false;
	}

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	std::istringstream ss(attribStr);
	std::string s; uint32_t a = 0;
	while (ss >> s) glBindAttribLocation(program, a++, s.c_str());

	glLinkProgram(program); //linking

	GLint linked;				//error checking
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::string log; log.resize(infoLogLength);
		glGetProgramInfoLog(program, infoLogLength, NULL, &log[0]);
		error = "Shader linker failure: "+log;
		//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader Error!", "Shader linker failure", handle);

		//SDL_Log("Shader linker failure: %s\n", log.c_str());
		glDeleteProgram(program);
		return false;
	}
	else
	{
		//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader Compiled!", "Shader linked sucessfully!", handle);
		SDL_Log("Shader linked sucessfully!");
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	SDL_Log("Shader program %d\n", program);

	SDL_Log("glError: %d\n", glGetError());

	return true;
}

GLuint SMshader::loadShaderFile(GLenum ShaderType, const char *strShaderFile, std::string &error)
{
	std::ifstream sfile(strShaderFile, std::ios::in | std::ios::binary);
	std::string shaderSource;
	sfile.seekg(0, std::ios::end);
	shaderSource.resize(sfile.tellg());
	sfile.seekg(0, std::ios::beg);
	sfile.read(&shaderSource[0], shaderSource.size());
	sfile.close();
	return loadShaderStr(ShaderType, shaderSource, error);
}

//GLuint SMshader::loadShaderFile2(const char *strShaderFile, std::string &error)
//{
//	std::ifstream sfile(strShaderFile, std::ios::in | std::ios::binary);
//	std::string shaderSource;
//	sfile.seekg(0, std::ios::end);
//	shaderSource.resize(sfile.tellg());
//	sfile.seekg(0, std::ios::beg);
//	sfile.read(&shaderSource[0], shaderSource.size());
//	sfile.close();
//
//	return loadShaderStr(ShaderType, shaderSource, error);
//}

GLuint SMshader::loadShaderStr(GLenum ShaderType, std::string shaderSource, std::string &error)
{
	GLuint shader = glCreateShader(ShaderType);
	const char *ss = shaderSource.c_str();
	glShaderSource(shader, 1, &ss, NULL);

	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::string log; log.resize(infoLogLength);
		glGetShaderInfoLog(shader, infoLogLength, NULL, &log[0]);
		error = "Compile failure in shader: "+log;
		glDeleteShader(shader);
		return -1;
	}
	else
	{
		//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader Compiled!", "Shader compiled sucessfully!", handle);
		SDL_Log("Shader compiled sucessfully!");
	}
	return shader;
}

GLint SMshader::activateTexture(GLuint texID, const char * uni, GLuint texActive)
{
	GLint texh = GetUniformLocation(uni);
	if (texh >= 0) {
		glActiveTexture(GL_TEXTURE0 + texActive);
		glBindTexture(GL_TEXTURE_2D, texID);
		//glUniform1i(texh, texActive);
	}
	return texh;
}

GLint SMshader::setTexture(const char * uni, GLint texLoc)
{
	GLint texh = GetUniformLocation(uni);
	if (texh >= 0) glUniform1i(texh, texLoc);
	return texh;
}

void SMshader::setupBasicShader()
{
	//Set this AFTER shader.Use
	
	// Basic rendering handles
	//illumRef = GetUniformLocation("u_illum");
	//fogColour = vec3f(1.f, 1.f, 1.f);
	//fogMaxDist = 400.f;
	//fogMinDist = 50.f;

	fogColourRef = GetSet3f("u_fogColour", fogColour);
	fogRangeRef = GetSetf("u_fogRange", 1.f / (fogMaxDist - fogMinDist));
	fogMaxRef = GetSetf("u_fogMaxDist", fogMaxDist);
	specRef = GetUniformLocation("u_specColour");
	diffRef = GetUniformLocation("u_diffuseColour");
	ambiRef = GetUniformLocation("u_ambientColour");
	emisRef = GetUniformLocation("u_emissiveColour");
	animRef = GetUniformLocation("u_animoffset");
	lightPosRef = GetUniformLocation("u_LightPos");
	texture1Ref = GetUniformLocation("u_Texture");

	// Basic matrices ..
	pspmtxh = GetUniformLocation("u_ProjMatrix");
	modmtxh = GetUniformLocation("u_ModelMatrix");
	
	// Setup texture location
	if (texture1Ref >= 0) glUniform1i(texture1Ref, 0);
}

void SMshader::setMaterial(SMmaterial &mat)
{
		//Set Specular colour
		if (mat.illum == 1) {
			Setf(fogRangeRef, 0.f); //no fog
		}
		else {
			Setf(fogRangeRef, 1.f / (fogMaxDist - fogMinDist));
		}
		//Seti(illumRef, mat.illum);
		Set4f(specRef, mat.colSpecular);
		Set4f(ambiRef, mat.colAmbient);
		Set4f(emisRef, mat.colEmissive);
		
		//Set diffuse colour
		mat.colDiffuse.w = mat.alpha;
		//if (mat.alpha < 1.0f) glDepthFunc(GL_LESS); else glDepthFunc(GL_LEQUAL);
		if (mat.alpha < 1.0f) {
			glAlphaFunc(GL_GREATER, mat.alphaTestValue);
		}
		Set4f(diffRef, mat.colDiffuse);
		
		//animate texture ...
		mat.animoffset.x += mat.animvec.x; 
		if (mat.animoffset.x > 0.999f) {
			mat.animoffset.x = 0.f;
			mat.animoffset.y += mat.animvec.y;
			if (mat.animoffset.y > 0.999f) mat.animoffset.y = 0.f;
		}
		Set2f(animRef, mat.animoffset);
		
		if (mat.texRef>=0) { 
			//if (lastTex!=0) glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mat.texRef);

			//lastTex = 0;
		}
		//if (mat.texBumpRef>=0) {
			//if (lastTex!=1) 
			//glActiveTexture(GL_TEXTURE1);
			//glBindTexture(GL_TEXTURE_2D, material.texBumpRef);
			//lastTex = 1;
			//glActiveTexture(GL_TEXTURE0);
		//}
	
}
