// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

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

// Playfield class
#include "Playfield.h"

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

Excavator excavator;

GLuint programID;

float zoomLevel = 1.0;

MVPHandler MVP(programID, zoomLevel);

Playfield playfield;

bool animationActive;

double animationStartTime;

static double limitFPS = 1.0 / 60.0;

double lastTime = glfwGetTime(), timer = lastTime;

double deltaTime = 0, nowTime = 0, startTime = 0;

double animationDuration = 0.5;

float animationDistance = 2.0;

float stepLength = animationDistance / (animationDuration * 60);

int frames = 0;

int updates = 0;

int keyPressed;

float startOrientation;

float endOrientation;

float startPos;

float endPos;

float coinHeight;

float coinOrientation;

bool moveDown;

int windowWidth = 1024;

int windowHeight = 768;

void setAnimationActive(bool status, int key)
{
	animationActive = status;
	if (status == true)
	{
		animationStartTime = glfwGetTime();
	}
	keyPressed = key;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (!animationActive) {
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_W:
			setAnimationActive(true, key);
			playfield.changeExcPos(1, 0, excavator.getOrientation());
			break;
		case GLFW_KEY_S:
			setAnimationActive(true, key);
			playfield.changeExcPos(-1, 0, excavator.getOrientation());
			break;
		case GLFW_KEY_A:
			setAnimationActive(true, key);
			playfield.changeExcPos(0, 1, excavator.getOrientation());
			break;
		case GLFW_KEY_D:
			setAnimationActive(true, key);
			playfield.changeExcPos(0, -1, excavator.getOrientation());
			break;
		case GLFW_KEY_Q:

			playfield.turnShovel(excavator.getOrientation(), 90);
			setAnimationActive(true, key);
			break;
		case GLFW_KEY_E:
			playfield.turnShovel(excavator.getOrientation(), -90);
			setAnimationActive(true, key);
			break;
		case GLFW_KEY_SPACE:
			moveDown = true;
			setAnimationActive(true, key);
			playfield.dig();
			break;
		case GLFW_KEY_UP:
			if (zoomLevel > 0.1)
			{
				zoomLevel -= 0.05;
			}
			else
			{
				zoomLevel = 0.1;
			}
			break;
		case GLFW_KEY_DOWN:
			if (zoomLevel < 2.0)
			{
				zoomLevel += 0.05;
			}
			else
			{
				zoomLevel = 2.0;
			}
			break;
		default:
			break;
		}
	}
}

void playAnimations() {

	double animationDuration;

	float animationDistance;

	float stepLength;
	
	// - Measure time
	nowTime = glfwGetTime();
	deltaTime += (nowTime - lastTime) / limitFPS;
	lastTime = nowTime;

	
	if (!animationActive)
	{
		startOrientation = abs(excavator.getOrientation());
		endOrientation = startOrientation + 90;
		startPos = 0;
		
		//endPos = 2;
	}

	// - Only update at 60 frames / s
	while (deltaTime >= 1.0) {
		if (animationActive)
		{
			switch (keyPressed)
			{
			case GLFW_KEY_W:
				animationDuration = 0.5;
				animationDistance = 2.0;
				stepLength = animationDistance / (animationDuration * 60);
				if (startPos < animationDistance)
				{
					excavator.moveBodyUp(playfield.getFieldSize(), stepLength);   // - Update function
				}
				startPos += stepLength;
				break;
			case GLFW_KEY_S:
				animationDuration = 0.5;
				animationDistance = 2.0;
				stepLength = animationDistance / (animationDuration * 60);
				if (startPos < animationDistance)
				{
					excavator.moveBodyDown(playfield.getFieldSize(), stepLength);
				}
				startPos += stepLength;
				break;
			case GLFW_KEY_A:
				animationDuration = 0.5;
				animationDistance = 2.0;
				stepLength = animationDistance / (animationDuration * 60);
				if (startPos < animationDistance)
				{
					excavator.moveBodyLeft(playfield.getFieldSize(), stepLength);
				}
				startPos += stepLength;
				break;
			case GLFW_KEY_D:
				animationDuration = 0.5;
				animationDistance = 2.0;
				stepLength = animationDistance / (animationDuration * 60);
				if (startPos < animationDistance)
				{
					excavator.moveBodyRight(playfield.getFieldSize(), stepLength);
				}
				startPos += stepLength;
				break;
			case GLFW_KEY_Q:
				animationDuration = 0.3;
				animationDistance = 90.0;
				stepLength = animationDistance / (animationDuration * 60);
				if (startOrientation < endOrientation)
				{
					excavator.rotateBodyCounterClockwise(stepLength);
				}
				startOrientation += stepLength;
				break;
			case GLFW_KEY_E:
				animationDuration = 0.3;
				animationDistance = 90.0;
				stepLength = animationDistance / (animationDuration * 60);
				if (startOrientation < endOrientation)
				{
					excavator.rotateBodyClockwise(stepLength);
				}
				startOrientation += stepLength;
				break;
			case GLFW_KEY_SPACE:
				animationDuration = 0.5;
				animationDistance = excavator.getMaxBaseAngle() -  excavator.getMinbaseAngle();
				stepLength = (animationDistance / (animationDuration * 60)) * 2;
				if (moveDown && excavator.getBaseJointAngle() < excavator.getMaxBaseAngle())
				{
					excavator.bendBaseJointDown(stepLength);
					if (excavator.getBaseJointAngle() >= excavator.getMaxBaseAngle()) {
						moveDown = false;
					}
				}
				else if (!moveDown && excavator.getBaseJointAngle() > excavator.getMinbaseAngle())
				{
					excavator.bendBaseJointUp(stepLength);
				}
				if(playfield.newTreasureFound)
				{
					glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 4);
					glm::mat4 Save = MVP.getModel();
					MVP.setModel(glm::translate(MVP.getModel(), glm::vec3(playfield.lastTresureX - 6, coinHeight, playfield.lastTresureZ - 6)));
					MVP.setModel(glm::rotate(MVP.getModel(), coinOrientation, glm::vec3(0.0, 1, 0.0)));
					MVP.setModel(glm::scale(MVP.getModel(), glm::vec3(1., 1., .1)));

					MVP.sendMVP();
					drawSphere(10, 10);

					MVP.setModel(Save);
					glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 1);
					coinHeight += 0.1;
					coinOrientation = fmod((coinOrientation + 24), 360);
				}
				break;
			default:
				break;
			}
			
			if (nowTime - animationStartTime >= animationDuration) {
				setAnimationActive(false, NULL);
				playfield.newTreasureFound = false;
			}
		}
		updates++;
		deltaTime--;
	}
	// - Render at maximum possible frames
	excavator.drawExcavator(MVP); // - Render function
	frames++;

	// - Reset after one second
	if (glfwGetTime() - timer > 1.0) {
		timer++;
		updates = 0, frames = 0;
	}
	excavator.drawExcavator(MVP);
}


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
	GLFWwindow* window = glfwCreateWindow(windowWidth, // Breite
										  windowHeight,  // Hoehe
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
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	MVP.setPID(programID);
	
	// Shader auch benutzen !
	glUseProgram(programID);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ("dragon.obj", vertices, uvs, normals);

	// Jedes Objekt eigenem VAO zuordnen, damit mehrere Objekte moeglich sind
	// VAOs sind Container fuer mehrere Buffer, die zusammen gesetzt werden sollen.
	GLuint VertexArrayIDCoin;
	glGenVertexArrays(1, &VertexArrayIDCoin);
	glBindVertexArray(VertexArrayIDCoin);

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
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
			2,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
	);

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
	GLuint ExcavationTexture = loadBMP_custom("digged_up.bmp");
	GLuint CoinTexture = loadBMP_custom("coin_texture.bmp");

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, RobotTexture);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, GrassTexture);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, ExcavationTexture);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, CoinTexture);


	// Eventloop
	while (!glfwWindowShouldClose(window))
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		MVP.setDefaultMVP(zoomLevel);
		
		/*
		// draws coin - scale, send, draw
		glm::mat4 Save = MVP.getModel();
		MVP.setModel(glm::scale(MVP.getModel(), glm::vec3(1.0, 1.0, 1.0)));
		MVP.sendMVP();

		glBindVertexArray(VertexArrayIDCoin);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		//
		MVP.setModel(Save);
		*/
		
		MVP.setModel(glm::scale(MVP.getModel(), glm::vec3(0.5, 0.5, 0.5)));
		MVP.sendMVP();
		

		// draw playfield
		glm::mat4 Save = MVP.getModel();
		
		playfield.drawPlayfield(MVP, programID);
		MVP.setModel(Save);

		// draw excavator
		glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 1);

		// - Measure time

		playAnimations();
		if (!animationActive)
		{
			coinHeight = 1;
			coinOrientation = 0;
		}
		if (playfield.newTreasureFound)
		{
			glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 4);
			glm::mat4 Save = MVP.getModel();
			MVP.setModel(glm::translate(MVP.getModel(), glm::vec3(playfield.lastTresureX - 6, coinHeight, playfield.lastTresureZ - 6)));
			MVP.setModel(glm::rotate(MVP.getModel(), coinOrientation, glm::vec3(0.0, 1, 0.0)));
			MVP.setModel(glm::scale(MVP.getModel(), glm::vec3(1., 1., .1)));

			MVP.sendMVP();
			drawSphere(10, 10);

			MVP.setModel(Save);
			glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 1);
			coinHeight += 0.05;
			coinOrientation = fmod((coinOrientation + 2), 360);
		}
		
		excavator.drawExcavator(MVP);
		// lamp position
		glm::vec4 lightPos = MVP.getModel() * glm::vec4(0, 10.0, 0.0, 1.0);
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

