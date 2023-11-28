#include "Cone.h"

// Constructor for Sphere.
Cone::Cone(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, int _level, float _bottomRadius, float _topRadius, float _height, bool _isSmooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) : Object(_window, _objPos, _objScale, _color, _textures, _camera) {
	level = _level;
	isSmooth = _isSmooth;
	bottomRadius = _bottomRadius;
	topRadius = _topRadius;
	height = _height;
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
void Cone::genTriangles() {
	genCone();
	setVBOandEBO(verts, indices, "Cone");
}

void Cone::genCone() {
	// Whenever we generate a new set a vertices and indices, we want to wipe the old ones.
	// In the future it'd be better to just add in the new vertices and update indices instead
	// of clearing both indices and verts and starting over but it's fine for now.
	verts.clear();
	indices.clear();

	if (level < 2)
		level = 2;
	if (level > 36)
		level = 36;

	int vertsPerCircle = (level * 2) + 2;

	glm::vec3* newPreVerts = new glm::vec3[vertsPerCircle];	delete[] preVerts;
	preVerts = newPreVerts;
	glm::vec2* newTexCoords = new glm::vec2[vertsPerCircle];	delete[] texCoords;
	texCoords = newTexCoords;

	preVerts[0] = glm::vec3(0.f);
	preVerts[1] = glm::vec3(0.f, 3.f, 0.f);


	float theta = 0.f;
	float anglePerVertex = (360.f / level);
	for (int ii = 2; ii < level + 2; ii++) {



		theta = glm::radians(ii * anglePerVertex);//anglePerVert;
		preVerts[ii] = glm::vec3(sin(theta), 0.f, cos(theta)) * bottomRadius;
		//std::cout << theta << "\t(" << sin(theta) << ", " << 0.f << ", " << cos(theta) << ")\n";
	}
	setVerticesVector();
}

//
void Cone::setVerticesVector() {
	srand(seed);
	glm::vec4 color1;
	glm::vec3 normal = glm::vec3(0.f, -1.f, 0.f);

	int t0, t1, t2 = 0;
	int indCount = 0;
	// Algorithm will essentially be: n, n+1, 0		for all n >= 1.
	for (int jj = 0; jj < level; jj++) {
		t0 = ((jj + 0) % level) + 2;
		t1 = ((jj + 1) % level) + 2;
		t2 = 0;

		glm::vec3 v1 = preVerts[t0];
		glm::vec3 v2 = preVerts[t1];
		glm::vec3 v3 = preVerts[t2];
		glm::vec3 v4 = preVerts[1];

		glm::vec2 tex1 = texCoords[t0];	glm::vec3 n1;
		glm::vec2 tex2 = texCoords[t1];	glm::vec3 n2;

		// Setting the colors of the object:
		if (randomColor) {
			float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			color1 = glm::vec4(r1, r2, r3, 1.f);
		}
		else
			color1 = color;


		n1 = glm::normalize(glm::cross((v1 - v2), (v3 - v2)));
		n2 = glm::normalize(glm::cross((v4 - v2), (v1 - v2)));


		verts.push_back(Vertex{ v1, n1, color1, tex1 });
		verts.push_back(Vertex{ v2, n1, color1, tex2 });
		verts.push_back(Vertex{ v3, n1, color1, tex2 });

		verts.push_back(Vertex{ v1, n2, color1, tex1 });
		verts.push_back(Vertex{ v2, n2, color1, tex2 });
		verts.push_back(Vertex{ v4, n2, color1, tex2 });
		//verts.push_back(Vertex{ v3, normal, color1, tex3 });

		//std::cout << t0 <<	"\t(" << v1.x << ", " << v1.y << ", " << v1.z << ")\n";
		//std::cout << t1 <<	"\t(" << v2.x << ", " << v2.y << ", " << v2.z << ")\n";
		//std::cout << t2	<<	"\t(" << v3.z << ", " << v3.y << ", " << v3.z << ")\n\n";


		// Finally, setting the values of the Indices. I have it in such a way,
		// that indices[n] = n; for all n; >= 0, < indices.size().
		for (int ind = 0; ind < 6; ind++) {
			indices.push_back(indCount);
			indCount++;
		}
	}
}

// Destructor of Sphere class.
Cone::~Cone() {
	delete[] preVerts;
	delete[] texCoords;
}

// Returns the number of Vertices per side of the plane at a given level of Tessellation.
int Cone::numVertsPerSide(int _level) {
	if (_level == 1)
		return 2;
	return 2 * numVertsPerSide(_level - 1) - 1;
}

// Sets a new level for the Sphere and calls genTriangles to create a new Sphere at the given level.
void Cone::setLevel(int _level) {
	level = _level;
	genTriangles();
}

// Redraws the Sphere where ever triangle has a random color.
void Cone::doRandomColors(bool _randomColor) {
	randomColor = _randomColor;
}

void Cone::reseed() {
	seed++;
}

// Redraws the Sphere where the Vertex normals are used instead of Surface normals of each Triangle.
void Cone::smoothSurface(bool _isSmooth) {
	isSmooth = _isSmooth;
}
