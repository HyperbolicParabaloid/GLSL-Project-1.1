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
// For time
uniform float time;

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


// Some noise and shnitzel
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

// Doing Phong shading
void main()
{
	vec4 pntLgt = pointLight();
	vec4 drtLgt = directionalLight();
	vec4 sptLgt = spotLight();
	//FragColor = mix(mix(drtLgt, pntLgt, pntLgt), sptLgt, sptLgt);
	//FragColor = mix(drtLgt, pntLgt, pntLgt);
	//FragColor = pntLgt;
	//return;

	vec2 uv = texCoord;
	
	vec4 noiseColor = pntLgt;
	float n = noise(uv + time);
	//float n = noise(uv);

	//if (int(uv.y) % 2 == 0)
	//	uv.x += 0.5;

	// Adds in the cell boarders so you can see what you're doing a little better.
	//if (fract(uv.x) > 0.99 || fract(uv.x) < 0.01)
	//	FragColor = vec4(1);
	//else if (fract(uv.y) > 0.99 || fract(uv.y) < 0.01)
	//	FragColor = vec4(1);
	//else
		FragColor = mix(noiseColor, vec4(1), n / 2);

	vec2 cPos = vec2(int(uv.x), int(uv.y));
	
	// The offset the loop will search around it's current cell block.
	// A.K.A., a search distance of 1 would mean the loop would look at
	// 1 additional cell block in all 4 directions.
	//
	// Generally doesn't need to be all that large unless the circle radius
	// is giant.
	int searchDist = 1;
	// Maximum and minimum time it will take a given circle to reach it's
	// maximum radius as it expands.
	float maxCrclTm = 0.5, minCrclTm = 0.25;//0.5;
	float maxRadius = 1.0, minRadius = 1.0;//0.5;

	float offset = 0.1;
	for (int jj = -searchDist; jj <= searchDist; jj++) {
		for (int ii = -searchDist; ii <= searchDist; ii++) {
			// With the ii and jj offset, this is the actual cell we're checking
			// to see if the given circle has expanded into it so we can draw it
			// properly.
			vec2 searchPos = vec2(cPos.x + ii, cPos.y + jj);

			// timeScaler		= Seconds until the circle reaches max radius, minCrclTm -> maxCrclTm
			// rand(searchPos)	= Max radius
			float timeScaler = rand(searchPos) * (maxCrclTm - minCrclTm) + minCrclTm;
			float circleMaxRadius = rand(searchPos) * (maxRadius - minRadius) + minRadius;

			// Everytime (time * timeScaler) reaches a new whole number,
			// we know that the maximum radius of the circle has been achieved
			// and therefore we can make a new circle with a new offset.
			offset = rand(searchPos + floor(time / timeScaler));
			searchPos.x -= rand(offset * 13);
			searchPos.y -= rand(offset * 17);

			//float newOffset = rand(floor(time / timeScaler));

			// Scales the maximum radius size.
			//float offsetScaler = clamp(rand(offset * circleMaxRadius) * 1.5, 0.75, 1.25);

			// The outer radius
			float radiusOuter;
			float radiusInner;

			// New Version where the max radius can change everytime.
			if (circleMaxRadius < 0.5)
				radiusOuter = fract(time / timeScaler) * ((circleMaxRadius)) / 10;
			else
				radiusOuter = fract(time / timeScaler) * ((circleMaxRadius)) / 4;
			//radiusOuter = fract(time / timeScaler) * circleMaxRadius;	// Old Version with set max radius per cell
			radiusInner = radiusOuter * clamp(offset * 1.5, 0.9, .95);

			// This is the distance between the point we're at right now in the offset,
			// cell, and the actual texture coodiantes of the given fragment. We need
			// to know this to see if the distance is less than the max radius so we
			// know we're within the circle.
			float cLength = length(texCoord - searchPos);
	
			// Pretty self explanitory: if distance > min but < max, draw the color.
			if ((cLength <= radiusOuter && cLength > radiusInner)) {
				//vec4 randomColor = vec4(rand(searchPos), rand(searchPos + 1), rand(searchPos + 2), 1);
				FragColor = mix(vec4(.75), FragColor, fract(time / timeScaler));
			} //else if (cLength <= radiusInner)
			//	FragColor = FragColor;
		}
	}
}