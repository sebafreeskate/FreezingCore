#include "Keyboard.h"



Keyboard::Keyboard(GLFWwindow * window)
	: HID(window)
{
}


Keyboard::~Keyboard()
{
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
Direction Keyboard::getMovement()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		return FORWARD;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		return BACKWARD;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		return LEFT;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		return RIGHT;
	}
	return NONE;
}
