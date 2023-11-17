#include "Plane.h"

Plane::Plane(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, bool _isSmooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) : Object(_window, _objPos, _objScale, _color, _textures, _camera) {
	genTriangles();
}


/*
* To explain what's going on: behold, my ASCII art.
+---------------------+---------------------+
|                    /|\                    |
|                 /   |   \                 |
|               /     |     \               |
|            /        |        \            |
|          /          |          \          |
|        /            |            \        |
|     /               |               \     |
|   /                 |                 \   |
|/                    |                    \|
+---------------------+---------------------+
|\                    |                    /|
|   \                 |                 /   |
|     \               |               /     |
|        \            |            /        |
|          \          |          /          |
|            \        |        /            |
|              \      |     /               |
|                 \   |   /                 |
|                    \|/                    |
+---------------------+---------------------+

					 ||
					 ||
					 ||
					 ||
					 \/

+----------+----------+----------+----------+
|         /|         /|\         |\         |
|      /   |      /   |   \      |   \      |
|   /      |   /      |      \   |      \   |
|/         |/         |         \|         \|
+----------+----------+----------+----------+
|         /|         /|\         |\         |
|      /   |      /   |   \      |   \      |
|   /      |   /      |      \   |      \   |
|/         |/         |         \|         \|
+----------+----------+----------+----------+
|\         |\         |         /|         /|
|   \      |   \      |      /   |      /   |
|      \   |      \   |   /      |   /      |
|         \|         \|/         |/         |
+----------+----------+----------+----------+
|\         |\         |         /|         /|
|   \      |   \      |      /   |      /   |
|      \   |      \   |   /      |   /      |
|         \|         \|/         |/         |
+----------+----------+----------+----------+
*/

void Plane::genTriangles() {
	glm::vec3 v1 = glm::vec3(-1.f, 0.f, -1.f);	// 0
	glm::vec3 v2 = glm::vec3(-1.f, 0.f, 1.f);	// 1
	glm::vec3 v3 = glm::vec3(1.f, 0.f, 1.f);	// 2
	glm::vec3 v4 = glm::vec3(1.f, 0.f, -1.f);	// 3
	

	glm::vec3 n1 = glm::cross(v4 - v3, v2 - v3);	// Front face normal
	
	glm::vec2 texCoord1 = glm::vec2(0.f, 1.f) * objScale;
	glm::vec2 texCoord2 = glm::vec2(0.f, 0.f) * objScale;
	glm::vec2 texCoord3 = glm::vec2(1.f, 0.f) * objScale;
	glm::vec2 texCoord4 = glm::vec2(1.f, 1.f) * objScale;

	//		VERTEX		/		NORMAL		/				COLOR				/	/		TEXTURE COORD	/
	std::vector <Vertex> sqrVertices = {
		Vertex { v1, n1, color, texCoord1 },
		Vertex { v2, n1, color, texCoord2 },
		Vertex { v3, n1, color, texCoord3 },
		Vertex { v4, n1, color, texCoord4 }
	};

	std::vector <GLuint> sqrIndices = {
		// Front Face
		0, 1, 2,
		2, 3, 0,
	};
	setVBOandEBO(sqrVertices, sqrIndices, "Sphere");
}

Plane::~Plane() {
}
