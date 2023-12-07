#include "Plane.h"

/*
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
// Constructor for Plane.
Plane::Plane(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, int _level, bool _isSmooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) : Object(_window, _objPos, _objScale, _color, _textures, _camera) {
	level = _level;
	isSmooth = _isSmooth;
	randomColor = false;
	seed = 1;
	genTriangles();
}


/*
* To explain what's going on: behold, my ASCII art.
* The sphere is made by creating a 2D plane as such,
* and wrapping it into a Octahedron.
*
* The Octahedron's points are then normalized, which
* creates a sphere.
*
* Tessellating the surface of the plane to different
* levels increases the number of triangles by 4 each
* time. Effectively making the surface of the sphere
* more and more smooth. With proper lighting, level 6
* is about all you need to make the sphere near perfect.
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

// Creates a new set of Vertex's and their associated indices to send to the Object
// class for drawing.
void Plane::genTriangles() {
	genOctahedron();
	setVBOandEBO(verts, indices, "Plane");
}

void Plane::genOctahedron() {
	// Whenever we generate a new set a vertices and indices, we want to wipe the old ones.
	// In the future it'd be better to just add in the new vertices and update indices instead
	// of clearing both indices and verts and starting over but it's fine for now.
	verts.clear();
	indices.clear();

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

	glm::vec3* newPreVerts = new glm::vec3[preVertsSize];	delete[] preVerts;
	preVerts = newPreVerts;
	glm::vec2* newTexCoords = new glm::vec2[preVertsSize];	delete[] texCoords;
	texCoords = newTexCoords;

	float temp;
	for (int vv = 0; vv < vertsPerSide; vv++) {
		for (int uu = 0; uu < vertsPerSide; uu++) {

			// Can I just make uu and vv go from 0->1, and make that my UV texture coodiantes?
			// If so, I just need a new array to hold them, and assign them along with the preVerts.
			// then, when preVerts get mapped into postVerts inside of the setNorms function, I can
			// do the same thing with the texture coords. Perhaps. We shall see. Tomorrow. When I have coffee.
			//
			// I lied. A little. Should be something like:
			texCoords[vertsPerSide * vv + uu] = glm::vec2(static_cast<float>(-uu) / (vertsPerSide - 1), static_cast<float>(-vv) / (vertsPerSide - 1)) * objScale;	// (0->1, 0->1)
			GLfloat x, y, z = 0.f;
			x = ((uu * 2) - (vertsPerSide - 1)) / (float)(vertsPerSide - 1);	// Goes from -1.f =>  1.f
			y = ((vertsPerSide - 1) - (vv * 2)) / (float)(vertsPerSide - 1);	// Goes from  1.f => -1.f
			//z += noise(glm::vec2(x, y)) / 10.f + noise(glm::vec2(x, y) * 10.f) / 20.f;

			if (false) {
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
			}

			// Normalizing the vector, places the vertices of the Octehdron on the surface of the sphere.
			//preVerts[vertsPerSide * vv + uu] = glm::normalize(glm::vec3(x, y, z));
			//preVerts[vertsPerSide * vv + uu] = glm::vec3(x, y, z);
			//z += noise(glm::vec2(-x, y) + float(glfwGetTime())) / 3.f;// +(noise((glm::vec2(-x, y) * float(seed)) * 1.2f) * 1 / 8) + (noise(glm::vec2(-x, y) * 2.f) * 1 / 16)) / 5.f;
			preVerts[vertsPerSide * vv + uu] = glm::vec3(-x, z, y);
		}
	}
	setVerticesVector();
}

//
void Plane::setVerticesVector() {
	int vertsPerSide = numVertsPerSide(level);
	int setpsPerSide = vertsPerSide - 2;

	int t0, t1, t2; // First triangle
	int t3, t4, t5; // Second triangle;

	int indCount = 0;

	glm::vec3 v1, v2, v3, v4, v5, v6;
	glm::vec3 n1, n2, n3, n4, n5, n6;
	glm::vec2 tex1, tex2, tex3, tex4, tex5, tex6;
	srand(seed);
	// This nested for-loop updates verts with all its Vertex information and
	// updates indices with all the right values.
	for (int vv = 0; vv <= setpsPerSide; vv++) {
		for (int uu = 0; uu <= setpsPerSide; uu++) {
			getPositions(uu, vv, vertsPerSide, v1, n1, v2, n2, v3, n3, v4, n4, v5, n5, v6, n6);
			getPositions(uu, vv, vertsPerSide, tex1, tex2, tex3, tex4, tex5, tex6);

			// Setting the colors of the object:
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

			// If the Sphere's lighting is supposed to be smooth, these normals will be the same
			// as the position (on a sphere, all vertex's point directly away from the center already).
			//
			// If we'd rather they appear angular, we can set that by getting the normal of the triangle
			// the vertexs are a part of, and using that for each Vertex.
			if (!isSmooth) {
				n1 = n2 = n3 = glm::normalize(glm::cross(v3 - v2, v1 - v2));
				n4 = n5 = n6 = glm::normalize(glm::cross(v6 - v5, v4 - v5));
			}
			
			verts.push_back(Vertex{ v1, n1, color1, tex1 });
			verts.push_back(Vertex{ v2, n2, color1, tex2 });
			verts.push_back(Vertex{ v3, n3, color1, tex3 });

			verts.push_back(Vertex{ v4, n4, color2, tex4 });
			verts.push_back(Vertex{ v5, n5, color2, tex5 });
			verts.push_back(Vertex{ v6, n6, color2, tex6 });
			// Finally, setting the values of the Indices. I have it in such a way,
			// that indices[n] = n; for all n; >= 0, < indices.size().
			for (int ii = 0; ii < 6; ii++) {
				indices.push_back(indCount);
				indCount++;
			}
		}
	}

}

glm::vec3 Plane::averageNormals(int _uu, int _vv, int vertsPerSide) {

	return glm::vec3(0.f, -1.f, 0.f);

	//	+-+
	//	|\|
	//	+-+
	glm::vec3 n = glm::vec3(0.f);
	int uu = _uu, vv = _vv;
	glm::vec3 vecs[9];
	int vecsCount = 0;
	for (int yy = 1; yy >= -1; yy--) {
		for (int xx = -1; xx <= 1; xx++) {
			if (uu + xx > 0 && uu + xx < vertsPerSide - 1 && vv + yy > 0 && vv + yy < vertsPerSide - 1) {
				uu += xx;
				vv += yy;
				vecs[vecsCount] = preVerts[uu + vertsPerSide * vv];
			}
			//vecs[vecsCount] = preVerts[uu + vertsPerSide * vv];
			vecsCount++;
		}
	}
	uu = _uu;
	vv = _vv;

	// / | \
	// --+--
	// \ | /
	if (uu == (vertsPerSide - 1) / 2 && vv == (vertsPerSide - 1) / 2) {
		n += glm::cross(vecs[1] - vecs[4], vecs[3] - vecs[4]);	// 1
		n += glm::cross(vecs[5] - vecs[4], vecs[1] - vecs[4]);	// 2
		n += glm::cross(vecs[7] - vecs[4], vecs[5] - vecs[4]);	// 3
		n += glm::cross(vecs[3] - vecs[4], vecs[7] - vecs[4]);	// 4
	}
	//	+-+
	//	|\|
	//	+-+
	else if ((uu >= (vertsPerSide - 1) / 2 && vv < (vertsPerSide - 1) / 2) || ((uu < (vertsPerSide - 1) / 2 && vv >= (vertsPerSide - 1) / 2))) {
		n += glm::cross(vecs[0] - vecs[4], vecs[3] - vecs[4]);	// 1
		n += glm::cross(vecs[1] - vecs[4], vecs[0] - vecs[4]);	// 2
		n += glm::cross(vecs[5] - vecs[4], vecs[1] - vecs[4]);	// 3
		n += glm::cross(vecs[8] - vecs[4], vecs[5] - vecs[4]);	// 4
		n += glm::cross(vecs[7] - vecs[4], vecs[8] - vecs[4]);	// 5
		n += glm::cross(vecs[3] - vecs[4], vecs[7] - vecs[4]);	// 6
	}

	//	+-+
	//	|/|
	//	+-+
	else {
		n += glm::cross(vecs[1] - vecs[4], vecs[3] - vecs[4]);	// 1
		n += glm::cross(vecs[2] - vecs[4], vecs[1] - vecs[4]);	// 2
		n += glm::cross(vecs[5] - vecs[4], vecs[2] - vecs[4]);	// 3
		n += glm::cross(vecs[7] - vecs[4], vecs[5] - vecs[4]);	// 4
		n += glm::cross(vecs[6] - vecs[4], vecs[7] - vecs[4]);	// 5
		n += glm::cross(vecs[3] - vecs[4], vecs[6] - vecs[4]);	// 6
	}
	std::cout << "n = (" << n.x << ", " << n.y << ", " << n.z << ")\n";
	return n;
}


void Plane::getPositions(int uu, int vv, int vertsPerSide, glm::vec3& v1, glm::vec3& n1, glm::vec3& v2, glm::vec3& n2, glm::vec3& v3, glm::vec3& n3, glm::vec3& v4, glm::vec3& n4, glm::vec3& v5, glm::vec3& n5, glm::vec3& v6, glm::vec3& n6)
{
	//	+-+
	//	|\|
	//	+-+
	if ((uu >= (vertsPerSide - 1) / 2 && vv < (vertsPerSide - 1) / 2) || ((uu < (vertsPerSide - 1) / 2 && vv >= (vertsPerSide - 1) / 2))) {
		v1 = preVerts[(uu + 0) + (vertsPerSide * (vv + 0))]; n1 = averageNormals((uu + 0), (vertsPerSide * (vv + 0)), vertsPerSide);
		v2 = preVerts[(uu + 0) + (vertsPerSide * (vv + 1))]; n2 = averageNormals((uu + 0), (vertsPerSide * (vv + 1)), vertsPerSide);
		v3 = preVerts[(uu + 1) + (vertsPerSide * (vv + 1))]; n3 = averageNormals((uu + 1), (vertsPerSide * (vv + 1)), vertsPerSide);

		v4 = preVerts[(uu + 1) + (vertsPerSide * (vv + 1))]; n4 = averageNormals((uu + 1), (vertsPerSide * (vv + 1)), vertsPerSide);
		v5 = preVerts[(uu + 1) + (vertsPerSide * (vv + 0))]; n5 = averageNormals((uu + 1), (vertsPerSide * (vv + 0)), vertsPerSide);
		v6 = preVerts[(uu + 0) + (vertsPerSide * (vv + 0))]; n6 = averageNormals((uu + 0), (vertsPerSide * (vv + 0)), vertsPerSide);
	}

	//	+-+
	//	|/|
	//	+-+
	else {
		v1 = preVerts[(uu + 1) + (vertsPerSide * (vv + 0))]; n1 = averageNormals((uu + 1), (vertsPerSide * (vv + 0)), vertsPerSide);
		v2 = preVerts[(uu + 0) + (vertsPerSide * (vv + 0))]; n2 = averageNormals((uu + 0), (vertsPerSide * (vv + 0)), vertsPerSide);
		v3 = preVerts[(uu + 0) + (vertsPerSide * (vv + 1))]; n3 = averageNormals((uu + 0), (vertsPerSide * (vv + 1)), vertsPerSide);

		v4 = preVerts[(uu + 0) + (vertsPerSide * (vv + 1))]; n4 = averageNormals((uu + 0), (vertsPerSide * (vv + 1)), vertsPerSide);
		v5 = preVerts[(uu + 1) + (vertsPerSide * (vv + 1))]; n5 = averageNormals((uu + 1), (vertsPerSide * (vv + 1)), vertsPerSide);
		v6 = preVerts[(uu + 1) + (vertsPerSide * (vv + 0))]; n6 = averageNormals((uu + 1), (vertsPerSide * (vv + 0)), vertsPerSide);
	}
}

void Plane::getPositions(int uu, int vv, int vertsPerSide, glm::vec2& t1, glm::vec2& t2, glm::vec2& t3, glm::vec2& t4, glm::vec2& t5, glm::vec2& t6) {
	//	+-+
	//	|\|
	//	+-+
	if ((uu >= (vertsPerSide - 1) / 2 && vv < (vertsPerSide - 1) / 2) || ((uu < (vertsPerSide - 1) / 2 && vv >= (vertsPerSide - 1) / 2))) {
		t1 = texCoords[(uu + 0) + (vertsPerSide * (vv + 0))];
		t2 = texCoords[(uu + 0) + (vertsPerSide * (vv + 1))];
		t3 = texCoords[(uu + 1) + (vertsPerSide * (vv + 1))];
		
		t4 = texCoords[(uu + 1) + (vertsPerSide * (vv + 1))];
		t5 = texCoords[(uu + 1) + (vertsPerSide * (vv + 0))];
		t6 = texCoords[(uu + 0) + (vertsPerSide * (vv + 0))];
	}

	//	+-+
	//	|/|
	//	+-+
	else {
		t1 = texCoords[(uu + 1) + (vertsPerSide * (vv + 0))];
		t2 = texCoords[(uu + 0) + (vertsPerSide * (vv + 0))];
		t3 = texCoords[(uu + 0) + (vertsPerSide * (vv + 1))];
		
		t4 = texCoords[(uu + 0) + (vertsPerSide * (vv + 1))];
		t5 = texCoords[(uu + 1) + (vertsPerSide * (vv + 1))];
		t6 = texCoords[(uu + 1) + (vertsPerSide * (vv + 0))];
	}
}

// Destructor of Sphere class.
Plane::~Plane() {
	delete[] preVerts;
	delete[] texCoords;
}

// Returns the number of Vertices per side of the plane at a given level of Tessellation.
int Plane::numVertsPerSide(int _level) {
	if (_level == 1)
		return 2;
	return 2 * numVertsPerSide(_level - 1) - 1;
}

// Sets a new level for the Sphere and calls genTriangles to create a new Sphere at the given level.
void Plane::setLevel(int _level) {
	level = _level;
	genTriangles();
}

// Redraws the Sphere where ever triangle has a random color.
void Plane::doRandomColors(bool _randomColor) {
	randomColor = _randomColor;
}

void Plane::reseed() {
	seed++;
}

// Redraws the Sphere where the Vertex normals are used instead of Surface normals of each Triangle.
void Plane::smoothSurface(bool _isSmooth) {
	isSmooth = _isSmooth;
}

void Plane::setScale(float _scale) {
	objScale = _scale;
}