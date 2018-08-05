#pragma once

//#include <GL/glew.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
	//build shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	//program usage 
	void use();
	GLuint programId() const;
	void setMat4(const GLchar * name, const glm::mat4 & mtrx);
	void setVec3(const GLchar * name, const GLfloat & v0, const GLfloat & v1, const GLfloat & v2);
	void setVec3(const GLchar * name, const glm::vec3 & v);
	void setFloat(const GLchar * name, const GLfloat & value);
	void setInt(const GLchar * name, const GLint& value);
private:
	//program indefener
	GLuint id;
};

