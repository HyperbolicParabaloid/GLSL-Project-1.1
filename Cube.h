#ifndef CUBE_CLASS_H
#define CUBE_CLASS_H

#include "Object.h"

class Cube : public Object
{
private:
	void genTriangles();
public:
	Cube(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera);
};

#endif
