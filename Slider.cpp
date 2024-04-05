#include "Slider.h"

// Constructor for Plane.
Slider::Slider(GLFWwindow* _window, glm::vec2 _start, glm::vec2 _end, glm::vec3 _objPos, glm::vec2 _textOffset, glm::vec3 _radi, float _characterScale, std::string _text, glm::uvec2(&_dictionary)[100], glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) : UI(_window, _objPos, _textOffset, _radi, _characterScale, _text, _dictionary, _color, _textures, _camera) {	
	start = _start;
	end = _end;
	startPos = objPos;
	lastTouched = 0.f;
	resetTime = 0.1f;
	name = "Slider";
}

Slider::~Slider() {}

void Slider::drag(glm::vec2 _cursorPos) {
	if (glfwGetTime() - lastTouched > resetTime)
		lastCursorPos = _cursorPos;

	lastTouched = glfwGetTime();
	glm::vec2 dir = _cursorPos - lastCursorPos;
	lastCursorPos = _cursorPos;
	sliderPos = glm::clamp(glm::vec2(objPos) + dir, start + glm::vec2(startPos), end + glm::vec2(startPos));
	value = dot(sliderPos - startPos - start, sliderPos - startPos - start) / dot(end - start, end - start);
	setNewPos(glm::vec3(sliderPos, 0.f));
	setNewString(toUppercase(name));
	appendNumber(value, 3);
}

float Slider::getValue() {
	return value;
}

