#include "Object.h"

// Object classes constructor. Just sets some member variables and updates the glm::mat4 model of the object.
Object::Object(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) {
	window = _window;
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

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

// Creates a new shader program and binds the adds the appropriate information into the GPU's memory so it can draw the
// object.
void Object::setVBOandEBO(std::vector <Vertex>& _vertices, std::vector <GLuint>& _indices, std::string msg) {
	// Generates Shader object using shaders object.vert and object.frag
	delete shaderProgram;
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
	delete shaderProgram;
}