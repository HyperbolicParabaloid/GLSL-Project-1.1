#include "Sphere.h"

// Constructor for Sphere.
Sphere::Sphere(GLFWwindow* _window, glm::vec3 _objPos, glm::vec3 _radi, float _objScale, int _level, bool _smooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) : Object(_window, _objPos, _objScale, _color, _textures, _camera) {
	level = _level;
	smooth = _smooth;
	randomColor = false;
	seed = 1;
	objRadius = 1.f;
	//radi = _radi;
	setScale(_radi * objScale);
	//model = glm::translate(glm::mat4(1.f), objPos);
	//model = glm::scale(model, radi);//glm::vec3(objScale));
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
void Sphere::genTriangles() {
	genOctahedron();
	//setVBOandEBO(verts, indices, "Sphere");
	setVBOandEBO("Sphere");
	//std::cout << "NEW SPHERE\n";
	for (int i = 0; i < indices.size(); i+=3) {
		glm::vec3 v0 = vertices[i + 0].pos;
		glm::vec3 v1 = vertices[i + 1].pos;
		glm::vec3 v2 = vertices[i + 2].pos;
		
		//std::cout << "gl_Position = data_in[0].projection * vec3(" << v0.x << ", " << v0.z << ", " << v0.z << ");\n";
		//std::cout << "Normal = vec3(" << v0.x << ", " << v0.z << ", " << v0.z << ");\n";
		//std::cout << "EmitVertex();\n\n";
		//
		//std::cout << "gl_Position = data_in[0].projection * vec3(" << v1.x << ", " << v1.z << ", " << v1.z << ");\n";
		//std::cout << "Normal = vec3(" << v1.x << ", " << v1.z << ", " << v1.z << ");\n";
		//std::cout << "EmitVertex();\n\n";
		//
		//std::cout << "gl_Position = data_in[0].projection * vec3(" << v2.x << ", " << v2.z << ", " << v2.z << ");\n";
		//std::cout << "Normal = vec3(" << v2.x << ", " << v2.z << ", " << v2.z << ");\n";
		//std::cout << "EmitVertex();\n";
		//std::cout << "EndPrimitive();\n\n";

	}
	//std::cout << "END SPHERE\n\n";
}

void Sphere::genOctahedron() {
	// Whenever we generate a new set a vertices and indices, we want to wipe the old ones.
	// In the future it'd be better to just add in the new vertices and update indices instead
	// of clearing both indices and verts and starting over but it's fine for now.
	vertices.clear();
	indices.clear();
	triangles.clear();

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
}

//
void Sphere::setVerticesVector() {
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

			// Setting the colors of the object:
			glm::vec4 color1, color2;
			if (randomColor) {
				float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				color1 = glm::vec4(r1, r2, r3, color.w);

				float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float r5 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float r6 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				color2 = glm::vec4(r4, r5, r6, color.w);
			}
			else
				color1 = color2 = color;

			// If the Sphere's lighting is supposed to be smooth, these normals will be the same
			// as the position (on a sphere, all vertex's point directly away from the center already).
			//
			// If we'd rather they appear angular, we can set that by getting the normal of the triangle
			// the vertexs are a part of, and using that for each Vertex.
			if (!smooth) {
				glm::vec3 n1 = glm::normalize(glm::cross(v3 - v2, v1 - v2));
				glm::vec3 n2 = glm::normalize(glm::cross(v6 - v5, v4 - v5));

				vertices.push_back(Vertex{ v1, n1, color1, tex1 });
				vertices.push_back(Vertex{ v2, n1, color1, tex2 });
				vertices.push_back(Vertex{ v3, n1, color1, tex3 });

				vertices.push_back(Vertex{ v4, n2, color2, tex4 });
				vertices.push_back(Vertex{ v5, n2, color2, tex5 });
				vertices.push_back(Vertex{ v6, n2, color2, tex6 });
			}
			else {
				//vertices.push_back(Vertex{ v1, glm::normalize(v1 / radi) / radi, color1, tex1 });
				//vertices.push_back(Vertex{ v2, glm::normalize(v2 / radi) / radi, color1, tex2 });
				//vertices.push_back(Vertex{ v3, glm::normalize(v3 / radi) / radi, color1, tex3 });
				//
				//vertices.push_back(Vertex{ v4, glm::normalize(v4 / radi) / radi, color2, tex4 });
				//vertices.push_back(Vertex{ v5, glm::normalize(v5 / radi) / radi, color2, tex5 });
				//vertices.push_back(Vertex{ v6, glm::normalize(v6 / radi) / radi, color2, tex6 });
				vertices.push_back(Vertex{ v1, glm::normalize(v1 / radi), color1, tex1 });
				vertices.push_back(Vertex{ v2, glm::normalize(v2 / radi), color1, tex2 });
				vertices.push_back(Vertex{ v3, glm::normalize(v3 / radi), color1, tex3 });
																		
				vertices.push_back(Vertex{ v4, glm::normalize(v4 / radi), color2, tex4 });
				vertices.push_back(Vertex{ v5, glm::normalize(v5 / radi), color2, tex5 });
				vertices.push_back(Vertex{ v6, glm::normalize(v6 / radi), color2, tex6 });
			}
			triangles.push_back(Triangle{ &vertices[indCount + 0], &vertices[indCount + 1] , &vertices[indCount + 2], &model });
			triangles.push_back(Triangle{ &vertices[indCount + 3], &vertices[indCount + 4] , &vertices[indCount + 5], &model });


			// Finally, setting the values of the Indices. I have it in such a way,
			// that indices[n] = n; for all n; >= 0, < indices.size().
			for (int ii = 0; ii < 6; ii++) {
				indices.push_back(indCount);
				indCount++;
			}
		}
	}

}

// Destructor of Sphere class.
Sphere::~Sphere() {
	delete[] preVerts;
	delete[] texCoords;
}

// Returns the number of Vertices per side of the plane at a given level of Tessellation.
int Sphere::numVertsPerSide(int _level) {
	if (_level == 1)
		return 2;
	return 2 * numVertsPerSide(_level - 1) - 1;
}

// Sets a new level for the Sphere and calls genTriangles to create a new Sphere at the given level.
void Sphere::setLevel(int _level) {
	level = _level;
	genTriangles();
}

// Redraws the Sphere where ever triangle has a random color.
void Sphere::doRandomColors(bool _randomColor) {
	randomColor = _randomColor;
}

void Sphere::reseed() {
	seed++;
}

// Redraws the Sphere where the Vertex normals are used instead of Surface normals of each Triangle.
void Sphere::smoothSurface(bool _smooth) {
	smooth = _smooth;
}

// Returns closest point on Ellipsoid struck by ray.
glm::vec3 Sphere::isRayTouching(glm::vec3 _rayStart, glm::vec3 _rayDir) {
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
void Sphere::calculateNewPos(glm::vec3 _nForce, float _t) {
	// Basic kinematic equations are:
	// position(t1) = position(t0) + velocity * t + acceleration * t^2.
	// Since acceleration = Force / Mass,
	// position(t1) = position(t0) + velocity * t + (Force / Mass) * t^2.
	// Shrimple.

	// Calculate new force.
	sForce = _nForce; // sForce += _nForce;
	// Set new position.
	sVelocity += 2.f * (sForce / sMass) * _t;
	objPos += sVelocity * _t + (sForce / sMass) * _t * _t;
	// Update model matrix.
	model = glm::translate(glm::mat4(1.f), objPos) * rotationMatrix * glm::scale(glm::mat4(1.f), radi);
}

// Resolves the collision between two Spheres.
void Sphere::resolveCollision(Sphere* _sphere, float _t) {
	// Ray from our sphere to _sphere.
	glm::vec3 meToThem = _sphere->objPos - objPos;
	glm::vec3 collisionNormal = glm::normalize(meToThem);

	float radius = 0.2f;

	// If the two spheres are touching
	if (glm::length(meToThem) < 2.f * radius) {
		// Setting the variables.
		glm::vec3 v1i = sVelocity;
		glm::vec3 v2i = _sphere->sVelocity;
		float m1 = sMass;
		float m2 = _sphere->sMass;

		// Project velocities onto collision normal
		float v1i_normal = glm::dot(v1i, collisionNormal);
		float v2i_normal = glm::dot(v2i, collisionNormal);

		// Calculate final velocities along collision normal using restitution
		float restitution = 0.8f;
		float v1f_normal = (v1i_normal * (m1 - m2) + 2 * m2 * v2i_normal) / (m1 + m2);
		float v2f_normal = (v2i_normal * (m2 - m1) + 2 * m1 * v1i_normal) / (m1 + m2);

		// Apply restitution only to the component of velocity along collision normal
		glm::vec3 v1f = v1i + (v1f_normal - v1i_normal) * collisionNormal * restitution;
		glm::vec3 v2f = v2i + (v2f_normal - v2i_normal) * collisionNormal * restitution;

		// Shifting the spheres outside of each other.
		glm::vec3 meCrntPos = objPos;
		glm::vec3 themCrntPos = _sphere->objPos;
		float overlap = 2.f * radius - glm::length(meToThem);
		setNewPos(meCrntPos - collisionNormal * overlap * 0.5f);
		_sphere->setNewPos(themCrntPos + collisionNormal * overlap * 0.5f);

		// Set final velocities
		sVelocity = v1f;
		_sphere->sVelocity = v2f;
	}
	applyBounds();
}
/*
// Ray from our sphere to _sphere.
	glm::vec3 meToThem = _sphere->objPos - objPos;

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

// Resolving bounds for Sphere to roam in.
void Sphere::applyBounds() {
	float restitution = 0.8f;
	// Currently not working.
	// +x
	if (objPos.x > 10.f - radi.x) {
		objPos.x = 10.f - radi.x;
		sVelocity.x = -sVelocity.x * restitution;
	}
	// -x
	if (objPos.x < -10.f + radi.x) {
		objPos.x = -10.f + radi.x;
		sVelocity.x = -sVelocity.x * restitution;
	}
	// +z
	if (objPos.z > 10.f - radi.z) {
		objPos.z = 10.f - radi.z;
		sVelocity.z = -sVelocity.z * restitution;
	}
	// -z
	if (objPos.z < -10.f + radi.z) {
		objPos.z = -10.f + radi.z;
		sVelocity.z = -sVelocity.z * restitution;
	}
	// -y
	if (objPos.y < 0.f + radi.y) {
		objPos.y = radi.y;
		sVelocity.y = -sVelocity.y * restitution;
	}
	return;
}
