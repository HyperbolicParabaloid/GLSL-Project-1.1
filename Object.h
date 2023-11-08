#ifndef OBJECT_CLASS_H
#define OBJECT_CLASS_H

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
#include<vector>

#include"shaderClass.h"
#include"Texture.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include "Camera.h"

/*
This class is for creating basic objects. It's meant to be a parent class.
*/
class Object
{
private:
	Shader *shaderProgram;
	Camera* camera;

	VAO objVAO;	// For holding VBO and EBO stuff
	VBO* objVBO;
	EBO* objEBO;

	GLfloat* vertices;
	int vertSize;
	GLuint* indices;
	int indSize;

	GLFWwindow* window;

	glm::mat4 model;
	glm::vec3 objPos;
	float objScale;

public:
	Object(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 color, Camera* _camera);
	Object(GLFWwindow* _window);
	~Object();
	void draw(glm::vec3 _lightPos, glm::vec4 _lightColor);
	glm::vec4 color;
	void setVBOandEBO(GLfloat* _vertices, int _vertSize, GLuint* _indices, int _indSize);
	void rotate(float rotationDegreeAngle, glm::vec3 axisOfRotation);
};

#endif