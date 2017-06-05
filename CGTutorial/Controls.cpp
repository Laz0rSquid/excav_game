#include "Controls.h"
#include "Excavator.h"
// Include GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>


Controls::Controls()
{
	max = 6.0;
}


Controls::~Controls()
{
}

void Controls::keyPress(GLFWwindow* window, int key, int scancode, int action, Excavator e) {
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_LEFT:
		e.moveBodyLeft(max);
		break;
	case GLFW_KEY_RIGHT:
		e.moveBodyRight(max);
		break;
	case GLFW_KEY_UP:
		e.moveBodyUp(max);
		break;
	case GLFW_KEY_DOWN:
		e.moveBodyDown(max);
		break;
	default:
		break;
	}
}