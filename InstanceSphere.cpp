#include "InstanceSphere.h"

// Constructor for InstanceSphere.
InstanceSphere::InstanceSphere(GLFWwindow* _window, unsigned int _instances, glm::vec3 _objPos, glm::vec3 _radi, float _objScale, int _level, bool _smooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) : Object(_window, _objPos, _objScale, _color, _textures, _camera) {
	level = _level;
	smooth = _smooth;
	randomColor = false;
	seed = 1;
	objRadius = 1.f;
	//radi = _radi;
	setScale(_radi * objScale);

	instances = _instances;
	genTriangles();
}


/*
* To explain what's going on: behold, my ASCII art.
* The sphere is made by creating a 2D plane as such,
* and wrapping it into a Octahedron.
*
* The Octahedron's points are then normalized, which
* creates a sphere.
*
* Tessellating the surface of the plane to different
* levels increases the number of triangles by 4 each
* time. Effectively making the surface of the sphere
* more and more smooth. With proper lighting, level 6
* is about all you need to make the sphere near perfect.
+---------------------+---------------------+
|                    /|\                    |
|                 /   |   \                 |
|               /     |     \               |
|            /        |        \            |
|          /          |          \          |
|        /            |            \        |
|     /               |               \     |
|   /                 |                 \   |
|/                    |                    \|
+---------------------+---------------------+
|\                    |                    /|
|   \                 |                 /   |
|     \               |               /     |
|        \            |            /        |
|          \          |          /          |
|            \        |        /            |
|              \      |     /               |
|                 \   |   /                 |
|                    \|/                    |
+---------------------+---------------------+

					 ||
					 ||
					 ||
					 ||
					 \/

+----------+----------+----------+----------+
|         /|         /|\         |\         |
|      /   |      /   |   \      |   \      |
|   /      |   /      |      \   |      \   |
|/         |/         |         \|         \|
+----------+----------+----------+----------+
|         /|         /|\         |\         |
|      /   |      /   |   \      |   \      |
|   /      |   /      |      \   |      \   |
|/         |/         |         \|         \|
+----------+----------+----------+----------+
|\         |\         |         /|         /|
|   \      |   \      |      /   |      /   |
|      \   |      \   |   /      |   /      |
|         \|         \|/         |/         |
+----------+----------+----------+----------+
|\         |\         |         /|         /|
|   \      |   \      |      /   |      /   |
|      \   |      \   |   /      |   /      |
|         \|         \|/         |/         |
+----------+----------+----------+----------+
*/

// Creates a new set of Vertex's and their associated indices to send to the Object
// class for drawing.
void InstanceSphere::genTriangles() {
	genOctahedron();
	setVBOandEBO("InstanceSphere");
}

void InstanceSphere::genOctahedron() {
	// Whenever we generate a new set a vertices and indices, we want to wipe the old ones.
	// In the future it'd be better to just add in the new vertices and update indices instead
	// of clearing both indices and verts and starting over but it's fine for now.
	//verticesGeneral.clear();
	//verticesIBall.clear();
	//balls.clear();
	//indices.clear();
	//vertices.clear();

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


	// Resizing verts and texCoords. 
	preVertsSize = vertsPerSide * vertsPerSide;

	glm::vec3* newPreVerts = new glm::vec3[preVertsSize];	delete[] preVerts;
	preVerts = newPreVerts;
	glm::vec2* newTexCoords = new glm::vec2[preVertsSize];	delete[] texCoords;
	texCoords = newTexCoords;

	float temp;
	for (int vv = 0; vv < vertsPerSide; vv++) {
		for (int uu = 0; uu < vertsPerSide; uu++) {

			// Can I just make uu and vv go from 0->1, and make that my UV texture coodiantes?
			// If so, I just need a new array to hold them, and assign them along with the preVerts.
			// then, when preVerts get mapped into postVerts inside of the setNorms function, I can
			// do the same thing with the texture coords. Perhaps. We shall see. Tomorrow. When I have coffee.
			//
			// I lied. A little. Should be something like:
			texCoords[vertsPerSide * vv + uu] = glm::vec2(static_cast<float>(-uu) / (vertsPerSide - 1), static_cast<float>(-vv) / (vertsPerSide - 1));	// (0->1, 0->1)
			GLfloat x, y, z = 0.f;
			x = ((uu * 2) - (vertsPerSide - 1)) / (float)(vertsPerSide - 1);	// Goes from -1.f =>  1.f
			y = ((vertsPerSide - 1) - (vv * 2)) / (float)(vertsPerSide - 1);	// Goes from  1.f => -1.f

			if (true) {
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
			}

			// Normalizing the vector, places the vertices of the Octehdron on the surface of the sphere.
			preVerts[vertsPerSide * vv + uu] = glm::normalize(glm::vec3(x, y, z));// *radi;
			//preVerts[vertsPerSide * vv + uu] = glm::vec3(x, y, z);
			//preVerts[vertsPerSide * vv + uu] = glm::vec3(-x, z, y);
		}
	}
	setVerticesVector();
	genBalls();
}

// Sets Vertices.
void InstanceSphere::setVerticesVector() {
	int vertsPerSide = numVertsPerSide(level);
	int setpsPerSide = vertsPerSide - 2;

	int t0, t1, t2; // First triangle
	int t3, t4, t5; // Second triangle;

	int indCount = 0;

	srand(seed);
	// This nested for-loop updates verts with all its Vertex information and
	// updates indices with all the right values.
	for (int vv = 0; vv <= setpsPerSide; vv++) {
		for (int uu = 0; uu <= setpsPerSide; uu++) {
			//	+-+
			//	|\|
			//	+-+
			if ((uu >= (vertsPerSide - 1) / 2 && vv < (vertsPerSide - 1) / 2) || ((uu < (vertsPerSide - 1) / 2 && vv >= (vertsPerSide - 1) / 2))) {
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

			// These are the position and texture coordiante glm::vecs for each of the two triangles per-square.
			glm::vec3 v1 = preVerts[t0];	glm::vec2 tex1 = texCoords[t0];
			glm::vec3 v2 = preVerts[t1];	glm::vec2 tex2 = texCoords[t1];
			glm::vec3 v3 = preVerts[t2];	glm::vec2 tex3 = texCoords[t2];

			glm::vec3 v4 = preVerts[t3];	glm::vec2 tex4 = texCoords[t3];
			glm::vec3 v5 = preVerts[t4];	glm::vec2 tex5 = texCoords[t4];
			glm::vec3 v6 = preVerts[t5];	glm::vec2 tex6 = texCoords[t5];

			// If the InstanceSphere's lighting is supposed to be smooth, these normals will be the same
			// as the position (on a sphere, all vertex's point directly away from the center already).
			//
			// If we'd rather they appear angular, we can set that by getting the normal of the triangle
			// the vertexs are a part of, and using that for each Vertex.
			if (!smooth) {
				glm::vec3 n1 = glm::normalize(glm::cross(v3 - v2, v1 - v2));
				glm::vec3 n2 = glm::normalize(glm::cross(v6 - v5, v4 - v5));

				verticesGeneral.push_back(VertexI{ v1, n1, tex1 });
				verticesGeneral.push_back(VertexI{ v2, n1, tex2 });
				verticesGeneral.push_back(VertexI{ v3, n1, tex3 });

				verticesGeneral.push_back(VertexI{ v4, n2, tex4 });
				verticesGeneral.push_back(VertexI{ v5, n2, tex5 });
				verticesGeneral.push_back(VertexI{ v6, n2, tex6 });
			}
			else {
				verticesGeneral.push_back(VertexI{ v1, glm::normalize(v1 / radi), tex1 });
				verticesGeneral.push_back(VertexI{ v2, glm::normalize(v2 / radi), tex2 });
				verticesGeneral.push_back(VertexI{ v3, glm::normalize(v3 / radi), tex3 });

				verticesGeneral.push_back(VertexI{ v4, glm::normalize(v4 / radi), tex4 });
				verticesGeneral.push_back(VertexI{ v5, glm::normalize(v5 / radi), tex5 });
				verticesGeneral.push_back(VertexI{ v6, glm::normalize(v6 / radi), tex6 });
			}


			// Finally, setting the values of the Indices. I have it in such a way,
			// that indices[n] = n; for all n; >= 0, < indices.size().
			for (int ii = 0; ii < 6; ii++) {
				indices.push_back(indCount);
				indCount++;
			}
		}
	}
}

// Generates instances for all balls.
void InstanceSphere::genBalls() {
	for (unsigned int i = 0; i < instances; i++) {
		glm::vec3 newPos;
		float offset = PI * i;
		newPos.x = cos(glm::radians(offset)) * 9.f;
		newPos.y = i;// *10.f;
		newPos.z = sin(glm::radians(offset)) * 9.f;

		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		glm::vec4 ballColor = glm::vec4(r1, r2, r3, color.w);
		float r = 0.2f;// glm::clamp(r1, 0.2f, 1.f);
		verticesIBall.push_back(VertexIBall{ newPos, glm::vec3(r), ballColor});
		balls.push_back(new Ball{ 1.f, glm::vec3(0.f), glm::vec3(0.f), newPos, glm::vec4(1.f), glm::vec3(r) });
		std::cout << "Added [" << i << "]\n";
	}
}

// Destructor of InstanceSphere class.
InstanceSphere::~InstanceSphere() {
	delete[] preVerts;
	delete[] texCoords;
}

// Returns the number of Vertices per side of the plane at a given level of Tessellation.
int InstanceSphere::numVertsPerSide(int _level) {
	if (_level == 1)
		return 2;
	return 2 * numVertsPerSide(_level - 1) - 1;
}

// Sets a new level for the InstanceSphere and calls genTriangles to create a new InstanceSphere at the given level.
void InstanceSphere::setLevel(int _level) {
	level = _level;
	genTriangles();
}

// Redraws the InstanceSphere where ever triangle has a random color.
void InstanceSphere::doRandomColors(bool _randomColor) {
	randomColor = _randomColor;
}

void InstanceSphere::reseed() {
	seed++;
}

// Redraws the InstanceSphere where the Vertex normals are used instead of Surface normals of each Triangle.
void InstanceSphere::smoothSurface(bool _smooth) {
	smooth = _smooth;
}

// Returns closest point on Ellipsoid struck by ray.
glm::vec3 InstanceSphere::isRayTouching(glm::vec3 _rayStart, glm::vec3 _rayDir) {
	glm::mat4 modelInv = inverse(model);
	glm::vec3 ro = glm::vec3(modelInv * glm::vec4(_rayStart, 1.0f));
	glm::vec3 rd = glm::normalize(glm::vec3(modelInv * glm::vec4(_rayDir, 0.0f)));

	float sr = 1.f; // Unit sphere radius.
	float a = dot(rd, rd); // Quadratic formula values a->c.
	float b = dot(ro, rd);
	float c = dot(ro, ro) - sr;
	float d = b * b - a * c; // Determinant.

	if (d < 0.f) {
		return glm::vec3(FLT_MAX);
	}

	float t = (-b - sqrt(d)) / a;
	glm::vec3 intersectionLocal = ro + rd * t;

	// Transform intersection point back to world space
	glm::vec3 intersectionWorld = glm::vec3(model * glm::vec4(intersectionLocal, 1.0f));

	return intersectionWorld;
}

// Calculates new position of the sphere over time, given a force.
void InstanceSphere::calculateNewPos(glm::vec3 _nForce) {
	crntTime = glfwGetTime();
	float t = float(crntTime - prevTime);
	//std::cout << "t = " << t << "\n";
	for (int b1 = 0; b1 < balls.size(); b1++) {
		Ball* b = balls[b1];
		// Calculate new force.
		b->sForce = _nForce; // sForce += _nForce;
		// Set new position.
		b->sVelocity += 2.f * (b->sForce / b->sMass) * t;
		b->ballPos += b->sVelocity * t + (b->sForce / b->sMass) * t * t;
		for (int b2 = 0; b2 < balls.size(); b2++) {
			if (b1 != b2)
				resolveCollision(b1, b2, t);
			// Update model matrix.
			//b->ballMat = glm::translate(glm::mat4(1.f), b->ballPos) * glm::scale(glm::mat4(1.f), radi);
			//instanceMatrices[matIndex] = b->ballMat;
		}
		applyBounds(b1);

	}
	prevTime = crntTime;
}

// Resolves the collision between two InstanceSpheres.
void InstanceSphere::resolveCollision(int bIndex1, int bIndex2, float _t) {
	Ball* b1 = balls[bIndex1];
	Ball* b2 = balls[bIndex2];

	// Ray from our sphere to b2.
	glm::vec3 meToThem = b2->ballPos - b1->ballPos;
	
	float radius = radi.x;
	float distance = radius * radius * 4.f;
	//float distance = b2->ballRadi.x * b1->ballRadi.x * 4.f;
	float separation = glm::dot(meToThem, meToThem);

	// If the two spheres are touching
	if (separation < distance) {

		glm::vec3 collisionNormal = glm::normalize(meToThem);

		// Setting the variables.
		glm::vec3 v1i = b1->sVelocity;
		glm::vec3 v2i = b2->sVelocity;
		float m1 = b1->sMass;
		float m2 = b2->sMass;
	
		// Project velocities onto collision normal
		float v1i_normal = glm::dot(v1i, -collisionNormal);
		float v2i_normal = glm::dot(v2i, collisionNormal);
	
		// Calculate final velocities along collision normal using restitution
		float v1f_normal = (v1i_normal * (m1 - m2) + 2 * m2 * v2i_normal) / (m1 + m2);
		float v2f_normal = (v2i_normal * (m2 - m1) + 2 * m1 * v1i_normal) / (m1 + m2);

		// Shifting the spheres outside of each other.
		float overlap = distance - separation;
		b1->ballPos -= collisionNormal * overlap * 1.5f;
		b2->ballPos += collisionNormal * overlap * 1.5f;

		// Set final velocities
		// Apply restitution only to the component of velocity along collision normal
		b1->sVelocity = v1i + abs(v1f_normal - v1i_normal) * -collisionNormal * restitution;
		b2->sVelocity = v2i + abs(v2f_normal - v2i_normal) * collisionNormal * restitution;
	}
}
/*
// Ray from our sphere to b2.
	glm::vec3 meToThem = b2->objPos - objPos;

	float radius = 1.f;

	// If the two spheres are touching
	if (glm::length(meToThem) < 2.f * radius) {
		// Setting the variables.
		glm::vec3 v1i = sVelocity;
		glm::vec3 v2i = _sphere->sVelocity;
		float m1 = sMass;
		float m2 = _sphere->sMass;


		// v1i * m1 + v2i + m2 = v1f * m1 + v2f + m2
		// AND
		// v1i + v1f = v2i + v2f
		// THEREFOR
		// v1i - v2i + v1f = v2f
		// (v1i * m1 + v2i * m2) = (v1f*m1) + (v1f * m2) + (v1i * m2) - (v2i * m2)
		// THEREFOR
		// (v1i * m1 + v2i * m2 - (v1i * m2) + (v2i * m2)) / (m1 + m2) = v1f
		// THEREFOR
		// v1i + v1f - v2i = v2f
		// Solving for final velocities using momentum conservation equation.
		glm::vec3 v1f = ((v1i * (m1 - m2)) + 2 * m2 * v2i) / (m1 + m2);
		glm::vec3 v2f = ((v2i * (m2 - m1)) + 2 * m1 * v1i) / (m1 + m2);

		// Shifting the spheres outside of each other.
		glm::vec3 meCrntPos = objPos;
		glm::vec3 themCrntPos = _sphere->objPos;
		float overlap = 2.f * radius - glm::length(meToThem);
		setNewPos(meCrntPos - glm::normalize(meToThem) * overlap * 0.5f);
		_sphere->setNewPos(themCrntPos + glm::normalize(meToThem) * overlap * 0.5f);

		// Setting final velocities with restitution.
		float restitution = 1.0f;
		sVelocity = v1f * restitution;
		//_sphere->sVelocity = v2f * restitution;
	}
*/

/*
// Equation for momentum:
	// v1i * m1 + v2i + m2 = v1f * m1 + v2f + m2
	// AND
	// v1i + v1f = v2i + v2f
	// THEREFOR
	// Substitute v1i - v2i + v1f = v2f for v2f
	// v1i * m1 + v2i + m2 = v1f * m1 + (v1f * m2) + (v1i * m2) - (v2i * m2)
	// THEREFOR
	// (v1i * m1 + v2i + m2 - (v1i * m2) + (v2i * m2)) / (m1 + m2) = v1f
	// THEREFOR
	// v1i + v1f - v2i = v2f
	//
	//
	// Force equation:
	// F = force; v = velocity; m = mass; d = distance (meters)
	// F = v * v * m / (2 d)
	// OR
	// F = v * m / t


	// Ray from our sphere to _sphere.
	glm::vec3 meToThem = _sphere->objPos - objPos;

	// If the two spheres be touching
	if (glm::length(meToThem) < 2.f) {
		// Setting the variables.
		glm::vec3 v1i = sVelocity;
		glm::vec3 v2i = _sphere->sVelocity;
		float m1 = sMass;
		float m2 = _sphere->sMass;

		// Solving for final velocities.
		glm::vec3 v1f = ((v1i * m1) + v2i * m2 - (v1i * m2) + (v2i * m2)) / (m1 + m2);
		glm::vec3 v2f = v1i + v1f - v2i;

		// Shifting the sphere's outside of each other.
		glm::vec3 meCrntPos = objPos;
		glm::vec3 themCrntPos = _sphere->objPos;
		setNewPos(meCrntPos - meToThem * (2.f - glm::length(meToThem)));
		_sphere->setNewPos(themCrntPos + meToThem * (2.f - glm::length(meToThem)));

		// Setting final velocities.
		sVelocity = v1f;// *0.9f;
		_sphere->sVelocity = v2f;// *0.9f;
	}
	applyBounds();
*/

// Resolving bounds for InstanceSphere to roam in.
void InstanceSphere::applyBounds(int matIndex) {
	Ball* b = balls[matIndex];

	glm::vec3 bPos = b->ballPos;
	glm::vec3 bVel = b->sVelocity;
	glm::vec3 r = b->ballRadi;

	// Currently not working.
	// +x
	if (bPos.x > 10.f - r.x) {
		bPos.x = 10.f - r.x;
		bVel.x = -bVel.x * restitution;
	}
	// -x
	if (bPos.x < -10.f + r.x) {
		bPos.x = -10.f + r.x;
		bVel.x = -bVel.x * restitution;
	}
	// +z
	if (bPos.z > 10.f - r.z) {
		bPos.z = 10.f - r.z;
		bVel.z = -bVel.z * restitution;
	}
	// -z
	if (bPos.z < -10.f + r.z) {
		bPos.z = -10.f + r.z;
		bVel.z = -bVel.z * restitution;
	}
	// -y
	if (bPos.y < 0.f + r.y) {
		bPos.y = r.y;
		bVel.y = -bVel.y * restitution;
	}

	b->ballPos = bPos;
	b->sVelocity = bVel;
	verticesIBall[matIndex].ballPos = bPos;
}


void InstanceSphere::draw(glm::vec3 _lightPos, glm::vec4 _lightColor) {
	// Activating the shader program and binding the VAO so OpenGL knows what we're trying to do.
	shaderProgram->Activate();
	VAO.Bind();

	if (isWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Assigning all relevant info to the shader.
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "camPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);

	camera->camMatrixForShader(*shaderProgram, "camMatrix");
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), _lightPos.x, _lightPos.y, _lightPos.z);
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), _lightColor.x, _lightColor.y, _lightColor.z, _lightColor.w);
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "time"), glfwGetTime());

	// Draw the actual mesh
	if (instances > 1) {
		glDrawElementsInstanced(triangleType, indices.size(), GL_UNSIGNED_INT, 0, instances);
		calculateNewPos(glm::vec3(0.f, -9.81f, 0.f));
		glBindBuffer(GL_ARRAY_BUFFER, instancesVBO_ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesIBall.size() * sizeof(VertexIBall), &verticesIBall[0]);
	}
	else
		glDrawElements(triangleType, indices.size(), GL_UNSIGNED_INT, 0);
}

void InstanceSphere::setVBOandEBO(std::string msg) {
	// Generates Shader object using shaders object.vert and object.frag
	delete shaderProgram;
	shaderProgram = new Shader("Instance_Sphere.vert", "Instance_Sphere.frag");
	shaderProgram->Activate();

	name = msg;

	VAO.Bind();

	// Setting VBO and EBO
	// Generates Vertex Buffer Object and links it to vertices
	VBO instanceVBO(verticesIBall);
	instancesVBO_ID = instanceVBO.ID;
	VBO VBO(verticesGeneral);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);


	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(VertexI), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(VertexI), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(VertexI), (void*)(6 * sizeof(float)));

	// Linking Instances.
	if (instances > 1) {
		VAO.LinkAttrib(instanceVBO, 3, 3, GL_FLOAT, sizeof(VertexIBall), (void*)(0));
		VAO.LinkAttrib(instanceVBO, 4, 3, GL_FLOAT, sizeof(VertexIBall), (void*)(3 * sizeof(float)));
		VAO.LinkAttrib(instanceVBO, 5, 4, GL_FLOAT, sizeof(VertexIBall), (void*)(6 * sizeof(float)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
	}

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	// Assign all the relevant information to the shader.
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
			glUniform1i(glGetUniformLocation(shaderProgram->ID, "useTex"), 1);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
			glUniform1i(glGetUniformLocation(shaderProgram->ID, "useTexSpec"), 1);
		}
		textures[i].texUnit(*shaderProgram, (type + num).c_str(), textures[i].unit);
		textures[i].Bind();
	}

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	instanceVBO.Unbind();
	EBO.Unbind();
}