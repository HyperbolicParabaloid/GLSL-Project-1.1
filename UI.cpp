#include "UI.h"

// Constructor for Plane.
UI::UI(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, int _level, bool _isSmooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) : Object(_window, _objPos, _objScale, _color, _textures, _camera) {
	level = _level;
	isSmooth = _isSmooth;
	randomColor = false;
	seed = 1;
	objRadius = sqrt(2);
	scale = glm::vec2(_objScale);
	genTriangles();
}

glm::vec3 UI::rayToObject(glm::vec3 _ray) {
	//return glm::vec3(FLT_MAX);
	return glm::vec3(objPos);
}

// Creates a new set of Vertex's and their associated indices to send to the Object
// class for drawing.
void UI::genTriangles() {
	genOctahedron();
	setVBOandEBO("UI");
}

void UI::genOctahedron() {
	// Whenever we generate a new set a vertices and indices, we want to wipe the old ones.
	// In the future it'd be better to just add in the new vertices and update indices instead
	// of clearing both indices and verts and starting over but it's fine for now.
	vertices.clear();
	indices.clear();
	triangles.clear();

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

	norm = glm::vec3(0.f, 0.f, -1.f);	// should be glm::vec3(0.f, -1.f, 0.f); !!!!!!!!!!!
	vertices.push_back(Vertex{ glm::vec3(1.f, 1.f, 0.f), norm, clr, glm::vec2(1.f, 1.f)});
	vertices.push_back(Vertex{ glm::vec3(-1.f, 1.f, 0.f), norm, clr, glm::vec2(-1.f, 1.f) });
	vertices.push_back(Vertex{ glm::vec3(-1.f, -1.f, 0.f), norm, clr, glm::vec2(-1.f, -1.f) });
	vertices.push_back(Vertex{ glm::vec3(1.f, -1.f, 0.f), norm, clr, glm::vec2(1.f, -1.f) });

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	glm::vec3 scalingVec = glm::vec3(objScale, objScale, 0.f);
	glm::vec3 screenPos = glm::vec3(objPos.x, objPos.y, 0.f);
	model = glm::translate(glm::mat4(1.f), screenPos - scalingVec);
	model = glm::scale(model, scalingVec);


	//int count = 0;
	//for (int vv = 0; vv < vertsPerSide; vv++) {
	//	for (int uu = 0; uu < vertsPerSide; uu++) {
	//
	//		// Can I just make uu and vv go from 0->1, and make that my UV texture coodiantes?
	//		// If so, I just need a new array to hold them, and assign them along with the preVerts.
	//		// then, when preVerts get mapped into postVerts inside of the setNorms function, I can
	//		// do the same thing with the texture coords. Perhaps. We shall see. Tomorrow. When I have coffee.
	//		//
	//		// I lied. A little. Should be something like:
	//		tex = glm::vec2(static_cast<float>(-uu) / (vertsPerSide - 1), static_cast<float>(-vv) / (vertsPerSide - 1)) * objScale;	// (0->1, 0->1)
	//		GLfloat x, y, z = -0.1f;
	//		x = ((uu + 2) - (vertsPerSide - 1)) / (float)(vertsPerSide - 1);	// Goes from -1.f =>  1.f
	//		y = ((vertsPerSide - 1) - (vv)) / (float)(vertsPerSide - 1);	// Goes from  1.f => -1.f
	//		//y += noise(glm::vec2(x * objScale + objPos.x, z * objScale + objPos.z)) / 10.f;
	//
	//		if (randomColor) {
	//			float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	//			float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	//			float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	//			clr = glm::vec4(r1, r2, r3, 1.f);
	//		}
	//		vert = glm::vec3(x, y, z);
	//		norm = glm::vec3(0.f, 0.f, -1.f);	// should be glm::vec3(0.f, -1.f, 0.f); !!!!!!!!!!!
	//
	//		vertices.push_back(Vertex{ vert, norm, clr, tex });
	//		if (uu + 1 < vertsPerSide && vv + 1 < vertsPerSide) {
	//			int i0 = vertsPerSide * vv + uu;
	//			int i1 = vertsPerSide * vv + (uu + 1);
	//			int i2 = vertsPerSide * (vv + 1) + uu;
	//			int i3 = vertsPerSide * (vv + 1) + (uu + 1);
	//
	//			indices.push_back(i2);
	//			indices.push_back(i1);
	//			indices.push_back(i0);
	//
	//			indices.push_back(i3);
	//			indices.push_back(i1);
	//			indices.push_back(i2);
	//		}
	//	}
	//}
	// Make this dependant on physics being enabled potentially.
	if (true)
		for (int ii = 0; ii < indices.size(); ii += 3)
			triangles.push_back(Triangle{ &vertices[indices[ii + 0]], &vertices[indices[ii + 1]] , &vertices[indices[ii + 2]], &model });
}

// Destructor of Sphere class.
UI::~UI() {
}

// Sets the screen coordinates of the UI object.
void UI::setScreenPos(glm::vec2 _screenCoords) {
	objPos = glm::vec3(_screenCoords, objPos.z);
	model = glm::translate(glm::mat4(1.f), glm::vec3(_screenCoords - scale, 0.0f));
	model = glm::scale(model, glm::vec3(scale, 0.f));
}

// Sets the X and Y scaling factors for the UI objects transformation.
void UI::setScale(glm::vec2 _scale) {
	scale = _scale;
	model = glm::translate(glm::mat4(1.f), objPos - glm::vec3(scale, 0.0f));
	model = glm::scale(model, glm::vec3(scale, 0.f));
}


glm::vec2 UI::getScale() {
	return scale;
}

// Returns the number of Vertices per side of the plane at a given level of Tessellation.
int UI::numVertsPerSide(int _level) {
	if (_level == 1)
		return 2;
	return 2 * numVertsPerSide(_level - 1) - 1;
}

// Sets a new level for the Sphere and calls genTriangles to create a new Sphere at the given level.
void UI::setLevel(int _level) {
	level = _level;
	genTriangles();
}

// Redraws the Sphere where ever triangle has a random color.
void UI::doRandomColors(bool _randomColor) {
	randomColor = _randomColor;
}

void UI::reseed() {
	seed++;
}

// Redraws the Sphere where the Vertex normals are used instead of Surface normals of each Triangle.
void UI::smoothSurface(bool _isSmooth) {
	isSmooth = _isSmooth;
}

void UI::setScale(float _scale) {
	objScale = _scale;
}