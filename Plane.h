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
	void getPositions(int uu, int vv, int vertsPerSide, glm::vec3& v1, glm::vec3& n1, glm::vec3& v2, glm::vec3& n2, glm::vec3& v3, glm::vec3& n3, glm::vec3& v4, glm::vec3& n4, glm::vec3& v5, glm::vec3& n5, glm::vec3& v6, glm::vec3& n6);
	void getPositions(int uu, int vv, int vertsPerSide, glm::vec2& t1, glm::vec2& t2, glm::vec2& t3, glm::vec2& t4, glm::vec2& t5, glm::vec2& t6);
	glm::vec3 averageNormals(int _uu, int _vv, int vertsPerSide);

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