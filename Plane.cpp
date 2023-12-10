#include "Plane.h"

// Constructor for Plane.
Plane::Plane(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, int _level, bool _isSmooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) : Object(_window, _objPos, _objScale, _color, _textures, _camera) {
	level = _level;
	isSmooth = _isSmooth;
	randomColor = false;
	seed = 1;
	objRadius = sqrt(2);
	genTriangles();
}

// Creates a new set of Vertex's and their associated indices to send to the Object
// class for drawing.
void Plane::genTriangles() {
	//triangleType = GL_TRIANGLE_STRIP;
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


	float temp;
	glm::vec3 vert;
	glm::vec3 norm;
	glm::vec2 tex;
	glm::vec4 clr;
	clr = color;

	int count = 0;
	for (int vv = 0; vv < vertsPerSide; vv++) {
		for (int uu = 0; uu < vertsPerSide; uu++) {

			// Can I just make uu and vv go from 0->1, and make that my UV texture coodiantes?
			// If so, I just need a new array to hold them, and assign them along with the preVerts.
			// then, when preVerts get mapped into postVerts inside of the setNorms function, I can
			// do the same thing with the texture coords. Perhaps. We shall see. Tomorrow. When I have coffee.
			//
			// I lied. A little. Should be something like:
			tex = glm::vec2(static_cast<float>(-uu) / (vertsPerSide - 1), static_cast<float>(-vv) / (vertsPerSide - 1)) * objScale;	// (0->1, 0->1)
			GLfloat x, z, y = 0.f;
			x = ((uu * 2) - (vertsPerSide - 1)) / (float)(vertsPerSide - 1);	// Goes from -1.f =>  1.f
			z = ((vertsPerSide - 1) - (vv * 2)) / (float)(vertsPerSide - 1);	// Goes from  1.f => -1.f

			if (randomColor) {
				float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				clr = glm::vec4(r1, r2, r3, 1.f);
			}
			vert = glm::vec3(x, y, z);
			norm = glm::vec3(0.f, -1.f, 0.f);

			verts.push_back(Vertex{ vert, norm, clr, tex });
			if (uu + 1 < vertsPerSide && vv + 1 < vertsPerSide) {
				int i0 = vertsPerSide * vv + uu;
				int i1 = vertsPerSide * vv + (uu + 1);
				int i2 = vertsPerSide * (vv + 1) + uu;
				int i3 = vertsPerSide * (vv + 1) + (uu + 1);

				indices.push_back(i2);
				indices.push_back(i1);
				indices.push_back(i0);

				indices.push_back(i3);
				indices.push_back(i1);
				indices.push_back(i2);
			}
		}
	}
	// Make this dependant on physics being enabled potentially.
	if (true)
		for (int ii = 0; ii < indices.size(); ii+=3)
			triangles.push_back(Triangle{ &verts[indices[ii + 0]], &verts[indices[ii + 1]] , &verts[indices[ii + 2]], &model });
}

// Destructor of Sphere class.
Plane::~Plane() {
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