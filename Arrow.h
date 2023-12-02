#ifndef ARROW_CLASS_H
#define ARROW_CLASS_H

#include"Object.h"

// Sphere class.
//
// Creates a Sphere of a given "smoothness" (A.K.A. the number of triangles it has)
// at a given position, with given textures etc.
class Arrow
{
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
	int startingIndex;

	bool isSmooth;
	bool randomColor;
	float objScale;

	float bottomRadius;
	float topRadius;
	glm::vec3 pointPos;
	glm::vec3 objPos;
	glm::vec3 pointingAt;
	glm::vec4 color;

	glm::mat4 model;

	int seed;
public:
	Arrow(glm::vec3 _objPos, float _objScale, int _level, float _bottomRadius, float _topRadius, glm::vec3 _pointPos, glm::vec3 _pointingAt, bool _isSmooth, glm::vec4 _color, int _startingIndex);
	~Arrow();
	void genCone();
	void setLevel(int _level);
	void doRandomColors(bool _randomColor);
	void reseed();
	void smoothSurface(bool _isSmooth);
	void setTopRadius(float _topRadius);
	void setBottomRadius(float _bottomRadius);
	void setTipPos(glm::vec3 _pointPos);
	std::vector <Vertex> getVerts();
	std::vector <GLuint> getInds();
};

#endif

<<<<<<< HEAD

=======
>>>>>>> 2178be62a09d83fc8dd09a2c1b0a6eb73075cfb3
