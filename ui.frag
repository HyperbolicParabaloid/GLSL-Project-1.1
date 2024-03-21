#version 400 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec4 color;
// Normal
in vec3 Normal;
// Current position
in vec3 crntPos;
// Inputs the texture coordinates from the Vertex Shader
flat in uvec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform int useTex;
uniform int useTexSpec;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
// For time
uniform float time;
uniform uvec2 pixels;

void main()
{
	vec2 uv = ((vec2(crntPos) + 1.f) / 2.f) * 8.f;
	ivec2 coord = ivec2((((crntPos.x + 1.f) / 2.f) * 8), (((-crntPos.y + 1.f) / 2.f) * 8));

	uint num = uint(texCoord.x);

	int bit = 0;
	int fragNum = int(coord.y * 8 + coord.x);

	if (fragNum >= 32)
		num = uint(texCoord.y);

	bit = int(bitfieldExtract(num, fragNum % 32, 1));


	// Adding Letters
	if (bit == 1)
		FragColor = color;
	else
		FragColor = vec4(0.f);


	//Adds in the cell boarders so you can see what you're doing a little better.
	if (fract(uv.x) > 0.9 || fract(uv.x) < 0.1)
		FragColor = vec4(1);
	else if (fract(uv.y) > 0.9 || fract(uv.y) < 0.1)
		FragColor = vec4(1);
	return;
}