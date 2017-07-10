#include "PlayfieldTile.h"
#include "objects.hpp"

PlayfieldTile::PlayfieldTile()
{
	isExcavated = false;
	hasTreasure = false;
	hasExcavator = false;
	hasShovel = false;
}

/*
PlayfieldTile::PlayfieldTile(float size, float xPos, float zPos)
{
	tileSize = size /2;
	xPosition = xPos;
	zPosition = zPos;
}
*/

PlayfieldTile::~PlayfieldTile()
{
}

void PlayfieldTile::setSize(float size)
{
	tileSize = size /2;
}

void PlayfieldTile::setPosition(float xPos, float zPos)
{
	xPosition = xPos;
	zPosition = zPos;
}

void PlayfieldTile::drawTile(MVPHandler mvp) {

	glm::mat4 Save = mvp.getModel();

	// plane
	mvp.setModel(glm::translate(mvp.getModel(), glm::vec3(xPosition, 0, zPosition)));
	mvp.setModel(glm::scale(mvp.getModel(), glm::vec3(tileSize, 0.01, tileSize)));
	mvp.sendMVP();
	drawCube();
	mvp.setModel(Save);
}