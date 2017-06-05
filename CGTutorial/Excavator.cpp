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
	mvp.sendMVP();
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
	if (xPosition < max - modelSize) {
		xPosition += vehicleStepLength;
	}
}
void Excavator::moveBodyRight(float max)
{
	if (xPosition > -max + modelSize) {
		xPosition -= vehicleStepLength;
	}
}
void Excavator::moveBodyUp(float max)
{
	if (zPosition < max - modelSize) {
		zPosition += vehicleStepLength;
	}
}
void Excavator::moveBodyDown(float max)
{
	if (zPosition > -max + modelSize) {
		zPosition -= vehicleStepLength;
	}
};
