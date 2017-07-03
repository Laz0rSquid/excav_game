#pragma once
#include "MVPHandler.h"
#include "PlayfieldTile.h"
class Playfield
{
public:
	Playfield();
	~Playfield();

	float fieldSize;
	int tileNumber;
	float tileSize;
	std::vector<PlayfieldTile> tiles;

	float getFieldSize();
	void drawPlayfield(MVPHandler mvp, GLuint programID);
};

