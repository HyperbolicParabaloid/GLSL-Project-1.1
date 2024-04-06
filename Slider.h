#ifndef SLIDER_CLASS_H
#define SLIDER_CLASS_H

#include"UI.h"

// Creates a simple 2D plane. For now.
class Slider : public UI
{
private:
	glm::vec2 start;
	glm::vec2 end;
	glm::vec2 startPos;
	glm::vec2 sliderPos;
	glm::vec2 lastCursorPos = glm::vec2(0.f);
	float value;
	float valueScaler;
	float lastTouched;
	float resetTime;

public:
	Slider(GLFWwindow* _window, float _valueScaler, glm::vec2 _start, glm::vec2 _end, glm::vec3 _objPos, glm::vec2 _textOffset, glm::vec3 _radi, float _characterScale, std::string _text, glm::uvec2(&_dictionary)[100], glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera);
	~Slider();

	void drag(glm::vec2 _cursorPos);
	float getValue();
};

#endif
