#pragma once
#include "HID.h"

class Keyboard :
	public HID

{
public:
	Keyboard(GLFWwindow * window);
	virtual ~Keyboard();
	virtual Direction getMovement();
};

