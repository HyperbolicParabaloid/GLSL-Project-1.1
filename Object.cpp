#include "Object.h"


// Object classes constructor. Just sets some member variables and updates the glm::mat4 model of the object.
Object::Object(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) {
	window = _window;
	doNormalArrows = false;
	objPos = _objPos;
	objScale = _objScale;
	color = _color;
	model = glm::translate(glm::mat4(1.f), objPos);
	model = glm::scale(model, glm::vec3(objScale));
	camera = _camera;
	textures = _textures;
}

void Object::setNewPos(glm::vec3 _objPos) {
	objPos = _objPos;
	model = glm::translate(glm::mat4(1.f), objPos);
	model = glm::scale(model, glm::vec3(objScale));
}

// Rotates the object about a given axis by a set angle in degrees.
void Object::rotate(float rotationDegreeAngle, glm::vec3 axisOfRotation) {
	model = glm::rotate(model, glm::radians(rotationDegreeAngle), axisOfRotation);
}

// Draws the object to the screen and sets the appropriate information in the Shader object about position, and lighting.
void Object::draw(glm::vec3 _lightPos, glm::vec4 _lightColor) {
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
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), _lightPos.x, _lightPos.y, _lightPos.z);
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), _lightColor.x, _lightColor.y, _lightColor.z, _lightColor.w);
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "time"), glfwGetTime());
	if (name == "Tree" || name == "Plane")
		glUniform3f(glGetUniformLocation(shaderProgram->ID, "startPos"), objPos.x / 25.f, objPos.y / 25.f, objPos.z / 25.f);
	// Draw the actual mesh
	glDrawElements(triangleType, indices.size(), GL_UNSIGNED_INT, 0);


}

// Creates a new shader program and binds the adds the appropriate information into the GPU's memory so it can draw the
// object.

void Object::setVBOandEBO(std::string msg) {
	// Generates Shader object using shaders object.vert and object.frag
	delete shaderProgram;
	if (msg == "Plane") {
		name = msg;
		shaderProgram = new Shader("grass.vert", "grass.frag");
	}
	else if (msg == "Tree") {
		name = msg;
		shaderProgram = new Shader("tree.vert", "tree.frag");
	}
	else
		shaderProgram = new Shader("object.vert", "object.frag");
	shaderProgram->Activate();

	//std::cout << "doNormalArrows = " << doNormalArrows << " for " << msg << "\n";
	//if (doNormalArrows == true) {
	//	//std::cout << "Drawing the arrows for " << msg <<  "\n";
	//	int vNum = 1;
	//	int maxSize = vertices.size();
	//	for (int vv = 0; vv < maxSize; vv++) {
	//		glm::vec3 conePos = vertices[vv].pos;
	//		float coneScale = 0.02f;
	//		int coneLevel = 4;
	//		float coneBottomRadius = 1.f;
	//		float coneTopRadius = 0.f;
	//		glm::vec3 conePointPos = glm::vec3(0.f, 5.f, 0.f);
	//		glm::vec3 conePointingAt = vertices[vv].norm;
	//		bool coneIsSmooth = true;
	//		glm::vec4 coneColor = glm::vec4(0.4f, 0.5f, 0.3f, 1.f); //vertices[vv].color;
	//		glm::vec4 shaftColor = glm::vec4(0.5f, 0.3f, 0.f, 1.f);// vertices[vv].color;
	//		int coneStartingIndex = indices.size();
	//		float randomizationEffect = 0.f;
	//
	//		//std::cout << "Drawing arrows #" << vNum << " at pos (" << conePos.x << ", " << conePos.y << ", " << conePos.z << ")\n";
	//		//vNum++;
	//
	//		//std::cout << "Drawing a " << msg << "\n";
	//		Arrow newArrow(conePos, coneScale, coneLevel, coneBottomRadius, coneTopRadius, conePointPos, conePointingAt, coneIsSmooth, shaftColor, coneColor, randomizationEffect, coneStartingIndex);
	//		std::vector <Vertex> coneVerts = newArrow.getVerts();
	//		std::vector <GLuint> coneInds = newArrow.getInds();
	//
	//		//std::cout << "vertices.size() BEFORE = " << vertices.size() << "\n";
	//		vertices.insert(vertices.end(), coneVerts.begin(), coneVerts.end());
	//		indices.insert(indices.end(), coneInds.begin(), coneInds.end());
	//	}
	//}

	VAO.Bind();
	// Setting VBO and EBO
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 4, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(10 * sizeof(float)));

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
		textures[i].texUnit(*shaderProgram, (type + num).c_str(), i);
		textures[i].Bind();
	}

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Object::setVBOandEBO(std::vector <Vertex>& _vertices, std::vector <GLuint>& _indices, std::string msg) {
	// Generates Shader object using shaders object.vert and object.frag
	delete shaderProgram;
	if (msg == "Plane") {
		name = msg;
		shaderProgram = new Shader("grass.vert", "grass.frag");
	}
	else if (msg == "Tree") {
		name = msg;
		shaderProgram = new Shader("tree.vert", "tree.frag");
	}
	else
		shaderProgram = new Shader("object.vert", "object.frag");
	shaderProgram->Activate();

	vertices = _vertices;
	indices = _indices;

	//std::cout << "doNormalArrows = " << doNormalArrows << " for " << msg << "\n";
	if (doNormalArrows == true) {
		//std::cout << "Drawing the arrows for " << msg <<  "\n";
		int vNum = 1;
		int maxSize = vertices.size();
		for (int vv = 0; vv < maxSize; vv++) {
			glm::vec3 conePos = vertices[vv].pos;
			float coneScale = 0.02f;
			int coneLevel = 4;
			float coneBottomRadius = 1.f;
			float coneTopRadius = 0.f;
			glm::vec3 conePointPos = glm::vec3(0.f, 5.f, 0.f);
			glm::vec3 conePointingAt = vertices[vv].norm;
			bool coneIsSmooth = true;
			glm::vec4 coneColor = glm::vec4(0.4f, 0.5f, 0.3f, 1.f); //vertices[vv].color;
			glm::vec4 shaftColor = glm::vec4(0.5f, 0.3f, 0.f, 1.f);// vertices[vv].color;
			int coneStartingIndex = indices.size();
			float randomizationEffect = 0.f;

			//std::cout << "Drawing arrows #" << vNum << " at pos (" << conePos.x << ", " << conePos.y << ", " << conePos.z << ")\n";
			//vNum++;

			//std::cout << "Drawing a " << msg << "\n";
			Arrow newArrow(conePos, coneScale, coneLevel, coneBottomRadius, coneTopRadius, conePointPos, conePointingAt, coneIsSmooth, shaftColor, coneColor, randomizationEffect, coneStartingIndex);
			std::vector <Vertex> coneVerts = newArrow.getVerts();
			std::vector <GLuint> coneInds = newArrow.getInds();

			//std::cout << "vertices.size() BEFORE = " << vertices.size() << "\n";
			vertices.insert(vertices.end(), coneVerts.begin(), coneVerts.end());
			indices.insert(indices.end(), coneInds.begin(), coneInds.end());
		}
	}

	VAO.Bind();
	// Setting VBO and EBO
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 4, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(10 * sizeof(float)));

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
		textures[i].texUnit(*shaderProgram, (type + num).c_str(), i);
		textures[i].Bind();
	}

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

// Object class's destructor. Just deletes the dynamically allocated Shader object.
Object::~Object() {
	shaderProgram->Delete();
	delete shaderProgram;
}

void Object::moveFirstVertex() {
	//vertices[0].pos.y += 0.1f;
	//(noise(aPos.xz + time) * 2.f) + (noise((aPos.xz + time) * 1.2) * 1/8) + (noise((aPos.xz + time) * 2) * 1/16);
	for (Vertex& v : vertices) {
		v.pos.y = noise(glm::vec2(v.texCoord.x + glfwGetTime(), v.texCoord.y + glfwGetTime()));
		std::cout << "Yaaaaas\n";
	}

	VAO.Bind();
	// Setting VBO and EBO
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 4, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(10 * sizeof(float)));

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

float Object::newrand(glm::vec2 co) {
	return glm::fract(sin(glm::dot(co, glm::vec2(12.9898, 78.233))) * 43758.5453);
}

float Object::noise(glm::vec2 n) {
	const glm::vec2 d = glm::vec2(0.0, 1.0);
	glm::vec2 b = floor(n), f = glm::smoothstep(glm::vec2(0.0), glm::vec2(1.0), fract(n));
	return glm::mix(glm::mix(newrand(b), newrand(b + glm::vec2(d.y, d.x)), f.x), glm::mix(newrand(b + glm::vec2(d.x, d.y)), newrand(b + glm::vec2(d.y, d.y)), f.x), f.y);
}

void Object::hotRealoadShader() {
	shaderProgram->hotReaload();
	shaderProgram->Activate();

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
		textures[i].texUnit(*shaderProgram, (type + num).c_str(), i);
		textures[i].Bind();
	}
}

void Object::toggleNormalArrows() {
	doNormalArrows = !doNormalArrows;
}


//				--------------------				//
//				Physics interactions				//
//				--------------------				//


// Returns true if the bounding spheres of two objects overlap. This does not necessarily mean that
// the two meshes actually touch. That's a much more expensive computation handled in getIntersection.
bool Object::isTouching(Object* obj) {
	float lenA = objScale * objRadius;
	float lenB = obj->objScale * obj->objRadius;
	if (glm::length(obj->objPos - objPos) <= lenA + lenB)
		return true;
	return false;
}
bool Object::isTouching(Triangle* tri) {
	tri->genCircle();
	float lenA = objScale * objRadius;
	float lenB = tri->radius;
	if (glm::length(tri->center - objPos) <= lenA + lenB)
		return true;// triangleIntersection(tri);
	return false;
}


bool Object::isTouching(Triangle* tri, int index) {
	tri->genCircle();
	float lenA = objScale * objRadius;
	float lenB = tri->radius;
	if (glm::length(tri->center - objPos) <= lenA + lenB)
		return triangleIntersection(tri, index);
	return false;
}


bool Object::triangleIntersection(Triangle* tri, int index) {
	// genCircle makes sure all the triangles are up to date with the objects current model matrix.
	triangles[index].genCircle();

	// denom will always be -1.f because we want the shortest path to the plane.
	//float denom = -1.f;

	// Normal of triangle, A.K.A., normal of plane.
	glm::vec3 triNorm = glm::normalize(glm::cross(tri->vec_1 - tri->vec_3, tri->vec_2 - tri->vec_3));
	glm::vec3 objTriCntr_to_tri = tri->vec_3 - triangles[index].center;

	// Here, t is the shortest distance from the center of the circle generated by triangles[index].
	float t = -glm::dot(objTriCntr_to_tri, -triNorm);// / denom;

	// If this distance t is less than the radius of the circle bounding triangles[index], then we know
	// it's worth investigated further.
	if (abs(t) <= triangles[index].radius) {
		// Need to do barycentric interpolation to test if the point is inside the triangle.

		// So think about it like this: we're in 3D to begin with: we're a sphere in space. Next we ask, "what's the closet
		// point from the center of our sphere to the plane that the triangle (tri) is on?" We calculate that, and then ask,
		// "is the distance from the center of our sphere to this point, less than the radius of our sphere?" If it's greater
		// than the radius, we know the triangle (tri) is not close enough to consider a collision with, so we can return false.
		// If instead, the distance from our sphere's center to this point is <= radius, then we can continue on.
		// 
		// From here, we now have two things to consider: 
		//	#1:	We're left with a point on a plane, and a left over amount of radius (sphere radius - distance from center to plane point)
		//	#2:	The triangle (tri) exists on the plane our point's on, and it's bounded by it's edges.
		// 
		// Okay, perfect. So now we just need to test, "is the distance from our point to any given edge <= 'left-over-radius'?"
		// We can do this in much the same way as before when we got the closet point to the triangles (tri) plane from the center
		// of the sphere, only this time, we're using the point on the (tri) plane and the new "plane" given by each edge.
		// 
		// This will tell us if we're a "left-over-radius" distance from an edge, which is enough to consider a collision when we're
		// NOT inside of the triangle. Because if we are inside of the triangle, and for this hypothetical imagine it's a very large
		// triangle, the distance may be very far from an edge. Regardless, we're still inside of the triangle and should consider a 
		// collision. That's where the second test comes in: "are all the distances to the edges, negatives?" If they are, then that
		// simply means we'd need to move backwards to get to each edge. If that's the case, we're inside of the triangle and we can
		// return true.
		// 
		// This is a long-winded explanation for what is essentially a short sphere-triangle intersection test, but it's worth noting
		// because this is the first of it's kind I've made.
		//
		// Btw, pretty sure there's some edge cases like if a tiny sphere is nestled in a corner or something that's not handled here.
		// We could handle it rather simply by just asking if the distance from the point on the plane to a vertex is less than the
		// left over radius. Not too wild, but I don't think we even need to handle it and I'm fighting for my life to optimize this
		// ridiculous set of calculations as is.
		glm::vec3 pointOnPlane = triangles[index].center + triNorm * t;

		glm::vec3 edge13_norm = glm::normalize(glm::cross(triNorm, tri->vec_1 - tri->vec_3));
		glm::vec3 edge21_norm = glm::normalize(glm::cross(triNorm, tri->vec_2 - tri->vec_1));
		glm::vec3 edge32_norm = glm::normalize(glm::cross(triNorm, tri->vec_3 - tri->vec_2));

		glm::vec3 pntOnPln_to_e13 = tri->vec_1 - pointOnPlane;
		glm::vec3 pntOnPln_to_e21 = tri->vec_2 - pointOnPlane;
		glm::vec3 pntOnPln_to_e32 = tri->vec_3 - pointOnPlane;

		float t_e_13 = -glm::dot(pntOnPln_to_e13, -edge13_norm);
		float t_e_21 = -glm::dot(pntOnPln_to_e21, -edge21_norm);
		float t_e_32 = -glm::dot(pntOnPln_to_e32, -edge32_norm);

		float leftOverRadius = triangles[index].radius - abs(t);

		// Returns true if either the point on the plane is close enough to an edge, or it's inside of the triangle. 
		//return (t_e_13 <= 0.f && t_e_21 <= 0.f && t_e_32 <= 0.f);
		//return ((abs(t_e_13) >= 0.f && abs(t_e_13) <= leftOverRadius) || abs(t_e_21) >= 0.f && abs(t_e_21) <= leftOverRadius || abs(t_e_32) >= 0.f && abs(t_e_32) <= leftOverRadius);
		//return ((t_e_13 <= 0.f && t_e_21 <= 0.f && t_e_32 <= 0.f) || ((abs(t_e_13) >= 0.f && abs(t_e_13) <= leftOverRadius) || abs(t_e_21) >= 0.f && abs(t_e_21) <= leftOverRadius || abs(t_e_32) >= 0.f && abs(t_e_32) <= leftOverRadius));


		// From here: we have to decide "do the actual triangles themselves touch?" (which is a little more intense than the sphere -> triangle).
		if (((t_e_13 <= 0.f && t_e_21 <= 0.f && t_e_32 <= 0.f) || ((abs(t_e_13) >= 0.f && abs(t_e_13) <= leftOverRadius) || abs(t_e_21) >= 0.f && abs(t_e_21) <= leftOverRadius || abs(t_e_32) >= 0.f && abs(t_e_32) <= leftOverRadius))) {
			//std::cout << "\tpointOnPlane = (" << pointOnPlane.x << ", " << pointOnPlane.y << ", " << pointOnPlane.z << ")\n";
			//return false;
			
			
			// So 4 distinct cases we care about:
			//	#1:	Co-planar triangles:	They are in the same plane, flat, one on top of the other. We either deal with it or don't. Hard to say.
			//								On one hand: imagine a falling plane or something. On the other: we would def deal with that a different 
			//								way. For now, we can ignore this case and move on.
			//	
			//	#2:	Parallel triangles:		They are on two parallel planes facing each other. They won't interact, so we can discard this.
			// 
			//	#3:	One edge(s) through:	Here, one triangle is piercing the other but only on one of it's edges. This is the first non-trivial
			//								case.
			// 
			//	#4: Two edge(s) through:	Here, one triangle is piercing the other but now two edges of one triangle pass between two edges of
			//								the other triangle. This is a slightly more interesting case to deal with, and is obviously non-trivial.
			// 
			// In both non-trivial cases, we need to check where exactly the intersections between the triangles lie, so we can respond with the
			// appropriate forces in each direction.

			glm::vec3 objTriNorm = glm::cross(triangles[index].vec_1 - triangles[index].vec_3, triangles[index].vec_2 - triangles[index].vec_3);
			float dotBetweenTriangleNorms = dot(objTriNorm, -triNorm);

			// Co-planar or just parallel. For now, we can disregard this.
			if (abs(dotBetweenTriangleNorms) < 0.000001f) {
				return false;
			}
			else {
				//return true;
				
#if 1
				glm::vec3 objTri_e_13 = triangles[index].vec_1 - triangles[index].vec_3;
				glm::vec3 objTri_e_21 = triangles[index].vec_2 - triangles[index].vec_1;
				glm::vec3 objTri_e_32 = triangles[index].vec_3 - triangles[index].vec_2;

				glm::vec3 objTri_to_plane_11 = tri->vec_1 - triangles[index].vec_1;
				glm::vec3 objTri_to_plane_22 = tri->vec_2 - triangles[index].vec_2;
				glm::vec3 objTri_to_plane_33 = tri->vec_3 - triangles[index].vec_3;


				float denom_e_13 = glm::dot(glm::normalize(objTri_e_13), triNorm);
				float denom_e_21 = glm::dot(glm::normalize(objTri_e_21), triNorm);
				float denom_e_32 = glm::dot(glm::normalize(objTri_e_32), triNorm);

				//if (denom_e_13 > 0.0000001f) {

				// THIS WORKS!
				//float t1 = (dot(tri->vec_1 - triangles[index].vec_1, triNorm) / denom_e_13) * (denom_e_13 > 0.00000000001f) - float(!(denom_e_13 > 0.00000000001f));
				//float t2 = (dot(tri->vec_2 - triangles[index].vec_2, triNorm) / denom_e_21) * (denom_e_21 > 0.00000000001f) - float(!(denom_e_21 > 0.00000000001f));
				//float t3 = (dot(tri->vec_3 - triangles[index].vec_3, triNorm) / denom_e_32) * (denom_e_32 > 0.00000000001f) - float(!(denom_e_32 > 0.00000000001f));
				//std::cout << "t1 = " << t1 << "\tt2 = " << t2 << "\tt3 = " << t3 << "\n";

				//std::cout << "tri->vec_1 = (" << tri->vec_1.x << ", " << tri->vec_1.y << ", " << tri->vec_1.z << ")\n";
				//std::cout << "tri->vec_2 = (" << tri->vec_2.x << ", " << tri->vec_2.y << ", " << tri->vec_2.z << ")\n";
				//std::cout << "tri->vec_3 = (" << tri->vec_3.x << ", " << tri->vec_3.y << ", " << tri->vec_3.z << ")\n";

				int passedBaryTests = 0;
				if (denom_e_13 > 0.00000000001f) {
					float t1 = -(dot(tri->vec_1 - triangles[index].vec_1, triNorm) / denom_e_13);
					//if (t1 >= 0.f){//&& t1 <= glm::length(objTri_e_13)) {
					if (t1 >= 0.f && t1 <= glm::length(objTri_e_13)) {
						glm::vec3 pointOnPlane = triangles[index].vec_1 - (glm::normalize(objTri_e_13) * t1);
						passedBaryTests += (barycentricInterpolation(tri, pointOnPlane));
						//std::cout << "t1 = " << t1 << "\tpointOnPlane = (" << pointOnPlane.x << ", " << pointOnPlane.y << ", " << pointOnPlane.z << ")\n";
					}
				}

				if (denom_e_21 > 0.00000000001f) {
					float t2 = -(dot(tri->vec_2 - triangles[index].vec_2, triNorm) / denom_e_21);
					//if (t2 >= 0.f){// && t2 <= glm::length(objTri_e_21)) {
					if (t2 >= 0.f && t2 <= glm::length(objTri_e_21)) {
						glm::vec3 pointOnPlane = triangles[index].vec_2 - (glm::normalize(objTri_e_21) * t2);
						passedBaryTests += (barycentricInterpolation(tri, pointOnPlane));
						//std::cout << "t2 = " << t2 << "\tpointOnPlane = (" << pointOnPlane.x << ", " << pointOnPlane.y << ", " << pointOnPlane.z << ")\n";
					}
				}
				
				if (denom_e_32 > 0.00000000001f) {
					float t3 = -(dot(tri->vec_3 - triangles[index].vec_3, triNorm) / denom_e_32);
					//if (t3 >= 0.f) {// && t3 <= glm::length(objTri_e_32)) {
					if (t3 >= 0.f && t3 <= glm::length(objTri_e_32)) {
						glm::vec3 pointOnPlane = triangles[index].vec_3 - (glm::normalize(objTri_e_32) * t3);
						passedBaryTests += (barycentricInterpolation(tri, pointOnPlane));
						//std::cout << "t3 = " << t3 << "\tpointOnPlane = (" << pointOnPlane.x << ", " << pointOnPlane.y << ", " << pointOnPlane.z << ")\n";
					}
				}
				//std::cout << "passedBaryTests = " << passedBaryTests << "\n\n";
				return (passedBaryTests > 0);
#endif
			}
		}
	}
	return false;
}

// Problem is probably this tbh.
bool Object::barycentricInterpolation(Triangle* tri, glm::vec3 p) {

	//float areaABC = length(glm::cross(tri->vec_2 - tri->vec_1, tri->vec_3 - tri->vec_1)) / 2.f;
	//float alpha	= glm::length(glm::cross(tri->vec_2 - p, tri->vec_3 - p)) / areaABC;
	//float beta	= glm::length(glm::cross(tri->vec_3 - p, tri->vec_1 - p)) / areaABC;
	//float gama = 1 - alpha - beta;
	//return ((gama + alpha + beta == 1.f)
	//		&& (0.f <= alpha && alpha <= 1.f)
	//		&& (0.f <= beta && beta <= 1.f)
	//		&& (0.f <= gama && gama <= 1.f));
	// Compute vectors        
	glm::vec3 v0 = tri->vec_3 - tri->vec_1;
	glm::vec3 v1 = tri->vec_2 - tri->vec_1;
	glm::vec3 v2 = p - tri->vec_1;

	// Compute dot products
	float dot00 = dot(v0, v0);
	float dot01 = dot(v0, v1);
	float dot02 = dot(v0, v2);
	float dot11 = dot(v1, v1);
	float dot12 = dot(v1, v2);

	// Compute barycentric coordinates
	float invDenom = 1.f / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	return (u >= 0.f) && (v >= 0.f) && (u + v < 1.f);
}

// Determine if a given triangle intersects with any of the triangles on the Object.
bool Object::triangleIntersection(Triangle* tri) {
	tri->genCircle();

	// First intersection method: on the same plane
	//
	// Let T1, T2 be triangles, and let { v10, v11, v12 }, and { v20, v21, v22 } be their
	// respective vertices. Finally, let Pi1 and Pi2 be the planes on which T1 and T2 lie
	// respectively.
	// 
	// The equation dot(n2, X) + d2 == 0 determines if a given point, X, is on the plane.
	// n2 denotes the normal of the plane. The dot product of two vectors gives the cos of
	// the angle between them.
	//
	// Therefore, for any given point, X, if the angle given by dot(n2, X) == d2, d2 being the
	// computed angle between the normal and a point on the plane (it'll always be 90 degrees btw)
	// then that point is on the plane as well. Rearranging: we get dot(n2, X) + d2 == 0.
	//
	// Therefore, if dot(n2, Xi) + d2 == 0, for all Xi = { v10, v11, v12 }, the the triangle T1 is
	// on the same plane as triangle T2.
	//
	// If however, all points dot(n2, Xi) + d2 != 0, for all Xi = { v10, v11, v12 }, and the sign of
	// dot(n2, Xi) + d2 are all the same, then all the points lie on one side of the triangle or the other.
	// If however, there are at least two with different signs, then the triangle T1 is stabbing through the
	// plane in some manner.
	//
	// Handling the same computation with T2 and Pi1 will let us reject trivial cases early and save
	// on computation time.

	bool anyHits = false;

	float lenA = 0.f;
	float lenB = tri->radius;
	for (int tt = 0; tt < triangles.size(); tt++) {
		triangles[tt].genCircle();
		lenA = triangles[tt].radius;
		//glm::length(tri->center - triangles[tt].center) <= lenA + lenB;
		glm::vec3 triNorm = glm::vec3(0.f, 1.f, 0.f);
		float p = dot(triangles[tt].center - tri->center, triNorm) / glm::length(triNorm);
		//std::cout << "p = " << p;

		if (p < lenA && p > -lenA) {
			glm::vec3 v20 = tri->vec_1;	// Triangle 2's vertices
			glm::vec3 v21 = tri->vec_2;
			glm::vec3 v22 = tri->vec_3;
			glm::vec3 n2 = cross(v21 - v20, v22 - v20);	// Triangle 2's normal
			float d2 = dot(-n2, v20);	// Triangle 2's cos(angle between -n2 and one of the vertices)

			glm::vec3 v10 = triangles[tt].vec_1;	// Triangle 2's vertices
			glm::vec3 v11 = triangles[tt].vec_2;
			glm::vec3 v12 = triangles[tt].vec_3;
			glm::vec3 n1 = cross(v11 - v10, v12 - v10);	// Triangle 2's normal
			float d1 = dot(-n1, v10);	// Triangle 2's cos(angle between -n2 and one of the vertices)

			float dot_n2_v10 = dot(n2, v10) + d2, dot_n1_v20 = dot(n1, v20) + d1;	// Calculating the sum of dot(n2/1, v1/2 0->2) + d2/1.
			float dot_n2_v11 = dot(n2, v11) + d2, dot_n1_v21 = dot(n1, v21) + d1;	// Result will be 0 if the given point is on the plane.
			float dot_n2_v12 = dot(n2, v12) + d2, dot_n1_v22 = dot(n1, v22) + d1;

			float same_v10_v11 = signbit(dot_n2_v10) == signbit(dot_n2_v11);
			float same_v12_v10 = signbit(dot_n2_v12) == signbit(dot_n2_v10);
			float same_v11_v12 = signbit(dot_n2_v11) == signbit(dot_n2_v12);

			float same_v20_v21 = signbit(dot_n1_v20) == signbit(dot_n1_v21);
			float same_v22_v20 = signbit(dot_n1_v22) == signbit(dot_n1_v20);
			float same_v21_v22 = signbit(dot_n1_v21) == signbit(dot_n1_v22);

			if ((dot_n2_v10 == 0 && dot_n2_v11 == 0 && dot_n2_v12 == 0) && (dot_n1_v20 == 0 && dot_n1_v21 == 0 && dot_n1_v22 == 0)) {
				// Both triangles on the same plane
				std::cout << "Triangle[" << tt << "Same plane\n";
				anyHits = true;
			}
			else if ((int(same_v10_v11) && int(same_v11_v12)) || (int(same_v20_v21) && int(same_v21_v22))) {
				// T1 is on one side of Pi2.
				// T2 is on one side of Pi1.
				std::cout << "Triangle[" << tt << "]Trivial reject\n";
				//anyHits = false;
			}
			else {
				glm::vec3 t2_same_side_1, t2_same_side_2, t2_opposite_side;
				glm::vec3 t1_same_side_1, t1_same_side_2, t1_opposite_side;

				t1_same_side_1 = (v10 * same_v10_v11) + (v12 * same_v12_v10) + (v11 * same_v11_v12);
				t1_same_side_2 = (v11 * same_v10_v11) + (v10 * same_v12_v10) + (v12 * same_v11_v12);
				t1_opposite_side = (v12 * same_v10_v11) + (v11 * same_v12_v10) + (v10 * same_v11_v12);

				t2_same_side_1 = (v20 * same_v20_v21) + (v22 * same_v22_v20) + (v21 * same_v21_v22);
				t2_same_side_2 = (v21 * same_v20_v21) + (v20 * same_v22_v20) + (v22 * same_v21_v22);
				t2_opposite_side = (v22 * same_v20_v21) + (v21 * same_v22_v20) + (v20 * same_v21_v22);

				// Some amount of T1 pierces through Pi2 (for T2 and Pi1 also) with the line representing this cross section being
				// L = O + tD,	with D = n1 x n2 being the direction of the line and O being some point on the line.
				// Since the planes overlap, the triangles are guaranteed to both be intersected by the line L.
				//
				// Therefore, if we get the intervals of this intersection for both triangles, then the overlap between the two set of
				// intervals, will represent the overlap of the two triangles.

				// The result of the overlap test does not change if we project onto the
				// coordinate axis with which it is most closely aligned.Therefore equation can be
				// simplified:

				glm::vec3 D = cross(n1, n2);	// Defines the line representing the intersection between Pi1 and Pi2.
				float D_max = std::max({ abs(D.x), abs(D.y), abs(D.z) });
				int D_max_x = (D_max == abs(D.x)), D_max_y = (D_max == abs(D.y)), D_max_z = (D_max == abs(D.z));
				float p1_same_1, p1_same_2, p1_opposite, p2_same_1, p2_same_2, p2_opposite;

				// Wrote this with branchless coding techniques to squeeze out some performance.
				p1_same_1 = (t1_same_side_1.x * D_max_x) + (t1_same_side_1.y * D_max_y) + (t1_same_side_1.z * D_max_z);
				p1_opposite = (t1_opposite_side.x * D_max_x) + (t1_opposite_side.y * D_max_y) + (t1_opposite_side.z * D_max_z);
				p1_same_2 = (t1_same_side_2.x * D_max_x) + (t1_same_side_2.y * D_max_y) + (t1_same_side_2.z * D_max_z);

				p2_same_1 = (t2_same_side_1.x * D_max_x) + (t2_same_side_1.y * D_max_y) + (t2_same_side_1.z * D_max_z);
				p2_opposite = (t2_opposite_side.x * D_max_x) + (t2_opposite_side.y * D_max_y) + (t2_opposite_side.z * D_max_z);
				p2_same_2 = (t2_same_side_2.x * D_max_x) + (t2_same_side_2.y * D_max_y) + (t2_same_side_2.z * D_max_z);

				float d1_same_1, d1_same_2, d1_opposite;
				float d2_same_1, d2_same_2, d2_opposite;

				d1_same_1 = (dot_n2_v10 * same_v10_v11) + (dot_n2_v12 * same_v12_v10) + (dot_n2_v11 * same_v11_v12);
				d1_same_2 = (dot_n2_v11 * same_v10_v11) + (dot_n2_v10 * same_v12_v10) + (dot_n2_v12 * same_v11_v12);
				d1_opposite = (dot_n2_v12 * same_v10_v11) + (dot_n2_v11 * same_v12_v10) + (dot_n2_v10 * same_v11_v12);

				d2_same_1 = (dot_n1_v20 * same_v20_v21) + (dot_n1_v22 * same_v22_v20) + (dot_n1_v21 * same_v21_v22);
				d2_same_2 = (dot_n1_v21 * same_v20_v21) + (dot_n1_v20 * same_v22_v20) + (dot_n1_v22 * same_v21_v22);
				d2_opposite = (dot_n1_v22 * same_v20_v21) + (dot_n1_v21 * same_v22_v20) + (dot_n1_v20 * same_v21_v22);

				float interval_11 = p1_same_1 + ((p1_opposite - p1_same_1) * (d1_same_1 / (d1_same_1 - d1_opposite)));
				float interval_12 = p1_same_2 + ((p1_opposite - p1_same_2) * (d1_same_2 / (d1_same_2 - d1_opposite)));

				float interval_21 = p2_same_1 + ((p2_opposite - p2_same_1) * (d2_same_1 / (d2_same_1 - d2_opposite)));
				float interval_22 = p2_same_2 + ((p2_opposite - p2_same_2) * (d2_same_2 / (d2_same_2 - d2_opposite)));


				// Gotta now see if intervals overlap.
				std::cout << "Triangle[" << tt << "] Gotta now see if intervals overlap.\n";

				anyHits = true;
			}
		}
	}
	return anyHits;
}

// Finds the exact spot (if it exits) on the current object's mesh where the given object's mesh is touching
// it. From this we can use their relative kinematic values to determine each ones new values based on this
// collision.
glm::vec3 Object::getIntersection(Object* obj) {
	// So in general, we need to ask every triangle on each object if they're colliding with each other.
	// This will be pretty obviously slow as death. Though it won't change from being O(n^2), adding a 
	// heuristic to guess at which triangles would be best to search first would be smart. 


	return glm::vec3(0.f);
}
