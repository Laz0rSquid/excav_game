#pragma once
#include "MVPHandler.h"
class Playfield
{
public:
	Playfield();
	~Playfield();

	float fieldSize;

	float getFieldSize();
	void drawPlayfield(MVPHandler mvp);
};

