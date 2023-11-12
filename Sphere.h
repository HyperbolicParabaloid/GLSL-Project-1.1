#ifndef SPHERE_CLASS_H
#define SPHERE_CLASS_H

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
#include<cstdlib>

#include"shaderClass.h"
#include"Texture.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include "Object.h"

class Sphere : public Object
{
private:
	void genTriangles();
	int numVertsPerSide(int _level);
	void setPreIndices();
	void setNorms();
	void setPostIndices();

	glm::vec3* preVerts = new glm::vec3[1];

	GLuint* indices = new GLuint[1];
	GLfloat* postVerts = new GLfloat[1];

	int postVertsSize;
	int preVertsSize;
	int indicesSize;
	int level;

	bool isSmooth;
	bool randomColor;
public:
	Sphere(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, int _level, bool _isSmooth, glm::vec4 _color, Camera* _camera);
	~Sphere();
	void genOctahedron();
	void setLevel(int _level);
	void doRandomColors(bool _randomColor);
	void smoothSurface(bool _isSmooth);
};

#endif