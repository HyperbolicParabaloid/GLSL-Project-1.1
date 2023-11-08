#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals per vertex
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec4 aColor;



// Outputs the color for the Fragment Shader
out vec4 color;
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
	
	gl_Position = camMatrix * vec4(crntPos, 1.f);
	
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	
	// Assigns normal
	//mat3 normalMatrix = mat3(camMatrix * model);
	//normalMatrix = inverse(normalMatrix);
	//normalMatrix = transpose(normalMatrix);
	//Normal = normalize(aNormal * normalMatrix);
	
	Normal = transpose(inverse(mat3(model))) * aNormal;		
}