#ifndef UI_CLASS_H
#define UI_CLASS_H

#include"Object.h"

// Creates a simple 2D plane. For now.
class UI : public Object
{
private:
	void genTriangles();

	glm::uvec2 *dictionary;

	bool isSmooth;
	bool randomColor;

	glm::vec2 scale;
	std::string text;

	int seed;
public:
	UI(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, std::string _text, glm::uvec2 (& _dictionary)[100], glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera);
	~UI();
	void genOctahedron();
	void setVBOandEBO(std::string msg);
	void doRandomColors(bool _randomColor);
	void reseed();
	void smoothSurface(bool _isSmooth);
	void setScale(float _scale);
	void setScreenPos(glm::vec2 _screenCoords);
	void setScale(glm::vec2 _scale);
	glm::vec3 rayToObject(glm::vec3 _ray);
	glm::vec2 getScale();
	bool isTouching(glm::vec2 _cursorPos);
	void setColor(glm::vec4 _color);
	
	void writeLetter(glm::uvec2 _pixels);
	void writeLetter(std::string code);
};

#endif
