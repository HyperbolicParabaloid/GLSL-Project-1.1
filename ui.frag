#version 400 core

// Outputs colors in RGBA
out vec4 FragColor;


// Outputs the color for the Fragment Shader
in vec4 color;
// Outputting normals to frag shader
in vec3 texCoord;
// Current position of vertex, used to calculate direction of flight
in vec3 crntPos;
// Outputs the texture coordinates to the fragment shader
flat in uvec2 Letter;

// Gets the Texture Unit from the main function
uniform sampler2D diffuse0;
uniform sampler2D diffuse1;
uniform int useTex;
uniform int useTexSpec;
uniform vec4 backgroundColor;
// For time
uniform float time;

void main()
{
	vec2 uv = ((vec2(texCoord) + 1.f) / 2.f) * 8.f;
	ivec2 coord = ivec2((((texCoord.x + 1.f) / 2.f) * 8), (((-texCoord.y + 1.f) / 2.f) * 8));

	int bit = 0;
	int fragNum = int(coord.y * 8 + coord.x);

	uint num = uint(Letter.x);
	if (fragNum >= 32)
		num = uint(Letter.y);

	bit = int(bitfieldExtract(num, fragNum % 32, 1));


	// Adding Letters
	if (bit == 1)
		if (useTex > 0)
			FragColor = texture(diffuse0, (texCoord.xy + 1.f) / 2.f);
		else
			FragColor = color;
	else
		if (useTex > 1)
			FragColor = backgroundColor * texture(diffuse1, (texCoord.xy + 1.f) / 2.f);
		else
			FragColor = backgroundColor;//vec4(0.f);


	//Adds in the cell boarders so you can see what you're doing a little better.
	//if (fract(uv.x) > 0.9 || fract(uv.x) < 0.1)
	//	FragColor = vec4(1);
	//else if (fract(uv.y) > 0.9 || fract(uv.y) < 0.1)
	//	FragColor = vec4(1);
	return;
}