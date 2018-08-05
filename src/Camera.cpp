#include "Camera.h"

	Camera::Camera(glm::vec3 position /*= glm::vec3(0.0f, 0.0f, 0.0f) */, glm::vec3 up /*= glm::vec3(0.0f, 1.0f, 0.0f) */,
		GLfloat yaw /*= defaults::YAW */, GLfloat pitch /*= defaults::PITCH*/ ) 
		: front(glm::vec3(0.0f, 0.0f, -1.0f)),
		movementSpeed(defaults::SPEED), mouseSensitivity(defaults::SENSITIVTY), zoomVal(defaults::ZOOM)
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)),
		movementSpeed(defaults::SPEED), mouseSensitivity(defaults::SENSITIVTY), zoomVal(defaults::ZOOM)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}

	void Camera::updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}

	void Camera::transpose(Direction direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->movementSpeed * deltaTime;
		if (direction == FORWARD)
			this->position += this->front * velocity;
		if (direction == BACKWARD)
			this->position -= this->front * velocity;
		if (direction == LEFT)
			this->position -= this->right * velocity;
		if (direction == RIGHT)
			this->position += this->right * velocity;
	}

	void Camera::setDirection(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch /* = true*/)
	{
		xoffset *= this->mouseSensitivity;
		yoffset *= this->mouseSensitivity;

		this->yaw += xoffset;
		this->pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->pitch > 89.0f)
				this->pitch = 89.0f;
			if (this->pitch < -89.0f)
				this->pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->updateCameraVectors();
	}

	void Camera::zoom(GLfloat yoffset)
	{
		if (this->zoomVal >= 1.0f && this->zoomVal <= 45.0f)
			this->zoomVal -= yoffset;
		if (this->zoomVal <= 1.0f)
			this->zoomVal = 1.0f;
		if (this->zoomVal >= 45.0f)
			this->zoomVal = 45.0f;
	}
