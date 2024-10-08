#ifndef SPHERE_CLASS_H
#define SPHERE_CLASS_H

#include"Object.h"

// Sphere class.
//
// Creates a Sphere of a given "smoothness" (A.K.A. the number of triangles it has)
// at a given position, with given textures etc.
class Sphere : public Object
{
private:
	void genTriangles();
	int numVertsPerSide(int _level);
	void setVerticesVector();

	//std::vector <Vertex> verts;		// Vertex vector that gets sent to Object class to put information into the GPU.
	//std::vector <GLuint> indices;	// GLuint vector that gets sent to Object class to put indices into the GPU.

	glm::vec3* preVerts = new glm::vec3[1];		// glm:V:ec3 array that gets resized and stuffed with vertex information.
	glm::vec2* texCoords = new glm::vec2[1];	// glm::VecW array that gets resized and stuffed with texture coordinate information.

	int preVertsSize;


	float sMass = PI * dot(radi, radi);
	glm::vec3 sForce = glm::vec3(0.f, -9.81 / 10.f, 0.f);
	glm::vec3 sVelocity = glm::vec3(0.f);
public:
	
	Sphere(GLFWwindow* _window, glm::vec3 _objPos, glm::vec3 _radi, float _objScale, int _level, bool _isSmooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera);
	~Sphere();
	void genOctahedron();
	void setLevel(int _level);
	void doRandomColors(bool _randomColor);
	void reseed();
	void smoothSurface(bool _isSmooth);

	// Ellipsoid intersection test.
	glm::vec3 isRayTouching(glm::vec3 _rayStart, glm::vec3 _rayDir);

	void calculateNewPos(glm::vec3 _newForce, float _timeElapsed);
	void resolveCollision(Sphere* _sphere, float _t);
	void applyBounds();

};

#endif