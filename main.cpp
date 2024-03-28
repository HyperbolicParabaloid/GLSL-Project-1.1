#include"Mesh.h"
#include"Cube.h"
#include"Sphere.h"
#include"Plane.h"
#include"UI.h"
#include"Cone.h"
#include"Arrow.h"
#include"Tree.h"
#include"HashTable.h"
#include"EditingSphere.h"

float rand(glm::vec2 co) {
	return glm::fract(sin(glm::dot(co, glm::vec2(12.9898, 78.233))) * 43758.5453);
}

float noise(glm::vec2 n) {
	const glm::vec2 d = glm::vec2(0.0, 1.0);
	glm::vec2 b = floor(n), f = glm::smoothstep(glm::vec2(0.0), glm::vec2(1.0), glm::fract(n));
	return glm::mix(glm::mix(rand(b), rand(b + glm::vec2(d.y, b.x)), f.x), glm::mix(rand(b + glm::vec2(d.x, b.y)), rand(b + glm::vec2(d.y, b.y)), f.x), f.y);
}



// Setting up array of vertices.
// Now I'm using this vertices array to hold the vertex info while
// using the indicies buffer to hold the index of each vertex that
// will be used to form each of the triangles (saves memory).
GLfloat vertices[] = {
	//			POSITION									COLOR
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.f,			0.8f, 0.5f, 0.0f,	// Lower Left	0
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.f,			0.8f, 0.0f, 0.5f,	// Lower Right	1
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.f,		0.0f, 0.5f, 0.5f,	// Upper Corner	2
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.f,		0.1f, 0.05f, 0.05f,	// Inner Left	3
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.f,		0.1f, 0.05f, 0.05f,	// Inner Right	4
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.f,			0.1f, 0.05f, 0.05f	// Inner Down	5
};
GLuint indices[] = {
	0, 3, 5,	// Lower Left triangle
	3, 2, 4,	// Lower Right triangle
	5, 4, 1		// Upper triangle
};

GLfloat sqr[] = {
	// POSITION					COLOR			TEXTURE COORDS
	-0.5f, -0.5f, 0.f,		0.0f, 0.9f, 0.9f,	0, 0,	// Lower Left	0
	0.5f, -0.5f, 0.f,		0.9f, 0.0f, 0.9f,	1, 0,	// Lower Right	1
	0.5f, 0.5f, 0.f,		0.9f, 0.9f, 0.0f,	1, 1,	// Upper Right	2
	-0.5f, 0.5f, 0.f,		0.9f, 0.9f, 0.9f,	0, 1	// Upper Left	3
};
GLuint sqrIndices[] = {
	0, 1, 2,	// Lower triangle
	3, 0, 2		// Upper triangle
};

// Vertices coordinates
Vertex vertices11[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices11[] =
{
	0, 1, 2,
	0, 2, 3
};


//// Vertices coordinates
//GLfloat pyramidVertices[] =
//{ //     COORDINATES     /        COLORS		/   TexCoord  /		Normals	/
//	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
//	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
//	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
//	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
//	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	0.5f, 1.0f
//};
//
//// Indices for vertices order
//GLuint pyramidIndices[] =
//{
//	0, 1, 2,
//	0, 2, 3,
//	0, 1, 4,
//	1, 2, 4,
//	2, 3, 4,
//	3, 0, 4
//};

// Vertices coordinates
GLfloat pyramidVertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 1.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 1.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.5f, 1.0f,     -0.8f, 0.5f,  0.0f, // Left Side

	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.5f, 1.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.5f, 1.0f,      0.8f, 0.5f,  0.0f, // Right side

	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.5f, 1.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

// Indices for vertices order
GLuint pyramidIndices[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};


GLfloat lightVertices[] =
{	// COORDINATES
	-0.1f, -0.1f, 0.1f,
	-0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, 0.1f,
	-0.1f, 0.1f, 0.1f,
	-0.1f, 0.1f, -0.1f,
	0.1f, 0.1f, -0.1f,
	0.1f, 0.1f, 0.1f
};


GLuint lightIndicies[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

glm::vec4 color = glm::vec4(.8f, .2f, .2f, 1.f);

glm::vec3* verts2 = new glm::vec3[1];
glm::vec3* norms2 = new glm::vec3[1];
GLuint* indices2 = new GLuint[1];
GLfloat* finalVerts2 = new GLfloat[1];
int finalVerts2Size;
int vertsSize;
int indicesSize;

glm::vec3 v1 = glm::vec3(1.f, 1.f, 1.f);	// 0
glm::vec3 v2 = glm::vec3(1.f, 1.f, -1.f);	// 1
glm::vec3 v3 = glm::vec3(1.f, -1.f, 1.f);	// 2
glm::vec3 v4 = glm::vec3(1.f, -1.f, -1.f);	// 3
glm::vec3 v5 = glm::vec3(-1.f, 1.f, 1.f);	// 4
glm::vec3 v6 = glm::vec3(-1.f, 1.f, -1.f);	// 5
glm::vec3 v7 = glm::vec3(-1.f, -1.f, 1.f);	// 6
glm::vec3 v8 = glm::vec3(-1.f, -1.f, -1.f);	// 7

glm::vec3 n1 = glm::cross(v1 - v3, v7 - v3);	// Front face normal
glm::vec3 n2 = glm::cross(v6 - v8, v4 - v8);	// Back face normal
glm::vec3 n3 = glm::cross(v2 - v1, v5 - v1);	// Top normal
glm::vec3 n4 = glm::cross(v4 - v8, v7 - v8);	// Bottom normal
glm::vec3 n5 = glm::cross(v2 - v4, v3 - v2);	// Right normal
glm::vec3 n6 = glm::cross(v5 - v7, v8 - v7);	// Left normal

GLfloat sqrVertices_2[] = {
	//		VERTEX		/		NORMAL		/				COLOR				/
	v7.x, v7.y, v7.z,	n1.x, n1.y, n1.z,	color.r, color.g, color.b, color.a,		// Front face
	v3.x, v3.y, v3.z,	n1.x, n1.y, n1.z,	color.r, color.g, color.b, color.a,
	v1.x, v1.y, v1.z,	n1.x, n1.y, n1.z,	color.r, color.g, color.b, color.a,
	v5.x, v5.y, v5.z,	n1.x, n1.y, n1.z,	color.r, color.g, color.b, color.a,

	v4.x, v4.y, v4.z,	n2.x, n2.y, n2.z,	color.r, color.g, color.b, color.a,		// Back face
	v8.x, v8.y, v8.z,	n2.x, n2.y, n2.z,	color.r, color.g, color.b, color.a,
	v6.x, v6.y, v6.z,	n2.x, n2.y, n2.z,	color.r, color.g, color.b, color.a,
	v2.x, v2.y, v2.z,	n2.x, n2.y, n2.z,	color.r, color.g, color.b, color.a,

	v5.x, v5.y, v5.z,	n3.x, n3.y, n3.z,	color.r, color.g, color.b, color.a,		// Top face
	v1.x, v1.y, v1.z,	n3.x, n3.y, n3.z,	color.r, color.g, color.b, color.a,
	v2.x, v2.y, v2.z,	n3.x, n3.y, n3.z,	color.r, color.g, color.b, color.a,
	v6.x, v6.y, v6.z,	n3.x, n3.y, n3.z,	color.r, color.g, color.b, color.a,

	v4.x, v4.y, v4.z,	n4.x, n4.y, n4.z,	color.r, color.g, color.b, color.a,		// Bottom face
	v8.x, v8.y, v8.z,	n4.x, n4.y, n4.z,	color.r, color.g, color.b, color.a,
	v7.x, v7.y, v7.z,	n4.x, n4.y, n4.z,	color.r, color.g, color.b, color.a,
	v3.x, v3.y, v3.z,	n4.x, n4.y, n4.z,	color.r, color.g, color.b, color.a,

	v3.x, v3.y, v3.z,	n5.x, n5.y, n5.z,	color.r, color.g, color.b, color.a,		// Right face
	v4.x, v4.y, v4.z,	n5.x, n5.y, n5.z,	color.r, color.g, color.b, color.a,
	v2.x, v2.y, v2.z,	n5.x, n5.y, n5.z,	color.r, color.g, color.b, color.a,
	v1.x, v1.y, v1.z,	n5.x, n5.y, n5.z,	color.r, color.g, color.b, color.a,

	v8.x, v8.y, v8.z,	n6.x, n6.y, n6.z,	color.r, color.g, color.b, color.a,		// Left face
	v7.x, v7.y, v7.z,	n6.x, n6.y, n6.z,	color.r, color.g, color.b, color.a,
	v5.x, v5.y, v5.z,	n6.x, n6.y, n6.z,	color.r, color.g, color.b, color.a,
	v6.x, v6.y, v6.z,	n6.x, n6.y, n6.z,	color.r, color.g, color.b, color.a
};

GLuint sqrIndices_2[] = {
	// Front Face
	0, 1, 2,
	2, 3, 0,

	// Back face
	4, 5, 6,
	6, 7, 4,
	//
	// Top face
	8, 9, 10,
	10, 11, 8,

	// Bottom face
	12, 13, 14,
	14, 15, 12,

	// Right face
	16, 17, 18,
	18, 19, 16,

	// Left face
	20, 21, 22,
	22, 23, 20
};



int viewWidth = 1000, viewHeight = 800;
int setCamera = 0;

// Converts a float to a string to a given decimal precision.
std::string floatToString(float _num, int _precision) {
	std::string longString = std::to_string(_num);
	std::string shortString;

	int n = _precision;
	int decimals = longString.length();
	for (int i = 0; i < longString.length(); i++) {
		shortString.push_back(longString[i]);
		if (longString[i] == '.') {
			decimals = i + n;
		}
		if (decimals <= i)
			break;
	}
	return shortString;
}

// Converts any lowercase letters in string to uppercase.
std::string toUppercase(std::string _str) {
	// 65->90 = A->Z;
	// 97->122 = A->Z;
	for (int i = 0; i < _str.length(); i++)
		if (_str[i] >= 97 && _str[i] <= 122)
			_str[i] = _str[i] - ('a' - 'A');
	return _str;
}



#if 1
int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(viewWidth, viewWidth, "GLFW Testing Window", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, viewWidth, viewWidth);


	// ---------------------------
	// Setting up the light source.
	// ---------------------------

	// Setting Shader object for light source.
	Shader lightShader("light.vert", "light.frag");

	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndicies, sizeof(lightIndicies));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();



	// Setting up matrices for both the light and pyramid models.
	glm::vec4 lightColor = glm::vec4(.8f, .8f, 1.f, 1.f);
	glm::vec3 lightPos = glm::vec3(1.5f, 1.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.f);
	lightModel = glm::translate(lightModel, lightPos);

	// Activating shader's and setting up model uniforms.
	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	float frac, whole;

	float rotation = 0.f;
	float motion = 0.0001f;

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	// Enabling transparency with Alpha value.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Enabling backface culling :D
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	// Initializing matrices for model, view and projection
	glm::mat4 view = glm::mat4(1.f);

	Camera camera(window, glm::vec2(viewWidth, viewHeight), glm::vec3(0.f, 0.5f, 2.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	camera.set_projection(glm::radians(60.f), viewWidth, (float)(viewWidth / viewHeight), 0.1f, 100.f);

	double lockoutTimer = 0;
	bool shouldRotate = false, shouldFly = false, capturingMotion = false;

	// Reading character file.
	std::fstream myFile;
	int characterIndex = 0;
	myFile.open("C:/Users/riley/source/repos/GLSL Project Working Directory/Character Libray/dictionary_list.txt");// , std::ios::app);

	int const dictionaryLength = 100;
	std::string dictionary[dictionaryLength];
	std::string theLine;
	glm::uvec2 intDict[100];

	myFile.clear();
	myFile.seekg(0, std::ios::beg);
	while (getline(myFile, theLine)) {
		std::string code = theLine.substr(2, theLine.size() - 1);
		int index = int(theLine[0]);
		dictionary[index] = code;
		int bitsX = 0;
		int bitsY = 0;

		for (int bit = 0; bit < 32; bit++) {
			if (code[bit] == '0') {
				bitsX |= 0 << (bit % 32);
			}
			else {
				bitsX |= 1 << (bit % 32);
			}
		}
		for (int bit = 32; bit < 64; bit++) {
			if (code[bit] == '0') {
				bitsY |= 0 << (bit % 32);
			}
			else {
				bitsY |= 1 << (bit % 32);
			}
		}
		intDict[index] = glm::uvec2(bitsX, bitsY);
		//std::cout << "Added " << code << " at [" << index << "]\n";
	}
	myFile.close();

	// Setting all other values to be spaces.
	for (int i = 0; i < dictionaryLength; i++) {
		if (dictionary[i].empty()) {
			dictionary[i] = dictionary[95];
			intDict[i] = glm::uvec2(0);
		}
	}

	//Texture tex("brick.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture textures[] = {
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	std::vector <Texture> empty;

	// Planes.
	glm::vec3 plane1Pos = glm::vec3(0.0f, 0.f, 0.0f);
	Plane plane1(window, plane1Pos, 10.f, 2, true, glm::vec4(0.f, 0.f, .2f, 1.f), tex, &camera);
	// Cubes.
	glm::vec3 cube3Pos = glm::vec3(0.0f, 2.f, -2.5f);
	Cube cube3(window, cube3Pos, 1.f, glm::vec4(.1f, .8f, .3f, 1.f), empty, &camera);
	// Spheres.
	int level = 5;
	glm::vec3 sphere1Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 sphere1Radi = glm::vec3(1.f, 1.f, 1.f);// / 25.f);
	Sphere sphere1(window, sphere1Pos, sphere1Radi, 1.0f, level, true, glm::vec4(.8f, .2f, .5f, 1.f), empty, &camera);
	Sphere sphere1Mirror(window, sphere1Pos, sphere1Radi, 1.0f, level, true, glm::vec4(0.f, .0f, .1f, 0.25f), empty, &camera);
	// Spheres.
	glm::vec3 sphere2Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 sphere2Radi = glm::vec3(1.f, 1.f, 1.0f);
	Sphere sphere2(window, sphere2Pos, sphere2Radi, 0.2f, 5, true, glm::vec4(1.0f, .1f, .1f, 1.f), empty, &camera);
	// Cones.
	glm::vec3 conePos = glm::vec3(0.f, 1.f, -1.f);
	float coneScale = 1.f;
	int coneLevel = 4;
	float coneBottomRadius = 1.f;
	float coneTopRadius = 0.f;
	glm::vec3 conePointPos = glm::vec3(0.f, 1.f, 0.f);
	bool coneIsSmooth = true;
	glm::vec4 coneShaftColor = glm::vec4(1.f), coneConeColor = glm::vec4(1.f);
	Cone cone1(window, conePos, coneScale, coneLevel, coneBottomRadius, coneTopRadius, conePointPos, coneIsSmooth, coneShaftColor, coneConeColor, empty, &camera);
	
	// UI elements.
	glm::vec3 UIRadi = glm::vec3(0.5f, 1.f, 1.f);
	float UICharacterScale = 1.f / 32.f;
	UI sphere1UI(window, glm::vec3(0.f, 0.f, 0.f), glm::vec2(-2.f, 1.f), UIRadi, UICharacterScale, "YEE OLD SPHERE1\nSTATS:", intDict, glm::vec4(1.f, 0.f, .2f, 1.f), empty, &camera);
	//UI sphere1UI(window, glm::vec3(0.0, 0.f, 0.f), glm::vec2(-1.f, 1.f), 1.f, 1.f / 32.f, "YEE OLD SPHERE1\nSTATS:", intDict, glm::vec4(1.f, 0.f, .2f, 1.f), empty, &camera);
	//UI sphere1UI(window, glm::vec3(0.0, 0.f, 0.f), glm::vec2(-1.f, 1.f), 1.f, 1.f / 32.f, "YEE OLD SPHERE1\nSTATS:", intDict, glm::vec4(1.f, 0.f, .2f, 1.f), empty, &camera);
	//UI sphere1UI(window, glm::vec3(0.0, 0.f, 0.f), glm::vec2(-1.f, 1.f), 1.f, 1.f / 32.f, "YEE OLD SPHERE1\nSTATS:", intDict, glm::vec4(1.f, 0.f, .2f, 1.f), empty, &camera);
	//UI sphere1UI(window, glm::vec3(0.0, 0.f, 0.f), glm::vec2(-1.f, 1.f), 1.f, 1.f / 32.f, "YEE OLD SPHERE1\nSTATS:", intDict, glm::vec4(1.f, 0.f, .2f, 1.f), empty, &camera);
	
	
	
	
	// Arrow objects,
	glm::vec3 arrowPos = glm::vec3(-2.f, 0.f, 0.f);
	float arrowScale = 1.f;
	int arrowLevel = 16;
	glm::vec3 arrowBottomRadius = glm::vec3(1.f);
	glm::vec3 arrowTopRadius = glm::vec3(0.5f);
	glm::vec3 arrowRadi = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 arrowPointPos = glm::vec3(0.f, 1.f, 0.f);
	bool arrowIsSmooth = true;
	glm::vec4 arrowShaftColor = glm::vec4(1.f), arrowConeColor = glm::vec4(1.f);
	Arrow arrow1(window, arrowPos, arrowScale, arrowLevel, arrowBottomRadius, arrowTopRadius, arrowRadi, arrowPointPos, arrowIsSmooth, arrowShaftColor, arrowConeColor, empty, &camera);
	
	std::vector <Object*> objectList;
	std::vector <UI*> UIList;

	int targetIndex = 0;

	bool trackingSphere = true;

	objectList.push_back(&plane1);
	objectList.push_back(&cube3);
	objectList.push_back(&sphere1);
	objectList.push_back(&sphere2);
	objectList.push_back(&cone1);
	objectList.push_back(&arrow1);
	objectList.push_back(&sphere1Mirror);
	objectList.push_back(&sphere1UI);
	Object* targetObj = objectList[targetIndex];

	int objectListSize = objectList.size();
	for (int o = 0; o < objectListSize; o++) {
		glm::vec3 UICanvasRadi = glm::vec3(1.f);
		glm::vec4 UIColor = glm::vec4(0.f, 1.f, 1.f, 1.f);
		std::string UIText = toUppercase(objectList[o]->name) + " (" + std::to_string(o) + ")";
		glm::vec3 UIPos = glm::vec3(0.f, 0.f, 0.f);

		float UITextScale;
		if (objectListSize > 32)
			UITextScale = 1.f / objectListSize; // objectListSize characters per line.
		else
			UITextScale = 1.f / 32.f; // 32 characters per line

		float textOffset = 0.5f / UITextScale;
		glm::vec2 UIRasterCoords = glm::vec2(1 - (UIText.length() / textOffset), 1 - (o / textOffset)); // -1 -> 1

		objectList.push_back(new UI{ window, UIPos, UIRasterCoords, UICanvasRadi, UITextScale, UIText, intDict, UIColor, empty, &camera });
	}


	bool first = true;
	float colorScaler = 64.f;
	float colorCount = 1.f;

	UIList.push_back(&sphere1UI);
	camera.set_camera_speed(10);

	bool randomColor = true;

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("object.vert", "object.frag");

	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTex"), 1);
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexSpec"), 1);

	int FPS = 0;
	int FPSCount = 0;
	double crntTime = 0.f, prevTime = 0.f;

	bool rotate = true;

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Setting info about the Sphere to the screen.
		std::string s1UIHeader = "STATS: " + toUppercase(targetObj->name) + "\n";
		std::string s1UIFPS =	" FPS    = " + std::to_string(FPS) + "\n";
		std::string s1UILevel = " LEVEL  = " + std::to_string(targetObj->level) + "\n";
		std::string s1UIPos =	" POS    = (" + floatToString(targetObj->objPos.x, 1) + ", " + floatToString(targetObj->objPos.y, 1) + ", " + floatToString(targetObj->objPos.z, 1) + ")\n";
		std::string s1UIScale = " SCALE  = (" + floatToString(targetObj->radi.x, 1) + ", " + floatToString(targetObj->radi.y, 1) + ", " + floatToString(targetObj->radi.z, 1) + ")\n";
		std::string s1UIVisuals = "\nVISUALS:\n";
		std::string s1UISmooth;
		if (targetObj->smooth)
			s1UISmooth = " SMOOTH = TRUE\n";
		else
			s1UISmooth = " SMOOTH = FALSE\n";
		std::string s1UINormals;
		if (targetObj->normalShaderProgram != nullptr)
			s1UINormals = " NORMS  = ON\n";
		else
			s1UINormals = " NORMS  = OFF\n";
		std::string s1UIColor;
		if (randomColor)
			s1UIColor = " COLOR  = SOLID\n";
		else
			s1UIColor = " COLOR  = RANDOM\n";
		std::string s1UIMotion;
		if (rotate)
			s1UIMotion = " MOTION = ROTATION\n";
		else
			s1UIMotion = " MOTION = (X,Y,Z)\n";
		
		sphere1UI.setNewString(	s1UIHeader	+ //"\n" + 
								s1UIPos		+ //"\n" + 
								s1UIScale	+ //"\n" + 
								s1UIFPS		+ //"\n" + 
								s1UILevel	+ //"\n" + 
								s1UIVisuals + //"\n" + 
								s1UINormals + //"\n" +
								s1UISmooth	+ //"\n" + 
								s1UIColor	+ //"\n" + 
								s1UIMotion, true);

		// Simple timer
		crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1) {
			prevTime = crntTime;
			FPS = FPSCount;
			FPSCount = 0;
		}
		else
			FPSCount++;

		// Aiming cone1 at sphere1.
		//arrow1.pointAt(glm::normalize(sphere1.objPos - arrow1.objPos), true);

		// Rotating the view about the x and y axis, and loads of other neat things.
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_F) && lockoutTimer <= crntTime) {
			lockoutTimer = crntTime + 0.2;
			capturingMotion = !capturingMotion;
			camera.motion_enabled(capturingMotion);
			std::cout << "Crotation\n";
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) {
			if (rotate)
				targetObj->rotate(.5f, glm::vec3(0.f, -1.f, 0.f));
			else
				targetObj->setNewPos(targetObj->objPos + glm::vec3(-0.01f, 0.f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
			if (rotate)
				targetObj->rotate(.5f, glm::vec3(-1.f, 0.f, 0.f));
			else
				targetObj->setNewPos(targetObj->objPos + glm::vec3(0.0f, 0.f, -0.01f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
			if (rotate)
				targetObj->rotate(.5f, glm::vec3(1.f, 0.f, 0.f));
			else
				targetObj->setNewPos(targetObj->objPos + glm::vec3(0.0f, 0.f, 0.01f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)) {
			if (rotate)
				targetObj->rotate(.5f, glm::vec3(0.f, 1.f, 0.f));
			else
				targetObj->setNewPos(targetObj->objPos + glm::vec3(0.01f, 0.f, 0.0f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_MINUS) && lockoutTimer <= crntTime) {
			targetObj->setLevel(targetObj->level - 1);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_EQUAL) && lockoutTimer <= crntTime) {
			targetObj->setLevel(targetObj->level + 1);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_R) && lockoutTimer <= crntTime) {
			targetObj->doRandomColors(randomColor);
			targetObj->setLevel(targetObj->level);
			randomColor = !randomColor;
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_C) && lockoutTimer <= crntTime) {
			//targetObj->reseed();
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Y) && lockoutTimer <= crntTime) {
			targetObj->smoothSurface(!targetObj->smooth);
			targetObj->setLevel(targetObj->level);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_L) && lockoutTimer <= crntTime) {
			if (!rotate)
				targetObj->setNewPos(targetObj->objPos + glm::vec3(0.f, 0.01f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_K) && lockoutTimer <= crntTime) {
			if (!rotate)
				targetObj->setNewPos(targetObj->objPos + glm::vec3(0.f, -0.01f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_T) && lockoutTimer <= crntTime) {
			targetIndex %= objectListSize;
			objectList[targetIndex + objectListSize]->setColor(glm::vec4(0.f));

			targetIndex++;

			targetIndex %= objectListSize;
			objectList[targetIndex + objectListSize]->setColor(glm::vec4(1.f));

			targetObj = objectList[targetIndex];

			lockoutTimer = crntTime + 0.2;
		}

		if (shouldFly) {
			if (camera.fly_to(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.125f, -1.f), true)) {
				shouldFly = false;
			}
		}

		// Mouse Buttons
		if (GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && lockoutTimer <= crntTime) {
			glm::vec3 camPos = camera.cameraPos;
			glm::vec3 cursorRay = camera.getCursorRay();
			glm::vec3 transformedPos = sphere1.isRayTouching(camPos, cursorRay);
			
			sphere2.setNewPos(transformedPos);


			// Doing UI checks.
			glm::vec2 cursorPos = camera.getCursorPos();
			
			// Figuring out the text scale so we can search for it properly.
			float UITextScale;
			if (objectListSize > 32)
				UITextScale = 1.f / objectListSize; // objectListSize characters per line.
			else
				UITextScale = 1.f / 32.f; // 32 characters per line

			targetIndex = targetIndex % objectListSize;
			
			// Getting index of the object's UI Canvas.
			float textOffset = 0.5f / UITextScale;
			int offset = floor((cursorPos.y - 1) * -textOffset);
			if (offset < objectListSize && offset >= 0) {
				// Checking if the cursor is touching the UI Canvas.
				if (objectList[(offset + objectListSize) % objectList.size()]->isCursorTouching(cursorPos)) {
					// Setting background colors of the old/new UI Canvases.
					objectList[(targetIndex + objectListSize) % objectList.size()]->setColor(glm::vec4(0.f));
					objectList[(offset + objectListSize) % objectList.size()]->setColor(glm::vec4(1.f));
					// If the Canvas was hit, we know we just requested to switch attention to the
					// respective object in ObjectList, and we set both that as targetObj as well
					// as setting the targetIndex appropriately.
					targetIndex = offset;
					targetObj = objectList[offset];
				}
			}

		}
		if (GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) && lockoutTimer <= crntTime) {
			rotate = !rotate;
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_I) && lockoutTimer <= crntTime) {
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Z) && lockoutTimer <= crntTime) {
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_X) && lockoutTimer <= crntTime) {
			lockoutTimer = crntTime + 0.5;
		}


		// Tracking movements.
		camera.track_movement();

		// Setting view matrix with camera class
		view = camera.get_view();

		// Drawing all the Objects in the list.
		for (auto obj : objectList)
			obj->draw(lightPos, lightColor);

		// Drawing the Light.
		lightShader.Activate();
		camera.camMatrixForShader(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndicies) / sizeof(int), GL_UNSIGNED_INT, 0);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}
	

	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
#endif

#if 0
int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(viewWidth, viewWidth, "GLFW Testing Window", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, viewWidth, viewWidth);


	// ---------------------------
	// Setting up the light source.
	// ---------------------------

	// Setting Shader object for light source.
	Shader lightShader("light.vert", "light.frag");

	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndicies, sizeof(lightIndicies));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();



	// Setting up matrices for both the light and pyramid models.
	glm::vec4 lightColor = glm::vec4(.8f, .8f, 1.f, 1.f);
	glm::vec3 lightPos = glm::vec3(1.5f, 1.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.f);
	lightModel = glm::translate(lightModel, lightPos);

	// Activating shader's and setting up model uniforms.
	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	float frac, whole;

	float rotation = 0.f, prevTime = glfwGetTime();
	float motion = 0.0001f;

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	// Enabling transparency with Alpha value.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Initializing matrices for model, view and projection
	glm::mat4 view = glm::mat4(1.f);

	Camera camera(window, glm::vec2(viewWidth, viewHeight), glm::vec3(0.f, 0.5f, 2.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	camera.set_projection(glm::radians(60.f), viewWidth, (float)(viewWidth / viewHeight), 0.1f, 100.f);

	double lockoutTimer = 0;
	bool shouldRotate = false, shouldFly = false, capturingMotion = false;

	// Cube(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 _color, Camera* _camera);
	//Texture tex("brick.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture textures[] = {
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts(vertices11, vertices11 + sizeof(vertices11) / sizeof(Vertex));
	std::vector <GLuint> ind(indices11, indices11 + sizeof(indices11) / sizeof(GLuint));

	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	std::vector <Texture> empty;

	glm::vec3 plane1Pos = glm::vec3(0.0f, -1.5f, 0.0f);
	//Plane plane1(window, plane1Pos, 40.f, true, glm::vec4(1.f, 1.f, 1.f, 1.f), tex, &camera);
	Plane plane1(window, plane1Pos, 10.f, 2, true, glm::vec4(0.f, 0.f, .2f, 1.f), empty, &camera);

	float cube1y = -1.5 + sqrt(3);
	glm::vec3 cube1Pos = glm::vec3(-2.5f, cube1y, -2.5f);
	Cube cube1(window, cube1Pos, 1.f, color, empty, &camera);
	cube1.rotate(45.f, glm::vec3(0.f, 0.f, 1.f));
	cube1.rotate(45.f, glm::vec3(0.f, 1.f, 0.f));
	glm::vec3 cube2Pos = glm::vec3(2.0f, -.5f, -2.5f);
	Cube cube2(window, cube2Pos, 1.f, glm::vec4(.2f, .5f, .8f, 1.f), empty, &camera);
	
	glm::vec3 cube3Pos = glm::vec3(0.0f, 2.f, -2.5f);
	Cube cube3(window, cube3Pos, 1.f, glm::vec4(.1f, .8f, .3f, 1.f), empty, &camera);
	
	int level = 2;
	glm::vec3 sphere1Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 sphere1Radi = glm::vec3(1.f, 1.f, 1.0f);
	Sphere sphere1(window, sphere1Pos, sphere1Radi, 1.0f, level, false, glm::vec4(.8f, .2f, .5f, 1.f), empty, &camera);






	std::fstream myFile;
	//char characterArray[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	//							'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
	//};
	int characterIndex = 0;
	myFile.open("C:/Users/riley/source/repos/GLSL Project Working Directory/Character Libray/dictionary_list.txt");// , std::ios::app);

	int const dictionaryLength = 100;
	std::string dictionary[dictionaryLength];
	std::string theLine;

	glm::uvec2 intDict[100];

	myFile.clear();
	myFile.seekg(0, std::ios::beg);

	/*
	1 2 3 4 5 6 7 8
	0 0 0 0 0 0 0 0 1
	0 0 0 1 1 0 0 0 2
	0 0 0 1 1 0 0 0 3
	0 0 0 1 1 0 0 0 4
	0 0 0 1 1 0 0 0 5
	0 0 0 0 0 0 0 0 6
	0 0 0 1 1 0 0 0 7
	0 0 0 1 1 0 0 0 8


	*/
	//     34 = "
	//	   35 = #
	//     36 = $
	//     37 = %
	//     38 = &
	//     39 = '
	//	   40 = (
	//	   41 = )
	//	   42 = *
	//     43 = +
	//     44 = ,
	//     45 = -
	//	   46 = .
	//     47 = /
	// 48->57 = 0->9;
	//     61 = =
	// 65->90 = A->Z;
	//	   95 = _	;
	while (getline(myFile, theLine)) {
		std::string code = theLine.substr(2, theLine.size() - 1);
		int index = int(theLine[0]);
		dictionary[index] = code;
		int bitsX = 0;
		int bitsY = 0;

		for (int bit = 0; bit < 32; bit++) {
			if (code[bit] == '0') {
				bitsX |= 0 << (bit % 32);
			}
			else {
				bitsX |= 1 << (bit % 32);
			}
		}
		for (int bit = 32; bit < 64; bit++) {
			if (code[bit] == '0') {
				bitsY |= 0 << (bit % 32);
			}
			else {
				bitsY |= 1 << (bit % 32);
			}
		}
		intDict[index] = glm::uvec2(bitsX, bitsY);
		//std::cout << "Added " << code << " at [" << index << "]\n";
	}

	// Setting all other values to be spaces.
	for (int i = 0; i < dictionaryLength; i++) {
		if (dictionary[i].empty()) {
			dictionary[i] = dictionary[95];
			intDict[i] = glm::uvec2(0);
		}
	}



	UI UI1(window, glm::vec3(0.0, 0.f, 0.f), glm::vec2(-1.f, 0.f),  1.f, 1.f / 64.f, "PAIGE", intDict, glm::vec4(1.f, 0.f, .2f, 1.f), empty, &camera);
	//UI UI2(window, glm::vec3(0.0, 0.f, 0.f), glm::vec2(-1.f, 0.f), 1.0f, 1.0f, "0", intDict, glm::vec4(1.f, 0.f, .2f, 1.f), empty, &camera);


	std::vector <Object*> objectList;
	std::vector <UI*> UIList;

	objectList.push_back(&sphere1);
	objectList.push_back(&cube1);
	objectList.push_back(&cube2);
	objectList.push_back(&cube3);
	objectList.push_back(&plane1);
	objectList.push_back(&UI1);
	//objectList.push_back(&UI2);


	float UI_Scale = 0.1f;
	bool first = true;
	float colorScaler = 64.f;
	float colorCount = 1.f;

	UIList.push_back(&UI1);
	//UIList.push_back(&UI2);
	//objectList.push_back(UIList.back());


	//for (float y = 1.f - UI_Scale * 2.f; y > -1.f + UI_Scale * 4.f; y -= UI_Scale * 2.f) {
	//	for (float x = -1.f + UI_Scale * 4.f; x < 1.f - UI_Scale * 2.f; x += UI_Scale * 2.f) {
	//		if (first) {
	//			UIList.push_back(&UI1);
	//			objectList.push_back(UIList.back());
	//			first = false;
	//		}
	//		else {
	//			glm::vec3 UIPos = glm::vec3(x, y, 0.f);
	//			UIList.push_back(new UI{ window, UIPos, 0.1f, 2, true, glm::vec4(colorCount / colorScaler, 0.f, .0f, 1.f), empty, &camera });
	//			objectList.push_back(UIList.back());
	//			colorCount++;
	//		}
	//	}
	//}

	camera.set_camera_speed(10);
	
	bool randomColor = true;
	bool isSmooth = true;

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("object.vert", "object.frag");

	// Create floor mesh
	Mesh floor(verts, ind, tex);

	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTex"), 1);
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexSpec"), 1);


	std::string text = UI1.getString();
	
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Simple timer
		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60)
		{
			
			//UI1.setNewString(text);
			//UI1.appendNumber(crntTime, 3);
			//rotation = 0.1f;
			//prevTime = crntTime;
		}

		//glm::vec2 cursorPos = camera.getCursorPos();
		//glm::vec2 newPos = glm::clamp(glm::vec2(UI1.objPos.x, cursorPos.y), glm::vec2(UI1.objPos.x, -0.5f), glm::vec2(UI1.objPos.x, 0.5f));
		//UI1.setScreenPos(newPos);
		//UI1.setNewString("(");
		//UI1.appendNumber(newPos.x, 3);
		//UI1.appendString(", ");
		//UI1.appendNumber(newPos.y, 3);
		//UI1.appendString(")");



		// Rotating the view about the x and y axis, and loads of other neat things.
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_F) && lockoutTimer <= crntTime) {
			lockoutTimer = crntTime + 0.2;
			capturingMotion = !capturingMotion;
			camera.motion_enabled(capturingMotion);
			std::cout << "Crotation\n";
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) {
			//sphere1.rotate(.5f, glm::vec3(0.f, -1.f, 0.f));
			UI1.setScreenPos(glm::vec2(UI1.objPos) - glm::vec2(0.01f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
			//sphere1.rotate(.5f, glm::vec3(-1.f, 0.f, 0.f));
			UI1.setScreenPos(glm::vec2(UI1.objPos) - glm::vec2(0.f, -0.01f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
			//sphere1.rotate(.5f, glm::vec3(1.f, 0.f, 0.f));
			UI1.setScreenPos(glm::vec2(UI1.objPos) - glm::vec2(0.f, 0.01f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)) {
			//sphere1.rotate(.5f, glm::vec3(0.f, 1.f, 0.f));
			UI1.setScreenPos(glm::vec2(UI1.objPos) - glm::vec2(-0.01f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_MINUS) && lockoutTimer <= crntTime) {
			level--;
			std::cout << "\nLevel: " << level << "\n";
			sphere1.setLevel(level);
			//plane1.setLevel(level);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_EQUAL) && lockoutTimer <= crntTime) {
			level++;
			std::cout << "\nLevel: " << level << "\n";
			sphere1.setLevel(level);
			//plane1.setLevel(level);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_R) && lockoutTimer <= crntTime) {
			sphere1.doRandomColors(randomColor);
			sphere1.setLevel(level);

			plane1.doRandomColors(randomColor);
			plane1.setLevel(level);
			randomColor = !randomColor;
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_C) && lockoutTimer <= crntTime) {
			sphere1.reseed();
			sphere1.setLevel(level);

			plane1.reseed();
			plane1.setLevel(level);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Y) && lockoutTimer <= crntTime) {
			isSmooth = !isSmooth;
			sphere1.smoothSurface(isSmooth);
			sphere1.setLevel(level);

			//plane1.smoothSurface(isSmooth);
			//plane1.setLevel(level);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_L) && lockoutTimer <= crntTime) {
			UI1.setScale(UI1.getScale() + glm::vec2(0.001f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_K) && lockoutTimer <= crntTime) {
			UI1.setScale(UI1.getScale() - glm::vec2(0.001f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_T) && lockoutTimer <= crntTime) {
			std::cout << "\nLevel: " << level << "\n";
			lockoutTimer = crntTime + 0.2;
			std::cout << "Flyin' in\n";
			shouldFly = true;
		}
		
		if (shouldFly) {
			if (camera.fly_to(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.125f, -1.f), true)) {
				shouldFly = false;
			}
		}

		// Toggling the tracking of user movement
		glm::vec3 newSphere1Pos = camera.track_movement();
		glm::vec3 cameraPos = camera.cameraPos;

		if (int(crntTime * 4) != int(prevTime * 4)) {
			//std::cout << "newSphere1Pos = (" << newSphere1Pos.x << ", " << newSphere1Pos.y << ", " << newSphere1Pos.z << ")" << std::endl;
			//std::cout << std::endl;
			prevTime = crntTime;
		}


		if (newSphere1Pos.x > -100.f)
			sphere1.setNewPos(newSphere1Pos);
		glm::vec3 newObjPos = sphere1.objPos;


		// Mouse Buttons
		if (GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && lockoutTimer <= crntTime) {
			//std::cout << "Yee-old LEFT click bruv.\n";
			//std::cout << "cursorPos = (" << cursorPos.x << ", " << cursorPos.y << ")" << std::endl;
			glm::vec2 cursorPos = camera.getCursorPos();
			for (auto UI_Obj : UIList) {
				//if (UI_Obj->isTouching(cursorPos)) {
					UI_Obj->setScreenPos(cursorPos);
				//}
			}
			lockoutTimer = crntTime + 0.02;
		}
		if (GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) && lockoutTimer <= crntTime) {
			
			//std::string newString = "123456";
			//UI1.setNewString(newString);
			UI1.appendString("6789");
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_I) && lockoutTimer <= crntTime) {
			//std::string m = std::to_string(1234.56789) + "/1-2+3,4=5" + '"' + "#$%&*()";

			UI1.setNewString("PAIGE");
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Z) && lockoutTimer <= crntTime) {
			std::cout << "Reading from file.\n";
			std::string line;

			myFile.clear();
			myFile.seekg(0, std::ios::beg);

			while (getline(myFile, line))
			{
				std::cout << line << "\n";
			}
			

			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_X) && lockoutTimer <= crntTime) {
			std::cout << "Decremented counter\n";
			characterIndex--;
			lockoutTimer = crntTime + 0.5;
		}


		// Setting view matrix with camera class
		view = camera.get_view();

		//floor.draw(camera, shaderProgram);

		// Drawing all the Objects in the list.
		
		for (auto obj : objectList)
			obj->draw(lightPos, lightColor);
		
		
		//UI1.draw(lightPos, lightColor);
		//glDisable(GL_BLEND);


		// Drawing the Light.
		lightShader.Activate();
		camera.camMatrixForShader(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndicies) / sizeof(int), GL_UNSIGNED_INT, 0);



		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}
	myFile.close();

	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	delete[] verts2;
	delete[] indices2;
	delete[] norms2;
	delete[] finalVerts2;
	return 0;
}
#endif

#if 0
int main()
{


	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(viewWidth, viewWidth, "GLFW Testing Window", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, viewWidth, viewWidth);


	// ---------------------------
	// Setting up the light source.
	// ---------------------------

	// Setting Shader object for light source.
	Shader lightShader("light.vert", "light.frag");

	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndicies, sizeof(lightIndicies));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();



	// Setting up matrices for both the light and pyramid models.
	glm::vec4 lightColor = glm::vec4(.8f, .8f, 1.f, 1.f);
	glm::vec3 lightPos = glm::vec3(1.5f, 5.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.f);
	lightModel = glm::translate(lightModel, lightPos);

	// Activating shader's and setting up model uniforms.
	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	float frac, whole;

	float rotation = 0.f, prevTime = glfwGetTime();
	float motion = 0.0001f;

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Initializing matrices for model, view and projection
	glm::mat4 view = glm::mat4(1.f);

	glm::vec3 camPos = glm::vec3(0.f, 0.5f, 2.f);
	Camera camera(window, glm::vec2(viewWidth, viewHeight), camPos, glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	camera.set_projection(glm::radians(60.f), (float)(viewWidth / viewHeight), 0.01f, 1000.f);

	double lockoutTimer = 0;
	bool shouldRotate = false, shouldFly = false, capturingMotion = false;

	// Cube(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 _color, Camera* _camera);
	//Texture tex("brick.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture textures[] = {
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
		Texture("seamless_green_grass_texture_01.png", "diffuse", 2, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("grass-normal-map.png", "specular", 3, GL_RED, GL_UNSIGNED_BYTE),
		Texture("tree_bark.png", "diffuse", 4, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("tree_bark_spec.png", "specular", 5, GL_RED, GL_UNSIGNED_BYTE)
	};

	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts(vertices11, vertices11 + sizeof(vertices11) / sizeof(Vertex));
	std::vector <GLuint> ind(indices11, indices11 + sizeof(indices11) / sizeof(GLuint));

	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	std::vector <Texture> empty;

	int planeLevel = 4;
	float planeaScale = 50.f;
	std::vector <Plane*> planeList;
	glm::vec3 planeaPos = glm::vec3(0.f, -1.5f, 0.0f);
	Plane planea(window, planeaPos, planeaScale, planeLevel, true, glm::vec4(0.f, .2f, .8f, 1.f), empty, &camera);
	Plane planeb(window, glm::vec3(23.f, -1.5f, 0.0f), planeaScale, planeLevel, true, glm::vec4(0.f, .2f, .8f, 1.f), empty, &camera);
	Plane planec(window, glm::vec3(-10.f, -1.5f, 0.f), planeaScale, planeLevel, true, glm::vec4(0.f, .2f, .8f, 1.f), empty, &camera);
	Plane planed(window, glm::vec3(0.f, 26.5f, 0.0f), planeaScale, planeLevel, true, glm::vec4(0.f, .2f, .8f, 1.f), empty, &camera);
	//planea.rotate(45, glm::vec3(0.f, 0.f, 1.f));
	//planea.rotate(90, glm::vec3(1.f, 0.f, 0.f));
	planea.doRandomColors(true);
	//planea.rotate(45, glm::vec3(0.f, 0.f, 1.f));
	planea.setLevel(planeLevel);

	/*
	//glm::vec3 plane1Pos = glm::vec3(-planeaScale * 2, -1.5f, -planeaScale * 2);
	//Plane plane1(window, plane1Pos, planeaScale, planeLevel, true, glm::vec4(1.f, .2f, .8f, 1.f), tex, &camera);
	//
	//glm::vec3 plane2Pos = glm::vec3(0.f, -1.5f, -planeaScale * 2);
	//Plane plane2(window, plane2Pos, planeaScale, planeLevel, true, glm::vec4(1.f, .2f, .8f, 1.f), tex, &camera);
	//
	//glm::vec3 plane3Pos = glm::vec3(planeaScale * 2, -1.5f, -planeaScale * 2);
	//Plane plane3(window, plane3Pos, planeaScale, planeLevel, true, glm::vec4(1.f, .2f, .8f, 1.f), tex, &camera);
	//
	//glm::vec3 plane4Pos = glm::vec3(-planeaScale * 2, -1.5f, 0.0f);
	//Plane plane4(window, plane4Pos, planeaScale, planeLevel, true, glm::vec4(1.f, .2f, .8f, 1.f), tex, &camera);
	//
	//glm::vec3 plane6Pos = glm::vec3(planeaScale * 2, -1.5f, 0.0f);
	//Plane plane6(window, plane6Pos, planeaScale, planeLevel, true, glm::vec4(1.f, .2f, .8f, 1.f), tex, &camera);
	//
	//glm::vec3 plane7Pos = glm::vec3(-planeaScale * 2, -1.5f, planeaScale * 2);
	//Plane plane7(window, plane7Pos, planeaScale, planeLevel, true, glm::vec4(1.f, .2f, .8f, 1.f), tex, &camera);
	//
	//glm::vec3 plane8Pos = glm::vec3(0.f, -1.5f, planeaScale * 2);
	//Plane plane8(window, plane8Pos, planeaScale, planeLevel, true, glm::vec4(1.f, .2f, .8f, 1.f), tex, &camera);
	//
	//glm::vec3 plane9Pos = glm::vec3(planeaScale * 2, -1.5f, planeaScale * 2);
	//Plane plane9(window, plane9Pos, planeaScale, planeLevel, true, glm::vec4(1.f, .2f, .8f, 1.f), tex, &camera);
	*/

	
	float startPlaneTime = glfwGetTime();
	int start = -2, end = 2;
	std::cout << "Making " << start << " -> " << end << " planes(" << end + 1 << "x" << end + 1 <<") took ";
	float offset = (planeaScale * 2);
	for (int vv = end; vv >= start; vv--) {
		for (int uu = start; uu <= end; uu++) {
			if (uu != vv || vv != 0) {
				glm::vec3 newPlanePos = glm::vec3(offset * vv, -1.5f, offset * uu);
				planeList.push_back(new Plane(window, newPlanePos, planeaScale, planeLevel, true, glm::vec4(1.f, .2f, .8f, 1.f), tex, &camera));
			}
		}
	}
	std::cout << glfwGetTime() - startPlaneTime << " seconds\n";
	
	float cube1y = -1.5 + sqrt(3);
	glm::vec3 cube1Pos = glm::vec3(-2.5f, cube1y, -2.5f);
	Cube cube1(window, cube1Pos, 1.f, color, empty, &camera);
	cube1.rotate(45.f, glm::vec3(0.f, 0.f, 1.f));
	cube1.rotate(45.f, glm::vec3(0.f, 1.f, 0.f));

	glm::vec3 cube2Pos = glm::vec3(2.0f, -.5f, -2.5f);
	Cube cube2(window, cube2Pos, 1.f, glm::vec4(.2f, .5f, .8f, 1.f), tex, &camera);

	glm::vec3 cube3Pos = glm::vec3(0.0f, 2.f, -2.5f);
	Cube cube3(window, cube3Pos, 1.f, glm::vec4(.1f, .8f, .3f, 1.f), empty, &camera);

	int level = 2;
	glm::vec3 sphere1Pos = glm::vec3(0.0f, 5.0f, -10.0f);
	glm::vec3 sphere1Radi = glm::vec3(1.0f, 1.0f, 1.0f);
	Sphere sphere1(window, sphere1Pos, sphere1Radi, 1.0f, level, false, glm::vec4(.8f, .2f, .5f, 1.f), empty, &camera);
	sphere1.isSolid = true;
	sphere1.rotate(45, glm::vec3(0.f, 0.f, 1.f));
	sphere1.setLevel(level);

	//glm::vec3 test1 = glm::vec3(1.f, 1.f, 0.f);
	//glm::vec3 test2 = glm::vec3(0.f, 1.f, 1.f);
	//glm::vec3 test3 = glm::vec3(0.f, 0.f, 1.f);
	//
	//glm::vec3 test_13_23 = glm::cross(test1 - test3, test2 - test3);
	//std::cout << "test_13_23 = (" << test_13_23.x << ", " << test_13_23.y << ", " << test_13_23.z << ")\n";
	//std::cout << "dot(n, -n) = " << glm::dot(test_13_23, -test_13_23) << "\n";
	//
	//glm::vec3 test_23_13 = glm::cross(test2 - test3, test1 - test3);
	//std::cout << "test_23_13 = (" << test_23_13.x << ", " << test_23_13.y << ", " << test_23_13.z << ")\n";
	//std::cout << "dot(n, -n) = " << glm::dot(test_23_13, -test_23_13) << "\n";
	//
	//glm::vec3 testCircleCenter = glm::vec3(0.f, 2.f, 10.f);
	//float t = glm::dot(test3 - testCircleCenter, testResult) / glm::dot(testResult, -testResult);
	//std::cout << "t = " << t << "\n";
	//
	//glm::vec3 testSpotOnPlane = testCircleCenter - testResult * t;
	//std::cout << "testSpotOnPlane = (" << testSpotOnPlane.x << ", " << testSpotOnPlane.y << ", " << testSpotOnPlane.z << ")\n";




	int coneLevel = 12;



	glm::vec3 cone1Pos = glm::vec3(2.0f, -1.5f, 2.5f);
	glm::vec3 cone1Tip = glm::vec3(0.0f, 2.0f, 0.0f);	// Tip Pos is relative to the cone's position.
	glm::vec4 cone1ShaftColor = glm::vec4(0.3f, 0.2f, 0.f, 1.f); //glm::vec4(1.f, .2f, .1f, 1.f);
	glm::vec4 cone1ConeColor = glm::vec4(0.2f, .8f, .1f, 1.f);

	//						_bottomRadius<---sqrt(2), _topRadius, _height, _isSmooth
	Cone cone1(window, cone1Pos, 1.f, coneLevel, 1.0f, 0.0f, cone1Tip, true, cone1ShaftColor, cone1ConeColor, empty, &camera);


	glm::vec3 tree1Pos = glm::vec3(0.f, 15.f, 0.f);
	glm::vec3 tree1PointPos = glm::vec3(0.f, 5.f, 0.f);
	int tree1ConeLevel = 5, tree1SphereLevel = 3;
	//(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, int _coneLevel, int _sphereLevel, float _bottomRadius, glm::vec3 _pointPos, bool _isSmooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera);
	Tree tree1(window, tree1Pos, 1.f, tree1ConeLevel, tree1SphereLevel, 1.0f, tree1PointPos, true, cone1ShaftColor, tex, &camera);

	std::vector <Object*> objectList;
	std::vector <Cone*> coneList;
	std::vector <Tree*> treeList;

	

	glm::vec3 newConePos = cone1Pos;
	coneLevel = 3;

	int hashTableSize = planea.vertices.size() / 2;
	HashTable hashTable(hashTableSize);
	int hashCount = 0;
	//std::cout << "hashTableSize = " << hashTableSize << "\n";
	//for (const Vertex& v : planea.vertices) {
	//	int index = hashTable.isInTable(v.pos);
	//	hashCount++;
	//	if (index == -1){//}&& hashCount % 2 == 0) {
	//		//std::cout << "index = " << index << "\n";
	//		hashTable.addItem(v.pos);
	//		glm::vec3 treePos = glm::vec3(v.pos.x * planeaScale, v.pos.y * planeaScale , v.pos.z * planeaScale) + planeaPos;
	//		treeList.push_back(new Tree(window, treePos, 0.4f, tree1ConeLevel, tree1SphereLevel, 1.0f, tree1PointPos, true, cone1ShaftColor, tex, &camera));
	//		//coneList.push_back(new Cone(window, treePos, 1.f, coneLevel, 1.0f, 0.0f, cone1Tip, true, cone1ShaftColor, cone1ConeColor, empty, &camera));
	//	}
	//	//std::cout << "Not in table, index = " << index << "\n";
	//}
	//objectList.push_back(&tree1);
	objectList.push_back(&sphere1);
	//objectList.push_back(&cube1);
	//objectList.push_back(&cube2);
	//objectList.push_back(&cube3);


	int startingSphereIndex = objectList.size();
	//for (const Vertex &vv : sphere1.vertices) {
	//	glm::vec3 sphereNewPos1 = vv.pos + (vv.norm * 5.f) + glm::vec3(0.f, 5.f, 0.f);//(1.f / (1000.0f / float(objectList.size())));
	//	//glm::vec3 sphereNewPos2 = vv.pos + (vv.norm * 5.f) + glm::vec3(0.f, 5.f, 0.f);//(10.f / (1000.0f / float(objectList.size())));
	//	glm::vec3 sphereNewRadi = glm::vec3(1.0f, 1.0f, 1.0f);
	//	objectList.push_back(new Sphere(window, sphereNewPos1, sphereNewRadi, 0.1f, 4, false, glm::vec4(.8f, .2f, .2f, 1.f), empty, &camera));
	//	//objectList.push_back(new Sphere(window, sphereNewPos2, sphereNewRadi, 0.1f, 4, false, glm::vec4(.8f, .2f, .2f, 1.f), empty, &camera));
	//}

	//for (int vv = 0; vv < sphere1.vertices.size() / (1 + (level - 2) * 2); vv++) {
	//	glm::vec3 sphereNewPos1 = sphere1.vertices[vv].pos + (sphere1.vertices[vv].norm * 5.f) + glm::vec3(0.f, 5.f, 0.f);//(1.f / (1000.0f / float(objectList.size())));
	//	glm::vec3 sphereNewPos2 = sphere1.vertices[vv].pos + (sphere1.vertices[vv].norm * 10.f) + glm::vec3(0.f, 5.f, 0.f);//(1.f / (1000.0f / float(objectList.size())));
	//	glm::vec3 sphereNewRadi = glm::vec3(1.0f, 1.0f, 1.0f);
	//	objectList.push_back(new Sphere(window, sphereNewPos1, sphereNewRadi, 0.1f, 4, false, glm::vec4(.8f, .2f, .2f, 1.f), empty, &camera));
	//	objectList.push_back(new Sphere(window, sphereNewPos2, sphereNewRadi, 0.1f, 4, false, glm::vec4(.8f, .2f, .2f, 1.f), empty, &camera));
	//}

	float camSpeed = 10.f;
	camera.set_camera_speed(camSpeed);
	bool randomColor = true;
	bool isSmooth = true;

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("object.vert", "object.frag");

	// Create floor mesh
	//Mesh floor(verts, ind, tex);

	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);


	glm::vec3 x_axis = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 y_axis = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 z_axis = glm::vec3(0.f, 0.f, 1.f);

	glm::vec3 x_cross_y = glm::cross(x_axis, y_axis);
	glm::vec3 y_cross_x = glm::cross(y_axis, x_axis);

	std::cout << "x_cross_y = (" << x_cross_y.x << ", " << x_cross_y.y << ", " << x_cross_y.z << ")\n";
	std::cout << "x_cross_y = (" << y_cross_x.x << ", " << y_cross_x.y << ", " << y_cross_x.z << ")\n";

	planeList.push_back(&planea);

	planeList.push_back(&planeb);
	planeb.rotate(45, glm::vec3(0.f, 0.f, 1.f));
	planeb.setLevel(planeLevel);
	
	planeList.push_back(&planec);
	planec.rotate(-45, glm::vec3(0.f, 0.f, 1.f));
	planec.setLevel(planeLevel);
	
	planeList.push_back(&planed);
	planed.rotate(180, glm::vec3(0.f, 0.f, 1.f));
	planed.setLevel(planeLevel);





	float planeScale1 = 5.f;

	float topRadius = 0.f;


	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTex"), 1);
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexSpec"), 1);

	// Main while loop
	float lastTime = glfwGetTime();
	int frames = 0;

	glfwSetScrollCallback(window, scroll_callback);	

	bool trackSphere = false;

	glm::vec3 pushingForce = glm::vec3(-10.f, 0.0f, 0.0f);
	//sphere1.applyForce(Force(sphere1.objPos - pushingForce, glm::vec3(-200.f, 0.0f, 0.0f)));

	while (!glfwWindowShouldClose(window))
	{
		frames++;
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Simple timer
		double crntTime = glfwGetTime();
		if (floor(crntTime) > floor(lastTime)) {
			std::cout << "Frames/second = " << frames << "\n";
			lastTime = crntTime;
			frames = 0;
		}

		if (crntTime - prevTime >= 1 / 60)
		{
			//rotation = 0.1f;
			prevTime = crntTime;
			//topRadius += 0.002f;
		}

		
		//planea.rotate(rotation, glm::vec3(1.f, 0.f, 0.f));


		//float newTopRadius = (1 - glm::fract(topRadius)) * (int(glm::floor(topRadius)) % 2) + (glm::fract(topRadius)) * (int(glm::floor(topRadius + 1)) % 2);
		//cone1.setHeight(newTopRadius);
		//cone1.setTopRadius(newTopRadius);
		//cone1.setBottomRadius(newTopRadius);

		//camera.setMousePos(sphere1.objPos - camera.cameraPos, true, true);


		sphere1.applyForce(Force(sphere1.objPos + glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.0f, -9.81f, 0.0f)));

#if 1
		startPlaneTime = glfwGetTime();
		for (int pp = 0; pp < planeList.size(); pp++) {
			if (planeList[pp]->isTouching(&sphere1)) {
				//std::cout << "plane[" << pp << "] (" << planeList[pp]->objPos.x << ", " << planeList[pp]->objPos.y << ", " << planeList[pp]->objPos.z << ") is touching sphere1 (" << sphere1.objPos.x << ", " << sphere1.objPos.y << ", " << sphere1.objPos.z << ")\n";
				glm::vec3 newSpherePos = planeList[pp]->objPos;
				glm::vec3 newSphereRadi = glm::vec3(planeList[pp]->objRadius * planeList[pp]->objScale);
				glm::vec4 color = planeList[pp]->color;
				
				Sphere newSphere(window, newSpherePos, newSphereRadi, 1.f, 3, true, color, empty, &camera);
				newSphere.isWireframe = true;
				newSphere.draw(lightPos, lightColor);


				glm::vec3 standardPos = glm::vec3(0.f);
				for (int ss = startingSphereIndex; ss < objectList.size(); ss++) {
					objectList[ss]->setNewPos(standardPos);
				}


				int sphereIndex = startingSphereIndex;
				glm::vec3 finalAveragePoint = glm::vec3(0.f);
				glm::vec3 finalPlaneNorm = glm::vec3(0.f);


				for (Triangle tri : planeList[pp]->triangles) {
					if (sphere1.isTouching(&tri)) {
						//newSphere.isWireframe = true;
						//newSphere.draw(lightPos, lightColor);
						//glm::vec3 newSpherePos = tri.center;
						//glm::vec3 newSphereRadi = glm::vec3(tri.radius);
						//glm::vec4 color = tri.v1->color;
						//
						//Sphere newSphere(window, newSpherePos, newSphereRadi, 1.f, 3, true, color, empty, &camera);
						//newSphere.isWireframe = true;
						//newSphere.draw(lightPos, lightColor);

						//int sphereIndex = startingSphereIndex;
						for (int tt = 0; tt < sphere1.triangles.size(); tt++) {
							

							glm::vec3 p11 = glm::vec3(-10000.f), p12 = glm::vec3(-10000.f), p13 = glm::vec3(-10000.f);
							glm::vec3 p21 = glm::vec3(-10000.f), p22 = glm::vec3(-10000.f), p23 = glm::vec3(-10000.f);
							sphere1.isTouching(&tri, tt, &p11, &p12, &p13, &p21, &p22, &p23);

							//if (sphere1.isTouching(&tri, tt, &p11, &p12, &p13, &p21, &p22, &p23)) {
								//std::cout << "Touching triangle centered at (" << tri.center.x << ", " << tri.center.y << ", " << tri.center.z << ")\n";
								//glm::vec3 newSpherePos = tri.center;
								//glm::vec3 newSphereRadi = glm::vec3(tri.radius);
								//glm::vec4 color = tri.v1->color;
								//Sphere newSphere(window, newSpherePos, newSphereRadi, 1.f, 3, true, color, empty, &camera);
								//newSphere.isWireframe = true;
								//newSphere.draw(lightPos, lightColor);
#if 0
								if (p11.x > -10000.f) {
									//std::cout << "p1 = (" << p1.x << ", " << p1.y << ", " << p1.z << ")\n";


									newSphere.isWireframe = true;
									newSphere.draw(lightPos, lightColor);
									glm::vec3 newSpherePos = p11;// sphere1.triangles[tt].center;
									glm::vec3 newSphereRadi = glm::vec3(sphere1.triangles[tt].radius) / 5.f;
									glm::vec4 color = sphere1.triangles[tt].v1->color;


									Sphere newSphere(window, newSpherePos, newSphereRadi, 1.f, 3, true, color, empty, &camera);
									newSphere.isWireframe = true;
									newSphere.draw(lightPos, lightColor);
								}
								if (p21.x > -10000.f) {
									//std::cout << "p1 = (" << p1.x << ", " << p1.y << ", " << p1.z << ")\n";


									newSphere.isWireframe = true;
									newSphere.draw(lightPos, lightColor);
									glm::vec3 newSpherePos = p21;// sphere1.triangles[tt].center;
									glm::vec3 newSphereRadi = glm::vec3(sphere1.triangles[tt].radius) / 5.f;
									glm::vec4 color = sphere1.triangles[tt].v1->color;


									Sphere newSphere(window, newSpherePos, newSphereRadi, 1.f, 3, true, color, empty, &camera);
									newSphere.isWireframe = true;
									newSphere.draw(lightPos, lightColor);
								}


								if (p12.x > -10000.f) {
									//std::cout << "p2 = (" << p2.x << ", " << p2.y << ", " << p2.z << ")\n";

								}
								if (p22.x > -10000.f) {
									//std::cout << "p2 = (" << p2.x << ", " << p2.y << ", " << p2.z << ")\n";


									newSphere.isWireframe = true;
									newSphere.draw(lightPos, lightColor);
									glm::vec3 newSpherePos = p22;// sphere1.triangles[tt].center;
									glm::vec3 newSphereRadi = glm::vec3(sphere1.triangles[tt].radius) / 5.f;
									glm::vec4 color = sphere1.triangles[tt].v1->color;


									Sphere newSphere(window, newSpherePos, newSphereRadi, 1.f, 3, true, color, empty, &camera);
									newSphere.isWireframe = true;
									newSphere.draw(lightPos, lightColor);
								}




								if (p13.x > -10000.f) {
									//std::cout << "p3 = (" << p3.x << ", " << p3.y << ", " << p3.z << ")\n";


									newSphere.isWireframe = true;
									newSphere.draw(lightPos, lightColor);
									glm::vec3 newSpherePos = p13;// sphere1.triangles[tt].center;
									glm::vec3 newSphereRadi = glm::vec3(sphere1.triangles[tt].radius) / 5.f;
									glm::vec4 color = sphere1.triangles[tt].v1->color;


									Sphere newSphere(window, newSpherePos, newSphereRadi, 1.f, 3, true, color, empty, &camera);
									newSphere.isWireframe = true;
									newSphere.draw(lightPos, lightColor);
								}
								if (p23.x > -10000.f) {
									//std::cout << "p3 = (" << p3.x << ", " << p3.y << ", " << p3.z << ")\n";


									newSphere.isWireframe = true;
									newSphere.draw(lightPos, lightColor);
									glm::vec3 newSpherePos = p23;// sphere1.triangles[tt].center;
									glm::vec3 newSphereRadi = glm::vec3(sphere1.triangles[tt].radius) / 5.f;
									glm::vec4 color = sphere1.triangles[tt].v1->color;


									Sphere newSphere(window, newSpherePos, newSphereRadi, 1.f, 3, true, color, empty, &camera);
									newSphere.isWireframe = true;
									newSphere.draw(lightPos, lightColor);
								}
#endif
#if 1

								if (p11.x > -10000.f) {
									finalAveragePoint += p11;
									finalPlaneNorm += glm::normalize(glm::cross(tri.vec_1 - tri.vec_2, tri.vec_3 - tri.vec_2));
									//objectList[sphereIndex]->setNewPos(p11);
									//objectList[sphereIndex]->isWireframe = true;
									sphereIndex += 1;
								}
								if (p21.x > -10000.f) {
									finalAveragePoint += p21;
									finalPlaneNorm += glm::normalize(glm::cross(tri.vec_1 - tri.vec_2, tri.vec_3 - tri.vec_2));
									//objectList[sphereIndex]->setNewPos(p21);
									//objectList[sphereIndex]->isWireframe = true;
									sphereIndex += 1;
								}


								if (p12.x > -10000.f) {
									finalAveragePoint += p12;
									finalPlaneNorm += glm::normalize(glm::cross(tri.vec_1 - tri.vec_2, tri.vec_3 - tri.vec_2));
									//objectList[sphereIndex]->setNewPos(p12);
									//objectList[sphereIndex]->isWireframe = true;
									sphereIndex += 1;

								}
								if (p22.x > -10000.f) {
									finalAveragePoint += p22;
									finalPlaneNorm += glm::normalize(glm::cross(tri.vec_1 - tri.vec_2, tri.vec_3 - tri.vec_2));
									//objectList[sphereIndex]->setNewPos(p22);
									//objectList[sphereIndex]->isWireframe = true;
									sphereIndex += 1;
								}




								if (p13.x > -10000.f) {
									finalAveragePoint += p13;
									finalPlaneNorm += glm::normalize(glm::cross(tri.vec_1 - tri.vec_2, tri.vec_3 - tri.vec_2));
									//objectList[sphereIndex]->setNewPos(p13);
									//objectList[sphereIndex]->isWireframe = true;
									sphereIndex += 1;									
								}
								if (p23.x > -10000.f) {
									finalAveragePoint += p23;
									finalPlaneNorm += glm::normalize(glm::cross(tri.vec_1 - tri.vec_2, tri.vec_3 - tri.vec_2));
									//objectList[sphereIndex]->setNewPos(p23);
									//objectList[sphereIndex]->isWireframe = true;
									sphereIndex += 1;
								}
#endif
							//}
						}
					}
				}
				newSphere.isWireframe = true;
				newSphere.draw(lightPos, lightColor);
				glm::vec3 finalSpherePos = finalAveragePoint / float(sphereIndex - startingSphereIndex);// sphere1.triangles[tt].center;
				finalPlaneNorm /= float(sphereIndex - startingSphereIndex);;
				glm::vec3 finalSphereRadi = glm::vec3(1.f);
				glm::vec4 finalSphereColor = glm::vec4(0.0f, 0.2, 1.f, 1.f);

				if (sphereIndex > startingSphereIndex) {
					//sphere1.applyForce(Force(finalSpherePos, -sphere1.velocity / (1.f / 60.f)));

					//Triangle *t = &planeList[pp]->triangles[0];
					//t->genCircle();
					//glm::vec3 finalPlaneNorm = glm::normalize(glm::cross(t->vec_1 - t->vec_2, t->vec_3 - t->vec_2));


					//glm::vec3 planeNorm = transpose(inverse(glm::mat3(planeList[pp]->model))) * planeList[pp]->triangles[0].v1->norm;
					//std::cout << "planeNorm = (" << finalPlaneNorm.x << ", " << finalPlaneNorm.y << ", " << finalPlaneNorm.z << ")\n";
					//std::cout << "returnVec = (" << returnVec.x << ", " << returnVec.y << ", " << returnVec.z << ")\n";
					//glm::vec3 returnVec = sphere1.isPointInsideModel(finalSpherePos);
					//sphere1.objPos += returnVec;

					sphere1.reflectAbout(Force(finalSpherePos, finalPlaneNorm));

					//glm::vec3 dragDir = glm::normalize(glm::cross(glm::cross(-sphere1.velocity, planeNorm), planeNorm));
					//if (glm::length(dragDir) > 0.001f) {
					//	std::cout << "doin drag\n";
					//	sphere1.applyForce(Force(finalSpherePos, -dragDir * 10.f));
					//}
				}


				Sphere finalSphere(window, finalSpherePos, finalSphereRadi, 0.1f, 3, true, finalSphereColor, empty, &camera);
				//finalSphere.isWireframe = true;
				finalSphere.draw(lightPos, lightColor);
				//std::cout << "sphereIndex = " << sphereIndex << "\n";
			}
			//else									std::cout << "plane[" << pp << "] (" << planeList[pp]->objPos.x << ", " << planeList[pp]->objPos.y << ", " << planeList[pp]->objPos.z << ") is NOT touching sphere1 (" << sphere1.objPos.x << ", " << sphere1.objPos.y << ", " << sphere1.objPos.z << ")\n";
			//if (sphere1.isTouching(planeList[pp]))	std::cout << "sphere1 (" << sphere1.objPos.x << ", " << sphere1.objPos.y << ", " << sphere1.objPos.z << ") is touching plane[" << pp << "] (" << planeList[pp]->objPos.x << ", " << planeList[pp]->objPos.y << ", " << planeList[pp]->objPos.z << ")\n\n";
			//else									std::cout << "sphere1 (" << sphere1.objPos.x << ", " << sphere1.objPos.y << ", " << sphere1.objPos.z << ") is NOT touching plane[" << pp << "] (" << planeList[pp]->objPos.x << ", " << planeList[pp]->objPos.y << ", " << planeList[pp]->objPos.z << ")\n\n";
		}
		//std::cout << "Searching through the planes took " << glfwGetTime() - startPlaneTime << " seconds\n";
#endif
		//sphere1.applyForce(Force(glm::vec3(0.f, 10000.f, 0.f), glm::vec3(0.0f, -9.81f, 0.0f)));
		sphere1.getNextPos();

		//sphere1.applyForce(Force(sphere1.objPos + sphere1.velocity, -sphere1.velocity * .1f));

		if (!shouldFly && trackSphere)
			camera.setMousePos(sphere1.objPos - camera.cameraPos, true, true);
		
		if (setCamera != 0) {
			if (setCamera > 0)
				camSpeed *= 1.5f;
			else 
				camSpeed /= 1.5f;
			camera.set_camera_speed(camSpeed);
			setCamera = 0;
		}
		// Rotating the view about the x and y axis, and loads of other neat things.
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_F) && lockoutTimer <= crntTime) {
			lockoutTimer = crntTime + 0.2;
			capturingMotion = !capturingMotion;
			camera.motion_enabled(capturingMotion);
			std::cout << "Crotation\n";
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) {
			tree1.rotate(.5f, glm::vec3(0.f, -1.f, 0.f));
			sphere1.applyForce(Force(sphere1.objPos + glm::vec3(1.f, 0.f, 0.f), glm::vec3(-camSpeed / 100.f, 0.f, 0.f)));
			//sphere1.setNewPos(sphere1.objPos + glm::vec3(-camSpeed / 100.f, 0.f, 0.f));
			//sphere1.rotate(.5f, glm::vec3(0.f, -1.f, 0.f));
			//planeaPos.x -= 0.01f;
			//planea.setNewPos(planeaPos);
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
			tree1.rotate(.5f, glm::vec3(-1.f, 0.f, 0.f));
			sphere1.applyForce(Force(sphere1.objPos + glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, -camSpeed / 100.f)));
			//sphere1.setNewPos(sphere1.objPos + glm::vec3(0.0f, 0.f, -camSpeed / 100.f));
			//sphere1.rotate(.5f, glm::vec3(-1.f, 0.f, 0.f));
			//planeaPos.y += 0.01f;
			//planea.setNewPos(planeaPos);
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
			tree1.rotate(.5f, glm::vec3(1.f, 0.f, 0.f));
			sphere1.applyForce(Force(sphere1.objPos + glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, camSpeed / 100.f)));
			//sphere1.setNewPos(sphere1.objPos + glm::vec3(0.0f, 0.f, camSpeed / 100.f));
			//sphere1.rotate(.5f, glm::vec3(1.f, 0.f, 0.f));
			//planeaPos.y -= 0.01f;
			//planea.setNewPos(planeaPos);
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)) {
			tree1.rotate(.5f, glm::vec3(0.f, 1.f, 0.f));
			sphere1.applyForce(Force(sphere1.objPos + glm::vec3(-1.f, 0.f, 0.f), glm::vec3(camSpeed / 100.f, 0.f, 0.f)));
			//sphere1.setNewPos(sphere1.objPos + glm::vec3(camSpeed / 100.f, 0.f, 0.f));
			//sphere1.rotate(.5f, glm::vec3(0.f, 1.f, 0.f));
			//planeaPos.x += 0.01f;
			//planea.setNewPos(planeaPos);
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Z)) {
			sphere1.setNewPos(sphere1.objPos + glm::vec3(0.0f, -camSpeed / 100.f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_X)) {
			sphere1.setNewPos(sphere1.objPos + glm::vec3(0.0f, camSpeed / 100.f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_B)) {
			sphere1.rotate(.5f, glm::vec3(0.f, 1.f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_MINUS) && lockoutTimer <= crntTime) {
			//planeLevel--;
			//coneLevel--;
			//std::cout << "\nLevel: " << level << "\n";
			//sphere1.setLevel(level);
			//for (Plane* p : planeList)
			//	p->setLevel(planeLevel);
			//cone1.setLevel(coneLevel);
			tree1ConeLevel--;
			tree1.setConeLevel(tree1ConeLevel);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_EQUAL) && lockoutTimer <= crntTime) {
			//planeLevel++;
			//coneLevel++;
			//std::cout << "\nLevel: " << level << "\n";
			//sphere1.setLevel(level);
			//for (Plane* p : planeList)
			//	p->setLevel(planeLevel);
			tree1ConeLevel++;
			tree1.setConeLevel(tree1ConeLevel);
			//cone1.setLevel(coneLevel);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_9) && lockoutTimer <= crntTime) {
			//std::cout << "\nLevel: " << level << "\n";
			//planeScale1 -= 1.f;
			//
			//for (Plane* p : planeList) {
			//	p->setScale(planeScale1);
			//	p->setLevel(planeLevel);
			//}
			//tree1SphereLevel--;
			//tree1.setSphereLevel(tree1SphereLevel);
			//lockoutTimer = crntTime + 0.2;
			planea.rotate(.5f, glm::vec3(0.f, 0.f, 1.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_0) && lockoutTimer <= crntTime) {
			//std::cout << "\nLevel: " << level << "\n";
			//planeScale1 += 1.f;
			//for (Plane* p : planeList) {
			//	p->setScale(planeScale1);
			//	p->setLevel(planeLevel);
			//}
			//tree1SphereLevel++;
			//tree1.setSphereLevel(tree1SphereLevel);
			//lockoutTimer = crntTime + 0.2;
			planea.rotate(-.5f, glm::vec3(0.f, 0.f, 1.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_R) && lockoutTimer <= crntTime) {
			//sphere1.doRandomColors(randomColor);
			//sphere1.setLevel(level);
			//
			////plane2.doRandomColors(randomColor);
			////plane2.setLevel(planeLevel);
			//for (Plane* p : planeList) {
			//	p->doRandomColors(randomColor);
			//	p->setLevel(planeLevel);
			//}
			//
			//
			//cone1.doRandomColors(randomColor);
			//cone1.setLevel(coneLevel);
			sphere1.setNewPos(glm::vec3(0.f));

			tree1.doRandomColors(randomColor);
			tree1.setConeLevel(tree1ConeLevel);

			randomColor = !randomColor;
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_N) && lockoutTimer <= crntTime) {
			//sphere1.toggleNormalArrows();
			//sphere1.setLevel(level);

			//cube2.toggleNormalArrows();
			//cube2.reGenTriangles();


			//for (Plane* p : planeList) {
			//	p->toggleNormalArrows();
			//	p->setLevel(planeLevel);
			//}

			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_C) && lockoutTimer <= crntTime) {
			sphere1.reseed();
			sphere1.setLevel(level);

			for (Plane* p : planeList) {
				p->reseed();
				p->setLevel(planeLevel);
			}
			lockoutTimer = crntTime + 0.2;
		}

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Y) && lockoutTimer <= crntTime) {
			isSmooth = !isSmooth;
			sphere1.smoothSurface(isSmooth);
			sphere1.setLevel(level);
			//
			//cone1.smoothSurface(isSmooth);
			//cone1.setLevel(coneLevel);
			tree1.smoothSurface(isSmooth);
			tree1.setConeLevel(tree1ConeLevel);

			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_T) && lockoutTimer <= crntTime) {
			std::cout << "\nLevel: " << level << "\n";
			lockoutTimer = crntTime + 0.2;
			std::cout << "Flyin' in\n";
			trackSphere = !trackSphere;
			shouldFly = trackSphere;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_I) && lockoutTimer <= crntTime) {
			std::cout << "\nMessing with Vertex: " << level << "\n";
			lockoutTimer = crntTime + 0.2;
			planea.moveFirstVertex();
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_H) && lockoutTimer <= crntTime) {
			std::cout << "\nReloading Shader: " << level << "\n";
			lockoutTimer = crntTime + 0.2;
			for (auto pln : planeList)
				pln->hotRealoadShader();
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_G) && lockoutTimer <= crntTime) {
			std::cout << "\nAdding Gravity\n";
			lockoutTimer = crntTime + 0.2;
			
		}
		if (shouldFly) {
			camera.set_camera_speed(2.f);
			camSpeed = 10.f;
			if (camera.fly_to(sphere1.objPos + glm::vec3(0.f, 0.f, 3.f), sphere1.objPos - camera.cameraPos, true)) {
				shouldFly = false;
				camera.set_camera_speed(camSpeed);
			}
		}

		// Toggling the tracking of user movement
		camera.track_movement();
		//plane1.moveFirstVertex();

		// Setting view matrix with camera class
		view = camera.get_view();

		//floor.draw(camera, shaderProgram);

		// Drawing all the Objects in the list.
		for (auto obj : objectList)
			obj->draw(lightPos, lightColor);

		for (auto cn : coneList)
			cn->draw(lightPos, lightColor);

		for (auto tr : treeList)
			tr->draw(lightPos, lightColor);

		for (Plane* p : planeList) {
			p->draw(lightPos, lightColor);
		}


		// Drawing the Light.
		lightShader.Activate();
		camera.camMatrixForShader(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndicies) / sizeof(int), GL_UNSIGNED_INT, 0);



		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	//for (auto obj : objectList)
	//	delete obj;
	delete[] verts2;
	delete[] indices2;
	delete[] norms2;
	delete[] finalVerts2;
	return 0;
}
#endif
