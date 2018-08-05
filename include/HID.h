#pragma once

#include <GLFW/glfw3.h>

#include "enums.h"

class HID
{
public:
	HID(GLFWwindow *window);
	virtual ~HID();
	virtual Direction getMovement() = 0;
protected: 
	GLFWwindow *window;
};

