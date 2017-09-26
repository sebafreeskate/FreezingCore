#pragma once


#include <GL/glew.h>

class Shader
{
public:
	//program indefener
	GLuint Program;
	//build shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	//program usage 
	void Use();
};

