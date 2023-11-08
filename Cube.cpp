#include "Cube.h"

Cube::Cube(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 _color, Camera* _camera) : Object (_window, _objPos, _objScale, _color, _camera) {
	genTriangles();
}

void Cube::genTriangles() {
	glm::vec3 v1 = glm::vec3(1.f, 1.f, 1.f);	// 0
	glm::vec3 v2 = glm::vec3(1.f, 1.f, -1.f);	// 1
	glm::vec3 v3 = glm::vec3(1.f, -1.f, 1.f);	// 2
	glm::vec3 v4 = glm::vec3(1.f, -1.f, -1.f);	// 3
	glm::vec3 v5 = glm::vec3(-1.f, 1.f, 1.f);	// 4
	glm::vec3 v6 = glm::vec3(-1.f, 1.f, -1.f);	// 5
	glm::vec3 v7 = glm::vec3(-1.f, -1.f, 1.f);	// 6
	glm::vec3 v8 = glm::vec3(-1.f, -1.f, -1.f);	// 7

	glm::vec3 n1 = glm::cross(v1 - v3, v7 - v3);	// Front face normal
	glm::vec3 n2 = glm::cross(v6 - v8, v4 - v8);	// Back face normal
	glm::vec3 n3 = glm::cross(v2 - v1, v5 - v1);	// Top normal
	glm::vec3 n4 = glm::cross(v4 - v8, v7 - v8);	// Bottom normal
	glm::vec3 n5 = glm::cross(v2 - v4, v3 - v2);	// Right normal
	glm::vec3 n6 = glm::cross(v5 - v7, v8 - v7);	// Left normal

	GLfloat sqrVertices[] = {
		//		VERTEX		/		NORMAL		/				COLOR				/
		v7.x, v7.y, v7.z,	n1.x, n1.y, n1.z,	color.r, color.g, color.b, color.a,		// Front face
		v3.x, v3.y, v3.z,	n1.x, n1.y, n1.z,	color.r, color.g, color.b, color.a,
		v1.x, v1.y, v1.z,	n1.x, n1.y, n1.z,	color.r, color.g, color.b, color.a,
		v5.x, v5.y, v5.z,	n1.x, n1.y, n1.z,	color.r, color.g, color.b, color.a,

		v4.x, v4.y, v4.z,	n2.x, n2.y, n2.z,	color.r, color.g, color.b, color.a,		// Back face
		v8.x, v8.y, v8.z,	n2.x, n2.y, n2.z,	color.r, color.g, color.b, color.a,
		v6.x, v6.y, v6.z,	n2.x, n2.y, n2.z,	color.r, color.g, color.b, color.a,
		v2.x, v2.y, v2.z,	n2.x, n2.y, n2.z,	color.r, color.g, color.b, color.a,

		v5.x, v5.y, v5.z,	n3.x, n3.y, n3.z,	color.r, color.g, color.b, color.a,		// Top face
		v1.x, v1.y, v1.z,	n3.x, n3.y, n3.z,	color.r, color.g, color.b, color.a,
		v2.x, v2.y, v2.z,	n3.x, n3.y, n3.z,	color.r, color.g, color.b, color.a,
		v6.x, v6.y, v6.z,	n3.x, n3.y, n3.z,	color.r, color.g, color.b, color.a,

		v4.x, v4.y, v4.z,	n4.x, n4.y, n4.z,	color.r, color.g, color.b, color.a,		// Bottom face
		v8.x, v8.y, v8.z,	n4.x, n4.y, n4.z,	color.r, color.g, color.b, color.a,
		v7.x, v7.y, v7.z,	n4.x, n4.y, n4.z,	color.r, color.g, color.b, color.a,
		v3.x, v3.y, v3.z,	n4.x, n4.y, n4.z,	color.r, color.g, color.b, color.a,

		v3.x, v3.y, v3.z,	n5.x, n5.y, n5.z,	color.r, color.g, color.b, color.a,		// Right face
		v4.x, v4.y, v4.z,	n5.x, n5.y, n5.z,	color.r, color.g, color.b, color.a,
		v2.x, v2.y, v2.z,	n5.x, n5.y, n5.z,	color.r, color.g, color.b, color.a,
		v1.x, v1.y, v1.z,	n5.x, n5.y, n5.z,	color.r, color.g, color.b, color.a,

		v8.x, v8.y, v8.z,	n6.x, n6.y, n6.z,	color.r, color.g, color.b, color.a,		// Left face
		v7.x, v7.y, v7.z,	n6.x, n6.y, n6.z,	color.r, color.g, color.b, color.a,
		v5.x, v5.y, v5.z,	n6.x, n6.y, n6.z,	color.r, color.g, color.b, color.a,
		v6.x, v6.y, v6.z,	n6.x, n6.y, n6.z,	color.r, color.g, color.b, color.a
	};

	GLuint sqrIndices[] = {
		// Front Face
		0, 1, 2,
		2, 3, 0,

		// Back face
		4, 5, 6,
		6, 7, 4,
		//
		// Top face
		8, 9, 10,
		10, 11, 8,

		// Bottom face
		12, 13, 14,
		14, 15, 12,

		// Right face
		16, 17, 18,
		18, 19, 16,

		// Left face
		20, 21, 22,
		22, 23, 20
	};
	setVBOandEBO(sqrVertices, sizeof(sqrVertices), sqrIndices, sizeof(sqrIndices));
}

void Cube::keepDrawing() {
	genTriangles();
}


Cube::Cube(GLFWwindow* _window) : Object(_window) {

}
