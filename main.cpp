#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ctime>
#include <time.h>
#include <stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/vec1.hpp>
#include<vector>
#include<algorithm>

#include"shaderClass.h"
#include"Texture.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include"Cube.h"
#include"Sphere.h"


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

int numVertsPerSide(int _level) {
	if (_level == 1)
		return 2;
	return 2 * numVertsPerSide(_level - 1) - 1;
}

void setPreIndices() {
	int vertsPerSide = pow(vertsSize, .5);
	int setpsPerSide = vertsPerSide - 2;
	
	// Resizing indices aray.
	indicesSize = pow(4, level - 1) * 2 * 3;
	GLuint* newInds = new GLuint[indicesSize];
	delete[] indices2;
	indices2 = newInds;

	int t0, t1, t2; // First triangle
	int t3, t4, t5; // Second triangle;

	int indCount = 0;
	for (int vv = 0; vv <= setpsPerSide; vv++) {
		for (int uu = 0; uu <= setpsPerSide; uu++) {
			//	+-+
			//	|\|
			//	+-+
			if ((uu >= setpsPerSide / 2 && vv < setpsPerSide / 2) || ((uu < setpsPerSide / 2 && vv >= setpsPerSide / 2))) {
				t0 = (uu + 0) + (vertsPerSide * (vv + 0));
				t1 = (uu + 0) + (vertsPerSide * (vv + 1));
				t2 = (uu + 1) + (vertsPerSide * (vv + 1));

				t3 = (uu + 1) + (vertsPerSide * (vv + 1));
				t4 = (uu + 1) + (vertsPerSide * (vv + 0));
				t5 = (uu + 0) + (vertsPerSide * (vv + 0));
			}

			//	+-+
			//	|/|
			//	+-+
			else {
				t0 = (uu + 1) + (vertsPerSide * (vv + 0));
				t1 = (uu + 0) + (vertsPerSide * (vv + 0));
				t2 = (uu + 0) + (vertsPerSide * (vv + 1));

				t3 = (uu + 0) + (vertsPerSide * (vv + 1));
				t4 = (uu + 1) + (vertsPerSide * (vv + 1));
				t5 = (uu + 1) + (vertsPerSide * (vv + 0));
			}
			indices2[indCount + 0] = t0;
			indices2[indCount + 1] = t1;
			indices2[indCount + 2] = t2;
			indices2[indCount + 3] = t3;
			indices2[indCount + 4] = t4;
			indices2[indCount + 5] = t5;
			indCount += 6;
		}
	}

}

void genOctahedron()
{
	if (level < 1)
		level = 1;
	if (level > 10)
		level = 10;

	// Num vertices per level
	//	1			2		3		4
	// 2x2=4 => 3x3=9 => 5x5=25 => 9x9=81
	// So basically, if you think about it you're doing:
	// NumVertsPerSide_X = NumVertsPerSide_X-1 + NumVertsPerSide_X-1 - 1
	int vertsPerSide = numVertsPerSide(level);

	// Resizing verts2
	vertsSize = vertsPerSide * vertsPerSide;
	glm::vec3* newVerts = new glm::vec3[vertsSize];
	delete[] verts2;
	verts2 = newVerts;

	float temp;
	for (int vv = 0; vv <= (vertsPerSide - 1); vv++) {
		for (int uu = 0; uu <= (vertsPerSide - 1); uu++) {
			GLfloat x, y, z = 0.f;
			x = ((uu * 2) - (vertsPerSide - 1)) / (float)(vertsPerSide - 1);	// Goes from -1.f =>  1.f
			y = ((vertsPerSide - 1) - (vv * 2)) / (float)(vertsPerSide - 1);	// Goes from  1.f => -1.f


			//std::cout << "Old: (\t" << x << ",\t" << y << ",\t" << z << ")\n";

			// Transforming the 2D plane to a 3D Octahedron.
			if (abs(x) + abs(y) <= 1)
				z = 1.f - (abs(x) + abs(y));
			else {
				temp = x;
				// 1st Quadrant
				if (x >= 0 && y >= 0) {
					x = 1 - y;
					y = 1 - temp;
				}

				// 2nd Quadrant
				else if (x < 0 && y >= 0) {
					x = -1 + y;
					y = 1 + temp;
				}

				// 3rd Quadrant
				else if (x >= 0 && y < 0) {
					x = 1 + y;
					y = -1 + temp;
				}

				// 4th Quadrant
				else if (x < 0 && y < 0) {
					x = -1 - y;
					y = -1 - temp;
				}
				z = -1.f + (abs(x) + abs(y));
			}
			//std::cout << "New: (\t" << x << ",\t" << y << ",\t" << z << ")\n";
			//std::cout << "\n";
	
			verts2[vertsPerSide * vv + uu] = glm::vec3(x, y, z);


		}
		std::cout << "\n";
	}
}

void setNorms() {
	
	int numTriangles = pow(4, level - 1) * 2;
	glm::vec3* newNorms = new glm::vec3[numTriangles];
	delete[] norms2;
	norms2 = newNorms;


	// Resizing final verts
	int numVertsPerTriangle = 3, numFloatsPerVert = 3, numFloatsPerNorm = 3, numFloatsPerColor = 4;


	finalVerts2Size = numTriangles * numVertsPerTriangle * numFloatsPerVert * numFloatsPerNorm * numFloatsPerColor;
	GLfloat* newFinalVerts2 = new GLfloat[finalVerts2Size];
	delete[] finalVerts2;
	finalVerts2 = newFinalVerts2;

	int normCount = 0;
	for (int ii = 0; ii < indicesSize; ii+=6) {
		glm::vec3 v1 = verts2[indices2[ii + 0]];
		glm::vec3 v2 = verts2[indices2[ii + 1]];
		glm::vec3 v3 = verts2[indices2[ii + 2]];

		glm::vec3 v4 = verts2[indices2[ii + 3]];
		glm::vec3 v5 = verts2[indices2[ii + 4]];
		glm::vec3 v6 = verts2[indices2[ii + 5]];

		glm::vec3 n1 = glm::cross(v3 - v2, v1 - v2);
		glm::vec3 n2 = glm::cross(v6 - v5, v4 - v5);

		norms2[normCount + 0] = n1;
		norms2[normCount + 1] = n2;

		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		glm::vec4 color1 = glm::vec4(r1, r2, r3, 1.f);

		float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r5 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r6 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		glm::vec4 color2 = glm::vec4(r4, r5, r6, 1.f);

		//						|	VERTEX		|	|	NORMAL		|	|				COLOR					|
		GLfloat tempVert1[] = { v1.x, v1.y, v1.z,	n1.x, n1.y, n1.z,	color1.r, color1.g, color1.b, color1.a };
		GLfloat tempVert2[] = { v2.x, v2.y, v2.z,	n1.x, n1.y, n1.z,	color1.r, color1.g, color1.b, color1.a };
		GLfloat tempVert3[] = { v3.x, v3.y, v3.z,	n1.x, n1.y, n1.z,	color1.r, color1.g, color1.b, color1.a };

		//						|	VERTEX		|	|	NORMAL		|	|				COLOR					|
		GLfloat tempVert4[] = { v4.x, v4.y, v4.z,	n2.x, n2.y, n2.z,	color2.r, color2.g, color2.b, color2.a };
		GLfloat tempVert5[] = { v5.x, v5.y, v5.z,	n2.x, n2.y, n2.z,	color2.r, color2.g, color2.b, color2.a };
		GLfloat tempVert6[] = { v6.x, v6.y, v6.z,	n2.x, n2.y, n2.z,	color2.r, color2.g, color2.b, color2.a };

		int steps = numFloatsPerVert + numFloatsPerNorm + numFloatsPerColor;

		// Copying the contents from the new temp verts into the finalVerts2 array
		std::copy(tempVert1, tempVert1 + steps, finalVerts2);
		std::copy(tempVert2, tempVert2 + steps, finalVerts2);
		std::copy(tempVert3, tempVert3 + steps, finalVerts2);
		std::copy(tempVert4, tempVert4 + steps, finalVerts2);
		std::copy(tempVert5, tempVert5 + steps, finalVerts2);
		std::copy(tempVert6, tempVert6 + steps, finalVerts2);
		normCount += 2;
	}
}

void setPostIndices() {
	for (int ii = 0; ii < indicesSize; ii++) {
		indices2[ii] = ii;
	}
}

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
	glm::vec3 cube1Pos = glm::vec3(-2.0f, -.5f, -2.5f);
	Cube cube1(window, cube1Pos, 1.f, color, &camera);
	cube1.rotate(45.f, glm::vec3(0.f, 0.f, 1.f));
	cube1.rotate(45.f, glm::vec3(0.f, 1.f, 0.f));

	glm::vec3 cube2Pos = glm::vec3(2.0f, -.5f, -2.5f);
	Cube cube2(window, cube2Pos, 1.f, glm::vec4(.2f, .5f, .8f, 1.f), &camera);

	glm::vec3 cube3Pos = glm::vec3(0.0f, 2.f, -2.5f);
	Cube cube3(window, cube3Pos, 1.f, glm::vec4(.1f, .8f, .3f, 1.f), &camera);

	glm::vec3 plane1Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	Sphere plane1(window, plane1Pos, .5f, 2, true, glm::vec4(.8f, .2f, .5f, 1.f), &camera);

	std::vector <Object*> objectList;

	objectList.push_back(&cube1);
	objectList.push_back(&cube2);
	objectList.push_back(&cube3);
	objectList.push_back(&plane1);

	camera.set_camera_speed(10);
	level = 2;
	bool randomColor = true;
	bool isSmooth = true;
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
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) {
			plane1.rotate(.5f, glm::vec3(0.f, -1.f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
			plane1.rotate(.5f, glm::vec3(1.f, 0.f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
			plane1.rotate(.5f, glm::vec3(-1.f, 0.f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)) {
			plane1.rotate(.5f, glm::vec3(0.f, 1.f, 0.f));
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_MINUS) && lockoutTimer <= crntTime) {
			level--;
			std::cout << "\nLevel: " << level << "\n";
			plane1.setLevel(level);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_EQUAL) && lockoutTimer <= crntTime) {
			level++;
			std::cout << "\nLevel: " << level << "\n";
			plane1.setLevel(level);
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_R) && lockoutTimer <= crntTime) {			
			plane1.doRandomColors(randomColor);
			plane1.setLevel(level);
			randomColor = !randomColor;
			lockoutTimer = crntTime + 0.2;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Y) && lockoutTimer <= crntTime) {
			isSmooth = !isSmooth;
			plane1.smoothSurface(isSmooth);
			plane1.setLevel(level);
			lockoutTimer = crntTime + 0.2;
		}

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_T) && lockoutTimer <= crntTime) {
			std::cout << "\nLevel: " << level << "\n";
			lockoutTimer = crntTime + 0.2;
			std::cout << "Flyin' in\n";
			shouldFly = true;
		}

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_C) && lockoutTimer <= crntTime) {
			std::cout << "\nLevel: " << level << "\n";
			

			std::cout << "\n";
			lockoutTimer = crntTime + 0.2;
			
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


		//cube1.draw(lightPos, lightColor);
		//cube2.draw(lightPos, lightColor);
		for (auto obj : objectList)
		{
			obj->draw(lightPos, lightColor);
		}

		

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