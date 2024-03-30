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

	vertices = {
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

		Vertex { v8, n4, color, texCoord2 },	// Bottom face
		Vertex { v4, n4, color, texCoord1 },
		Vertex { v3, n4, color, texCoord4 },
		Vertex { v7, n4, color, texCoord3 },

		Vertex { v3, n5, color, texCoord1 },	// Right face
		Vertex { v4, n5, color, texCoord2 },
		Vertex { v2, n5, color, texCoord3 },
		Vertex { v1, n5, color, texCoord4 },

		Vertex { v8, n6, color, texCoord1 },	// Left face
		Vertex { v7, n6, color, texCoord2 },
		Vertex { v5, n6, color, texCoord3 },
		Vertex { v6, n6, color, texCoord4 }
	};

	indices = {
		// Front Face
		0, 1, 2,
		2, 3, 0,

		// Back face
		4, 5, 6,
		6, 7, 4,

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
	setVBOandEBO("Cube");
}

void Cube::reGenTriangles() {
	genTriangles();
}

glm::vec3 Cube::isRayTouching(glm::vec3 _rayStart, glm::vec3 _rayDir) {
	glm::mat4 modelInv = inverse(model);
	glm::vec3 ro = glm::vec3(modelInv * glm::vec4(_rayStart, 1.0f));
	glm::vec3 rd = glm::normalize(glm::vec3(modelInv * glm::vec4(_rayDir, 0.0f)));
	
	// Looping over all the triangles in the Cube.
	for (int i = 0; i < indices.size(); i += 3) {
		// Getting vertices of the triangle.
		glm::vec3 v0 = vertices[indices[i + 0]].pos;
		glm::vec3 v1 = vertices[indices[i + 1]].pos;
		glm::vec3 v2 = vertices[indices[i + 2]].pos;

		// Setting edges to compute cross product/plane normal.
		glm::vec3 e0 = v2 - v0;
		glm::vec3 e1 = v1 - v0;
		glm::vec3 planeNormal = glm::cross(e0, e1);
		glm::vec3 point = pointOnPlane(v0, planeNormal, ro, rd);

		// Ray didn't intercept plane.
		if (point.x == FLT_MAX)
			continue;

		// Ray DID intercept plane.
		if (barycentricInterpolation(v0, v1, v2, point)) {
			return glm::vec3(model * glm::vec4(point, 1.f));
		}
	}
	
	return glm::vec3(FLT_MAX);
}

// Determines where on a plane a ray intersects it.
glm::vec3 Cube::pointOnPlane(glm::vec3 planePoint, glm::vec3 planeNormal, glm::vec3 rayStart, glm::vec3 rayDirection) {
	float denom = glm::dot(glm::normalize(rayDirection), planeNormal);
	if (denom > 0.00000000001f) {
		float t = (dot(planePoint - rayStart, planeNormal) / denom);
		if (t >= 0.f) {
			return rayStart + (glm::normalize(rayDirection) * t);
		}
	}
	return glm::vec3(FLT_MAX);
}

// Determines if point is on the triangle given by the 3 vertices.
bool Cube::barycentricInterpolation(glm::vec3 _vert0, glm::vec3 _vert1, glm::vec3 _vert2, glm::vec3 _p) {
	// Compute vectors        
	glm::vec3 v0 = _vert2 - _vert0;
	glm::vec3 v1 = _vert1 - _vert0;
	glm::vec3 v2 = _p - _vert0;

	// Compute dot products
	float dot00 = dot(v0, v0);
	float dot01 = dot(v0, v1);
	float dot02 = dot(v0, v2);
	float dot11 = dot(v1, v1);
	float dot12 = dot(v1, v2);

	// Compute barycentric coordinates
	float invDenom = 1.f / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	return ((u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.01f));
}
