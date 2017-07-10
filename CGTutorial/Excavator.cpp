#include "Excavator.h"
#include <iostream>
#include<conio.h>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <thread>

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
	orientation = 0.0;
	vehicleStepLength = 0.1;
	modelSize = 1.0;
	baseJointBentXY = 30.0;
	maxBaseAngle = 60;
	minBaseAngle = 30;
	secondJoint = 120.0;
	armLength = 1.5;
	armPieceLength = armLength * 0.7;
	armMoveLength = armLength * 1.4;
	

	armWidth = 0.2;
}

Excavator::~Excavator()
{
}

void Excavator::drawExcavator(MVPHandler mvp)
{
	// draw body
	drawBaseBody(mvp);
	
	//draw cabin
	drawCabin(mvp);
	
	glm::mat4 Save = mvp.getModel();
	// draw base arm
	mvp.setModel(glm::translate(mvp.getModel(), glm::vec3(xPosition, modelSize, zPosition)));
	mvp.setModel(glm::rotate(mvp.getModel(), orientation, glm::vec3(0.0, 1.0, 0.0)));
	
	drawBaseArm(mvp);
	// draw second arm
	mvp.setModel(glm::rotate(mvp.getModel(), baseJointBentXY, glm::vec3(1.0, 0.0, 0.0)));
	mvp.setModel(glm::translate(mvp.getModel(), glm::vec3(0.0, armMoveLength + 0.4, 0.0)));

	drawOtherArm(mvp);
	// draw "shovel"
	drawShovel(mvp);
	mvp.setModel(Save);
}

void Excavator::drawBaseBody(MVPHandler mvp)
{
	glm::mat4 Save = mvp.getModel();
	mvp.setModel(glm::translate(mvp.getModel(), glm::vec3(xPosition, modelSize, zPosition)));
	mvp.setModel(glm::rotate(mvp.getModel(), orientation, glm::vec3(0.0, 1.0, 0.0)));
	mvp.setModel(glm::scale(mvp.getModel(), glm::vec3(modelSize, 0.5*modelSize, modelSize)));

	mvp.sendMVP();
	drawCube();
	mvp.setModel(Save);
}

void Excavator::drawCabin(MVPHandler mvp)
{
	glm::mat4 Save = mvp.getModel();
	mvp.setModel(glm::translate(mvp.getModel(), glm::vec3(xPosition, modelSize, zPosition)));
	mvp.setModel(glm::rotate(mvp.getModel(), orientation, glm::vec3(0.0, 1.0, 0.0)));
	mvp.setModel(glm::scale(mvp.getModel(), glm::vec3(0.5 * modelSize, 1.4*modelSize, 0.5 * modelSize)));

	mvp.sendMVP();
	drawCube();

	mvp.setModel(Save);
}

void Excavator::drawBaseArm(MVPHandler mvp)
{	
	glm::mat4 Save = mvp.getModel();
	mvp.setModel(glm::rotate(mvp.getModel(), baseJointBentXY, glm::vec3(1.0, 0.0, 0.0)));
	mvp.setModel(glm::translate(mvp.getModel(), glm::vec3(0.0, armLength, 0.0)));
	mvp.setModel(glm::scale(mvp.getModel(), glm::vec3(armWidth, armPieceLength, armWidth)));
	
	mvp.sendMVP();
	drawCube();
	
	mvp.setModel(Save);
}

void Excavator::drawOtherArm(MVPHandler mvp)
{
	glm::mat4 Save = mvp.getModel();
	mvp.setModel(glm::rotate(mvp.getModel(), secondJoint, glm::vec3(1.0, 0.0, 0.0)));
	mvp.setModel(glm::translate(mvp.getModel(), glm::vec3(0.0, armPieceLength, 0.0)));
	mvp.setModel(glm::scale(mvp.getModel(), glm::vec3(armWidth, armPieceLength, armWidth)));

	mvp.sendMVP();
	drawCube();

	mvp.setModel(Save);
}

void Excavator::drawShovel(MVPHandler mvp)
{
	glm::mat4 Save = mvp.getModel();
	
	mvp.setModel(glm::rotate(mvp.getModel(), secondJoint, glm::vec3(1.0, 0.0, 0.0)));
	mvp.setModel(glm::translate(mvp.getModel(), glm::vec3(0.0, armMoveLength, 0.0)));
	mvp.setModel(glm::scale(mvp.getModel(), glm::vec3(0.4, 0.4, 0.4)));
	
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

float Excavator::getOrientation()
{
	if (fmod(orientation, 360) < 0)
	{
		return 360 + fmod(orientation, 360);
	}
	else
	{
		return fmod(orientation, 360);
	}
}

// Body movement
void Excavator::moveBodyLeft(float max, float stepLength)
{
	if (xPosition < max - 1) {
		xPosition += stepLength;
	}
}
void Excavator::moveBodyRight(float max, float stepLength)
{
	if (xPosition > -max + 1) {
		xPosition -= stepLength;
	}
}
void Excavator::moveBodyUp(float max, float stepLength)
{
	if (zPosition < max - 1) {
		zPosition += stepLength;
	}
}
void Excavator::moveBodyDown(float max, float stepLength)
{
	if (zPosition > -max + 1) {
		zPosition -= stepLength;
	}
}

// Body rotation
void Excavator::rotateBodyCounterClockwise(float stepLength)
{
	orientation = fmod(orientation + stepLength, 360);
}

void Excavator::rotateBodyClockwise(float stepLength)
{
	orientation = fmod(orientation - stepLength, 360);
}

// Arm movement
void Excavator::bendBaseJointDown(float stepLength)
{
	baseJointBentXY += stepLength;
}
void Excavator::bendBaseJointUp(float stepLength)
{
	baseJointBentXY -= stepLength;
}

float Excavator::getBaseJointAngle()
{
	return baseJointBentXY;
}

float Excavator::getMaxBaseAngle()
{
	return maxBaseAngle;
}

float Excavator::getMinbaseAngle()
{
	return minBaseAngle;
}
;
