#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ctime>
#include <time.h>
#include <stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/vec1.hpp>

#include"shaderClass.h"
#include"Texture.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include "Camera.h"


#define PI 3.1415926538


// Setting up array of vertices.
	// Now I'm using this vertices array to hold the vertex info while
	// using the indicies buffer to hold the index of each vertex that
	// will be used to form each of the triangles (saves memory).
GLfloat vertices[] = {
	//			POSITION									COLOR
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.f,			0.8f, 0.5f, 0.0f,	// Lower Left	0
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.f,			0.8f, 0.0f, 0.5f,	// Lower Right	1
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.f,		0.0f, 0.5f, 0.5f,	// Upper Corner	2
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.f,		0.1f, 0.05f, 0.05f,	// Inner Left	3
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.f,		0.1f, 0.05f, 0.05f,	// Inner Right	4
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.f,			0.1f, 0.05f, 0.05f	// Inner Down	5
};
GLuint indices[] = {
	0, 3, 5,	// Lower Left triangle
	3, 2, 4,	// Lower Right triangle
	5, 4, 1		// Upper triangle
};

GLfloat sqr[] = {
	// POSITION					COLOR			TEXTURE COORDS
	-0.5f, -0.5f, 0.f,		0.0f, 0.9f, 0.9f,	0, 0,	// Lower Left	0
	0.5f, -0.5f, 0.f,		0.9f, 0.0f, 0.9f,	1, 0,	// Lower Right	1
	0.5f, 0.5f, 0.f,		0.9f, 0.9f, 0.0f,	1, 1,	// Upper Right	2
	-0.5f, 0.5f, 0.f,		0.9f, 0.9f, 0.9f,	0, 1	// Upper Left	3
};
GLuint sqrIndices[] = {
	0, 1, 2,	// Lower triangle
	3, 0, 2		// Upper triangle
};

// Vertices coordinates
GLfloat pyramidVertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	0.5f, 1.0f
};
/*
// Vertices coordinates
GLfloat pyramidVertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	0.5f, 1.0f
};
*/
// Indices for vertices order
GLuint pyramidIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

int viewWidth = 800, viewHeight = 800;

glm::vec2 getRotationAngle(GLFWwindow *window) {
	double xPos, yPos;
	glm::vec3 screenCenter = glm::vec3(0.f, 0.f, -1000.f);
	glm::vec3 cursorPosX, cursorPosY;
	float cursorMagX, cursorMagY, screenMag, _acos, _asin;

	// Making it so we can rotate the camera.
	// Getting the cursor's position in the window and setting the center to be the center of the window.
	glfwGetCursorPos(window, &xPos, &yPos);
	cursorPosX = glm::vec3(xPos - viewWidth / 2.f,	0.f,						-1000.f);
	cursorPosY = glm::vec3(0.f,						yPos - viewHeight / 2.f,	-1000.f);

	// Getting acos and asin angles in degrees.
	if (cursorPosX.x > 0.f)
		_acos = 180 * acos(glm::dot(cursorPosX, screenCenter) / (glm::length(cursorPosX) * glm::length(screenCenter)));
	else
		_acos = -180 * acos(glm::dot(cursorPosX, screenCenter) / (glm::length(cursorPosX) * glm::length(screenCenter)));

	if (cursorPosY.y > 0.f)
		_asin = 180 * acos(glm::dot(cursorPosY, screenCenter) / (glm::length(cursorPosY) * glm::length(screenCenter)));
	else
		_asin = -180 * acos(glm::dot(cursorPosY, screenCenter) / (glm::length(cursorPosY) * glm::length(screenCenter)));

	//std::cout << "( " << cursorPos.x << ", " << cursorPos.y << " )\tacos(x) = " << _acos << "\tasin(x) = " << _asin << std::endl;
	return glm::vec2(_acos, _asin);
}

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(viewWidth, viewWidth, "GLFW Testing Window", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, viewWidth, viewWidth);


	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Using Texture class to handle all the binding, activating and parameter setting of the texture.
	Texture tex("137215-barack-face-vector-obama-png-image-high-quality.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	tex.texUnit(shaderProgram, "tex0", 0);
	tex.Bind();

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(pyramidVertices, sizeof(pyramidVertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(pyramidIndices, sizeof(pyramidIndices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	GLuint rotationUniID = glGetUniformLocation(shaderProgram.ID, "rotationDegree");

	float frac, whole;
	glUniform1f(uniID, 0.5);
	
	float rotation = 0.f, prevTime = glfwGetTime();

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Initializing matrices for model, view and projection
	glm::mat4 model = glm::mat4(1.f);
	glm::mat4 view = glm::mat4(1.f);
	glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)(viewWidth / viewHeight), 0.1f, 100.f);

	Camera camera(window, glm::vec2(viewWidth, viewHeight), glm::vec3(0.f, 0.5f, 2.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	model = glm::translate(model, glm::vec3(0.f, 0.f, -5.f));

	double lockoutTimer = 0;
	bool shouldRotate = false, shouldFly = false, captureingMotion = false;

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		// Simple timer
		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60)
		{
			rotation = 0.5f;
			prevTime = crntTime;
		}

		// Rotating the model about the y axis
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

		// Rotating the view about the x and y axis
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_F) && lockoutTimer <= crntTime) {
			lockoutTimer = crntTime + 0.2;
			captureingMotion = !captureingMotion;
			camera.motion_enabled(captureingMotion);
			std::cout << "Crotation\n";
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_G) && lockoutTimer <= crntTime) {
			//lockoutTimer = crntTime + 0.2;
			//std::cout << "Crotation\n";
			//camera.motion_enabled(true);
			//shouldFly = true;
			//captureingMotion = true;
		}

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_T) && lockoutTimer <= crntTime) {
			lockoutTimer = crntTime + 0.2;
			std::cout << "Crotation\n";
			shouldFly = true;
		}

		if (shouldFly) {
			//if (camera.fly_to(glm::vec3(0.f, 0.f, -5.f))) 
			if (camera.fly_to(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.125f, -1.f), true)) {
				shouldFly = false;
			}
		}

		// Toggling the tracking of user movement
		camera.track_movement();

		// Setting view matrix with camera class
		view = camera.get_view();

		// Actually setting uniforms to be the values of model, veiw and projection
		GLuint uniModel = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		GLuint uniView = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

		GLuint uniProjection = glGetUniformLocation(shaderProgram.ID, "projection");
		glUniformMatrix4fv(uniProjection, 1, GL_FALSE, glm::value_ptr(projection));

		tex.Bind();

		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	// Making sure to use the tex classes delete function.
	tex.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}