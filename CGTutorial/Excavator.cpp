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
	baseJointBentXY = 0.0;
	baseJointRotationXZ = 0.0;
}

Excavator::~Excavator()
{
}

void Excavator::drawExcavator(MVPHandler mvp)
{
	/*
	Model = glm::rotate(Model, joint1Z, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 Save = Model;

	Model = glm::translate(Model, glm::vec3(0.0, h * 0.5, 0.0));
	Model = glm::scale(Model, glm::vec3(h * 0.15, h * 0.5, h * 0.15));
	sendMVP();
	drawSphere(10, 10);
	Model = Save;
	*/

	// draw body
	glm::mat4 Save = mvp.getModel();
	mvp.setModel(glm::translate(mvp.getModel(), glm::vec3(xPosition, modelSize, zPosition)));
	mvp.setModel(glm::scale(mvp.getModel(), glm::vec3(0.8 * modelSize, 0.5*modelSize, modelSize)));
	mvp.sendMVP();
	drawCube();

	// draw arm
	drawBaseArm(mvp, 2);
	drawOtherArm(mvp, 2);
}

void Excavator::drawBaseArm(MVPHandler mvp, float length)
{	
	mvp.setModel(glm::translate(mvp.getModel(), glm::vec3(0.0, modelSize, 0.0)));
	mvp.setModel(glm::rotate(mvp.getModel(), baseJointRotationXZ, glm::vec3(0.0, 1.0, 0.0)));
	mvp.setModel(glm::rotate(mvp.getModel(), baseJointBentXY, glm::vec3(1.0, 0.0, 0.0)));
	
	glm::mat4 Save = mvp.getModel();
	mvp.setModel(glm::translate(mvp.getModel(), glm::vec3(0.0, length, 0.0)));
	mvp.setModel(glm::scale(mvp.getModel(), glm::vec3(length * 0.1, length * 0.7, length * 0.1)));
	mvp.sendMVP();
	drawCube();
	

	mvp.setModel(Save);
}
void Excavator::drawOtherArm(MVPHandler mvp, float length)
{

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
}
void Excavator::bendBaseJointDown()
{
	baseJointBentXY += 3 * vehicleStepLength;
}
void Excavator::bendBaseJointUp()
{
	baseJointBentXY -= 3 * vehicleStepLength;
}
void Excavator::rotateBaseJointClockwise()
{
	baseJointRotationXZ -= 8 * vehicleStepLength;
}
void Excavator::rotateBaseJointCounterClockwise()
{
	baseJointRotationXZ += 8 * vehicleStepLength;
}
;
