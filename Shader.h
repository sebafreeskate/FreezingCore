#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

