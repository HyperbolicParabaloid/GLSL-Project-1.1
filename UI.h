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
	float characterScale;
	std::string text;

	int seed;
	int realLetters;
	float xCount;
	float yCount;

	glm::vec2 textOffset;
public:
	UI(GLFWwindow* _window, glm::vec3 _objPos, glm::vec2 _textOffset, float _objScale, float _characterScale, std::string _text, glm::uvec2(&_dictionary)[100], glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera);
	~UI();
	void genOctahedron();
	void setVBOandEBO(std::string msg);

	void setNewString(std::string _code, bool _removeFormatting);
	void setNewString(std::string _code);
	void setNewNumber(double _num, int _precision);
	void appendString(std::string _code);
	void appendNumber(double _num, int _precision);

	std::string getString();

	void setScreenPos(glm::vec2 _screenCoords);
	void setScale(glm::vec2 _scale);
	glm::vec3 rayToObject(glm::vec3 _ray);
	glm::vec2 getScale();
	bool isTouching(glm::vec2 _cursorPos);
	void setColor(glm::vec4 _color);
};

#endif
