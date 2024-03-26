#include "Arrow.h"

// Constructor for Arrow.
Arrow::Arrow(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, int _level, glm::vec3 _bottomRadius, glm::vec3 _topRadius, glm::vec3 _pointPos, bool _smooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) : Object(_window, _objPos, _objScale, _color, _textures, _camera) {
	level = _level;
	smooth = _smooth;
	bottomRadius = _bottomRadius;
	topRadius = _topRadius;
	pointPos = _pointPos;
	startingPos = glm::vec3(0.f);
	randomColor = false;
	seed = 1;
	//indCount = 0;
	shaftColor = coneColor = color;
	genTriangles();
}

// Constructor for Arrow.
Arrow::Arrow(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, int _level, glm::vec3 _bottomRadius, glm::vec3 _topRadius, glm::vec3 _radi, glm::vec3 _pointPos, bool _smooth, glm::vec4 _shaftColor, glm::vec4 _coneColor, std::vector <Texture>& _textures, Camera* _camera) : Object(_window, _objPos, _objScale, _shaftColor, _textures, _camera) {
	level = _level;
	smooth = _smooth;
	bottomRadius = _bottomRadius;
	topRadius = _topRadius;
	pointPos = _pointPos;
	objPos = _objPos;
	startingPos = glm::vec3(0.f);
	randomColor = false;	
	seed = 1;
	//indCount = 0;
	shaftColor = _shaftColor;
	coneColor = _coneColor;
	radi = _radi;
	setScale(radi);
	genTriangles();
}

// Creates a new set of Vertex's and their associated indices to send to the Object
// class for drawing.
void Arrow::genTriangles() {
	vertices.clear();
	indices.clear();

	glm::vec3 bottomCenter_R = startingPos;
	glm::vec3 topCenter_R = pointPos;
	glm::vec3 bottomRadius_R = bottomRadius;
	glm::vec3 topRadius_R = topRadius;

	bool doBottomRadius = false;
	bool doTopRadius = false;

	if (level < 2)
		level = 2;
	if (level > 36)
		level = 36;

	int numCones = 3;
	int indCount = 0;
	for (int n = 0; n < numCones; n++) {
		genCone(bottomCenter_R, topCenter_R, bottomRadius_R, topRadius_R, doBottomRadius, doTopRadius, indCount);
		bottomCenter_R = topCenter_R;
		topCenter_R += topCenter_R;
		bottomRadius_R /= 2.f;
		topRadius_R /= 2.f;
	}

	setVBOandEBO("Arrow");
}

void Arrow::genCircle(glm::vec3 _pos, glm::vec3 _norm, glm::vec3 _radi, glm::vec2 _angles, int& _indCount) {
	float angle1 = _angles.x;
	float angle2 = _angles.y;

	glm::vec3 wallNorm1 = glm::vec3(cos(angle1), 0.f, -sin(angle1));
	glm::vec3 wallNorm2 = glm::vec3(cos(angle2), 0.f, -sin(angle2));
	glm::vec3 centerNorm = _norm;

	glm::vec3 wallPos1 = _pos + wallNorm1 * _radi;
	glm::vec3 wallPos2 = _pos + wallNorm2 * _radi;
	glm::vec3 center = _pos;

	glm::vec4 triColor;
	// Setting the colors of the object:
	if (randomColor) {
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		triColor = glm::vec4(r1, r2, r3, 1.f);
	}
	else
		triColor = color;


	glm::vec2 texCoordWall1 = wallNorm1;
	glm::vec2 texCoordWall2 = wallNorm2;
	glm::vec2 texCoordCenter = glm::vec2(0.f);

	vertices.push_back(Vertex{ wallPos1, centerNorm, triColor, texCoordWall1 });
	vertices.push_back(Vertex{ wallPos2, centerNorm, triColor, texCoordWall2 });
	vertices.push_back(Vertex{ center, centerNorm, triColor, texCoordCenter });

	indices.push_back(_indCount + 0);
	indices.push_back(_indCount + 1);
	indices.push_back(_indCount + 2);
	_indCount += 3;
}

void Arrow::genWall(glm::vec3 _startPos, glm::vec3 _endPos, glm::vec3 _startRadi, glm::vec3 _endRadi, glm::vec2 _angles, int& _indCount) {
	float angle1 = _angles.x;
	float angle2 = _angles.y;

	// Normals for use in creating positions.
	glm::vec3 wallNorm1 = glm::vec3(cos(angle1), 0.f, -sin(angle1));
	glm::vec3 wallNorm2 = glm::vec3(cos(angle2), 0.f, -sin(angle2));

	// Positions.
	glm::vec3 bottomWallPos1 = _startPos + wallNorm1 * _startRadi;
	glm::vec3 bottomWallPos2 = _startPos + wallNorm2 * _startRadi;
	glm::vec3 topWallPos1 = _endPos + wallNorm1 * _endRadi;
	glm::vec3 topWallPos2 = _endPos + wallNorm2 * _endRadi;

	// Setting the colors of the object:
	glm::vec4 bottomTopTriColor, topBottomTriColor;
	if (randomColor) {
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		bottomTopTriColor = glm::vec4(r1, r2, r3, 1.f);

		r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		topBottomTriColor = glm::vec4(r1, r2, r3, 1.f);
	}
	else
		bottomTopTriColor = topBottomTriColor = color;

	// Texture Coords.
	glm::vec2 texCoordBottomWall1 = _startPos + wallNorm1;
	glm::vec2 texCoordBottomWall2 = _startPos + wallNorm2;
	glm::vec2 texCoordTopWall1 = _endPos + wallNorm1;
	glm::vec2 texCoordTopWall2 = _endPos + wallNorm2;



	// Generating actual normals vectors for lighting.
	if (smooth) { // Smooth lighting.
		glm::vec3 bottomNorm1, bottomNorm2, topNorm1, topNorm2;

		glm::vec3 up = _endPos - _startPos;
		glm::vec3 forward1 = bottomWallPos1 - _startPos;
		glm::vec3 bottomTop1 = topWallPos1 - bottomWallPos1;
		glm::vec3 right1 = glm::cross(forward1, up);

		glm::vec3 norm1 = glm::cross(bottomTop1, right1);

		glm::vec3 forward2 = bottomWallPos2 - _startPos;
		glm::vec3 bottomTop2 = topWallPos2 - bottomWallPos2;
		glm::vec3 right2 = glm::cross(forward2, up);
		glm::vec3 norm2 = glm::cross(bottomTop2, right2);

		bottomNorm1 = topNorm1 = glm::normalize(norm1 / _startRadi);
		bottomNorm2 = topNorm2 = glm::normalize(norm2 / _endRadi);

		// bw1 tw1 tw2
		vertices.push_back(Vertex{ bottomWallPos1, bottomNorm1, topBottomTriColor, texCoordBottomWall1 });
		vertices.push_back(Vertex{ topWallPos1, topNorm1, topBottomTriColor, texCoordTopWall1 });
		vertices.push_back(Vertex{ topWallPos2, topNorm2, topBottomTriColor, texCoordTopWall2 });
		// tw2 bw2 bw1
		vertices.push_back(Vertex{ topWallPos2, topNorm2, topBottomTriColor, texCoordTopWall2 });
		vertices.push_back(Vertex{ bottomWallPos2, bottomNorm2, topBottomTriColor, texCoordBottomWall2 });
		vertices.push_back(Vertex{ bottomWallPos1, bottomNorm1, topBottomTriColor, texCoordBottomWall1 });
	}
	else { // Not smooth lighting.
		glm::vec3 bottomTopNorm, topBottomNorm;
		// bw1 tw1 tw2
		glm::vec3 topBottomEdge1 = bottomWallPos1 - topWallPos1;
		glm::vec3 topBottomEdge2 = topWallPos2 - topWallPos1;
		topBottomNorm = glm::normalize(glm::cross(topBottomEdge1, topBottomEdge2));

		// tw2 bw2 bw1
		glm::vec3 bottomTopEdge1 = topWallPos2 - bottomWallPos2;
		glm::vec3 bottomTopEdge2 = bottomWallPos1 - bottomWallPos2;
		bottomTopNorm = glm::normalize(glm::cross(bottomTopEdge1, bottomTopEdge2));

		// bw1 tw1 tw2
		vertices.push_back(Vertex{ bottomWallPos1, topBottomNorm, topBottomTriColor, texCoordBottomWall1 });
		vertices.push_back(Vertex{ topWallPos1, topBottomNorm, topBottomTriColor, texCoordTopWall1 });
		vertices.push_back(Vertex{ topWallPos2, topBottomNorm, topBottomTriColor, texCoordTopWall2 });
		// tw2 bw2 bw1
		vertices.push_back(Vertex{ topWallPos2, bottomTopNorm, topBottomTriColor, texCoordTopWall2 });
		vertices.push_back(Vertex{ bottomWallPos2, bottomTopNorm, topBottomTriColor, texCoordBottomWall2 });
		vertices.push_back(Vertex{ bottomWallPos1, bottomTopNorm, topBottomTriColor, texCoordBottomWall1 });
	}

	// Triangle 1, top-to-bottom.
	indices.push_back(_indCount + 0);
	indices.push_back(_indCount + 1);
	indices.push_back(_indCount + 2);
	// Triangle 2, bottom-to-top.
	indices.push_back(_indCount + 3);
	indices.push_back(_indCount + 4);
	indices.push_back(_indCount + 5);

	_indCount += 6;
}

// Generates a Cone of given dimensions.
void Arrow::genCone(glm::vec3 _startPos, glm::vec3 _endPos, glm::vec3 _startRadius, glm::vec3 _endRadius, bool _doingBottomCircle, bool _doingTopCircle, int &_indCount) {
	// Seeding random;
	srand(seed);

	// If the radius is too small to matter, there's no point wasting memory drawin
	// the circle that caps off that point.
	if (abs(glm::length(_startRadius)) < 0.01f)
		_doingBottomCircle = false;
	if (abs(glm::length(_endRadius)) < 0.01f)
		_doingTopCircle = false;

	// This is the angular step-size for the generation of the Cone's vertices.
	float anglePerVertex = (PI * 2.f) / level;

	// Loops over all required angles.
	for (int l = 0; l < level; l++) {
		// Setting angles for this triangle level.
		float angle1 = anglePerVertex * l;
		float angle2 = anglePerVertex * (l + 1);
		glm::vec2 angles = glm::vec2(angle1, angle2);

		// --------------------------------------- //
		// Generating the Lower Circle of the Cone //
		// --------------------------------------- //
		if (_doingBottomCircle)
			genCircle(_startPos, _startPos - _endPos, _startRadius, angles, _indCount);

		// --------------------------------------- //
		//    Generating the Walls of the Cone     //
		// --------------------------------------- //
		glm::vec3 _startRadi = glm::vec3(_startRadius);
		glm::vec3 _endRadi = glm::vec3(_endRadius);
		if (abs(glm::length(_startRadius)) >= 0.01f || abs(glm::length(_endRadius)) >= 0.01f)
			genWall(_startPos, _endPos, _startRadi, _endRadi, angles, _indCount);

		// --------------------------------------- //
		// Generating the Upper Circle of the Cone.//
		// --------------------------------------- //
		if (_doingTopCircle)
			genCircle(_endPos, _endPos - _startPos, _endRadius, angles, _indCount);
	}	
}

void Arrow::setTopRadius(glm::vec3 _topRadius) {
	topRadius = _topRadius;
	setLevel(level);
}

void Arrow::setBottomRadius(glm::vec3 _bottomRadius) {
	bottomRadius = _bottomRadius;
	setLevel(level);
}

void Arrow::setTipPos(glm::vec3 _pointPos) {
	pointPos = _pointPos;
	setLevel(level);
}

void Arrow::setVBOandEBO(std::string msg) {
	// Generates Shader object using shaders object.vert and object.frag
	delete shaderProgram;
	name = msg;
	
	shaderProgram = new Shader("GEOMETRY_TEST.vert", "GEOMETRY_TEST.frag", "GEOMETRY_TEST.geom");

	shaderProgram->Activate();

	VAO.Bind();

	// Setting VBO and EBO
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);


	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 4, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(10 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

//void Arrow::draw(glm::vec3 _lightPos, glm::vec4 _lightColor) {
//	// Activating the shader program and binding the VAO so OpenGL knows what we're trying to do.
//	shaderProgram->Activate();
//	VAO.Bind();
//
//	if (isWireframe)
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	else
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//
//	// Assigning all relevant info to the shader.
//	glUniform3f(glGetUniformLocation(shaderProgram->ID, "camPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
//	camera->camMatrixForShader(*shaderProgram, "camMatrix");
//	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
//	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), _lightPos.x, _lightPos.y, _lightPos.z);
//	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), _lightColor.x, _lightColor.y, _lightColor.z, _lightColor.w);
//	glUniform1f(glGetUniformLocation(shaderProgram->ID, "time"), glfwGetTime());
//
//	// Draw the actual mesh
//	glDrawElements(triangleType, indices.size(), GL_UNSIGNED_INT, 0);
//}


// Destructor of Arrow class.
Arrow::~Arrow() {
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
void Arrow::smoothSurface(bool _smooth) {
	smooth = _smooth;
	setLevel(level);
}
