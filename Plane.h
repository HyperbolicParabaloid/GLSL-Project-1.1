#ifndef PLANE_CLASS_H
#define PLANE_CLASS_H

#include "Object.h"

// Creates a simple 2D plane. For now.
class Plane : public Object
{
private:
	void genTriangles();
public:
	Plane(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, bool _isSmooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera);
	~Plane();
};

#endif