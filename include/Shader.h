#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
;
class Shader
{
public:

	Shader() { mProgram = glCreateProgram(); }
	~Shader() { glDeleteProgram(mProgram); }

	Shader & activate();
	Shader & attach(std::string const & filename);
	GLuint   create(std::string const & filename);
	GLuint   get() { return mProgram; }
	Shader & link();

	// Wrap Calls to glUniform
	void bind(unsigned int location, GLfloat value);
	void bind(unsigned int location, GLuint value);
	void bind(unsigned int location, glm::mat4 const & matrix);
	void bind(unsigned int location, glm::vec3 const & vec);
	void bind(unsigned int location, glm::vec4 const & vec);
	template<typename T> Shader & bind(std::string const & name, T&& value)
	{
		int location = glGetUniformLocation(mProgram, name.c_str());
		if (location == -1) fprintf(stderr, "Missing Uniform: %s\n", name.c_str());
		else bind(location, std::forward<T>(value));
		return *this;
	}

private:

	Shader(Shader const &) = delete;
	Shader & operator=(Shader const &) = delete;

	GLuint mProgram;
	GLint  mStatus;
	GLint  mLength;

};