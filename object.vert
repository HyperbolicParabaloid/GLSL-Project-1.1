#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals per vertex
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec4 aColor;
// Texture coords
layout (location = 3) in vec2 aTex;



// Outputs the color for the Fragment Shader
out vec4 color;
// Outputting normals to frag shader
out vec3 Normal;
// Current position of vertex, used to calculate direction of flight
out vec3 crntPos;
// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

// Controls the scale of the vertices
uniform mat4 model;
//uniform mat4 projection;
uniform mat4 camMatrix;
// For time
uniform float time;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float rand(float n){return fract(sin(n) * 43758.5453123);}

float noise(float p){
	float fl = floor(p);
  float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}
	
float noise(vec2 n) {
	const vec2 d = vec2(0.0, 1.0);
  vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
	return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}

void main()
{
	//vec4 scaledPos = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
	crntPos = vec3(model * vec4(aPos, 1.f));

	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(crntPos, 1.f);

	texCoord = aTex;
	
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;

	// This will rotate the surface normals along with the model.
	Normal = transpose(inverse(mat3(model))) * aNormal;

}