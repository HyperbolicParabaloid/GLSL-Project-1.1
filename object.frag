#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec4 color;
// Normal
in vec3 Normal;
// Current position
in vec3 crntPos;

// Gets the Texture Unit from the main function
//uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

// Doing Phong shading
void main()
{
	// ambient lighting
	float ambient = 0.20f;
	
	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;
	
	// outputs final color
	FragColor = color * lightColor * (diffuse + ambient + specular);
	
}