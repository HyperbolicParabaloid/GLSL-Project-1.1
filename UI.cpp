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
	glm::vec4 clr = color;

	norm = glm::vec3(0.f, 0.f, -1.f);	// should be glm::vec3(0.f, -1.f, 0.f); !!!!!!!!!!!
	vertices.push_back(Vertex{ glm::vec3(-1.f, 1.f, 0.f), norm, clr, glm::vec2(-1.f, 1.f)});
	vertices.push_back(Vertex{ glm::vec3(1.f, 1.f, 0.f), norm, clr, glm::vec2(1.f, 1.f) });
	vertices.push_back(Vertex{ glm::vec3(-1.f, -1.f, 0.f), norm, clr, glm::vec2(-1.f, -1.f) });
	vertices.push_back(Vertex{ glm::vec3(1.f, -1.f, 0.f), norm, clr, glm::vec2(1.f, -1.f) });

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(1);

	glm::vec3 scalingVec = glm::vec3(objScale, objScale, 0.f);
	glm::vec3 screenPos = glm::vec3(objPos.x, objPos.y, 0.f);
	model = glm::translate(glm::mat4(1.f), screenPos - scalingVec);
	model = glm::scale(model, scalingVec);

	//if (true)
	//	for (int ii = 0; ii < indices.size(); ii += 3)
	//		triangles.push_back(Triangle{ &vertices[indices[ii + 0]], &vertices[indices[ii + 1]] , &vertices[indices[ii + 2]], &model });
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

// Returns whether 2D coordiantes are touching the UI Object.
bool UI::isTouching(glm::vec2 _cursorPos)
{
	// Barycentric test.
	glm::vec3 p = glm::vec3(_cursorPos, 0.f);
	for (int i = 0; i < indices.size(); i+=3) {
		glm::vec3 vec_1 = glm::vec3(model * glm::vec4(vertices[indices[i + 0]].pos, 1.f));
		glm::vec3 vec_2 = glm::vec3(model * glm::vec4(vertices[indices[i + 1]].pos, 1.f));
		glm::vec3 vec_3 = glm::vec3(model * glm::vec4(vertices[indices[i + 2]].pos, 1.f));

		// Compute vectors        
		glm::vec3 v0 = vec_3 - vec_1;
		glm::vec3 v1 = vec_2 - vec_1;
		glm::vec3 v2 = p - vec_1;

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
		bool result = ((u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.01f));

		//std::cout << "Is point p = (" << p.x << ", " << p.y << ", " << p.z << "), in triangle? [" << result << "]\n" << std::endl;
		if (result)
			return result;

	}
	return false;
}

void UI::setColor(glm::vec4 _color) {
	color = _color;
	genTriangles();
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