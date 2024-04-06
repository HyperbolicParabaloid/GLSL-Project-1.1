#ifndef INSTANCESPHERE_CLASS_H
#define INSTANCESPHERE_CLASS_H

#include"Object.h"

struct Ball {
	float sMass = 1.f;// PI* dot(radi, radi);
	glm::vec3 sForce = glm::vec3(0.f);
	glm::vec3 sVelocity = glm::vec3(0.f);
	glm::vec3 ballPos = glm::vec3(0.f);
	glm::vec4 ballColor = glm::vec4(1.f);
	glm::vec3 ballRadi = glm::vec3(0.2f);
};

// InstanceSphere class.
//
// Creates a InstanceSphere of a given "smoothness" (A.K.A. the number of triangles it has)
// at a given position, with given textures etc.
class InstanceSphere : public Object
{
private:
	void genTriangles();
	int numVertsPerSide(int _level);
	void setVerticesVector();
	void genBalls();

	//std::vector <Vertex> verts;		// Vertex vector that gets sent to Object class to put information into the GPU.
	//std::vector <GLuint> indices;	// GLuint vector that gets sent to Object class to put indices into the GPU.

	glm::vec3* preVerts = new glm::vec3[1];		// glm:V:ec3 array that gets resized and stuffed with vertex information.
	glm::vec2* texCoords = new glm::vec2[1];	// glm::VecW array that gets resized and stuffed with texture coordinate information.

	int preVertsSize;

	double crntTime = 0.f, prevTime = 0.f;

	std::vector <VertexI> verticesGeneral;
	std::vector <VertexIBall> verticesIBall;
	std::vector <Ball*> balls;

	unsigned int instances;
	unsigned int instancesVBO_ID;
	float restitution = 0.4f;
	float velocityDampeningRestitution = 1.f;
	float dampeningSensitivity = 0.f;

public:

	InstanceSphere(GLFWwindow* _window, unsigned int _instances, glm::vec3 _objPos, glm::vec3 _radi, float _objScale, int _level, bool _isSmooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera);
	~InstanceSphere();
	void genOctahedron();
	void setLevel(int _level);
	void doRandomColors(bool _randomColor);
	void reseed();
	void smoothSurface(bool _isSmooth);

	// Ellipsoid intersection test.
	glm::vec3 isRayTouching(glm::vec3 _rayStart, glm::vec3 _rayDir);

	void calculateNewPos(glm::vec3 _newForce);
	void resolveCollision(int bIndex1, int bIndex2, float _t);
	void applyBounds(int matIndex);

	void setVBOandEBO(std::string msg);
	void draw(glm::vec3 _lightPos, glm::vec4 _lightColor);
	void setVelocityDampeningRestitution(float _dampeningValue);
	void setDampeningSensitivity(float _sensitivityValue);

};

#endif