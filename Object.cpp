#include "Object.h"


// Object classes constructor. Just sets some member variables and updates the glm::mat4 model of the object.
Object::Object(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) {
	window = _window;
	doNormalArrows = false;
	objPos = _objPos;
	objScale = _objScale;
	color = _color;
	radi = glm::vec3(objScale);
	model = glm::translate(glm::mat4(1.f), objPos);
	model = glm::scale(model, glm::vec3(objScale));
	rotationMatrix = glm::mat4(1.f);
	camera = _camera;
	textures = _textures;

	pastObjPos = glm::vec3(0.f);
	samePosCount = 0;

	deltaTime = 1.f / 60.f;
	mass = 10.f;
	velocity = glm::vec3(0.f);
	rotationalVelocity = 0.f;
	rotationalAxis = glm::vec3(0.f);


	randomColor = false;
	smooth = true;

	// Not really used for all objects, so starts at -1.
	level = -1;

	//pointingAt = glm::vec3(0.f, 1.f, 0.f);
	normalShaderProgram = nullptr;
}

glm::vec3 Object::rayToObject(glm::vec3 _ray) {
	//return glm::vec3(FLT_MAX);
	return glm::vec3(0.f);
}

// Translates the object to a new position in world space.
void Object::setNewPos(glm::vec3 _objPos) {
	//std::cout << "rotationMatrix =  | " << rotationMatrix[0][0] << ", " << rotationMatrix[0][1] << ", " << rotationMatrix[0][2] << ", " << rotationMatrix[0][3] << "|\n";
	//std::cout << "rotationMatrix =  | " << rotationMatrix[1][0] << ", " << rotationMatrix[1][1] << ", " << rotationMatrix[1][2] << ", " << rotationMatrix[1][3] << "|\n";
	//std::cout << "rotationMatrix =  | " << rotationMatrix[2][0] << ", " << rotationMatrix[2][1] << ", " << rotationMatrix[2][2] << ", " << rotationMatrix[2][3] << "|\n";
	//std::cout << "rotationMatrix =  | " << rotationMatrix[3][0] << ", " << rotationMatrix[3][1] << ", " << rotationMatrix[3][2] << ", " << rotationMatrix[3][3] << "|\n\n";
	objPos = _objPos;
	model = glm::translate(glm::mat4(1.f), objPos) * rotationMatrix * glm::scale(glm::mat4(1.f), radi);
}

// Rotates the object about a given axis by a set angle in degrees.
void Object::rotate(float rotationDegreeAngle, glm::vec3 axisOfRotation) {
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.f), glm::radians(rotationDegreeAngle), axisOfRotation);
	rotationMatrix *= rotMat;
	model = glm::translate(glm::mat4(1.f), objPos)  * rotationMatrix * glm::scale(glm::mat4(1.f), radi);
	//model = model * rotMat;
}

// Sets the radi scaling value.
void Object::setScale(glm::vec3 _radi) {
	radi = _radi;
	model = glm::translate(glm::mat4(1.f), objPos) * rotationMatrix * glm::scale(glm::mat4(1.f), radi);
}

// Sets the radi scaling value.
void Object::setScale(float _scale) {
	objScale = _scale;
	model = glm::translate(glm::mat4(1.f), objPos) * rotationMatrix * glm::scale(glm::mat4(1.f), glm::vec3(objScale));
}

/* NOT WORKING
// Returns closest point on Ellipsoid struck by ray.
glm::vec3 Object::isRayTouching(glm::vec3& _rayStart, glm::vec3& _rayDir) {
	glm::mat4 modelInv = inverse(model);
	glm::vec3 ro = _rayStart - objPos; // glm::vec3(modelInv * glm::vec4(_rayStart, 1.f));// _rayStart - objPos;//
	glm::vec3 rd = _rayDir; //glm::vec3(modelInv * glm::vec4(_rayDir, 1.f)); //

	glm::vec3 r = glm::vec3(1.f, 1.f, 1.f / 25.f);
	
	glm::vec3 r2 = r * r; // Squared radius.
	float a = dot(rd, rd / r2); // Quadratic formula values a->c.
	float b = dot(ro, rd / r2);
	float c = dot(ro, ro / r2);
	float d = b * b - a * (c - 1.f); // Determinant.
	
	if (d < 0.f) { // If the determinant is less than 0.f, then the ray missed.
		return glm::vec3(0.0);
	}
	
	float t = (-b - sqrt(d)) / a;
	return _rayStart + _rayDir * t;
}
*/
// Determines if a ray hits any part of the objects mesh.
glm::vec3 Object::isRayTouching(glm::vec3 _rayStart, glm::vec3 _rayDir) {
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
glm::vec3 Object::pointOnPlane(glm::vec3 planePoint, glm::vec3 planeNormal, glm::vec3 rayStart, glm::vec3 rayDirection) {
	// Denom is the cos of the angle between teh ray direction and the normal of the plane.
	// If the ray is at more than a 90 degree angle, (cos(90) => cos(180) = (0 -> -1)) then
	// the denom will be less than 0. This means ray the hits a backface of an object won't count it as a hit.
	float denom = glm::dot(glm::normalize(rayDirection), planeNormal);

	// If the denom is greater than 0, we have hit the plane and will return that point.
	if (denom > 0.f) {
		float t = (dot(planePoint - rayStart, planeNormal) / denom);
		if (t >= 0.f) {
			return rayStart + (glm::normalize(rayDirection) * t);
		}
	}

	// If we miss the plane we just return this FLT_MAX to communicate the failure.
	return glm::vec3(FLT_MAX);
}

// Determines if point is on the triangle given by the 3 vertices.
bool Object::barycentricInterpolation(glm::vec3 _vert0, glm::vec3 _vert1, glm::vec3 _vert2, glm::vec3 _p) {
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
	return ((u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.f));
}

bool Object::isCursorTouching(glm::vec2 _cursorPos)
{
	if (isRayTouching(camera->cameraPos, camera->getCursorRay()) == glm::vec3(0.f))
		return false;
	return true;
}


void Object::setLevel(int _level) {
	std::cout << "N/A";
}

void Object::setColor(glm::vec4 _color) {
	color = _color;
}

// Draws the object to the screen and sets the appropriate information in the Shader object about position, and lighting.
void Object::draw(glm::vec3 _lightPos, glm::vec4 _lightColor) {
	// Activating the shader program and binding the VAO so OpenGL knows what we're trying to do.
	shaderProgram->Activate();
	VAO.Bind();

	if (isWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Assigning all relevant info to the shader.
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "camPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
	camera->camMatrixForShader(*shaderProgram, "camMatrix");
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), _lightPos.x, _lightPos.y, _lightPos.z);
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), _lightColor.x, _lightColor.y, _lightColor.z, _lightColor.w);
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "time"), glfwGetTime());
	if (name == "Tree" || name == "Plane")
		glUniform3f(glGetUniformLocation(shaderProgram->ID, "startPos"), objPos.x / 25.f, objPos.y / 25.f, objPos.z / 25.f);
		
	// Draw the actual mesh
	glDrawElements(triangleType, indices.size(), GL_UNSIGNED_INT, 0);

	// Drawing normals if applicable.
	if (normalShaderProgram != nullptr)
		drawNormals(_lightPos, _lightColor);


}

// Creates a new shader program and binds the adds the appropriate information into the GPU's memory so it can draw the
// object.

void Object::setVBOandEBO(std::string msg) {
	// Generates Shader object using shaders object.vert and object.frag
	delete shaderProgram;
	name = msg;
	if (msg == "Plane") {
		shaderProgram = new Shader("grass.vert", "grass.frag");
	}
	else if (msg == "Tree") {
		shaderProgram = new Shader("tree.vert", "tree.frag");
	}
	else if (msg == "UI") {
		shaderProgram = new Shader("ui.vert", "ui.frag");
	}
	else
		shaderProgram = new Shader("object.vert", "object.frag");
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

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	// Assign all the relevant information to the shader.
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
			glUniform1i(glGetUniformLocation(shaderProgram->ID, "useTex"), 1);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
			glUniform1i(glGetUniformLocation(shaderProgram->ID, "useTexSpec"), 1);
		}
		textures[i].texUnit(*shaderProgram, (type + num).c_str(), textures[i].unit);
		textures[i].Bind();
	}

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();

	
}

void Object::drawNormals(glm::vec3 _lightPos, glm::vec4 _lightColor) {
	// Activating the shader program and binding the VAO so OpenGL knows what we're trying to do.
	normalShaderProgram->Activate();
	VAO.Bind();

	if (isWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Assigning all relevant info to the shader.
	glUniform3f(glGetUniformLocation(normalShaderProgram->ID, "camPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
	camera->camMatrixForShader(*normalShaderProgram, "camMatrix");
	glUniformMatrix4fv(glGetUniformLocation(normalShaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(glGetUniformLocation(normalShaderProgram->ID, "lightPos"), _lightPos.x, _lightPos.y, _lightPos.z);
	glUniform4f(glGetUniformLocation(normalShaderProgram->ID, "lightColor"), _lightColor.x, _lightColor.y, _lightColor.z, _lightColor.w);
	glUniform1f(glGetUniformLocation(normalShaderProgram->ID, "time"), glfwGetTime());

	// Draw the actual mesh
	glDrawElements(triangleType, indices.size(), GL_UNSIGNED_INT, 0);
}
void Object::setNormalsVBOandEBO() {
	// Generates Shader object using shaders object.vert and object.frag
	delete normalShaderProgram;
	
	normalShaderProgram = new Shader("GEOMETRY_NORMALS.vert", "GEOMETRY_NORMALS.frag", "GEOMETRY_NORMALS.geom");
	normalShaderProgram->Activate();


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


void Object::setVBOandEBO(std::vector <Vertex>& _vertices, std::vector <GLuint>& _indices, std::string msg) {
	// Generates Shader object using shaders object.vert and object.frag
	delete shaderProgram;
	name = msg;
	if (msg == "Plane") {
		shaderProgram = new Shader("grass.vert", "grass.frag");
	}
	else if (msg == "Tree") {
		shaderProgram = new Shader("tree.vert", "tree.frag");
	}
	else if (msg == "UI") {
		std::cout << "UI ELEMENT\n";
		shaderProgram = new Shader("ui.vert", "ui.frag");
	}
	else
		shaderProgram = new Shader("object.vert", "object.frag");
	shaderProgram->Activate();

	vertices = _vertices;
	indices = _indices;

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

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	// Assign all the relevant information to the shader.
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
			glUniform1i(glGetUniformLocation(shaderProgram->ID, "useTex"), 1);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
			glUniform1i(glGetUniformLocation(shaderProgram->ID, "useTexSpec"), 1);
		}
		textures[i].texUnit(*shaderProgram, (type + num).c_str(), i);
		textures[i].Bind();
	}

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

// Object class's destructor. Just deletes the dynamically allocated Shader object.
Object::~Object() {
	shaderProgram->Delete();
	delete shaderProgram;
}

void Object::moveFirstVertex() {
	//vertices[0].pos.y += 0.1f;
	//(noise(aPos.xz + time) * 2.f) + (noise((aPos.xz + time) * 1.2) * 1/8) + (noise((aPos.xz + time) * 2) * 1/16);
	for (Vertex& v : vertices) {
		v.pos.y = noise(glm::vec2(v.texCoord.x + glfwGetTime(), v.texCoord.y + glfwGetTime()));
		std::cout << "Yaaaaas\n";
	}

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

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

float Object::newrand(glm::vec2 co) {
	return glm::fract(sin(glm::dot(co, glm::vec2(12.9898, 78.233))) * 43758.5453);
}

float Object::noise(glm::vec2 n) {
	const glm::vec2 d = glm::vec2(0.0, 1.0);
	glm::vec2 b = floor(n), f = glm::smoothstep(glm::vec2(0.0), glm::vec2(1.0), fract(n));
	return glm::mix(glm::mix(newrand(b), newrand(b + glm::vec2(d.y, d.x)), f.x), glm::mix(newrand(b + glm::vec2(d.x, d.y)), newrand(b + glm::vec2(d.y, d.y)), f.x), f.y);
}

void Object::hotRealoadShader() {
	shaderProgram->hotReaload();
	shaderProgram->Activate();

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	// Assign all the relevant information to the shader.
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
			glUniform1i(glGetUniformLocation(shaderProgram->ID, "useTex"), 1);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
			glUniform1i(glGetUniformLocation(shaderProgram->ID, "useTexSpec"), 1);
		}
		textures[i].texUnit(*shaderProgram, (type + num).c_str(), i);
		textures[i].Bind();
	}
}

// Toggles random colors.
void Object::doRandomColors(bool _randomColor) {
	randomColor = _randomColor;
}

bool Object::getRandomColor()
{
	return randomColor;
}

void Object::smoothSurface(bool _isSmooth) {
	smooth = _isSmooth;
}

// Aims object in the given direction.
void Object::pointAt(glm::vec3 _direction, bool _isTopPointing) {
	// Removing the y axis from _direction and normalizing it gives us yawPlaneDir, which we use to reset
	// the rolling effect so that cone isn't rolling all over the place when it tracks a target.
	// Forward is the forward direction (down the -z axis).
	glm::vec3 forward = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 yawPlaneDir = glm::normalize(glm::vec3(_direction.x, 0.f, _direction.z));
	float yaw = glm::dot(forward, yawPlaneDir);

	// Sets Top (+y axis) of object to pitch to _direction.
	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 right = glm::cross(up, _direction);
	float topPitch = glm::dot(up, _direction);
	// Sets Forward (-z axis) of object to pitch to _direction.
	glm::vec3 pitchPlaneDir = glm::cross(yawPlaneDir, _direction);
	float forwardPitch = glm::dot(yawPlaneDir, _direction);

	// We have 3 matrices here: objMat, objRot, and rotMat.
	glm::mat4 objMat = glm::translate(glm::mat4(1.f), objPos) * glm::scale(glm::mat4(1.f), radi);// model;
	glm::mat4 objRot = glm::mat4(1.f);
	glm::mat4 rotMat = glm::mat4(1.f);

	// My attempt at tryng to fix the teleporting/disappearing effects of trying to "aim"
	// directly below you.
	if (_direction == up) {
		model = objMat;
		return;
	}
	else if (_direction == -up) {
		if (_isTopPointing) {
			objRot = glm::rotate(glm::mat4(1.f), float(PI), glm::vec3(1.f, 0.f, 0.f));// Aim
			rotMat = glm::rotate(glm::mat4(1.f), float(PI), -up); // Roll
		}
		else {
			objRot = glm::rotate(glm::mat4(1.f), float(PI / 2.f), glm::vec3(1.f, 0.f, 0.f));// Aim
			rotMat = glm::rotate(glm::mat4(1.f), float(PI / 2.f), -up); // Roll
		}
		rotationMatrix = objRot * rotMat;
		model = objMat * rotationMatrix;
		return;
	}

	// Making sure it's a real number.
	// TL:DR: should probably have just used quaternions because this rotation crap issue
	// where the object disappears if the thing it's pointing at is directly below it is rough
	// and mostly do to rounding errors. Could probably fix it by checking if the new direction
	// is either equal to and opposite of up.
	if (_isTopPointing && 
		!isnan(right.x) && !isnan(right.y) && !isnan(right.z) && !isnan(acos(topPitch)) &&
		!isinf(right.x) && !isinf(right.y) && !isinf(right.z) && !isinf(acos(topPitch)) &&
		abs(topPitch) > 0.f && abs(topPitch) < 1.f)
		objRot = glm::rotate(glm::mat4(1.f), acos(topPitch), right);// Aim

	if (!_isTopPointing && 
		!isnan(pitchPlaneDir.x) && !isnan(pitchPlaneDir.y) && !isnan(pitchPlaneDir.z) && !isnan(acos(forwardPitch)) &&
		!isinf(pitchPlaneDir.x) && !isinf(pitchPlaneDir.y) && !isinf(pitchPlaneDir.z) && !isinf(acos(forwardPitch)) &&
		abs(forwardPitch) > 0.f && abs(forwardPitch) < 1.f)
		objRot = glm::rotate(glm::mat4(1.f), acos(forwardPitch), pitchPlaneDir);// Aim
	
	// Making sure it's a real number.
	if (_direction.x < 0.f && !isnan(yaw))
		rotMat = glm::rotate(glm::mat4(1.f), acos(yaw), up); // Roll
	else if (!isnan(yaw))
		rotMat = glm::rotate(glm::mat4(1.f), acos(yaw), -up); // Roll
	
	rotationMatrix = objRot * rotMat;
	model = objMat * rotationMatrix;
}

void Object::toggleNormalArrows() {
	doNormalArrows = !doNormalArrows;
}


//				--------------------				//
//				Physics interactions				//
//				--------------------				//


// Returns true if the bounding spheres of two objects overlap. This does not necessarily mean that
// the two meshes actually touch. That's a much more expensive computation handled in getIntersection.
bool Object::isTouching(Object* obj) {
	float lenA = objScale * objRadius;
	float lenB = obj->objScale * obj->objRadius;
	if (glm::length(obj->objPos - objPos) <= lenA + lenB)
		return true;
	return false;
}
bool Object::isTouching(Triangle* tri) {
	tri->genCircle();
	float lenA = objScale * objRadius;
	float lenB = tri->radius;
	if (glm::length(tri->center - objPos) <= lenA + lenB)
		return true;// triangleIntersection(tri);
	return false;
}

bool Object::isTouching(Triangle* tri, int index, glm::vec3* _p11, glm::vec3* _p12, glm::vec3* _p13, glm::vec3* _p21, glm::vec3* _p22, glm::vec3* _p23) {
	tri->genCircle();
	float lenA = objScale * objRadius;
	float lenB = tri->radius;
	if (glm::length(tri->center - objPos) <= lenA + lenB)
		return triangleIntersection(tri, index, _p11, _p12, _p13, _p21, _p22, _p23);
	return false;
}

bool Object::triangleIntersection(Triangle* tri, int index, glm::vec3* _p11, glm::vec3* _p12, glm::vec3* _p13, glm::vec3* _p21, glm::vec3* _p22, glm::vec3* _p23) {
	if (!isSolid)
		return false;

	// genCircle makes sure all the triangles are up to date with the objects current model matrix.
	triangles[index].genCircle();
	tri->genCircle();

	// denom will always be -1.f because we want the shortest path to the plane.
	//float denom = -1.f;

	// Normal of triangle, A.K.A., normal of plane.
	glm::vec3 triNorm = glm::normalize(glm::cross(tri->vec_1 - tri->vec_3, tri->vec_2 - tri->vec_3));
	glm::vec3 objTriCntr_to_tri = tri->vec_3 - triangles[index].center;

	// Here, t is the shortest distance from the center of the circle generated by triangles[index].
	float t = -glm::dot(objTriCntr_to_tri, -triNorm);// / denom;

	// If this distance t is less than the radius of the circle bounding triangles[index], then we know
	// it's worth investigated further.
	if (abs(t) <= triangles[index].radius) {
		// Need to do barycentric interpolation to test if the point is inside the triangle.

		// So think about it like this: we're in 3D to begin with: we're a sphere in space. Next we ask, "what's the closet
		// point from the center of our sphere to the plane that the triangle (tri) is on?" We calculate that, and then ask,
		// "is the distance from the center of our sphere to this point, less than the radius of our sphere?" If it's greater
		// than the radius, we know the triangle (tri) is not close enough to consider a collision with, so we can return false.
		// If instead, the distance from our sphere's center to this point is <= radius, then we can continue on.
		// 
		// From here, we now have two things to consider: 
		//	#1:	We're left with a point on a plane, and a left over amount of radius (sphere radius - distance from center to plane point)
		//	#2:	The triangle (tri) exists on the plane our point's on, and it's bounded by it's edges.
		// 
		// Okay, perfect. So now we just need to test, "is the distance from our point to any given edge <= 'left-over-radius'?"
		// We can do this in much the same way as before when we got the closet point to the triangles (tri) plane from the center
		// of the sphere, only this time, we're using the point on the (tri) plane and the new "plane" given by each edge.
		// 
		// This will tell us if we're a "left-over-radius" distance from an edge, which is enough to consider a collision when we're
		// NOT inside of the triangle. Because if we are inside of the triangle, and for this hypothetical imagine it's a very large
		// triangle, the distance may be very far from an edge. Regardless, we're still inside of the triangle and should consider a 
		// collision. That's where the second test comes in: "are all the distances to the edges, negatives?" If they are, then that
		// simply means we'd need to move backwards to get to each edge. If that's the case, we're inside of the triangle and we can
		// return true.
		// 
		// This is a long-winded explanation for what is essentially a short sphere-triangle intersection test, but it's worth noting
		// because this is the first of it's kind I've made.
		//
		// Btw, pretty sure there's some edge cases like if a tiny sphere is nestled in a corner or something that's not handled here.
		// We could handle it rather simply by just asking if the distance from the point on the plane to a vertex is less than the
		// left over radius. Not too wild, but I don't think we even need to handle it and I'm fighting for my life to optimize this
		// ridiculous set of calculations as is.
		glm::vec3 pointOnPlane = triangles[index].center + triNorm * t;

		glm::vec3 edge13_norm = glm::normalize(glm::cross(triNorm, tri->vec_1 - tri->vec_3));
		glm::vec3 edge21_norm = glm::normalize(glm::cross(triNorm, tri->vec_2 - tri->vec_1));
		glm::vec3 edge32_norm = glm::normalize(glm::cross(triNorm, tri->vec_3 - tri->vec_2));

		glm::vec3 pntOnPln_to_e13 = tri->vec_1 - pointOnPlane;
		glm::vec3 pntOnPln_to_e21 = tri->vec_2 - pointOnPlane;
		glm::vec3 pntOnPln_to_e32 = tri->vec_3 - pointOnPlane;

		float t_e_13 = -glm::dot(pntOnPln_to_e13, -edge13_norm);
		float t_e_21 = -glm::dot(pntOnPln_to_e21, -edge21_norm);
		float t_e_32 = -glm::dot(pntOnPln_to_e32, -edge32_norm);

		float leftOverRadius = triangles[index].radius - abs(t);

		// Returns true if either the point on the plane is close enough to an edge, or it's inside of the triangle. 
		//return (t_e_13 <= 0.f && t_e_21 <= 0.f && t_e_32 <= 0.f);
		//return ((abs(t_e_13) >= 0.f && abs(t_e_13) <= leftOverRadius) || abs(t_e_21) >= 0.f && abs(t_e_21) <= leftOverRadius || abs(t_e_32) >= 0.f && abs(t_e_32) <= leftOverRadius);
		//return ((t_e_13 <= 0.f && t_e_21 <= 0.f && t_e_32 <= 0.f) || ((abs(t_e_13) >= 0.f && abs(t_e_13) <= leftOverRadius) || abs(t_e_21) >= 0.f && abs(t_e_21) <= leftOverRadius || abs(t_e_32) >= 0.f && abs(t_e_32) <= leftOverRadius));


		// From here: we have to decide "do the actual triangles themselves touch?" (which is a little more intense than the sphere -> triangle).
		if (((t_e_13 <= 0.f && t_e_21 <= 0.f && t_e_32 <= 0.f) || ((abs(t_e_13) >= 0.f && abs(t_e_13) <= leftOverRadius) || abs(t_e_21) >= 0.f && abs(t_e_21) <= leftOverRadius || abs(t_e_32) >= 0.f && abs(t_e_32) <= leftOverRadius))) {
			//std::cout << "\tpointOnPlane = (" << pointOnPlane.x << ", " << pointOnPlane.y << ", " << pointOnPlane.z << ")\n";
			//return false;
			
			
			// So 4 distinct cases we care about:
			//	#1:	Co-planar triangles:	They are in the same plane, flat, one on top of the other. We either deal with it or don't. Hard to say.
			//								On one hand: imagine a falling plane or something. On the other: we would def deal with that a different 
			//								way. For now, we can ignore this case and move on.
			//	
			//	#2:	Parallel triangles:		They are on two parallel planes facing each other. They won't interact, so we can discard this.
			// 
			//	#3:	One edge(s) through:	Here, one triangle is piercing the other but only on one of it's edges. This is the first non-trivial
			//								case.
			// 
			//	#4: Two edge(s) through:	Here, one triangle is piercing the other but now two edges of one triangle pass between two edges of
			//								the other triangle. This is a slightly more interesting case to deal with, and is obviously non-trivial.
			// 
			// In both non-trivial cases, we need to check where exactly the intersections between the triangles lie, so we can respond with the
			// appropriate forces in each direction.

			glm::vec3 objTriNorm = glm::cross(triangles[index].vec_1 - triangles[index].vec_3, triangles[index].vec_2 - triangles[index].vec_3);
			float dotBetweenTriangleNorms = dot(objTriNorm, -triNorm);

			// Co-planar or just parallel. For now, we can disregard this.
			if (false){//(dotBetweenTriangleNorms < 0.0f) {
				return false;
			}
			else {
				glm::vec3 objTri_e_13 = triangles[index].vec_1 - triangles[index].vec_3; glm::vec3 tri_e_13 = tri->vec_1 - tri->vec_3;
				glm::vec3 objTri_e_21 = triangles[index].vec_2 - triangles[index].vec_1; glm::vec3 tri_e_21 = tri->vec_2 - tri->vec_1;
				glm::vec3 objTri_e_32 = triangles[index].vec_3 - triangles[index].vec_2; glm::vec3 tri_e_32 = tri->vec_3 - tri->vec_2;

				float denom_objTri_13 = glm::dot(glm::normalize(objTri_e_13), triNorm); float denom_tri_13 = glm::dot(glm::normalize(tri_e_13), objTriNorm);
				float denom_objTri_21 = glm::dot(glm::normalize(objTri_e_21), triNorm);	float denom_tri_21 = glm::dot(glm::normalize(tri_e_21), objTriNorm);
				float denom_objTri_32 = glm::dot(glm::normalize(objTri_e_32), triNorm);	float denom_tri_32 = glm::dot(glm::normalize(tri_e_32), objTriNorm);

				// THIS WORKS!
				int passedBaryTests = 0;
#if 1
				//
				//		--------------------------------
				//			Object piercing tri test
				//		--------------------------------
				//
				*_p11 = pointProjection(tri, tri->vec_1, triNorm, triangles[index].vec_1, objTri_e_13);
				if (_p11->x < -100000.f)
					*_p11 = pointProjection(tri, tri->vec_1, -triNorm, triangles[index].vec_1, objTri_e_13);
				passedBaryTests += (_p11->x >= -100000.f);

				*_p12 = pointProjection(tri, tri->vec_2, triNorm, triangles[index].vec_2, objTri_e_21);
				if (_p12->x < -100000.f)
					*_p12 = pointProjection(tri, tri->vec_2, -triNorm, triangles[index].vec_2, objTri_e_21);
				passedBaryTests += (_p12->x >= -100000.f);

				*_p13 = pointProjection(tri, tri->vec_3, triNorm, triangles[index].vec_3, objTri_e_32);
				if (_p13->x < -100000.f)
					*_p13 = pointProjection(tri, tri->vec_3, -triNorm, triangles[index].vec_3, objTri_e_32);
				passedBaryTests += (_p13->x >= -100000.f);
				
				//
				//		--------------------------------
				//			Tri piercing object test
				//		--------------------------------
				//
				*_p21 = pointProjection(&triangles[index], triangles[index].vec_1, objTriNorm, tri->vec_1, tri_e_13);
				if (_p21->x < -100000.f) 
					*_p21 = pointProjection(&triangles[index], triangles[index].vec_1, -objTriNorm, tri->vec_1, tri_e_13);
				passedBaryTests += (_p21->x >= -100000.f);

				*_p22 = pointProjection(&triangles[index], triangles[index].vec_2, objTriNorm, tri->vec_2, tri_e_21);
				if (_p22->x < -100000.f)
					*_p22 = pointProjection(&triangles[index], triangles[index].vec_2, -objTriNorm, tri->vec_2, tri_e_21);
				passedBaryTests += (_p22->x >= -100000.f);

				*_p23 = pointProjection(&triangles[index], triangles[index].vec_3, objTriNorm, tri->vec_3, tri_e_32);
				if (_p23->x < -100000.f)
					*_p23 = pointProjection(&triangles[index], triangles[index].vec_3, -objTriNorm, tri->vec_3, tri_e_32);
				passedBaryTests += (_p23->x >= -100000.f);

				return (passedBaryTests > 0);
#endif

			}
		}
	}
	return false;
}

glm::vec3 Object::pointProjection(Triangle* triangle, glm::vec3 planePoint0, glm::vec3 planeNormal, glm::vec3 rayStart, glm::vec3 rayDirection) {
	float denom = glm::dot(glm::normalize(rayDirection), planeNormal);

	//if (denom > 0.00000000001f) {
		float t = -(dot(planePoint0 - rayStart, planeNormal) / denom);
		if (t >= 0.0f && t <= glm::length(rayDirection)) {
			glm::vec3 pointOnTri = rayStart - (glm::normalize(rayDirection) * t);
			if (barycentricInterpolation(triangle, pointOnTri)) {
				return pointOnTri;
			}
		}
	//}
	return glm::vec3(-1000000.f);
}

bool Object::pointProjection(Triangle* triangle, glm::vec3 planePoint0, glm::vec3 planeNormal, glm::vec3 rayStart, glm::vec3 rayDirection, glm::vec3* _p) {
	float denom = glm::dot(glm::normalize(rayDirection), planeNormal);
	
	if (denom > 0.00000000001f) {
		float t = -(dot(planePoint0 - rayStart, planeNormal) / denom);
		if (t >= 0.f && t <= abs(glm::length(rayDirection))) {
			glm::vec3 pointOnTri = rayStart - (glm::normalize(rayDirection) * t);
			if (barycentricInterpolation(triangle, pointOnTri)) {
				*_p = pointOnTri;
				return true;
			}
		}
	}
	return false;
}

// Problem is probably this tbh.
bool Object::barycentricInterpolation(Triangle* tri, glm::vec3 p) {
	// Compute vectors        
	glm::vec3 v0 = tri->vec_3 - tri->vec_1;
	glm::vec3 v1 = tri->vec_2 - tri->vec_1;
	glm::vec3 v2 = p - tri->vec_1;

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


void Object::applyForce(Force f) {
	//std::cout << "f.dir = (" << f.dir.x << ", " << f.dir.y << ", " << f.dir.z << ")\n";

	float coefficient_of_friction = 0.8f;

	glm::vec3 forcePos_to_center = objPos - f.pos;
	float theta_forcePos_to_center_Dot_forceDir = 0.f;

	glm::vec3 forcePos_to_center_X_forceDir = glm::cross(glm::normalize(forcePos_to_center), glm::normalize(f.dir));
	theta_forcePos_to_center_Dot_forceDir = acos(glm::dot(glm::normalize(forcePos_to_center), glm::normalize(f.dir)));

	velocity += f.dir * deltaTime * cos(theta_forcePos_to_center_Dot_forceDir);// glm::normalize(f.dir)* linearAcceleration* deltaTime;
	//rotationalVelocity = (glm::length(f.dir) * sin(theta_forcePos_to_center_Dot_forceDir)) / (glm::length(forcePos_to_center));
	//rotationalAxis = forcePos_to_center_X_forceDir;

	//std::cout << "theta_forcePos_to_center_Dot_forceDir = " << theta_forcePos_to_center_Dot_forceDir << "\n";

	//float linearAcceleration;
	//float angularAcceleration;
	//
	//if (glm::degrees(theta_forcePos_to_center_Dot_forceDir) > 0.f && glm::degrees(theta_forcePos_to_center_Dot_forceDir) < 180.f) {
	//
	//	linearAcceleration = glm::length(f.dir) * cos(theta_forcePos_to_center_Dot_forceDir);
	//	angularAcceleration = glm::length(f.dir) * sin(theta_forcePos_to_center_Dot_forceDir);
	//}
	//else {
	//	linearAcceleration = glm::length(f.dir);
	//	angularAcceleration = 0.f;
	//}

	//std::cout << "forcePos_to_center_X_forceDir = (" << forcePos_to_center_X_forceDir.x << ", " << forcePos_to_center_X_forceDir.y << ", " << forcePos_to_center_X_forceDir.z << ")\n";
	//std::cout << "linearAcceleration = " << linearAcceleration << "\n";
	//std::cout << "linearAcceleration * deltaTime = " << linearAcceleration * deltaTime << "\n";

	//std::cout << "\nvelocity = (" << velocity.x << ", " << velocity.y << ", " << velocity.z << ")\n";
	

	// \/ * cos(theta_forcePos_to_center_Dot_forceDir)
	//std::cout << "velocity = (" << velocity.x << ", " << velocity.y << ", " << velocity.z << ")\n";
	//rotationalAxis = forcePos_to_center_X_forceDir;
	//rotationalVelocity += angularAcceleration * deltaTime * glm::length(forcePos_to_center);

	//std::cout << "forcePos_to_center = (" << forcePos_to_center.x << ", " << forcePos_to_center.y << ", " << forcePos_to_center.z << ")\n";
	//std::cout << "forcePos_to_center_X_forceDir = (" << forcePos_to_center_X_forceDir.x << ", " << forcePos_to_center_X_forceDir.y << ", " << forcePos_to_center_X_forceDir.z << ")\n";
	//
	//std::cout << "theta_forcePos_to_center_Dot_forceDir = " << theta_forcePos_to_center_Dot_forceDir << "\tsin(theta) = " << sin(glm::radians(theta_forcePos_to_center_Dot_forceDir)) << "\tcos(theta) = " << cos(glm::radians(theta_forcePos_to_center_Dot_forceDir)) << "\n";
}

void Object::reflectAbout(Force f) {

	float bounciness = 1.2f;
	float drag = -0.01f;
	
	glm::vec3 normal = glm::normalize(f.dir);
	float angle = acos(glm::dot(normal, normalize(velocity)));

	// Returning out if the angle is less than 90 degrees, because that means we struck the back face of
	// the mesh, which we don't want to reflect off of.
	glm::vec3 returnVec = isPointInsideModel(f.pos);
	objPos += returnVec;
	if (glm::degrees(angle) < 90.f) {
		return;
	}

	glm::vec3 normal_cross_velocity = glm::cross(normal, velocity);

	float theta = acos(glm::dot(normal, -normalize(velocity)));
	glm::vec3 axis = glm::cross(normal, -normalize(velocity));

	rotationalVelocity = -glm::radians((glm::length(velocity - velocity * normal) / length(objPos - f.pos)) * sin(theta));
	rotationalAxis = axis;

	//glm::mat4 rotationMatrix = glm::mat4(1.f);
	//glm::rotate(rotationMatrix, angle, normal_cross_velocity);

	glm::vec3 reflect = glm::vec3(rotationMatrix * glm::vec4(normal, 1.f)) * glm::length(velocity * normal);// *cos(theta);
	velocity += (reflect * bounciness) + ((velocity + reflect) * drag);

	// Just to ensure objects don't sink through walls when they get pinched


	// NEED TO MAKE WAY BETTER VERSION OF THIS ONE!
	//if (glm::length(objPos - f.pos) - objRadius < 0.f) {
	//	objPos -= glm::normalize(objPos - f.pos) * (glm::length(objPos - f.pos) - objRadius);
	//}

	//glm::vec3 reflect = normal * glm::length(velocity) * 1.8f;

	//std::cout << "reflect = (" << reflect.x << ", " << reflect.y << ", " << reflect.z << ")\n";


	//glm::vec3 forwardDir = -glm::normalize(glm::cross(glm::cross(velocity, normal), normal) * velocity) * glm::length(velocity);

	//glm::vec3 conservation_of_energy_constant = 0.5f * mass * (velocity * velocity);
	//velocity = sqrt((conservation_of_energy_constant * 2.f) / mass);

	//velocity = normal * glm::length(velocity) * 1.5f + velocity;// -velocity * percent_of_velocity_exchange;
	////applyForce(Force(f.pos, f.dir));
	//glm::vec3 dragVelocity = (normal * glm::length(velocity) + velocity) * -.01f;
	//glm::vec3 dragVelocity = glm::vec3(0.f);

	//std::cout << "forwardDir = (" << forwardDir.x << ", " << forwardDir.y << ", " << forwardDir.z << ")\n";
	//if (glm::length(forwardDir) >= 0.00000001f) 
	//	velocity += (forwardDir) * 0.01f;
	////applyForce(Force(f.pos, dragVelocity));
	//velocity += dragVelocity;

	//float angle = acos(glm::dot(normal , -normalize(velocity)));
	//float velocity_magnitude = glm::length(velocity);
	//
	//glm::vec3 velocity_X_normal = glm::cross(normal, -normalize(velocity));

	//if (abs(glm::degrees(angle)) < 179.9f && abs(glm::degrees(angle)) > 0.1f) {
	//	//angle *= !(ceil(abs(glm::degrees(angle))) < 179.f && floor(abs(glm::degrees(angle))) > 1.f);
	//
	//	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.f), -angle, velocity_X_normal);
	//
	//	glm::vec4 scaled_normal = glm::vec4(normal * velocity_magnitude, 1);
	//
	//
	//	//std::cout << "\nvelocity mag = (" << velocity_magnitude << "\n";
	//	//std::cout << "Old velocity = (" << velocity.x << ", " << velocity.y << ", " << velocity.z << "), angle = " << glm::degrees(angle) << "\n";
	//
	//	velocity = glm::vec3(rotationMatrix * scaled_normal) * percent_of_velocity_exchange;
	//
	//	//std::cout << "New velocity = (" << velocity.x << ", " << velocity.y << ", " << velocity.z << "), angle = " << glm::degrees(angle) << "\n";
	//}
	//else if (abs(glm::degrees(angle)) == 0.f) {
	//}
}


void Object::getNextPos() {

	float terminal_velocity = 100.f;
	if (glm::length(velocity) > terminal_velocity)
		velocity = glm::normalize(velocity) * terminal_velocity;

	objPos += velocity * deltaTime;

	glm::mat4 newModel = glm::translate(glm::mat4(1.f), objPos);
	if (glm::length(rotationalAxis) > 0.f && abs(rotationalVelocity) > 0.f)
		newModel = glm::rotate(newModel, rotationalVelocity, rotationalAxis);

	//glm::mat3 rotationMatrix = (1.0f / objScale) * glm::mat3(model);
	//newModel = newModel * glm::mat4(rotationMatrix);

	model = glm::scale(newModel, glm::vec3(objScale));
}


glm::vec3 Object::isPointInsideModel(glm::vec3 p) {

	float t = 10000.f;
	glm::vec3 returnVec = glm::vec3(0.f);

	for (Triangle tri : triangles) {
		tri.genCircle();
		glm::vec3 triNorm = glm::cross(tri.vec_1 - tri.vec_3, tri.vec_2 - tri.vec_3);

		// Normal of triangle, A.K.A., normal of plane.
		glm::vec3 triCenter = tri.vec_3 - p;

		// Here, t is the shortest distance from the center of the circle generated by triangles[index].
		if (-glm::dot(triCenter, -triNorm) < t) {
			t = -glm::dot(triCenter, -triNorm);
			returnVec = -triNorm * t;
		}
	

		//if (t <= 0.1f)
		//	returnVec = glm::vec3(0.f);// -triNorm * t;
	}

	return returnVec;
}

