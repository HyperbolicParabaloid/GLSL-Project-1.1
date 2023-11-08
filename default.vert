#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;
// Normals per vertex
layout (location = 3) in vec3 aNormal;



// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;
// Outputting normals to frag shader
out vec3 Normal;
// Current position of vertex, used to calculate direction of flight
out vec3 crntPos;

// Controls the scale of the vertices
uniform float scale;
uniform float rotationDegree;

// Controls the scale of the vertices
uniform mat4 model;
//uniform mat4 projection;
uniform mat4 camMatrix;


void main()
{
	crntPos = vec3(model * vec4(aPos, 1.f));

	// Outputs the positions/coordinates of all vertices

	gl_Position = camMatrix * model * vec4(crntPos, 1.f);

	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = aTex;
	// Assigns normal

	mat3 normalMatrix = mat3(model);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);
	Normal = normalize(aNormal * normalMatrix);

	//Normal = aNormal;

	//Normal = aNormal;//vec3(model * vec4(aNormal, 1.f));
}