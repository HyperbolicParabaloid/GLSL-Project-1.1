#include "Cube.h"

Cube::Cube(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) : Object (_window, _objPos, _objScale, _color, _textures, _camera) {
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

	glm::vec2 texCoord1 = glm::vec2(0.f, 0.f);
	glm::vec2 texCoord2 = glm::vec2(1.f, 0.f);
	glm::vec2 texCoord3 = glm::vec2(1.f, 1.f);
	glm::vec2 texCoord4 = glm::vec2(0.f, 1.f);

	std::vector <Vertex> sqrVertices = {
		//		VERTEX		/		NORMAL		/				COLOR				/	/		TEXTURE COORD	/
		Vertex { v7, n1, color, texCoord1 },	// Front face
		Vertex { v3, n1, color, texCoord2 },
		Vertex { v1, n1, color, texCoord3 },
		Vertex { v5, n1, color, texCoord4 },

		Vertex { v4, n2, color, texCoord1 },	// Back face
		Vertex { v8, n2, color, texCoord2 },
		Vertex { v6, n2, color, texCoord3 },
		Vertex { v2, n2, color, texCoord4 },

		Vertex { v5, n3, color, texCoord1 },	// Top face
		Vertex { v1, n3, color, texCoord2 },
		Vertex { v2, n3, color, texCoord3 },
		Vertex { v6, n3, color, texCoord4 },

		Vertex { v4, n4, color, texCoord1 },	// Bottom face
		Vertex { v8, n4, color, texCoord2 },
		Vertex { v7, n4, color, texCoord3 },
		Vertex { v3, n4, color, texCoord4 },

		Vertex { v3, n5, color, texCoord1 },	// Right face
		Vertex { v4, n5, color, texCoord2 },
		Vertex { v2, n5, color, texCoord3 },
		Vertex { v1, n5, color, texCoord4 },

		Vertex { v8, n6, color, texCoord1 },	// Left face
		Vertex { v7, n6, color, texCoord2 },
		Vertex { v5, n6, color, texCoord3 },
		Vertex { v6, n6, color, texCoord4 }
	};

	std::vector <GLuint> sqrIndices = {
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
	setVBOandEBO(sqrVertices, sqrIndices, "Cube");
}