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
	float getOrientation();
	
	void drawExcavator(MVPHandler mvp);
	void moveBodyLeft(float max, float stepLength);
	void moveBodyRight(float max, float stepLength);
	void moveBodyUp(float max, float stepLength);
	void moveBodyDown(float max, float stepLength);
	void bendBaseJointDown();
	void bendBaseJointUp();
	void rotateBodyCounterClockwise(float stepLength);
	void rotateBodyClockwise(float stepLength);
	//void rotateBaseJointClockwise();
	//void rotateBaseJointCounterClockwise();
	void drawBaseArm(MVPHandler mvp, float length);
	void drawOtherArm(MVPHandler mvp, float length);
	
private:
	float xPosition;
	float zPosition;
	float orientation;
	float vehicleStepLength;
	float modelSize;
	float baseJointBentXY;
	// float baseJointRotationXZ;
};