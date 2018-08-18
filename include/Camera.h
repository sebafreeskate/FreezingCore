
#pragma once

#include <vector>

// GL Includes
//#include <GL/glew.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "enums.h"

namespace defaults {
	// Default camera values
	const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat SPEED = 3.0f;
	const GLfloat SENSITIVTY = 0.15f;
	const GLfloat ZOOM = 45.0f;
}

// A camera class that processes input and calculates the corresponding Eular Angles, 
// Vectors and Matrices for use in OpenGL

class Camera
{
public:

	//vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		GLfloat yaw = defaults::YAW, GLfloat pitch = defaults::PITCH);

	//scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void transpose(Direction dir, GLfloat deltaTime);

	// Processes input received from a mouse-like input system. Expects the offset value in both the x and y direction.
	void setDirection(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void zoom(GLfloat yoffset);

	GLfloat getZoomVal() {
		return zoomVal;
	}

	const glm::vec3& getPosition() const;

private :
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	// Eular Angles
	GLfloat yaw;
	GLfloat pitch;
	// Camera options
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoomVal;
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors();

};