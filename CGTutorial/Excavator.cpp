#include "Excavator.h"
#include <iostream>
#include<conio.h>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// Achtung, die OpenGL-Tutorials nutzen glfw 2.7, glfw kommt mit einem veränderten API schon in der Version 3 

// Befindet sich bei den OpenGL-Tutorials unter "common"
#include "shader.hpp"

// Wuerfel und Kugel
#include "objects.hpp"

// Ab Uebung5 werden objloader.hpp und cpp benoetigt
#include "objloader.hpp"

// Ab Uebung7 werden texture.hpp und cpp benoetigt
#include "texture.hpp"

// vehicle position

Excavator::Excavator()
{
	xPosition = 0.0;
	zPosition = 0.0;
	vehicleStepLength = 0.1;
	modelSize = 0.8;
}

Excavator::~Excavator()
{
}

void Excavator::drawExcavator(MVPHandler mvp)
{
	glm::mat4 Save = mvp.getModel();

	mvp.setModel(glm::translate(mvp.getModel(), glm::vec3(xPosition, modelSize, zPosition)));
	mvp.setModel(glm::scale(mvp.getModel(), glm::vec3(modelSize, modelSize, modelSize)));
	mvp.sendMVP(); // encapsulate in its own class
	drawSphere(10, 10);
	mvp.setModel(Save);
}

void Excavator::setXPos(float x)
{
	xPosition = x;
}

void Excavator::setZPos(float z)
{
	zPosition = z;
}

float Excavator::getXPos()
{
	return xPosition;
}

float Excavator::getZPos()
{
	return zPosition;
}

//movement
void Excavator::moveBodyLeft(float max)
{
	moveBody(vehicleStepLength, 0.0, max);
}
void Excavator::moveBodyRight(float max)
{
	moveBody(-vehicleStepLength, 0.0, max);
}
void Excavator::moveBodyUp(float max)
{
	moveBody(0.0, vehicleStepLength, max);
}
void Excavator::moveBodyDown(float max)
{
	moveBody(0.0, -vehicleStepLength, max);
}
void Excavator::moveBody(float x, float z, float max)
{
	xPosition += x;
	zPosition += z;
	/*
	if (xPosition < max - modelSize && xPosition > -max + modelSize && zPosition < max - modelSize && zPosition > -max + modelSize)
	{
		xPosition += x;
		zPosition += z;
	}
	*/
};

/*
switch (key)
{
case GLFW_KEY_ESCAPE:
glfwSetWindowShouldClose(window, GL_TRUE);
break;
case GLFW_KEY_LEFT:
if (vehicleX < maxRange - modelSize)
{
vehicleX += vehicleStepLength;
}
break;
case GLFW_KEY_RIGHT:
if (vehicleX > -maxRange + modelSize)
{
vehicleX -= vehicleStepLength;
}
break;
case GLFW_KEY_UP:
if (vehicleZ < maxRange - modelSize)
{
vehicleZ += vehicleStepLength;
}
break;
case GLFW_KEY_DOWN:
if (vehicleZ > -maxRange + modelSize)
{
vehicleZ -= vehicleStepLength;
}
break;
default:
break;
}

*/