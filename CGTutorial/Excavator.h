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
	
private:
	float xPosition;
	float zPosition;
	float vehicleStepLength;
	float modelSize;
};