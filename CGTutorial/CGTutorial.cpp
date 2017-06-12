// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// Achtung, die OpenGL-Tutorials nutzen glfw 2.7, glfw kommt mit einem veränderten API schon in der Version 3 

// Befindet sich bei den OpenGL-Tutorials unter "common"
#include "shader.hpp"

// Wuerfel und Kugel
#include "objects.hpp"

// Ab Uebung5 werden objloader.hpp und cpp benoetigt
#include "objloader.hpp"

// Ab Uebung7 werden texture.hpp und cpp benoetigt
#include "texture.hpp"

// MVPHandler class
#include "MVPHandler.h"

// excavator class
#include "Excavator.h"

// Controls class
#include "Controls.h"

// Playfield class
#include "Playfield.h"

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

Excavator excavator;

GLuint programID;

MVPHandler MVP(programID);

Playfield playfield;

Controls ctrls(playfield.getFieldSize());

//needed later
float angleX = 0.0;
float angleY = 0.0;
float angleZ = 0.0;

float joint1X = 0.0;
float joint1Z = 0.0;
float joint2 = 0.0;
float joint3 = 0.0;

// max length of worldplane
//float maxRange = 6.0;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ctrls.keyPress(window, key, scancode, action, excavator, MVP);
}

/*
// draws plane
void drawPlayfield(MVPHandler mvp) {
	
	glm::mat4 Save = mvp.getModel();

	// plane
	mvp.setModel(glm::scale(MVP.getModel(), glm::vec3(maxRange,0.0,maxRange)));
	mvp.sendMVP();
	drawCube();
	mvp.setModel(Save);
}
*/

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Fehler werden auf stderr ausgegeben, s. o.
	glfwSetErrorCallback(error_callback);

	// Open a window and create its OpenGL context
	// glfwWindowHint vorher aufrufen, um erforderliche Resourcen festzulegen
	GLFWwindow* window = glfwCreateWindow(1024, // Breite
										  768,  // Hoehe
										  "CG - Tutorial", // Ueberschrift
										  NULL,  // windowed mode
										  NULL); // shared windoe

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make the window's context current (wird nicht automatisch gemacht)
    glfwMakeContextCurrent(window);

	// Initialize GLEW
	// GLEW ermöglicht Zugriff auf OpenGL-API > 1.1
	glewExperimental = true; // Needed for core profile

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Auf Keyboard-Events reagieren
	glfwSetKeyCallback(window, key_callback);

	// Turquoise background
	glClearColor(0.0f, 0.7f, 0.7f, 0.0f);

	// z-test activate
	glEnable(GL_DEPTH_TEST);

	// pixels behind others get deleted
	glDepthFunc(GL_LESS);

	// Create and compile our GLSL program from the shaders
	// programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	MVP.setPID(programID);
	
	// Shader auch benutzen !
	glUseProgram(programID);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ("teapot.obj", vertices, uvs, normals);

	// Jedes Objekt eigenem VAO zuordnen, damit mehrere Objekte moeglich sind
	// VAOs sind Container fuer mehrere Buffer, die zusammen gesetzt werden sollen.
	GLuint VertexArrayIDTeapot;
	glGenVertexArrays(1, &VertexArrayIDTeapot);
	glBindVertexArray(VertexArrayIDTeapot);

	// Ein ArrayBuffer speichert Daten zu Eckpunkten (hier xyz bzw. Position)
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer); // Kennung erhalten
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // Daten zur Kennung definieren
												 // Buffer zugreifbar für die Shader machen
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);


	// Erst nach glEnableVertexAttribArray kann DrawArrays auf die Daten zugreifen...
	glEnableVertexAttribArray(0); // siehe layout im vertex shader: location = 0 
	glVertexAttribPointer(0,  // location = 0 
		3,  // Datenformat vec3: 3 floats fuer xyz 
		GL_FLOAT,
		GL_FALSE, // Fixedpoint data normalisieren ?
		0, // Eckpunkte direkt hintereinander gespeichert
		(void*)0); // abweichender Datenanfang ? 

	// normal buffer
	GLuint normalbuffer; // Hier alles analog für Normalen in location == 2
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2); // siehe layout im vertex shader 
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// texture buffer
	GLuint uvbuffer; // Hier alles analog für Texturkoordinaten in location == 1 (2 floats u und v!)
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // siehe layout im vertex shader 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Load the texture
	GLuint RobotTexture = loadBMP_custom("robot_texture.bmp");
	GLuint GrassTexture = loadBMP_custom("GrasTextureAlternative.bmp");

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, RobotTexture);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, GrassTexture);

	// Eventloop
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		MVP.setDefaultMVP();

		/*
		// rotates cube by angle on the y axis
		Model = glm::rotate(Model, angleX, glm::vec3(0.0, 1.0, 0.0));
		Model = glm::rotate(Model, angleY, glm::vec3(1.0, 0.0, 0.0));
		Model = glm::rotate(Model, angleZ, glm::vec3(0.0, 0.0, 1.0));
		*/
		//
		
		// camera position
		glm::mat4 Save = MVP.getModel();
		MVP.setModel(glm::translate(MVP.getModel(), glm::vec3(1.5, 0.0, 0.0)));
		MVP.setModel(Save);
		MVP.setModel(glm::scale(MVP.getModel(), glm::vec3(0.5, 0.5, 0.5)));
		MVP.sendMVP();
		
		// draw playfield
		glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 2);
		//playfield.drawPlayfield(MVP);
		
		// draw excavator
		glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 1);
		double thisTime = glfwGetTime();
		if (ctrls.animationActive == true)
		{
			excavator.animateBodyForward(thisTime - lastTime, playfield.getFieldSize());
			lastTime = thisTime;
		}
		if (thisTime - lastTime >= 5.0) {
			ctrls.animationActive = false;
		}

		excavator.drawExcavator(MVP);

		// lamp position
		glm::vec4 lightPos = MVP.getModel() * glm::vec4(0, 10.9, 5.0, 1);
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
		
		// Swap buffers
		glfwSwapBuffers(window);

		// Poll for and process events 
        glfwPollEvents();
	} 

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &normalbuffer);

	glDeleteBuffers(1, &uvbuffer);
	glDeleteTextures(1, &RobotTexture);
	glDeleteTextures(2, &GrassTexture);

	glDeleteProgram(programID);


	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

