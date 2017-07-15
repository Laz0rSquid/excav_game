#include "Playfield.h"
#include "objects.hpp"
#include "Math.h"
#include "PlayfieldTile.h"
#include <iostream>
#include <random>


Playfield::Playfield()
{
	fieldSize = 14.0;
	tileSize = 2.0;
	tileNumber = pow(fieldSize / tileSize, 2);
	tileSideNumber = fieldSize / tileSize;
	tiles.resize(tileSideNumber);
	shovelPos = { 4,3 };
	excPos = { 3,3 };
	for (int i = 0; i < tileSideNumber; ++i)
	{
		tiles[i].resize(tileSideNumber);
	}
	//std::cout << "tilesNumber:" << tileNumber << std::endl;
	//std::cout << "tilesSize:" << tiles.size() << std::endl;

	float xPos = 0.0;
	float zPos = 0.0;

	for (int i = 0; i < tileSideNumber; i++)
	{
		for (int j = 0; j < tileSideNumber; j++)
		{
			tiles[i][j].setSize(tileSize);
			tiles[i][j].setPosition(xPos, zPos);
			//std::cout << "modulo:" << (i+1) % int(fieldSize / tileSize) << std::endl;
			if ((j + 1) % int(tileSideNumber) == 0)
			{
				xPos = 0.0;
				zPos += tileSize;
			}
			else
			{
				xPos += tileSize;
			}
			//std::cout << "tilePos:" << tiles[i].xPosition << "; " << tiles[i].zPosition << std::endl;
		}
	}

	tiles[0][1].hasTreasure = true;
	tiles[1][5].hasTreasure = true;
	tiles[2][2].hasTreasure = true;
	tiles[3][5].hasTreasure = true;
	tiles[4][0].hasTreasure = true;
	tiles[5][4].hasTreasure = true;
	tiles[6][6].hasTreasure = true;
	tiles[3][3].hasExcavator = true;
	newTreasureFound = false;
}


Playfield::~Playfield()
{
}

float Playfield::getFieldSize()
{
	return fieldSize / 2;
}

void Playfield::drawPlayfield(MVPHandler mvp, GLuint programID) {
	//std::cout << "tilesSize:" << tiles.size() << std::endl;
	glm::mat4 Save = mvp.getModel();
	mvp.setModel(glm::translate(mvp.getModel(), glm::vec3(-fieldSize/2 + 1.0, 0, -fieldSize/2 + 1.0)));
	for (int i = 0; i < tileSideNumber; i++)
	{
		for (int j = 0; j < tileSideNumber; j++)
		{
			tiles[i][j].hasExcavator = false;
			tiles[i][j].hasShovel = false;
		}
	}
	if (shovelPos[0] >= 0 && shovelPos[0] <= 6 && shovelPos[1] >= 0 && shovelPos[1] <= 6)
	{
		tiles[shovelPos[0]][shovelPos[1]].hasShovel = true;
	}
	tiles[excPos[0]][excPos[1]].hasExcavator = true;
	for (int i = 0; i < tileSideNumber; i++)
	{
		for (int j = 0; j < tileSideNumber; j++)
		{
			if (tiles[i][j].isExcavated) {
				glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 3);
			} else {
				glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 2);
			}
			tiles[i][j].drawTile(mvp);
		}
	}
	mvp.setModel(Save);
}

void Playfield::changeExcPos(int x, int z, int orientation)
{
	if ((excPos[0] + x >= 0) && (excPos[0] + x <= 6)) {
		excPos[0] += x;
	}
	if ((excPos[1] + z >= 0) && (excPos[1] + z <= 6))
	{
		excPos[1] += z;
	}

	if (orientation == 0)
	{
		if ((shovelPos[0] + x >= 1) && (shovelPos[0] + x <= 7)) {
			shovelPos[0] += x;
		}
		if ((shovelPos[1] + z >= 0) && (shovelPos[1] + z <= 6))
		{
			shovelPos[1] += z;
		}
	}
	else if (orientation == 90)
	{
		if ((shovelPos[0] + x >= 0) && (shovelPos[0] + x <= 6)) {
			shovelPos[0] += x;
		}
		if ((shovelPos[1] + z >= 1) && (shovelPos[1] + z <= 7))
		{
			shovelPos[1] += z;
		}
	}
	else if (orientation == 180)
	{
		if ((shovelPos[0] + x >= -1) && (shovelPos[0] + x <= 5)) {
			shovelPos[0] += x;
		}
		if ((shovelPos[1] + z >= 0) && (shovelPos[1] + z <= 6))
		{
			shovelPos[1] += z;
		}
	}
	else if (orientation == 270)
	{
		if ((shovelPos[0] + x >= 0) && (shovelPos[0] + x <= 6)) {
			shovelPos[0] += x;
		}
		if ((shovelPos[1] + z >= -1) && (shovelPos[1] + z <= 5))
		{
			shovelPos[1] += z;
		}
	}
}

void Playfield::turnShovel(int orientation, int turn)
{
	int orientationPlusTurn;
	if (fmod(orientation + turn, 360) < 0)
	{
		orientationPlusTurn = 360 + fmod(orientation + turn, 360);
	}
	else
	{
		orientationPlusTurn = fmod(orientation + turn, 360);
	}
	if (orientationPlusTurn == 0)
	{
		shovelPos[0] = excPos[0] + 1;
		shovelPos[1] = excPos[1];
	}
	else if (orientationPlusTurn == 90)
	{
		shovelPos[0] = excPos[0];
		shovelPos[1] = excPos[1] + 1;
	}
	else if (orientationPlusTurn == 180)
	{
		shovelPos[0] = excPos[0] - 1;
		shovelPos[1] = excPos[1];
	}
	else if (orientationPlusTurn == 270)
	{
		shovelPos[0] = excPos[0];
		shovelPos[1] = excPos[1] - 1;
	}
}

void Playfield::dig()
{
	if ((shovelPos[0] >= 0) && (shovelPos[0] <= 6) && (shovelPos[1] >= 0) && (shovelPos[1] <= 6)) {
		tiles[shovelPos[0]][shovelPos[1]].isExcavated = true;
		if (tiles[shovelPos[0]][shovelPos[1]].hasTreasure && !tiles[shovelPos[0]][shovelPos[1]].treasureAlreadyFound)
		{
			tiles[shovelPos[0]][shovelPos[1]].treasureAlreadyFound = true;
			newTreasureFound = true;
			lastTresureX = tiles[shovelPos[0]][shovelPos[1]].xPosition;
			lastTresureZ = tiles[shovelPos[0]][shovelPos[1]].zPosition;
		}
	}
}