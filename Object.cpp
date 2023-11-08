#include "Object.h"
Object::Object(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 _color, Camera* _camera) {
	window = _window;
	objPos = _objPos;
	objScale = _objScale;
	color = _color;
	model = glm::translate(glm::mat4(1.f), objPos);
	camera = _camera;
}

Object::Object(GLFWwindow* _window) {
	window = _window;
	objPos = glm::vec3(1.f);
	objScale = 1.f;

	/*
	VBO verts;
	EBO indicies;
	*/
};

void Object::rotate(float rotationDegreeAngle, glm::vec3 axisOfRotation) {
	model = glm::rotate(model, glm::radians(rotationDegreeAngle), axisOfRotation);
}

void Object::draw(glm::vec3 _lightPos, glm::vec4 _lightColor) {
	// Tell OpenGL which Shader Program we want to use
	shaderProgram->Activate();

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), _lightPos.x, _lightPos.y, _lightPos.z);
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "camPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), _lightColor.x, _lightColor.y, _lightColor.z, _lightColor.w);
	camera->camMatrixForShader(*shaderProgram, "camMatrix");

	// Bind the VAO so OpenGL knows to use it
	objVAO.Bind();

	// Draw primitives, number of indices, datatype of indices, index of indices
	glDrawElements(GL_TRIANGLES, indSize / sizeof(int), GL_UNSIGNED_INT, 0);

}

void Object::setVBOandEBO(GLfloat* _vertices, int _vertSize, GLuint* _indices, int _indSize) {
	// Generates Shader object using shaders object.vert and object.frag
	shaderProgram = new Shader("object.vert", "object.frag");

	vertSize = _vertSize;
	indSize = _indSize;

	// Binding VAO
	objVAO.Bind();

	// Setting VBO and EBO
	// Generates Vertex Buffer Object and links it to vertices
	objVBO = new VBO(_vertices, vertSize);
	// Generates Element Buffer Object and links it to indices
	objEBO = new EBO(_indices, indSize);

	// Links VBO attributes such as coordinates and colors to VAO
	objVAO.LinkAttrib(*objVBO, 0, 3, GL_FLOAT, 10 * sizeof(float), (void*)0);
	objVAO.LinkAttrib(*objVBO, 1, 3, GL_FLOAT, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	objVAO.LinkAttrib(*objVBO, 2, 4, GL_FLOAT, 10 * sizeof(float), (void*)(6 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	objVAO.Unbind();
	objVBO->Unbind();
	objEBO->Unbind();
}

Object::~Object() {
	delete(shaderProgram);
	objVAO.Delete();
	objVBO->Delete();
	objEBO->Delete();
	delete(objVBO);
	delete(objEBO);
}