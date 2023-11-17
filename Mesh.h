#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>	
#include<vector>

#include"shaderClass.h"
#include"Texture.h"
#include"VAO.h"
#include"EBO.h"
#include"Camera.h"

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	VAO VAO;
	Mesh(std::vector <Vertex>& _vertices, std::vector <GLuint>& _indices, std::vector <Texture>& _textures);
	Mesh();
	void draw(Camera& _camera, Shader& _shader);
};

#endif