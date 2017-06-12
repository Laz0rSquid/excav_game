#pragma once
#include "MVPHandler.h"
class Excavator
{
public:
	Excavator();
	~Excavator();
	
	void setXPos(float x);
	void setZPos(float z);
	float getXPos();
	float getZPos();
	
	void drawExcavator(MVPHandler mvp);
	void moveBodyLeft(float max);
	void moveBodyRight(float max);
	void moveBodyUp(float max);
	void moveBodyDown(float max);
	void bendBaseJointDown();
	void bendBaseJointUp();
	void rotateBaseJointClockwise();
	void rotateBaseJointCounterClockwise();
	void drawBaseArm(MVPHandler mvp, float length);
	void drawOtherArm(MVPHandler mvp, float length);
	void animateBodyForward(float secondsElaped, float max);
	void task(float max, MVPHandler mvp);
	
private:
	float xPosition;
	float zPosition;
	float vehicleStepLength;
	float modelSize;
	float baseJointBentXY;
	float baseJointRotationXZ;
};