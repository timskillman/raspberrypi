#ifndef __SMSHADER
#define __SMSHADER

#include <SDL.h>
#include <vector>
#include <string>
#include "SMmatrix.h"
#include "SMmaterial.h"
#include "SMvector.h"

class SMshader {
public:
	SMshader(const char *vertShaderFile, const char *fragShaderFile, std::string attribs, std::string &error) : program(-1)
	{
		vertshader = loadShaderFile(GL_VERTEX_SHADER, vertShaderFile, error);
		if (error == "") 
			fragshader = loadShaderFile(GL_FRAGMENT_SHADER, fragShaderFile, error);
		if (error == "") 
			CreateShaderProgram(vertshader, fragshader, attribs, error);
	}

	SMshader() : program(-1), fogMinDist(50.f), fogMaxDist(500.f), fogColour(1.f,1.f,1.f) {};
	~SMshader() {};

	GLuint loadShaderFile(GLenum eShaderType, const char *strShaderFile, std::string &error);
	GLuint loadShaderStr(GLenum eShaderType, std::string shaderSource, std::string &error);
	bool CreateShaderProgram(GLuint &vertexShader, GLuint &fragmentShader, std::string &attribStr, std::string &error);

	inline void Use() { glUseProgram(program); }
	inline GLint GetUniformLocation(const char * name) { return glGetUniformLocation(program, name); }
	inline void Setf(GLint att, float val) { glUniform1f(att, val); }
	inline void Seti(GLint att, GLint val) { glUniform1i(att, val); }
	inline void Set2f(GLint att, const vec2f vec) { glUniform2fv(att, 1, &vec.x); }
	inline void Set3f(GLint att, const vec3f vec) { glUniform3fv(att, 1, &vec.x); }
	inline void Set4f(GLint att, const vec4f vec) { glUniform4fv(att, 1, &vec.x); }
	inline void SetMatrix4f(GLint att, const SMmatrix &matrix) { glUniformMatrix4fv(att, 1, GL_FALSE, &matrix.matrix[0]); }

	inline GLint GetAttribLocation(const char * name) { return glGetAttribLocation(program, name); }
	
	inline GLint GetSetMatrix4f(const char * name, const SMmatrix &matrix) {
		GLint uniRef = glGetUniformLocation(program, name);
		if (uniRef>=0) glUniformMatrix4fv(uniRef, 1, GL_FALSE, &matrix.matrix[0]);
		return uniRef;
	}
	
	inline GLint GetSeti(const char * name, const GLint val) {
		GLint uniloc = glGetUniformLocation(program, name);
		if (uniloc >= 0) glUniform1i(uniloc, val);
		return uniloc;
	}
	
	inline GLint GetSetf(const char * name, const float val) {
		GLint uniloc = glGetUniformLocation(program, name);
		if (uniloc >= 0) glUniform1f(uniloc, val);
		return uniloc;
	}
	
	inline GLint GetSet2f(const char * name, const vec2f vec) {
		GLint uniloc = glGetUniformLocation(program, name);
		if (uniloc >= 0) glUniform2fv(uniloc, 1, &vec.x);
		return uniloc;
	}
	
	inline GLint GetSet3f(const char * name, const vec3f vec) {
		GLint uniloc = glGetUniformLocation(program, name);
		if (uniloc >= 0) glUniform3fv(uniloc, 1, &vec.x);
		return uniloc;
	}
	
	inline GLint GetSet4f(const char * name, const vec4f vec) {
		GLint uniloc = glGetUniformLocation(program, name);
		if (uniloc >= 0) glUniform4fv(uniloc, 1, &vec.x);
		return uniloc;
	}
	
	void setupBasicShader();
	void setMaterial(SMmaterial &mat);
	void setProjectionMatrix(const SMmatrix &matrix) { SetMatrix4f(pspmtxh, matrix); }
	void SetModelMatrix(const SMmatrix &matrix) { SetMatrix4f(modmtxh, matrix); }
	
	GLint activateTexture(GLuint texID, const char * uni, GLuint texActive);
	
	GLint setTexture(const char * uni, GLint texLoc);

	float fogMinDist;
	float fogMaxDist;
	vec3f fogColour;

private:

	GLint program;
	GLuint vertshader;
	GLuint fragshader;
	
	//Shader refs for basic shader (needs a more generic approach!)
	GLint illumRef;
	GLint diffRef;
	GLint specRef;
	GLint ambiRef;
	GLint emisRef;
	GLint animRef;
	GLint fogColourRef;
	GLint fogRangeRef;
	GLint fogMaxRef;
	GLint lightPosRef;
	GLint pspmtxh;
	GLint modmtxh;
	GLint texture1Ref;
	
};
#endif
