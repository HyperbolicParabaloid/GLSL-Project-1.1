#ifndef TREE_CLASS_H
#define TREE_CLASS_H

#include"Object.h"


struct Limb {
	
};

// Sphere class.
//
// Creates a Sphere of a given "smoothness" (A.K.A. the number of triangles it has)
// at a given position, with given textures etc.
class Tree : public Object
{
private:
	void genTriangles();
	void genCone(glm::vec3 _startPos, glm::vec3 _endPos, float _bottomRadius, float _topRadius);
	void genDome(float _topRadius);
	void setConeVertices();
	void setDomeVertices();
	int numVertsPerSide(int _level);

	glm::vec3* preVerts = new glm::vec3[1];		// glm:V:ec3 array that gets resized and stuffed with vertex information.
	glm::vec2* texCoords = new glm::vec2[1];	// glm::VecW array that gets resized and stuffed with texture coordinate information.

	int preVertsSize;
	int coneLevel;
	int sphereLevel;

	bool isSmooth;
	bool randomColor;

	float trunkRadius;
	float globalScale;

	glm::vec3 trunkPointPos;
	glm::vec3 trunkPointingAt;
	glm::vec3 objPos;

	glm::mat4 limbModel;

	glm::vec4 shaftColor, coneColor;

	int branchNum;
	int indCount;
	int maxDepth;
	int seed;
public:
	Tree(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, int _coneLevel, int _sphereLevel, float _bottomRadius, glm::vec3 _pointPos, bool _isSmooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera);
	~Tree();
	void setSphereLevel(int _sphereLevel);
	void setConeLevel(int _coneLevel);
	void doRandomColors(bool _randomColor);
	void reseed();
	void smoothSurface(bool _isSmooth);

	void newLimb(glm::vec3 _parentStartPos, glm::vec3 _parentEndPos, glm::vec3 _parentPointingAt, float _parentEndRadius, int _crntDepth);
	void newLimbs(glm::vec3 _parentStartPos, glm::vec3 _parentEndPos, glm::vec3 _parentPointingAt, float _parentEndRadius, int _crntDepth);
};

#endif

