#pragma once
#include "MVPHandler.h"
#include "PlayfieldTile.h"
class Playfield
{
public:
	Playfield();
	~Playfield();

	float fieldSize;
	float getFieldSize();
	void drawPlayfield(MVPHandler mvp, GLuint programID);

	std::vector <std::vector<PlayfieldTile> > tiles;
	int tileNumber;
	float tileSize;
	float tileSideNumber;
	
	std::vector <int> excPos;
	void changeExcPos(int x, int z, int orientation);

	std::vector <int> shovelPos;
	void turnShovel(int orientation, int turn);
};

