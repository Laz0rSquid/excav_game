#pragma once
#include "MVPHandler.h"
class PlayfieldTile
{
public:
	PlayfieldTile();
	~PlayfieldTile();

	float tileSize;
	float xPosition;
	float zPosition;

	bool isExcavated;
	bool hasTreasure;
	bool treasureAlreadyFound;
	bool hasExcavator;
	bool hasShovel;
	bool animationAlreadyPlayed;

	void setSize(float size);
	void setPosition(float xPos, float zPos);
	void drawTile(MVPHandler mvp);
};

