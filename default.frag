#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;
// Normal
in vec3 Normal;
// Current position
in vec3 crntPos;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

// Doing Phong shading
void main()
{
	// To raise the min light level we add ambient lighting
	float ambient = 0.2f;

	// Make sure normal is, "normalized"
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);

	// A dot product of two vectors returns the cos of the angle between them. Getting this, we can specify how intense
	// the light should be based on the angle, with flat being of course the brightest.
	float diffuse = max(dot(normal, lightDirection), 0.f);

	float specularLight = 0.5f;
	vec3 viewDir = normalize(camPos - crntPos);
	vec3 reflectionDir = reflect(-lightDirection, normal);
	float specularAmount = pow(max(dot(viewDir, reflectionDir), 0.f), 8);
	float specular = specularLight * specularAmount;

	// We can now scale the overall color by this diffuse amount, to get the final color.
	// Adding ambient to diffuse before scaling light makes this work.
	FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
}