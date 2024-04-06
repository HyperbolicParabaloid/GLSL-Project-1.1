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

	int seed;
	int realLetters;
	float xCount;
	float yCount;

	unsigned int VBO_ID = 0;

protected:
	glm::vec2 textOffset;
	glm::vec4 backgroundColor;
	float characterScale;
	std::string text;
	std::string toUppercase(std::string _str);

public:
	UI(GLFWwindow* _window, glm::vec3 _objPos, glm::vec2 _textOffset, glm::vec3 _radi, float _characterScale, std::string _text, glm::uvec2(&_dictionary)[100], glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera);
	~UI();
	void genOctahedron();
	void setVBOandEBO(std::string msg);
	void draw(glm::vec3 _lightPos, glm::vec4 _lightColor);

	void setNewString(std::string _code, bool _removeFormatting);
	void setNewString(std::string _code);
	void setNewNumber(double _num, int _precision);
	void appendString(std::string _code);
	void appendNumber(double _num, int _precision);

	void setColor(glm::vec4 _color);

	std::string getString();

	void setScreenPos(glm::vec2 _screenCoords);
	void setScale(glm::vec3 _scale);
	glm::vec3 rayToObject(glm::vec3 _ray);
	glm::vec3 getScale();

	glm::vec3 isRayTouching(glm::vec3 _rayStart, glm::vec3 _rayDir);
	bool isCursorTouching(glm::vec2 _cursorPos);
	void setCharacterColor(glm::vec4 _color);
};

#endif
