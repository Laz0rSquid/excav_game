#pragma once
#include "MVPHandler.h"
class PlayfieldTile
{
public:
	PlayfieldTile();
	PlayfieldTile(float size, float xPos, float zPos);
	~PlayfieldTile();

	float tileSize;
	float xPosition;
	float zPosition;

	void setSize(float size);
	void setPosition(float xPos, float zPos);
	void drawTile(MVPHandler mvp);

};

