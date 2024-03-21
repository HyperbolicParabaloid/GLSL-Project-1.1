#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<stdio.h>
#include<iostream>
#include<vector>

// In the future I plan on using these to hold information about each vertex instead of just
// packing everything into a massive array.
struct Vertex {
	glm::vec3 pos = glm::vec3(0.f);	glm::vec3 norm;	glm::vec4 color = glm::vec4(0.f);	glm::vec2 texCoord = glm::vec2(0.f);
};
struct VertexUI {
	glm::vec3 pos = glm::vec3(0.f);	glm::vec3 norm;	glm::vec4 color = glm::vec4(0.f);	glm::uvec2 texCoord = glm::uvec2(0);
};
struct Triangle {
	Triangle(Vertex* _v1, Vertex* _v2, Vertex* _v3, glm::mat4* _model) {
		model = _model;
		v1 = _v1; v2 = _v2; v3 = _v3;
		//genCircle();
	}

	// Function for calculating the current world-space coordinates of the triangle in question
	// and generating the center position in world space along with a radius for the bounding
	// sphere.
	void genCircle() {
		vec_1 = glm::vec3(*model * (glm::vec4(v1->pos, 1.f)));
		vec_2 = glm::vec3(*model * (glm::vec4(v2->pos, 1.f)));
		vec_3 = glm::vec3(*model * (glm::vec4(v3->pos, 1.f)));

		center = (vec_1 + vec_2 + vec_3) / 3.f;
		radius = std::max({ glm::length(vec_1 - center), glm::length(vec_2 - center), glm::length(vec_3 - center) });
	}
	// Pointers to the stored vertices.
	Vertex* v1;
	Vertex* v2;
	Vertex* v3;

	// World space coordiantes of the vertices.
	glm::vec3 vec_1;
	glm::vec3 vec_2;
	glm::vec3 vec_3;

	// Model for calculating the new world-space coordinates of the vertices.
	glm::mat4* model;
	glm::vec3 center;
	float radius;
};

struct Force {
	Force(glm::vec3 _pos, glm::vec3 _dir) {
		pos = _pos;
		dir = _dir;
	}
	glm::vec3 pos;
	glm::vec3 dir;
};

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(GLfloat* vertices, GLsizeiptr size);
	VBO(std::vector<Vertex>& vertices);
	VBO(std::vector<VertexUI>& verticesUI);

	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

#endif