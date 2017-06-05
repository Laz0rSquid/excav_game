#pragma once
class Excavator
{
public:
	Excavator();
	~Excavator();
	void drawExcavator();
	void setXPos(float x);
	void setZPos(float z);
	float getXPos();
	float getZPos();

private:
	float xPosition;
	float zPosition;
	float vehicleStepLength;
	float modelSize;
};

