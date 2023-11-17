#include "Mesh.h"


Mesh::Mesh(std::vector <Vertex>& _vertices, std::vector <GLuint>& _indices, std::vector <Texture>& _textures) {
	//std::cout << "Setting it up in mesh\n";
	//for (int ii = 0; ii < _vertices.size(); ii++) {
	//	std::cout << "(" << _vertices[ii].pos.x << ", " << _vertices[ii].pos.y << ", " << _vertices[ii].pos.z << ")\n";
	//}
	//std::cout << "\n\n";
	//for (int ii = 0; ii < _vertices.size(); ii+=3) {
	//	std::cout << "(" << _indices[ii + 0] << ", " << _indices[ii + 1] << ", " << _indices[ii + 2] << ")\n";
	//}


	vertices = _vertices;
	indices = _indices;
	textures = _textures;

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

Mesh::Mesh() {

}

void Mesh::draw(Camera& _camera, Shader& _shader) {
	//_shader.Activate();
	VAO.Bind();

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].texUnit(_shader, (type + num).c_str(), i);
		textures[i].Bind();
	}
	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(_shader.ID, "camPos"), _camera.cameraPos.x, _camera.cameraPos.y, _camera.cameraPos.z);
	_camera.camMatrixForShader(_shader, "camMatrix");

	//std::cout << "DRAWING " << vertices.size() << "\n";

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}