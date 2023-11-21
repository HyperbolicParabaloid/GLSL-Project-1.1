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
// For the level of the objects tesselation
uniform float offset;

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

float applyNoise(vec3 p) {
	float newY = (noise(p.xz + time) * 2.f) + (noise((p.xz + time) * 1.2) * 1/8) + (noise((p.xz + time) * 2) * 1/16);
	return newY / 10.f;
}

vec3 calculateNoiseWave() {
	//// TL:DR, aPos.xy: (-1, -1) => (1, 1)
	//if ((aPos.x >= 0.f && aPos.z < 0.f) || (aPos.x < 0.f && aPos.z >= 0)) {
	//
	//}

	/*
	Pos1	Pos2	Pos3

	Pos4	aPos	Pos5

	Pos6	Pos7	Pos8
	*/
	//vec3 Pos1 = vec3(aPos.x - offset, aPos.y,	aPos.z - offset	);	Pos1.y = applyNoise(Pos1);
	vec3 Pos2 = vec3(aPos.x			, aPos.y,	aPos.z - offset );	Pos2.y = applyNoise(Pos2);
	//vec3 Pos3 = vec3(aPos.x	+ offset, aPos.y,	aPos.z - offset );	Pos3.y = applyNoise(Pos3);

	vec3 Pos4 = vec3(aPos.x - offset, aPos.y,	aPos.z			);	Pos4.y = applyNoise(Pos4);
	vec3 Pos5 = vec3(aPos.x	+ offset, aPos.y,	aPos.z			);	Pos5.y = applyNoise(Pos5);

	//vec3 Pos6 = vec3(aPos.x - offset, aPos.y,	aPos.z + offset	);	Pos6.y = applyNoise(Pos6);
	vec3 Pos7 = vec3(aPos.x			, aPos.y,	aPos.z + offset );	Pos7.y = applyNoise(Pos7);
	//vec3 Pos8 = vec3(aPos.x	+ offset, aPos.y,	aPos.z + offset );	Pos8.y = applyNoise(Pos8);

	vec3 newNormal = vec3(0);
	newNormal = cross(Pos2 - aPos, Pos4 - aPos);
	//newNormal += cross(Pos5 - aPos, Pos2 - aPos);
	//newNormal += cross(Pos7 - aPos, Pos5 - aPos);
	//newNormal += cross(Pos4 - aPos, Pos7 - aPos);

	return newNormal;
}



void main()
{
	crntPos = aPos;
	crntPos.y = applyNoise(crntPos);
	//float newY = (noise(aPos.xz + time) * 2.f) + (noise((aPos.xz + time) * 1.2) * 1/8) + (noise((aPos.xz + time) * 2) * 1/16);
	//crntPos.y += newY / 10.f;
	crntPos = vec3(model * vec4(crntPos, 1.f));

	//crntPos = vec3(model * vec4(aPos, 1.f));

	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(crntPos, 1.f);

	texCoord = aTex;
	color = aColor;
	
	//color = mix(aColor, vec4(0.f, 0.5, 1.f, 1.f), newY);

	// Assigns the colors from the Vertex Data to "color"
	//color = clamp(aColor * newY, aColor * 0.7, vec4(1));//clamp(aColor * crntPos.y, vec4(0.1, 0.1, 0.1, 1), vec4(1));
	
	// This will rotate the surface normals along with the model
	Normal = transpose(inverse(mat3(model))) * aNormal;

}