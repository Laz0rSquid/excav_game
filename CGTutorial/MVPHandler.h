#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

class MVPHandler
{
public:
	MVPHandler();
	~MVPHandler();

private:
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;

public:
	void sendMVP(GLuint programID);
	void setDefaultMVP();
	glm::mat4 getModel();
	void setModel(glm::mat4 m);
};

