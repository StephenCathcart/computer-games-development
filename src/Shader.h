#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>

#define SHADER_VERTEX	0
#define SHADER_FRAGMENT 1
#define SHADER_GEOMETRY 2
#define SHADER_TCS		3
#define SHADER_TES		4

using namespace std;

class Shader {
public:
	Shader(const string &vertex,const string &fragment ,const string &geometry = "",const string &tcs = "",const string &tes = "");
	~Shader(void);

	GLuint	getShaderProgram() { return program; }
	bool	linkProgram();

protected:
	void	setDefaultAttributes();
	bool	loadShaderFile(string from ,string &into);
	GLuint	generateShader(string from ,GLenum type);

	GLuint	objects[5];
	GLuint	program;

	bool	loadFailed ;
};

