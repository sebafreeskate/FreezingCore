#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
	//build shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	//program usage 
	void Use();
	GLuint programId();
private:
	//program indefener
	GLuint id;
};

