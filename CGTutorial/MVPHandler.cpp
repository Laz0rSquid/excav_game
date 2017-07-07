#include "MVPHandler.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


MVPHandler::MVPHandler(GLuint pid, float zoomLevel)
{
	programID = pid;
	setDefaultMVP(zoomLevel);
}


MVPHandler::~MVPHandler()
{
}


void MVPHandler::sendMVP()
{
	glm::mat4 MVP = Projection * View * Model;
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform, konstant fuer alle Eckpunkte
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);
}

void MVPHandler::setDefaultMVP(float zoomLevel)
{
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera matrix
	View = glm::lookAt(glm::vec3(zoomLevel * 3, zoomLevel * 6, zoomLevel *  -8), // Camera is at (0,0,-5), in World Space
		glm::vec3(0, 0, 0),  // and looks at the origin
		glm::vec3(0, 1, 0)); // Head is up (set to 0,-1,0 to look upside-down)

	// Model matrix : an identity matrix (model will be at the origin)
	Model = glm::mat4(1.0f);
}

glm::mat4 MVPHandler::getModel()
{
	return Model;
}
void MVPHandler::setModel(glm::mat4 m)
{
	Model = m;
}

void MVPHandler::setPID(GLuint pid)
{
	programID = pid;
}