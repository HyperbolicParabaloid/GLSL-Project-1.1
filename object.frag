#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec4 color;
// Normal
in vec3 Normal;
// Current position
in vec3 crntPos;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform int useTex;
uniform int useTexSpec;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

// Point light, illuminates the enviorment like a candle. There's a "point"
// of light from which the intensity fades with the inverse square of the
// distance. We'll use a more complicated quadratic instead of an inverse
// square tho.
//
// Basically this is light from a candle or something of that nature.
vec4 pointLight() {
	// We need the distance from the light to our current pos. Ez.
	vec3 lightVec = lightPos - crntPos;
	float dist = length(lightVec);
	float a = 0.02;	// Quadratic term:	controls how fast the light dies out
	float b = 0.01f;	// Linear term:		controls how far the light reaches
	float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.2f;
	
	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	// outputs final color
	if (useTex == 0) {
		FragColor = color * (diffuse * intensity + ambient + specular * intensity * lightColor);
	}
	else {
		FragColor = texture(diffuse0, texCoord) * (diffuse * intensity + ambient + specular * intensity) * lightColor;
		if (useTex == 0)
			FragColor = texture(diffuse0, texCoord) * (diffuse * intensity + ambient + specular * intensity) * lightColor;
		else
			FragColor = texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + (specular * texture(specular0, texCoord).r * intensity) * lightColor;

	}
	return FragColor;
}


// Directional light is like light from the sun. The intensity doesn't fade with distance,
// and the direction of the light is assumed to be the same wherever you're standing.
//
// This is what you'd want to use for something like the sun for example.
vec4 directionalLight() {
	// ambient lighting
	float ambient = 0.2f;
	
	// Position of the light
	vec3 lightVec = vec3(0.0, 1.0f, 5.0f);

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	// outputs final color
	if (useTex == 0) {
		FragColor = color * (diffuse + ambient + specular * lightColor);
	}
	else {
		FragColor = texture(diffuse0, texCoord) * (diffuse + ambient + specular) * lightColor;
		if (useTex == 0)
			FragColor = texture(diffuse0, texCoord) * (diffuse + ambient + specular) * lightColor;
		else
			FragColor = texture(diffuse0, texCoord) * (diffuse + ambient) + (specular * texture(specular0, texCoord).r) * lightColor;

	}
	return FragColor;
}

// Spot light is like that of a flashlight. We usually specify at least an outer and inner cone
// of the light to make the gradient less harsh and more realistic. 
vec4 spotLight() {
	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

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
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
	
	// outputs final color
	if (useTex == 0) {
		FragColor = color * lightColor * (diffuse * intensity + ambient + specular * intensity);
	}
	else {
		FragColor = texture(diffuse0, texCoord) * lightColor * (diffuse * intensity + ambient + specular * intensity);
		if (useTex == 0)
			FragColor = texture(diffuse0, texCoord) * lightColor * (diffuse * intensity + ambient + specular * intensity);
		else
			FragColor = texture(diffuse0, texCoord) * lightColor * (diffuse * intensity + ambient) + (specular * texture(specular0, texCoord).r * intensity);

	}
	return FragColor;
}

// Doing Phong shading
void main()
{
	vec4 pntLgt = pointLight();
	vec4 drtLgt = directionalLight();
	vec4 sptLgt = spotLight();
	//FragColor = mix(mix(drtLgt, pntLgt, pntLgt), sptLgt, sptLgt);
	//FragColor = mix(drtLgt, pntLgt, pntLgt);
	FragColor = drtLgt;
}