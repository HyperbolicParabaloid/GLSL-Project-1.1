#version 400 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals per vertex
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec4 aColor;
// Texture coords
layout (location = 3) in uvec2 aTex;



// Outputs the color for the Fragment Shader
out vec4 color;
// Outputting normals to frag shader
out vec3 Normal;
// Current position of vertex, used to calculate direction of flight
out vec3 crntPos;
// Outputs the texture coordinates to the fragment shader
flat out uvec2 texCoord;

// Controls the scale of the vertices
uniform mat4 model;
//uniform mat4 projection;
uniform mat4 camMatrix;
// For time
uniform float time;
// Starting position of object
uniform vec3 startPos;


void main()
{
	gl_Position = model * vec4(aPos, 1.f);

	crntPos = aPos;
	color = aColor;
	Normal = aNormal;
	texCoord = aTex;
}