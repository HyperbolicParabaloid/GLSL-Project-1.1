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

#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include"shaderClass.h"
#include"Camera.h"

/*
This class is for creating basic objects. It's meant to be a parent class.
*/
class Object
{
private:
	GLFWwindow* window;

	glm::mat4 model;
	glm::vec3 objPos;
	std::vector <Texture> textures;

	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;

	VAO VAO;

public:
	Shader* shaderProgram;
	Camera* camera;
	Object(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 color, std::vector <Texture>& _textures, Camera* _camera);
	~Object();
	void draw(glm::vec3 _lightPos, glm::vec4 _lightColor);
	glm::vec4 color;
	float objScale;
	void setVBOandEBO(std::vector <Vertex>& _vertices, std::vector <GLuint>& _indices, std::string msg);
	//void setVBOandEBO(std::vector <Triangle>* _triangles, int _trianglesSize, GLuint* _indices, int _indSize, std::string msg);
	void rotate(float rotationDegreeAngle, glm::vec3 axisOfRotation);
	void moveFirstVertex();

	void hotRealoadShader();

	float newrand(glm::vec2 co);
	float noise(glm::vec2 n);
};

#endif