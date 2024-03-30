#ifndef CONE_CLASS_H
#define CONE_CLASS_H

#include"Object.h"

class Cone : public Object
{
private:
	void genTriangles();
	void genCircle(bool _isBottom, glm::vec3 _pos, glm::vec3 _norm, glm::vec3 _radi, glm::vec2 _angles, int& _indCount);
	void genWall(glm::vec3 _startPos, glm::vec3 _endPos, glm::vec3 _startRadi, glm::vec3 _endRadi, glm::vec2 _angles, int& _indCount);
	int numVertsPerSide(int _level);

	bool randomColor;

	glm::vec3 bottomRadius;
	glm::vec3 topRadius;
	glm::vec3 pointPos;
	glm::vec3 startingPos;
	//glm::vec3 objPos;
	glm::vec3 pointingAt;

	glm::vec4 shaftColor, coneColor;

	int seed;
public:
	Cone(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, int _level, glm::vec3 _bottomRadius, glm::vec3 _topRadius, glm::vec3 _radi, glm::vec3 _pointPos, bool _isSmooth, glm::vec4 _shaftColor, glm::vec4 _coneColor, std::vector <Texture>& _textures, Camera* _camera);
	~Cone();
	void genCone(glm::vec3 _bottomCenter, glm::vec3 _topCenter, glm::vec3 _bottomRadius, glm::vec3 _topRadius, bool _doingBottomCircle, bool _doingTopCircle, int& _indCount);
	void setLevel(int _level);
	void doRandomColors(bool _randomColor);
	void reseed();
	void smoothSurface(bool _isSmooth);
	void setTopRadius(glm::vec3 _topRadius);
	void setBottomRadius(glm::vec3 _bottomRadius);
	void setTipPos(glm::vec3 _pointPos);

	// To do the yee-old Geometry shader.
	void setVBOandEBO(std::string msg);
	//void draw(glm::vec3 _lightPos, glm::vec4 _lightColor);
};

#endif

