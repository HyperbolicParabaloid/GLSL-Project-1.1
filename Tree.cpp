#include "Tree.h"

// Constructor for Cone.
Tree::Tree(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, int _coneLevel, int _sphereLevel, float _bottomRadius, glm::vec3 _pointPos, bool _isSmooth, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) : Object(_window, _objPos, _objScale, _color, _textures, _camera) {
	coneLevel = _coneLevel;
	sphereLevel = _sphereLevel;
	isSmooth = _isSmooth;
	trunkRadius = _bottomRadius;
	trunkPointPos = _pointPos;
	trunkPointingAt = glm::vec3(0.f, 1.f, 0.f);
	objPos = _objPos;
	randomColor = false;
	seed = 1;
	indCount = 0;
	shaftColor = coneColor = color;
	genTriangles();
}

// This function should determine how it needs to add the limbs and whatnot.
void Tree::genTriangles() {
	globalScale = 0.7f;
	float time = glfwGetTime();
	vertices.clear();
	indices.clear();
	indCount = 0;
	branchNum = 0;
	maxDepth = 7;
	newLimbs(glm::vec3(0.f), trunkPointPos, trunkPointingAt, trunkRadius, 0);
	//setVBOandEBO(vertices, indices, "Tree");
	setVBOandEBO("Tree");
	//std::cout << "Time: " << glfwGetTime() - time << "\n";
	std::cout << "Time: " << glfwGetTime() - time << "\tsize: " << vertices.size() << "\n";
	return;
	/*
	int limbs = 20;


	newLimb(glm::vec3(0.f), trunkPointPos, trunkPointingAt, trunkRadius, 0);

	glm::vec3 newLimbPos = trunkPointPos;
	glm::vec3 newLimbPointPos = trunkPointPos * 0.8f;
	glm::vec3 newLimbPointingAt = trunkPointingAt;
	float newLimbRadius = trunkRadius * 0.8f;

	for (int ll = 0; ll < limbs; ll++) {
		// You have to reset the scale and then translate the new position in order to put it where
		// it needs to be, A.K.A., the end of the last limb.
		limbModel = glm::scale(limbModel, glm::vec3(0.f));
		newLimbPos = glm::vec3(limbModel * glm::vec4(newLimbPointPos, 1));
		newLimb(newLimbPos, newLimbPointPos, newLimbPointingAt, newLimbRadius, 0);

		newLimbPointPos = newLimbPointPos * 0.8f;
		newLimbRadius = newLimbRadius * 0.8f;

		if (ll % 2 == 0)
			newLimbPointingAt.x += 0.2f;
		else
			newLimbPointingAt.x -= 0.2f;

	}
	setVBOandEBO(verts, indices, "Tree");
	*/
}


void Tree::newLimb(glm::vec3 _startPos, glm::vec3 _endPos, glm::vec3 _pointingAt, float _radius, int _crntDepth) {

	glm::vec3 startPos = _startPos;
	glm::vec3 endPos = _endPos;
	glm::vec3 pointingAt = _pointingAt;
	float bottomRadius = _radius;
	float topRadius = bottomRadius * globalScale;

	limbModel = glm::mat4(1.f);
	glm::vec3 axis = glm::cross(endPos, pointingAt);
	float angle = acos(glm::dot(glm::normalize(endPos), glm::normalize(pointingAt)));
	limbModel = glm::translate(limbModel, startPos);
	if (ceil(abs(glm::degrees(angle))) < 179.f && floor(abs(glm::degrees(angle))) > 1.f) {
		limbModel = glm::rotate(limbModel, angle, axis);
	}
	else if (floor(abs(glm::degrees(angle))) >= 179.f) {
		endPos.y *= -1;
	}

	genCone(startPos, endPos, bottomRadius, topRadius);
	limbModel = glm::translate(limbModel, endPos); limbModel = glm::scale(limbModel, glm::vec3(topRadius));
	genDome(topRadius);
}


void Tree::newLimbs(glm::vec3 _startPos, glm::vec3 _endPos, glm::vec3 _pointingAt, float _radius, int _crntDepth) {
	if (_crntDepth >= maxDepth)
		return;

	glm::vec3 startPos = _startPos;
	glm::vec3 endPos = _endPos;
	glm::vec3 pointingAt = _pointingAt;
	float bottomRadius = _radius;
	float topRadius = bottomRadius * globalScale * (_crntDepth != maxDepth - 1);


	limbModel = glm::mat4(1.f);
	glm::vec3 axis = glm::cross(endPos, pointingAt);
	float angle = acos(glm::dot(glm::normalize(endPos), glm::normalize(pointingAt)));
	limbModel = glm::translate(limbModel, startPos);
	if (ceil(abs(glm::degrees(angle))) < 179.f && floor(abs(glm::degrees(angle))) > 1.f) {
	//angle *= !(ceil(abs(glm::degrees(angle))) < 179.f && floor(abs(glm::degrees(angle))) > 1.f);
		limbModel = glm::rotate(limbModel, angle, axis);
	}
	else  {
		endPos.y *= 1 + -2 * (floor(abs(glm::degrees(angle))) >= 179.f);
	}

	genCone(startPos, endPos, bottomRadius, topRadius);
	if (topRadius == 0.f)
		return;
	limbModel = glm::translate(limbModel, endPos); limbModel = glm::scale(limbModel, glm::vec3(topRadius));
	genDome(topRadius);

	glm::vec3 newLimbPos = startPos;
	glm::vec3 newLimbPointPos = endPos * globalScale * (newrand(endPos + startPos) * 0.5f + 1.0f);
	glm::vec3 newLimbPointingAt = pointingAt;
	float newLimbRadius = topRadius;

	// You have to reset the scale and then translate the new position in order to put it where
	// it needs to be, A.K.A., the end of the last limb.
	//newLimbPointPos = newLimbPointPos * 0.8f;
	//newLimbRadius = newLimbRadius * 0.8f;
	limbModel = glm::scale(limbModel, glm::vec3(0.f));
	newLimbPos = glm::vec3(limbModel * glm::vec4(newLimbPointPos, 1));

	branchNum += 1;
	float crntTime = float(glfwGetTime());

	int numBranchesPerNode = (floor(newrand(glm::vec2(newLimbPos * crntTime * 506683.f)) * 1.25f) + 2.f) * (_crntDepth < 4) + 1.f * (_crntDepth >= 4);
	for (int bb = 0; bb < numBranchesPerNode; bb++) {

		newLimbPointingAt = glm::normalize(newLimbPointingAt);
		newLimbPointingAt.x += newrand(glm::vec2(newLimbPointPos.y * branchNum, newLimbPointPos.z * -branchNum) * 3637.f * crntTime) * 2.f - 1.f;
		newLimbPointingAt.y += (newrand(glm::vec2(newLimbPointPos.x * -branchNum, newLimbPointPos.z * branchNum) * 1787.f * crntTime) * 2.f - 0.25f) * (_crntDepth >= 5);
		newLimbPointingAt.z += newrand(glm::vec2(newLimbPointPos.x * branchNum, newLimbPointPos.y * branchNum) * 7841.f * crntTime) * 2.f - 1.f;
		newLimbs(newLimbPos, newLimbPointPos, newLimbPointingAt, newLimbRadius, _crntDepth + 1);
	}
}

// Generates the cone's vertices.
void Tree::genCone(glm::vec3 _startPos, glm::vec3 _endPos, float _bottomRadius, float _topRadius) {
	// Whenever we generate a new set a vertices and indices, we want to wipe the old ones.
	// In the future it'd be better to just add in the new vertices and update indices instead
	// of clearing both indices and verts and starting over but it's fine for now.

	if (coneLevel < 2)
		coneLevel = 2;
	if (coneLevel > 36)
		coneLevel = 36;

	int vertsPerCone = (coneLevel * 2) + 2;

	glm::vec3* newPreVerts = new glm::vec3[vertsPerCone];	delete[] preVerts;
	preVerts = newPreVerts;
	glm::vec2* newTexCoords = new glm::vec2[vertsPerCone + 2]; delete[] texCoords;
	texCoords = newTexCoords;

	preVerts[0] = _startPos;
	preVerts[1] = _endPos;

	float theta1 = 0.f, theta2 = 0.f;
	float anglePerVertex = (360.f / coneLevel);
	for (int ii = 1; ii <= coneLevel; ii++) {
		int evenIndex = ii * 2;
		int oddIndex = ii * 2 + 1;

		float u = float(ii - 1) / float(coneLevel);

		theta1 = glm::radians(ii * anglePerVertex);
		theta2 = glm::radians(ii * anglePerVertex);		//theta2 = glm::radians((ii + 0.5f) * anglePerVertex2);//anglePerVert;	// This is to offset the top/bottom vertices. It doesn't look very good tbh.

		float x = sin(theta1), y = cos(theta1);
		preVerts[evenIndex] = glm::vec3(limbModel * glm::vec4(x * _bottomRadius, 0.f, y * _bottomRadius, 1));//glm::mat3(model) * (((glm::vec3(sin(theta1), 0.f, cos(theta1)) * bottomRadius) + objPos));
		preVerts[oddIndex] = glm::vec3(limbModel * glm::vec4(x * _topRadius, _endPos.y, y * _topRadius, 1));//glm::mat3(model) * (glm::vec3(sin(theta2) * topRadius, 0.f, cos(theta2) * topRadius) + objPos + pointPos);

		texCoords[evenIndex] = glm::vec2(u * 4, 0);
		texCoords[oddIndex] = glm::vec2(u * 4, 1);	// 0.25 should be 1
	}
	texCoords[vertsPerCone] = glm::vec2(4, 0);
	texCoords[vertsPerCone + 1] = glm::vec2(4, 1);// 0.25 should be 1
	setConeVertices();
}
// Adds the new cone's vertices and indices into the classes vertices and indcies vectors.
void Tree::setConeVertices() {
	srand(seed);
	glm::vec4 color1, color2;
	glm::vec3 n1, n2, n3, n4;

	// Value to tell the counter when to wrap back to begining to finish the triangles.
	int evenLength = (coneLevel * 2);

	// Values for the vertices indexes.
	int lwrCntr = 0, uprCntr = 1;
	int lwrTriVrt1, lwrTriVrt2, uprTriVrt1, uprTriVrt2;

	// Algorithm will essentially be: n, n+1, 0		for all n >= 1.
	for (int jj = 1; jj <= coneLevel; jj++) {
		lwrTriVrt1 = (jj * 2);
		lwrTriVrt2 = ((jj * 2) % evenLength) + 2;

		uprTriVrt1 = (jj * 2) + 1;
		uprTriVrt2 = ((jj * 2) % evenLength) + 3;

		glm::vec3 v1 = preVerts[lwrTriVrt1];	glm::vec2 tex1 = texCoords[lwrTriVrt1];
		glm::vec3 v2 = preVerts[lwrTriVrt2];	glm::vec2 tex2 = texCoords[lwrTriVrt1 + 2];
		//glm::vec3 v3 = preVerts[lwrCntr];		glm::vec2 tex3 = texCoords[lwrCntr];
		//glm::vec3 v4 = preVerts[uprCntr];		glm::vec2 tex4 = texCoords[uprCntr];

		glm::vec3 v5 = preVerts[uprTriVrt1];	glm::vec2 tex5 = texCoords[uprTriVrt1];
		glm::vec3 v6 = preVerts[uprTriVrt2];	glm::vec2 tex6 = texCoords[uprTriVrt1 + 2];

		// Setting the colors of the object:
		if (randomColor) {
			float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			color1 = glm::vec4(r1, r2, r3, 1.f);

			float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float r5 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float r6 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			color2 = glm::vec4(r4, r5, r6, 1.f);
		}
		else
			color1 = color2 = color;

		// Norms for each of the 4 triangles.
		//n1 = glm::normalize(glm::cross((v1 - v2), (v3 - v2)));
		n1 = n2 = glm::normalize(glm::cross((v5 - v2), (v1 - v2)));
		//n3 = glm::normalize(glm::cross((v4 - v6), (v5 - v6)));
		n3 = n4 = glm::normalize(glm::cross((v5 - v6), (v2 - v6)));

		if (isSmooth) {
			// Setting the previous and future indices.
			int prevjj, futrjj;
			if (jj - 1 <= 0) {
				prevjj = coneLevel;
			}
			else
				prevjj = jj - 1;
			if (jj + 1 > coneLevel) {
				futrjj = 1;
			}
			else
				futrjj = jj + 1;

			// Getting the previous faces nromal.
			lwrTriVrt1 = (prevjj * 2);
			lwrTriVrt2 = ((prevjj * 2) % evenLength) + 2;
			uprTriVrt1 = (prevjj * 2) + 1;
			uprTriVrt2 = ((prevjj * 2) % evenLength) + 3;
			glm::vec3 prev1 = preVerts[lwrTriVrt1]; glm::vec3 prev2 = preVerts[lwrTriVrt2]; glm::vec3 prev5 = preVerts[uprTriVrt1]; glm::vec3 prev6 = preVerts[uprTriVrt2];
			glm::vec3 preNorm = glm::normalize(glm::cross((prev5 - prev2), (prev1 - prev2)));

			// Getting the future faces nromal.
			lwrTriVrt1 = (futrjj * 2);
			lwrTriVrt2 = ((futrjj * 2) % evenLength) + 2;
			uprTriVrt1 = (futrjj * 2) + 1;
			glm::vec3 ftrv1 = preVerts[lwrTriVrt1]; glm::vec3 ftrv2 = preVerts[lwrTriVrt2]; glm::vec3 ftrv5 = preVerts[uprTriVrt1];
			glm::vec3 ftrNorm = glm::normalize(glm::cross((ftrv5 - ftrv2), (ftrv1 - ftrv2)));

			// Finally, setting the future normal and previous normals.
			n1 = n3 = glm::normalize((preNorm + n2) / 2.f);
			n2 = n4 = glm::normalize((ftrNorm + n2) / 2.f);
		}

		// Low-to-high side triangle.
		vertices.push_back(Vertex{ v1, n1, color1, tex1 });
		vertices.push_back(Vertex{ v2, n2, color1, tex2 });
		vertices.push_back(Vertex{ v5, n1, color1, tex5 });

		// High-to-low side triangle.
		vertices.push_back(Vertex{ v5, n3, color2, tex5 });
		vertices.push_back(Vertex{ v6, n4, color2, tex6 });
		vertices.push_back(Vertex{ v2, n4, color2, tex2 });

		// Finally, setting the values of the Indices. I have it in such a way,
		// that indices[n] = n; for all n; >= 0, < indices.size().
		for (int ind = 0; ind < 12; ind++) {
			indices.push_back(indCount);
			indCount++;
		}
	}
}

// Generates the dome's vertices.
void Tree::genDome(float endRadius) {
	// Whenever we generate a new set a vertices and indices, we want to wipe the old ones.
	// In the future it'd be better to just add in the new vertices and update indices instead
	// of clearing both indices and verts and starting over but it's fine for now.

	if (sphereLevel < 1)
		sphereLevel = 1;
	if (sphereLevel > 10)
		sphereLevel = 10;

	// Num vertices per level
	//	1			2		3		4
	// 2x2=4 => 3x3=9 => 5x5=25 => 9x9=81
	// So basically, if you think about it you're doing:
	// NumVertsPerSide_X = NumVertsPerSide_X-1 + NumVertsPerSide_X-1 - 1
	int vertsPerSide = numVertsPerSide(sphereLevel);


	// Resizing verts and texCoords. 
	preVertsSize = vertsPerSide * vertsPerSide;

	glm::vec3* newPreVerts = new glm::vec3[preVertsSize];	delete[] preVerts;
	preVerts = newPreVerts;
	glm::vec2* newTexCoords = new glm::vec2[preVertsSize];	delete[] texCoords;
	texCoords = newTexCoords;

	float temp;
	for (int vv = 0; vv < vertsPerSide; vv++) {
		for (int uu = 0; uu < vertsPerSide; uu++) {

			// Can I just make uu and vv go from 0->1, and make that my UV texture coordinates?
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

			//glm::mat4 rotatingModel = glm::mat4(1.f);
			//limbModel = glm::rotate(limbModel, glm::radians(-45.f), glm::vec3(0.f, 0.f, 1.f));
			//preVerts[vertsPerSide * vv + uu] = glm::vec3(rotatingModel * glm::vec4(x, y, z, 1));
			// Normalizing the vector, places the vertices of the Octehdron on the surface of the sphere.
			preVerts[vertsPerSide * vv + uu] = glm::normalize(glm::vec3(x, y, z));
		}
	}
	glm::mat4 rotatingModel = glm::mat4(1.f);
	limbModel = glm::rotate(limbModel, glm::radians(-45.f), glm::vec3(0.f, 0.f, 1.f));
	setDomeVertices();
}
// Adds the new dome's vertices and indices into the classes vertices and indcies vectors.
void Tree::setDomeVertices() {
	int vertsPerSide = numVertsPerSide(sphereLevel);
	int setpsPerSide = vertsPerSide - 2;

	int t0, t1, t2; // First triangle
	int t3, t4, t5; // Second triangle;

	srand(seed);
	// This nested for-loop updates verts with all its Vertex information and
	// updates indices with all the right values.
	for (int vv = 0; vv <= setpsPerSide; vv++) {
		for (int uu = 0; uu <= setpsPerSide; uu++) {
			if (vv + uu <= setpsPerSide) {
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
				//glm::vec3(limbModel * glm::vec4(n1, 1))
				glm::vec3 n1 = glm::normalize(preVerts[t0]); glm::vec3 v1 = glm::vec3(limbModel * glm::vec4(n1, 1));	glm::vec2 tex1 = texCoords[t0];
				glm::vec3 n2 = glm::normalize(preVerts[t1]); glm::vec3 v2 = glm::vec3(limbModel * glm::vec4(n2, 1));	glm::vec2 tex2 = texCoords[t1];
				glm::vec3 n3 = glm::normalize(preVerts[t2]); glm::vec3 v3 = glm::vec3(limbModel * glm::vec4(n3, 1));	glm::vec2 tex3 = texCoords[t2];

				glm::vec3 n4 = glm::normalize(preVerts[t3]); glm::vec3 v4 = glm::vec3(limbModel * glm::vec4(n4, 1));	glm::vec2 tex4 = texCoords[t3];
				glm::vec3 n5 = glm::normalize(preVerts[t4]); glm::vec3 v5 = glm::vec3(limbModel * glm::vec4(n5, 1));	glm::vec2 tex5 = texCoords[t4];
				glm::vec3 n6 = glm::normalize(preVerts[t5]); glm::vec3 v6 = glm::vec3(limbModel * glm::vec4(n6, 1));	glm::vec2 tex6 = texCoords[t5];

				// Setting the colors of the object:
				glm::vec4 color1, color2;
				if (randomColor) {
					float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					color1 = glm::vec4(r1, r2, r3, 1.f);

					float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float r5 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float r6 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					color2 = glm::vec4(r4, r5, r6, 1.f);
				}
				else
					color1 = color2 = color;

				// If the Sphere's lighting is supposed to be smooth, these normals will be the same
				// as the position (on a sphere, all vertex's point directly away from the center already).
				//
				// If we'd rather they appear angular, we can set that by getting the normal of the triangle
				// the vertexs are a part of, and using that for each Vertex.
				if (!isSmooth) {
					glm::vec3 n1 = n2 = n3 = glm::normalize(glm::cross(v3 - v2, v1 - v2));
					glm::vec3 n4 = n5 = n6 = glm::normalize(glm::cross(v6 - v5, v4 - v5));
				}
				vertices.push_back(Vertex{ v1, n1, color1, tex1 });
				vertices.push_back(Vertex{ v2, n2, color1, tex2 });
				vertices.push_back(Vertex{ v3, n3, color1, tex3 });

				vertices.push_back(Vertex{ v4, n4, color2, tex4 });
				vertices.push_back(Vertex{ v5, n5, color2, tex5 });
				vertices.push_back(Vertex{ v6, n6, color2, tex6 });
				// Finally, setting the values of the Indices. I have it in such a way,
				// that indices[n] = n; for all n; >= 0, < indices.size().
				for (int ii = 0; ii < 6; ii++) {
					indices.push_back(indCount);
					indCount++;
				}
			}
		}
	}
}


int Tree::numVertsPerSide(int _level) {
	if (_level == 1)
		return 2;
	return 2 * numVertsPerSide(_level - 1) - 1;
}



// Destructor of Cone class.
Tree::~Tree() {
	delete[] preVerts;
	delete[] texCoords;
}

// Sets a new level for the Sphere and calls genTriangles to create a new Sphere at the given level.
void Tree::setSphereLevel(int _sphereLevel) {
	sphereLevel = _sphereLevel;
	genTriangles();
}

// Sets a new level for the Sphere and calls genTriangles to create a new Sphere at the given level.
void Tree::setConeLevel(int _coneLevel) {
	coneLevel = _coneLevel;
	genTriangles();
}


// Redraws the Sphere where ever triangle has a random color.
void Tree::doRandomColors(bool _randomColor) {
	randomColor = _randomColor;
}

void Tree::reseed() {
	seed++;
}

// Redraws the Sphere where the Vertex normals are used instead of Surface normals of each Triangle.
void Tree::smoothSurface(bool _isSmooth) {
	isSmooth = _isSmooth;
}
