#ifndef CUBE_CLASS_H
#define CUBE_CLASS_H

#include "Object.h"

class Cube : public Object
{
private:
	void genTriangles();
public:
	Cube(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera);
	void reGenTriangles();
	void doRandomColors(bool _randomColor);

	glm::vec3 isRayTouching(glm::vec3 _rayStart, glm::vec3 _rayDir);
	glm::vec3 pointOnPlane(glm::vec3 planePoint, glm::vec3 planeNormal, glm::vec3 rayStart, glm::vec3 rayDirection);
	bool barycentricInterpolation(glm::vec3 _vert0, glm::vec3 _vert1, glm::vec3 _vert2, glm::vec3 _p);
};

#endif
