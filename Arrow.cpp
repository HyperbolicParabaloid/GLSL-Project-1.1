#include "Arrow.h"

// Constructor for Cone.
Arrow::Arrow(glm::vec3 _objPos, float _objScale, int _level, float _bottomRadius, float _topRadius, glm::vec3 _pointPos, glm::vec3 _pointingAt, bool _isSmooth, glm::vec4 _color, int _startingIndex) {
	level = _level;
	isSmooth = _isSmooth;
	bottomRadius = _bottomRadius;
	topRadius = _topRadius;
	pointPos = _pointPos;
	objPos = _objPos;
	randomColor = false;
	seed = 1;
	model = glm::mat4(1.f);
	glm::vec3 axis = glm::cross(_pointPos, _pointingAt);
	float angle = acos(glm::dot(glm::normalize(_pointPos), glm::normalize(_pointingAt)));
	//angle = glm::degrees(angle);	I cannot believe THIS was the problem the whole time.
	//								I shall leave it here as a momento of my hubris, a warning.

	//std::cout << "Angle = " << angle << "\n";
	//std::cout << "axis = (" << axis.x << ", " << axis.y << ", " << axis.z << ")\n\n";

	model = glm::translate(model, objPos);
	model = glm::rotate(model, angle, axis);
	model = glm::scale(model, glm::vec3(_objScale));
	color = _color;
	startingIndex = _startingIndex;
	objScale = _objScale;
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
void Arrow::genTriangles() {
	genCone();
	//setVBOandEBO(verts, indices, "Cone");
}

std::vector <Vertex> Arrow::getVerts() {
	return verts;
}
std::vector <GLuint> Arrow::getInds() {
	return indices;
}

void Arrow::genCone() {
	// Whenever we generate a new set a vertices and indices, we want to wipe the old ones.
	// In the future it'd be better to just add in the new vertices and update indices instead
	// of clearing both indices and verts and starting over but it's fine for now.
	verts.clear();
	indices.clear();

	if (level < 2)
		level = 2;
	if (level > 36)
		level = 36;

	int vertsPerCone = (level * 2) + 2;

	glm::vec3* newPreVerts = new glm::vec3[vertsPerCone];	delete[] preVerts;
	preVerts = newPreVerts;
	glm::vec2* newTexCoords = new glm::vec2[vertsPerCone + 2]; delete[] texCoords;
	texCoords = newTexCoords;

	preVerts[0] = glm::vec3(model * glm::vec4(0.f, 0.f, 0.f, 1.f));// glm::mat3(model)* objPos;
	preVerts[1] = glm::vec3(model * glm::vec4(0.f, 1.f, 0.f, 1.f)); // glm::mat3(model)* pointPos;



	float theta1 = 0.f, theta2 = 0.f;
	float anglePerVertex = (360.f / level);
	for (int ii = 1; ii <= level; ii++) {
		int evenIndex = ii * 2;
		int oddIndex = ii * 2 + 1;

		float u = float(ii - 1) / float(level);

		theta1 = glm::radians(ii * anglePerVertex);
		theta2 = glm::radians(ii * anglePerVertex);		//theta2 = glm::radians((ii + 0.5f) * anglePerVertex2);//anglePerVert;	// This is to offset the top/bottom vertices. It doesn't look very good tbh.

		float x = sin(theta1), y = cos(theta1);
		preVerts[evenIndex] = glm::vec3(model * glm::vec4(x * bottomRadius,	0.f,	y * bottomRadius,	1));//glm::mat3(model) * (((glm::vec3(sin(theta1), 0.f, cos(theta1)) * bottomRadius) + objPos));
		preVerts[oddIndex] = glm::vec3(model * glm::vec4(x * topRadius,		5.f,	y * topRadius,		1));//glm::mat3(model) * (glm::vec3(sin(theta2) * topRadius, 0.f, cos(theta2) * topRadius) + objPos + pointPos);

		texCoords[evenIndex] = glm::vec2(u * 4, 0);
		texCoords[oddIndex] = glm::vec2(u * 4, 1);	// 0.25 should be 1
	}
	texCoords[vertsPerCone] = glm::vec2(4, 0);
	texCoords[vertsPerCone + 1] = glm::vec2(4, 1);// 0.25 should be 1
	setVerticesVector();
	
}


void Arrow::setVerticesVector() {
	srand(seed);
	glm::vec4 color1, color2;
	glm::vec3 n1, n2, n3, n4;

	// Value to tell the counter when to wrap back to begining to finish the triangles.
	int evenLength = (level * 2);

	// Values for the vertices indexes.
	int lwrCntr = 0, uprCntr = 1;
	int lwrTriVrt1, lwrTriVrt2, uprTriVrt1, uprTriVrt2;
	int indCount = 0;
	// Algorithm will essentially be: n, n+1, 0		for all n >= 1.
	for (int jj = 1; jj <= level; jj++) {
		lwrTriVrt1 = (jj * 2);
		lwrTriVrt2 = ((jj * 2) % evenLength) + 2;

		uprTriVrt1 = (jj * 2) + 1;
		uprTriVrt2 = ((jj * 2) % evenLength) + 3;

		glm::vec3 v1 = preVerts[lwrTriVrt1];	glm::vec2 tex1 = texCoords[lwrTriVrt1];
		glm::vec3 v2 = preVerts[lwrTriVrt2];	glm::vec2 tex2 = texCoords[lwrTriVrt1 + 2];
		glm::vec3 v3 = preVerts[lwrCntr];		glm::vec2 tex3 = texCoords[lwrCntr];
		glm::vec3 v4 = preVerts[uprCntr];		glm::vec2 tex4 = texCoords[uprCntr];

		glm::vec3 v5 = preVerts[uprTriVrt1];	glm::vec2 tex5 = texCoords[uprTriVrt1];
		glm::vec3 v6 = preVerts[uprTriVrt2];	glm::vec2 tex6 = texCoords[uprTriVrt1 + 2];

		// Setting the colors of the object:
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

		// Norms for each of the 4 triangles.
		n1 = glm::normalize(glm::cross((v1 - v2), (v3 - v2)));
		n2 = glm::normalize(glm::cross((v5 - v2), (v1 - v2)));
		n3 = glm::normalize(glm::cross((v4 - v6), (v5 - v6)));
		n4 = glm::normalize(glm::cross((v5 - v6), (v2 - v6)));

		if (isSmooth) {
			// Setting the previous and future indices.
			int prevjj, futrjj;
			if (jj - 1 <= 0) {
				prevjj = level;
			}
			else
				prevjj = jj - 1;
			if (jj + 1 > level) {
				futrjj = 1;
			}
			else
				futrjj = jj + 1;

			// NOTE! // The top-to-bottom triangle is the problem! Not sure why, I'm investigating.


			// Getting the previous faces nromal.
			lwrTriVrt1 = (prevjj * 2);
			lwrTriVrt2 = ((prevjj * 2) % evenLength) + 2;
			uprTriVrt1 = (prevjj * 2) + 1;
			uprTriVrt2 = ((prevjj * 2) % evenLength) + 3;
			glm::vec3 prev1 = preVerts[lwrTriVrt1]; glm::vec3 prev2 = preVerts[lwrTriVrt2]; glm::vec3 prev5 = preVerts[uprTriVrt1]; glm::vec3 prev6 = preVerts[uprTriVrt2];
			glm::vec3 preNorm = glm::normalize(glm::cross((prev5 - prev2), (prev1 - prev2)));

			//std::cout << "prev2 = (" << prev2.x << ", " << prev2.y << ", " << prev2.z << "\n";
			//std::cout << "prev5 = (" << prev5.x << ", " << prev5.y << ", " << prev5.z << "\n";
			//std::cout << "prev6 = (" << prev6.x << ", " << prev6.y << ", " << prev6.z << "\n\n";

			// Getting the future faces nromal.
			lwrTriVrt1 = (futrjj * 2);
			lwrTriVrt2 = ((futrjj * 2) % evenLength) + 2;
			uprTriVrt1 = (futrjj * 2) + 1;
			glm::vec3 ftrv1 = preVerts[lwrTriVrt1]; glm::vec3 ftrv2 = preVerts[lwrTriVrt2]; glm::vec3 ftrv5 = preVerts[uprTriVrt1];
			glm::vec3 ftrNorm = glm::normalize(glm::cross((ftrv5 - ftrv2), (ftrv1 - ftrv2)));

			//std::cout << "ftrv5 = (" << ftrv5.x << ", " << ftrv5.y << ", " << ftrv5.z << "\n";
			//std::cout << "ftrv2 = (" << ftrv2.x << ", " << ftrv2.y << ", " << ftrv2.z << "\n";
			//std::cout << "ftrv1 = (" << ftrv1.x << ", " << ftrv1.y << ", " << ftrv1.z << "\n\n";

			// Finally, setting the future normal and previous normals.
			glm::vec3 avgNorm1 = glm::normalize((preNorm + n2) / 2.f);
			glm::vec3 avgNorm2 = glm::normalize((ftrNorm + n2) / 2.f);

			//std::cout << "avgNorm1 = (" << avgNorm1.x << ", " << avgNorm1.y << ", " << avgNorm1.z << "\n";
			//std::cout << "avgNorm2 = (" << avgNorm2.x << ", " << avgNorm2.y << ", " << avgNorm2.z << "\n\n";

			// Lower circle triangle.
			verts.push_back(Vertex{ v1, n1, color1, tex1 });
			verts.push_back(Vertex{ v2, n1, color1, tex2 });
			verts.push_back(Vertex{ v3, n1, color1, tex3 });

			// Low-to-high side triangle.
			verts.push_back(Vertex{ v1, avgNorm1, color1, tex1 });
			verts.push_back(Vertex{ v2, avgNorm2, color1, tex2 });
			verts.push_back(Vertex{ v5, avgNorm1, color1, tex5 });

			// Upper circle triangle.
			verts.push_back(Vertex{ v5, n3, color2, tex5 });
			verts.push_back(Vertex{ v6, n3, color2, tex6 });
			verts.push_back(Vertex{ v4, n3, color2, tex4 });

			// High-to-low side triangle.	// NOTE! // The top-to-bottom triangle is the problem! Not sure why, I'm investigating.
			verts.push_back(Vertex{ v5, avgNorm1, color2, tex5 });	// Change back
			verts.push_back(Vertex{ v6, avgNorm2, color2, tex6 });
			verts.push_back(Vertex{ v2, avgNorm2, color2, tex2 });
		}
		else {
			// Lower circle triangle.
			verts.push_back(Vertex{ v1, n1, color1, tex1 });
			verts.push_back(Vertex{ v2, n1, color1, tex2 });
			verts.push_back(Vertex{ v3, n1, color1, tex3 });

			// Low-to-high side triangle.f
			verts.push_back(Vertex{ v1, n2, color1, tex1 });
			verts.push_back(Vertex{ v2, n2, color1, tex2 });
			verts.push_back(Vertex{ v5, n2, color1, tex5 });

			// Upper circle triangle.
			verts.push_back(Vertex{ v5, n3, color2, tex5 });
			verts.push_back(Vertex{ v6, n3, color2, tex6 });
			verts.push_back(Vertex{ v4, n3, color2, tex4 });

			// High-to-low side triangle.
			verts.push_back(Vertex{ v5, n4, color2, tex5 });
			verts.push_back(Vertex{ v6, n4, color2, tex6 });
			verts.push_back(Vertex{ v2, n4, color2, tex2 });
		}

		// Finally, setting the values of the Indices. I have it in such a way,
		// that indices[n] = n; for all n; >= 0, < indices.size().
		for (int ind = 0; ind < 12; ind++) {
			indices.push_back(indCount + startingIndex);
			indCount++;
		}
	}
}

void Arrow::setTopRadius(float _topRadius) {
	topRadius = _topRadius;
	setLevel(level);
}

void Arrow::setBottomRadius(float _bottomRadius) {
	bottomRadius = _bottomRadius;
	setLevel(level);
}

void Arrow::setTipPos(glm::vec3 _pointPos) {
	pointPos = _pointPos;
	setLevel(level);
}


// Destructor of Cone class.
Arrow::~Arrow() {
	delete[] preVerts;
	delete[] texCoords;
}

// Returns the number of Vertices per side of the plane at a given level of Tessellation.
int Arrow::numVertsPerSide(int _level) {
	if (_level == 1)
		return 2;
	return 2 * numVertsPerSide(_level - 1) - 1;
}

// Sets a new level for the Sphere and calls genTriangles to create a new Sphere at the given level.
void Arrow::setLevel(int _level) {
	level = _level;
	genTriangles();
}

// Redraws the Sphere where ever triangle has a random color.
void Arrow::doRandomColors(bool _randomColor) {
	randomColor = _randomColor;
}

void Arrow::reseed() {
	seed++;
}

// Redraws the Sphere where the Vertex normals are used instead of Surface normals of each Triangle.
void Arrow::smoothSurface(bool _isSmooth) {
	isSmooth = _isSmooth;
}
