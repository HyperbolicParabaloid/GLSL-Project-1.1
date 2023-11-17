#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<vector>

// In the future I plan on using these to hold information about each vertex instead of just
// packing everything into a massive array.
struct Vertex {
	glm::vec3 pos;	glm::vec3 norm;	glm::vec4 color;	glm::vec2 texCoord;
	//Vertex() {
	//	pos = glm::vec3(0.f);
	//	norm = glm::vec3(0.f);
	//	color = glm::vec4(0.f);
	//	texCoord = glm::vec2(0.f);
	//}
	//Vertex(glm::vec3 _pos, glm::vec3 _norm, glm::vec4 _color, glm::vec2 _texCoord) {
	//	pos = _pos;
	//	norm = _norm;
	//	color = _color;
	//	texCoord = _texCoord;
	//}
};
struct Triangle {
	Vertex verts[3];
	Triangle(Vertex _v0, Vertex _v1, Vertex _v2) {
		verts[0] = _v0;
		verts[1] = _v1;
		verts[2] = _v2;
	}
};

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(GLfloat* vertices, GLsizeiptr size);
	VBO(std::vector<Vertex>& vertices);

	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

#endif