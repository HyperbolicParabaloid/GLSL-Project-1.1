#include "Object.h"


// Object classes constructor. Just sets some member variables and updates the glm::mat4 model of the object.
Object::Object(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) {
	window = _window;
	doNormalArrows = false;
	objPos = _objPos;
	objScale = _objScale;
	color = _color;
	model = glm::translate(glm::mat4(1.f), objPos);
	model = glm::scale(model, glm::vec3(objScale));
	camera = _camera;
	textures = _textures;
}

// Rotates the object about a given acis by a set angle in degrees.
void Object::rotate(float rotationDegreeAngle, glm::vec3 axisOfRotation) {
	model = glm::rotate(model, glm::radians(rotationDegreeAngle), axisOfRotation);
}

// Draws the object to the screen and sets the appropriate information in the Shader object about position, and lighting.
void Object::draw(glm::vec3 _lightPos, glm::vec4 _lightColor) {
	// Activating the shader program and binding the VAO so OpenGL knows what we're trying to do.
	shaderProgram->Activate();
	VAO.Bind();

	// Assigning all relvant info to the shader.
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "camPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
	camera->camMatrixForShader(*shaderProgram, "camMatrix");
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), _lightPos.x, _lightPos.y, _lightPos.z);
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), _lightColor.x, _lightColor.y, _lightColor.z, _lightColor.w);
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "time"), glfwGetTime());

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


}

// Creates a new shader program and binds the adds the appropriate information into the GPU's memory so it can draw the
// object.
void Object::setVBOandEBO(std::vector <Vertex>& _vertices, std::vector <GLuint>& _indices, std::string msg) {
	// Generates Shader object using shaders object.vert and object.frag
	delete shaderProgram;
	if (msg == "Plane") {
		shaderProgram = new Shader("wave.vert", "wave.frag");
		glUniform1f(glGetUniformLocation(shaderProgram->ID, "offset"), 2.f / (sqrt(_vertices.size()) - 1.f));
	}
	else
		shaderProgram = new Shader("object.vert", "object.frag");
	shaderProgram->Activate();

	vertices = _vertices;
	indices = _indices;

	//std::cout << "doNormalArrows = " << doNormalArrows << " for " << msg << "\n";
	if (doNormalArrows == true) {
		//std::cout << "Drawing the arrows for " << msg <<  "\n";
		int vNum = 1;
		int maxSize = vertices.size();
		for (int vv = 0; vv < maxSize; vv++) {
			glm::vec3 conePos = vertices[vv].pos;
			float coneScale = 0.02f;
			int coneLevel = 4;
			float coneBottomRadius = 1.f;
			float coneTopRadius = 0.f;
			glm::vec3 conePointPos = glm::vec3(0.f, 5.f, 0.f);
			glm::vec3 conePointingAt = vertices[vv].norm;
			bool coneIsSmooth = true;
			glm::vec4 coneColor = vertices[vv].color;
			int coneStartingIndex = indices.size();

			//std::cout << "Drawing arrows #" << vNum << " at pos (" << conePos.x << ", " << conePos.y << ", " << conePos.z << ")\n";
			//vNum++;

			//std::cout << "Drawing a " << msg << "\n";
			Arrow newArrow(conePos, coneScale, coneLevel, coneBottomRadius, coneTopRadius, conePointPos, conePointingAt, coneIsSmooth, coneColor, coneStartingIndex);
			std::vector <Vertex> coneVerts = newArrow.getVerts();
			std::vector <GLuint> coneInds = newArrow.getInds();

			//std::cout << "vertices.size() BEFORE = " << vertices.size() << "\n";
			vertices.insert(vertices.end(), coneVerts.begin(), coneVerts.end());
			indices.insert(indices.end(), coneInds.begin(), coneInds.end());
		}
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

void Object::toggleNormalArrows() {
	doNormalArrows = !doNormalArrows;
}
