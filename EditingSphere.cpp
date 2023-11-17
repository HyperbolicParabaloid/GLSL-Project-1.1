#include "EditingSphere.h"

//EditingSphere::EditingSphere(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, int _level, bool _isSmooth, glm::vec4 _color, Texture* _tex, Texture* _texSpec, Camera* _camera) : Object(_window, _objPos, _objScale, _color, _tex, _texSpec, _camera) {
//	level = _level;
//	isSmooth = _isSmooth;
//	randomColor = false;
//	genTriangles();
//}


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

void EditingSphere::genTriangles() {
	genOctahedron();
	//setVBOandEBO(postVerts, postVertsSize * sizeof(float), indices, indicesSize * sizeof(int), "Sphere");
}

void EditingSphere::genOctahedron() {
	if (level < 1)
		level = 1;
	if (level > 10)
		level = 10;

	// Num vertices per level
	//	1			2		3		4
	// 2x2=4 => 3x3=9 => 5x5=25 => 9x9=81
	// So basically, if you think about it you're doing:
	// NumVertsPerSide_X = NumVertsPerSide_X-1 + NumVertsPerSide_X-1 - 1
	int vertsPerSide = numVertsPerSide(level);

	// Resizing verts and texCoords. 
	preVertsSize = vertsPerSide * vertsPerSide;
	glm::vec3* newPreVerts = new glm::vec3[preVertsSize];
	delete[] preVerts;
	preVerts = newPreVerts;

	glm::vec2* newTexCoords = new glm::vec2[preVertsSize];
	delete[] texCoords;
	texCoords = newTexCoords;

	float temp;
	for (int vv = 0; vv <= (vertsPerSide - 1); vv++) {
		for (int uu = 0; uu <= (vertsPerSide - 1); uu++) {

			// Can I just make uu and vv go from 0->1, and make that my UV texture coodiantes?
			// If so, I just need a new array to hold them, and assign them along with the preVerts.
			// then, when preVerts get mapped into postVerts inside of the setNorms function, I can
			// do the same thing with the texture coords. Perhaps. We shall see. Tomorrow. When I have coffee.
			//
			// I lied. A little. Should be something like:
			//texCoords[vertsPerSide * vv + uu] = glm::vec2((uu / (vertsPerSide - 1)), (vv / (vertsPerSide - 1)));	// (0->1, 0->1)
			texCoords[vertsPerSide * vv + uu] = glm::vec2(static_cast<float>(-uu) / (vertsPerSide - 1), static_cast<float>(-vv) / (vertsPerSide - 1));	// (0->1, 0->1)
			//std::cout << "UV = (" << -uu << ", " << -vv << ")\n";

			GLfloat x, y, z = 0.f;
			x = ((uu * 2) - (vertsPerSide - 1)) / (float)(vertsPerSide - 1);	// Goes from -1.f =>  1.f
			y = ((vertsPerSide - 1) - (vv * 2)) / (float)(vertsPerSide - 1);	// Goes from  1.f => -1.f

			// Transforming the 2D plane to a 3D Octahedron.
			if (abs(x) + abs(y) <= 1)
				z = 1.f - (abs(x) + abs(y));
			else {
				temp = x;
				// 1st Quadrant
				if (x >= 0 && y >= 0) {
					x = 1 - y;
					y = 1 - temp;
				}

				// 2nd Quadrant
				else if (x < 0 && y >= 0) {
					x = -1 + y;
					y = 1 + temp;
				}

				// 3rd Quadrant
				else if (x >= 0 && y < 0) {
					x = 1 + y;
					y = -1 + temp;
				}

				// 4th Quadrant
				else if (x < 0 && y < 0) {
					x = -1 - y;
					y = -1 - temp;
				}
				z = -1.f + (abs(x) + abs(y));
			}

			preVerts[vertsPerSide * vv + uu] = glm::normalize(glm::vec3(x, y, z));
			//preVerts[vertsPerSide * vv + uu] = glm::vec3(x, y, z);	// For making it a plane, don't normalize
		}
	}
	setPreIndices();
	//setNorms();
	//setPostIndices();
}

void EditingSphere::setPreIndices() {
	int vertsPerSide = numVertsPerSide(level);
	int setpsPerSide = vertsPerSide - 2;

	// Resizing indices aray.
	indicesSize = pow(4, level - 1) * 2 * 3;
	GLuint* newInds = new GLuint[indicesSize];
	delete[] indices;
	indices = newInds;

	int t0, t1, t2; // First triangle
	int t3, t4, t5; // Second triangle;

	int indCount = 0;
	for (int vv = 0; vv <= setpsPerSide; vv++) {
		for (int uu = 0; uu <= setpsPerSide; uu++) {
			//	+-+
			//	|\|
			//	+-+
			if ((uu >= (vertsPerSide - 1) / 2 && vv < (vertsPerSide - 1) / 2) || ((uu < (vertsPerSide - 1) / 2 && vv >= (vertsPerSide - 1) / 2))) {
				t0 = (uu + 0) + (vertsPerSide * (vv + 0));
				t1 = (uu + 0) + (vertsPerSide * (vv + 1));
				t2 = (uu + 1) + (vertsPerSide * (vv + 1));

				t3 = (uu + 1) + (vertsPerSide * (vv + 1));
				t4 = (uu + 1) + (vertsPerSide * (vv + 0));
				t5 = (uu + 0) + (vertsPerSide * (vv + 0));
			}

			//	+-+
			//	|/|
			//	+-+
			else {
				t0 = (uu + 1) + (vertsPerSide * (vv + 0));
				t1 = (uu + 0) + (vertsPerSide * (vv + 0));
				t2 = (uu + 0) + (vertsPerSide * (vv + 1));

				t3 = (uu + 0) + (vertsPerSide * (vv + 1));
				t4 = (uu + 1) + (vertsPerSide * (vv + 1));
				t5 = (uu + 1) + (vertsPerSide * (vv + 0));
			}

			glm::vec4 color1, color2;
			if (randomColor) {
				float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				color1 = glm::vec4(r1, r2, r3, 1.f);

				float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float r5 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float r6 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				color2 = glm::vec4(r4, r5, r6, 1.f);
			}
			else
				color1 = color2 = color;

			glm::vec3 v1 = preVerts[t0];	glm::vec2 tex1 = texCoords[t0];
			glm::vec3 v2 = preVerts[t1];	glm::vec2 tex2 = texCoords[t1];
			glm::vec3 v3 = preVerts[t2];	glm::vec2 tex3 = texCoords[t2];

			glm::vec3 v4 = preVerts[t3];	glm::vec2 tex4 = texCoords[t3];
			glm::vec3 v5 = preVerts[t4];	glm::vec2 tex5 = texCoords[t4];
			glm::vec3 v6 = preVerts[t5];	glm::vec2 tex6 = texCoords[t5];

			glm::vec3 n1 = glm::normalize(glm::cross(v3 - v2, v1 - v2));
			glm::vec3 n2 = glm::normalize(glm::cross(v6 - v5, v4 - v5));

			//Vertex vert1(v1, n1, color1, tex1);
			//Vertex vert2(v2, n1, color1, tex2);
			//Vertex vert3(v3, n1, color1, tex3);
			//
			//Vertex vert4(v4, n2, color2, tex4);
			//Vertex vert5(v5, n2, color2, tex5);
			//Vertex vert6(v6, n2, color2, tex6);

			//triangles.push_back(Triangle(vert1, vert2, vert3));
			//triangles.push_back(Triangle(vert4, vert5, vert6));
			for (int ii = 0; ii < 6; ii++) {
				indices[indCount + ii] = indCount + ii;
			}
			indCount += 6;
		}
	}

}

void EditingSphere::setNorms() {
	int numTriangles = pow(4, level - 1) * 2;

	// Resizing final verts
	int numVertsPerTriangle = 3, numFloatsPerVert = 3, numFloatsPerNorm = 3, numFloatsPerColor = 4, numFloatsPerTextureCoords = 2;
	int offset = numFloatsPerVert + numFloatsPerNorm + numFloatsPerColor + numFloatsPerTextureCoords;

	postVertsSize = numTriangles * numVertsPerTriangle * offset;
	GLfloat* newPostVerts = new GLfloat[postVertsSize];
	delete[] postVerts;
	postVerts = newPostVerts;

	int normCount = 0;
	for (int ii = 0; ii < indicesSize; ii += 6) {
		glm::vec3 v1 = preVerts[indices[ii + 0]];	glm::vec2 t1 = texCoords[indices[ii + 0]];
		glm::vec3 v2 = preVerts[indices[ii + 1]];	glm::vec2 t2 = texCoords[indices[ii + 1]];
		glm::vec3 v3 = preVerts[indices[ii + 2]];	glm::vec2 t3 = texCoords[indices[ii + 2]];

		glm::vec3 v4 = preVerts[indices[ii + 3]];	glm::vec2 t4 = texCoords[indices[ii + 3]];
		glm::vec3 v5 = preVerts[indices[ii + 4]];	glm::vec2 t5 = texCoords[indices[ii + 4]];
		glm::vec3 v6 = preVerts[indices[ii + 5]];	glm::vec2 t6 = texCoords[indices[ii + 5]];

		glm::vec3 n1 = glm::normalize(glm::cross(v3 - v2, v1 - v2));
		glm::vec3 n2 = glm::normalize(glm::cross(v6 - v5, v4 - v5));

		glm::vec4 color1, color2;
		if (randomColor) {
			float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			color1 = glm::vec4(r1, r2, r3, 1.f);

			float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float r5 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float r6 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			color2 = glm::vec4(r4, r5, r6, 1.f);
		}
		else
			color1 = color2 = color;

		// Copying the contents from the new temp verts into the finalVerts2 array
		int count = ii * offset;

		if (!isSmooth) {
			//						|	VERTEX		|	|	NORMAL		|	|				COLOR					|	|	TEXTURE COORD		|
			GLfloat tempVert1[] = { v1.x, v1.y, v1.z,	n1.x, n1.y, n1.z,	color1.r, color1.g, color1.b, color1.a,		t1.x, t1.y };
			GLfloat tempVert2[] = { v2.x, v2.y, v2.z,	n1.x, n1.y, n1.z,	color1.r, color1.g, color1.b, color1.a,		t2.x, t2.y };
			GLfloat tempVert3[] = { v3.x, v3.y, v3.z,	n1.x, n1.y, n1.z,	color1.r, color1.g, color1.b, color1.a,		t3.x, t3.y };

			//						|	VERTEX		|	|	NORMAL		|	|				COLOR					|	|	TEXTURE COORD		|
			GLfloat tempVert4[] = { v4.x, v4.y, v4.z,	n2.x, n2.y, n2.z,	color2.r, color2.g, color2.b, color2.a,		t4.x, t4.y };
			GLfloat tempVert5[] = { v5.x, v5.y, v5.z,	n2.x, n2.y, n2.z,	color2.r, color2.g, color2.b, color2.a,		t5.x, t5.y };
			GLfloat tempVert6[] = { v6.x, v6.y, v6.z,	n2.x, n2.y, n2.z,	color2.r, color2.g, color2.b, color2.a,		t6.x, t6.y };

			std::copy(tempVert1, tempVert1 + offset, postVerts + count + offset * 0);
			std::copy(tempVert2, tempVert2 + offset, postVerts + count + offset * 1);
			std::copy(tempVert3, tempVert3 + offset, postVerts + count + offset * 2);
			std::copy(tempVert4, tempVert4 + offset, postVerts + count + offset * 3);
			std::copy(tempVert5, tempVert5 + offset, postVerts + count + offset * 4);
			std::copy(tempVert6, tempVert6 + offset, postVerts + count + offset * 5);
		}
		else {
			//						|	VERTEX		|	|	NORMAL		|	|				COLOR					|	|	TEXTURE COORD		|
			GLfloat tempVert1[] = { v1.x, v1.y, v1.z,	v1.x, v1.y, v1.z,	color1.r, color1.g, color1.b, color1.a,		t1.x, t1.y };
			GLfloat tempVert2[] = { v2.x, v2.y, v2.z,	v2.x, v2.y, v2.z,	color1.r, color1.g, color1.b, color1.a,		t2.x, t2.y };
			GLfloat tempVert3[] = { v3.x, v3.y, v3.z,	v3.x, v3.y, v3.z,	color1.r, color1.g, color1.b, color1.a,		t3.x, t3.y };

			//						|	VERTEX		|	|	NORMAL		|	|				COLOR					|	|	TEXTURE COORD		|
			GLfloat tempVert4[] = { v4.x, v4.y, v4.z,	v4.x, v4.y, v4.z,	color2.r, color2.g, color2.b, color2.a,		t4.x, t4.y };
			GLfloat tempVert5[] = { v5.x, v5.y, v5.z,	v5.x, v5.y, v5.z,	color2.r, color2.g, color2.b, color2.a,		t5.x, t5.y };
			GLfloat tempVert6[] = { v6.x, v6.y, v6.z,	v6.x, v6.y, v6.z,	color2.r, color2.g, color2.b, color2.a,		t6.x, t6.y };

			std::copy(tempVert1, tempVert1 + offset, postVerts + count + offset * 0);
			std::copy(tempVert2, tempVert2 + offset, postVerts + count + offset * 1);
			std::copy(tempVert3, tempVert3 + offset, postVerts + count + offset * 2);
			std::copy(tempVert4, tempVert4 + offset, postVerts + count + offset * 3);
			std::copy(tempVert5, tempVert5 + offset, postVerts + count + offset * 4);
			std::copy(tempVert6, tempVert6 + offset, postVerts + count + offset * 5);
		}

		normCount += 2;
	}
}

void EditingSphere::setPostIndices() {
	for (int ii = 0; ii < indicesSize; ii++) {
		indices[ii] = ii;
	}
}

EditingSphere::~EditingSphere() {
	delete[] preVerts;
	delete[] indices;
	delete[] postVerts;
	delete[] texCoords;
}

int EditingSphere::numVertsPerSide(int _level) {
	if (_level == 1)
		return 2;
	return 2 * numVertsPerSide(_level - 1) - 1;
}

void EditingSphere::setLevel(int _level) {
	level = _level;
	genTriangles();
}

void EditingSphere::doRandomColors(bool _randomColor) {
	randomColor = _randomColor;
}

void EditingSphere::smoothSurface(bool _isSmooth) {
	isSmooth = _isSmooth;
}