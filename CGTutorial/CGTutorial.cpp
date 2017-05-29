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




void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
// vehicle position
float vehicleX = 0.0;
float vehicleZ = 0.0;

// max length of worldplane
float maxRange = 6.0;

// step length of vehicle
float vehicleStepLength = 0.1;

// model size
float modelSize = 0.8;

//needed later
float angleX = 0.0;
float angleY = 0.0;
float angleZ = 0.0;

float joint1X = 0.0;
float joint1Z = 0.0;
float joint2 = 0.0;
float joint3 = 0.0;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_LEFT:
		if (vehicleX < maxRange - modelSize)
		{
			vehicleX += vehicleStepLength;
		}
		break;
	case GLFW_KEY_RIGHT:
		if (vehicleX > -maxRange + modelSize)
		{
			vehicleX -= vehicleStepLength;
		}
		break;
	case GLFW_KEY_UP:
		if (vehicleZ < maxRange - modelSize)
		{
			vehicleZ += vehicleStepLength;
		}
		break;
	case GLFW_KEY_DOWN:
		if (vehicleZ > -maxRange + modelSize)
		{
			vehicleZ -= vehicleStepLength;
		}
		break;
	default:
		break;
	}
}


// Diese Drei Matrizen global (Singleton-Muster), damit sie jederzeit modifiziert und
// an die Grafikkarte geschickt werden koennen
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
GLuint programID;

void sendMVP()
{
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; 
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform, konstant fuer alle Eckpunkte
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);
}

void drawModel(float size) {
	glm::mat4 Save = Model;

	Model = glm::translate(Model, glm::vec3(vehicleX, size, vehicleZ));
	Model = glm::scale(Model, glm::vec3(size, size, size));
	sendMVP();
	drawSphere(10, 10);
	drawWireCube();
	Model = Save;
}

// draws plane
void drawPlayfield() {
	
	glm::mat4 Save = Model;

	// plane
	Model = glm::scale(Model, glm::vec3(maxRange,0.0,maxRange));
	sendMVP();
	drawCube();
	Model = Save;
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

	// make textures
	//glGenTextures(1, &RobotTexture);
	//glGenTextures(2, &GrassTexture);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, RobotTexture);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, GrassTexture);

	// Set our "myTextureSampler" sampler to user Texture Unit 0
	//glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0);
	//glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 1);

	// Eventloop
	while (!glfwWindowShouldClose(window))
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

		//
		//glm::vec3 lightPos = glm::vec3(4, 4, -4);
		
		
		// Camera matrix
		View = glm::lookAt(glm::vec3(3,6,-8), // Camera is at (0,0,-5), in World Space
						   glm::vec3(0,0,0),  // and looks at the origin
						   glm::vec3(0,1,0)); // Head is up (set to 0,-1,0 to look upside-down)
		
		// Model matrix : an identity matrix (model will be at the origin)
		Model = glm::mat4(1.0f);

		/*
		// rotates cube by angle on the y axis
		Model = glm::rotate(Model, angleX, glm::vec3(0.0, 1.0, 0.0));
		Model = glm::rotate(Model, angleY, glm::vec3(1.0, 0.0, 0.0));
		Model = glm::rotate(Model, angleZ, glm::vec3(0.0, 0.0, 1.0));
		*/
		//
		glm::mat4 Save = Model;
		Model = glm::translate(Model, glm::vec3(1.5, 0.0, 0.0));
		//


		//
		Model = Save;
		Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
		sendMVP();
		
		// custom draw functions
		glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 2);
		drawPlayfield();
		
		// draw model
		glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 1);
		drawModel(modelSize);

		// lamp position
		glm::vec4 lightPos = Model * glm::vec4(0, 0.9, 0, 1);
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);


		//Model = glm::translate(Model, glm::vec3(0.0, 0.9, 0.0));
		

		// drawWireCube() replaced with drawCube()
		// drawCube();		

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

	glDeleteProgram(programID);


	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

