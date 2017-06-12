#pragma once
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include "Excavator.h"

class Controls
{
public:
	Controls(float size);
	~Controls();

private:
	float max;

public:
	void keyPress(GLFWwindow* window, int key, int scancode, int action, Excavator& e, MVPHandler mvp);
};

