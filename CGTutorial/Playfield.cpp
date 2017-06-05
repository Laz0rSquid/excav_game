#include "Playfield.h"
#include "objects.hpp"


Playfield::Playfield()
{
	fieldSize = 6.0;
}


Playfield::~Playfield()
{
}

float Playfield::getFieldSize()
{
	return fieldSize;
}

void Playfield::drawPlayfield(MVPHandler mvp) {

	glm::mat4 Save = mvp.getModel();

	// plane
	mvp.setModel(glm::scale(mvp.getModel(), glm::vec3(fieldSize, 0.0, fieldSize)));
	mvp.sendMVP();
	drawCube();
	mvp.setModel(Save);
}