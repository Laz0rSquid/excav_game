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
	tiles.resize(tileNumber);
	std::cout << "tilesNumber:" << tileNumber << std::endl;
	std::cout << "tilesSize:" << tiles.size() << std::endl;

	float xPos = 0.0;
	float zPos = 0.0;

	for (int i = 0; i < tileNumber; i++)
	{
		tiles[i].setSize(tileSize);
		tiles[i].setPosition(xPos, zPos);
		std::cout << "modulo:" << (i+1) % int(fieldSize / tileSize) << std::endl;
		if ((i+1) % int(fieldSize / tileSize) == 0)
		{
			xPos = 0.0;
			zPos += tileSize;
		}
		else
		{
			xPos += tileSize;
		}
		std::cout << "tilePos:" << tiles[i].xPosition << "; " << tiles[i].zPosition << std::endl;
	}
}


Playfield::~Playfield()
{
}

float Playfield::getFieldSize()
{
	return fieldSize;
}

void Playfield::drawPlayfield(MVPHandler mvp, GLuint programID) {
	//std::cout << "tilesSize:" << tiles.size() << std::endl;
	for each (PlayfieldTile tile in tiles)
	{
		tile.drawTile(mvp);
	}


	/*
	glm::mat4 Save = mvp.getModel();

	// plane
	mvp.setModel(glm::scale(mvp.getModel(), glm::vec3(fieldSize, 0.0, fieldSize)));
	mvp.sendMVP();
	drawCube();
	//mvp.setModel(Save);

	for (int i = 0; i < tileNumber; i++)
	{
		if (i % 2)
		{
			glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 2);
			tiles[i].drawTile(mvp);
		}
		else
		{
			glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 1);
			tiles[i].drawTile(mvp);
		}
	}

	Save = mvp.getModel();
	*/

}