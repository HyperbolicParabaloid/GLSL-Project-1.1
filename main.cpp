#include"Mesh.h"
#include"Cube.h"
#include"Sphere.h"
#include"Plane.h"

#define PI 3.1415926538

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
int level;

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

int viewWidth = 1600, viewHeight = 1600;

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

	//Cube cube(window, glm::vec3(0.f, 0.f, 0.f), 1.f, glm::vec4(1.f));

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("object.vert", "object.frag");//Shader shaderProgram("default.vert", "default.frag");

	// Using Texture class to handle all the binding, activating and parameter setting of the texture.
	//Texture tex("137215-barack-face-vector-obama-png-image-high-quality.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	//tex.texUnit(shaderProgram, "tex0", 0);
	//tex.Bind();	REMEMBERTODELETE

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices	REMEMBERTODELETE
	VBO VBO1(sqrVertices_2, sizeof(sqrVertices_2));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(sqrIndices_2, sizeof(sqrIndices_2));

	// Links VBO attributes such as coordinates and colors to VAO	REMEMBERTODELETE
	//VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	//VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	//VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	//VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 10 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 4, GL_FLOAT, 10 * sizeof(float), (void*)(6 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


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
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(-1.0f, .0f, -1.75f);
	glm::mat4 pyramidModel = glm::mat4(1.f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);
	//pyramidModel = glm::rotate(pyramidModel, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Activating shader's and setting up model uniforms.
	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);




	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	GLuint rotationUniID = glGetUniformLocation(shaderProgram.ID, "rotationDegree");

	float frac, whole;
	glUniform1f(uniID, 0.5);
	
	float rotation = 0.f, prevTime = glfwGetTime();
	float motion = 0.0001f;

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Initializing matrices for model, view and projection
	glm::mat4 view = glm::mat4(1.f);

	Camera camera(window, glm::vec2(viewWidth, viewHeight), glm::vec3(0.f, 0.5f, 2.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	camera.set_projection(glm::radians(45.f), (float)(viewWidth / viewHeight), 0.1f, 100.f);

	double lockoutTimer = 0;
	bool shouldRotate = false, shouldFly = false, capturingMotion = false;

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
			rotation = 0.1f;
			prevTime = crntTime;
			//pyramidPos.x += motion;
			//lightPos.x += motion;
		}

		//std::cout << "Pos = " << pyramidPos.x << "\n";

		//if (abs(pyramidPos.x) > .01f)
			//motion = -motion;

		// Rotating the model about the y axis
		//pyramidModel = glm::rotate(pyramidModel, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		//pyramidModel = glm::translate(pyramidModel, pyramidPos);
		// TL:DR the rotation won't do anything cool because the triangles normals aren't based off the models position
		// or rotation so they're always facing in the same direction.

		// Rotating the view about the x and y axis
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_F) && lockoutTimer <= crntTime) {
			lockoutTimer = crntTime + 0.2;
			capturingMotion = !capturingMotion;
			camera.motion_enabled(capturingMotion);
			std::cout << "Crotation\n";
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_G) && lockoutTimer <= crntTime) {
			
		}

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_T) && lockoutTimer <= crntTime) {
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
		camera.track_movement();

		// Setting view matrix with camera class
		view = camera.get_view();

		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
		//glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
		camera.camMatrixForShader(shaderProgram, "camMatrix");

		//tex.Bind();	REMEMBERTODELETE

		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();

		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(sqrIndices_2) / sizeof(int), GL_UNSIGNED_INT, 0);	// REMEMBERTODELETE

		lightShader.Activate();
		camera.camMatrixForShader(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndicies) / sizeof(int), GL_UNSIGNED_INT, 0);


		//cube.draw(lightPos, camera);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	// Making sure to use the tex classes delete function.
	//tex.Delete(); REMEMBERTODELETE
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
#endif

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
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
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

	Camera camera(window, glm::vec2(viewWidth, viewHeight), glm::vec3(0.f, 0.5f, 2.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	camera.set_projection(glm::radians(60.f), (float)(viewWidth / viewHeight), 0.1f, 100.f);

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
	Plane plane1(window, plane1Pos, 40.f, true, glm::vec4(1.f, 1.f, 1.f, 1.f), tex, &camera);
	
	float cube1y = -1.5 + sqrt(3);
	glm::vec3 cube1Pos = glm::vec3(-2.5f, cube1y, -2.5f);
	Cube cube1(window, cube1Pos, 1.f, color, empty, &camera);
	cube1.rotate(45.f, glm::vec3(0.f, 0.f, 1.f));
	cube1.rotate(45.f, glm::vec3(0.f, 1.f, 0.f));
	
	glm::vec3 cube2Pos = glm::vec3(2.0f, -.5f, -2.5f);
	Cube cube2(window, cube2Pos, 1.f, glm::vec4(.2f, .5f, .8f, 1.f), empty, &camera);
	
	glm::vec3 cube3Pos = glm::vec3(0.0f, 2.f, -2.5f);
	Cube cube3(window, cube3Pos, 1.f, glm::vec4(.1f, .8f, .3f, 1.f), empty, &camera);

	glm::vec3 sphere1Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	Sphere sphere1(window, sphere1Pos, .5f, 2, true, glm::vec4(.8f, .2f, .5f, 1.f), empty, &camera);

	std::vector <Object*> objectList;

	objectList.push_back(&sphere1);
	objectList.push_back(&cube1);
	objectList.push_back(&cube2);
	objectList.push_back(&cube3);
	objectList.push_back(&plane1);


	//camera.set_camera_speed(10);
	level = 2;
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
			rotation = 0.1f;
			prevTime = crntTime;
		}

		// Rotating the view about the x and y axis, and loads of other neat things.
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_F) && lockoutTimer <= crntTime) {
			lockoutTimer = crntTime + 0.2;
			capturingMotion = !capturingMotion;
			camera.motion_enabled(capturingMotion);
			std::cout << "Crotation\n";
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) {
			sphere1.rotate(.5f, glm::vec3(0.f, -1.f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
			sphere1.rotate(.5f, glm::vec3(-1.f, 0.f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
			sphere1.rotate(.5f, glm::vec3(1.f, 0.f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)) {
			sphere1.rotate(.5f, glm::vec3(0.f, 1.f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_MINUS) && lockoutTimer <= crntTime) {
			level--;
			std::cout << "\nLevel: " << level << "\n";
			sphere1.setLevel(level);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_EQUAL) && lockoutTimer <= crntTime) {
			level++;
			std::cout << "\nLevel: " << level << "\n";
			sphere1.setLevel(level);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_R) && lockoutTimer <= crntTime) {			
			sphere1.doRandomColors(randomColor);
			sphere1.setLevel(level);
			randomColor = !randomColor;
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_C) && lockoutTimer <= crntTime) {
			sphere1.reseed();
			sphere1.setLevel(level);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Y) && lockoutTimer <= crntTime) {
			isSmooth = !isSmooth;
			sphere1.smoothSurface(isSmooth);
			sphere1.setLevel(level);
			lockoutTimer = crntTime + 0.2;
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
		camera.track_movement();

		// Setting view matrix with camera class
		view = camera.get_view();

		//floor.draw(camera, shaderProgram);

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
	delete[] verts2;
	delete[] indices2;
	delete[] norms2;
	delete[] finalVerts2;
	return 0;
}
#endif