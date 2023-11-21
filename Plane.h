#ifndef PLANE_CLASS_H
#define PLANE_CLASS_H

#include "Object.h"

// Creates a simple 2D plane. For now.
class Plane : public Object
{
//private:
//	void genTriangles();
//public:
//	Plane(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, bool _isSmooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera);
//	~Plane();
private:
	void genTriangles();
	int numVertsPerSide(int _level);
	void setVerticesVector();

	std::vector <Vertex> verts;		// Vertex vector that gets sent to Object class to put information into the GPU.
	std::vector <GLuint> indices;	// GLuint vector that gets sent to Object class to put indices into the GPU.

	glm::vec3* preVerts = new glm::vec3[1];		// glm:V:ec3 array that gets resized and stuffed with vertex information.
	glm::vec2* texCoords = new glm::vec2[1];	// glm::VecW array that gets resized and stuffed with texture coordinate information.

	int preVertsSize;
	int level;

	bool isSmooth;
	bool randomColor;

	int seed;
public:
	Plane(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, int _level, bool _isSmooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera);
	~Plane();
	void genOctahedron();
	void setLevel(int _level);
	void doRandomColors(bool _randomColor);
	void reseed();
	void smoothSurface(bool _isSmooth);
	void setScale(float _scale);
};

#endif