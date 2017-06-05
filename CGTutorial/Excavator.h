#pragma once
class Excavator
{
public:
	Excavator();
	~Excavator();
	void drawExcavator();
	void setXPos(float);
	void setZPos(float);
	float getXPos();
	float getZPos();

private:
	float xPosition;
	float zPosition;
	float vehicleStepLength;
	float modelSize;
};

