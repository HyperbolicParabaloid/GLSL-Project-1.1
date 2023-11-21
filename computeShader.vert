#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 Pos;
// Normals per vertex
layout (location = 1) in vec3 Normal;
// Colors
layout (location = 2) in vec4 Color;
// Texture coords
layout (location = 3) in vec2 Tex;



vec3 aPos;

vec3 aNormal;

vec4 aColor;

vec2 aTex;

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
	aPos = vec3(Pos.x, Pos.y + 2, Pos.z);
	aNormal = Normal;
	aColor = vec4(1);
	aTex = Tex;
}