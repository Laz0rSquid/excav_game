#include "Playfield.h"
#include "objects.hpp"
#include "Math.h"
#include "PlayfieldTile.h"
#include <iostream>


Playfield::Playfield()
{
	fieldSize = 14.0;
	tileSize = 2.0;
	tileNumber = pow(fieldSize / tileSize, 2);
	tileSideNumber = fieldSize / tileSize;
	tiles.resize(tileSideNumber);
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
			tiles[i][j].drawTile(mvp);
		}
	}
	mvp.setModel(Save);
}