#include "UI.h"

// Constructor for Plane.
UI::UI(GLFWwindow* _window, glm::vec3 _objPos, glm::vec2 _textOffset, glm::vec3 _radi, float _characterScale, std::string _text, glm::uvec2 (&_dictionary)[100], glm::vec4 _color, std::vector <Texture>& _textures, Camera* _camera) : Object(_window, _objPos, _characterScale, _color, _textures, _camera) {
	text = _text;
	dictionary = _dictionary;
	randomColor = false;
	seed = 1;
	realLetters = _text.length();

	radi = _radi;
	characterScale = _characterScale;

	backgroundColor = glm::vec4(0.f);

	textOffset = _textOffset;
	genTriangles();
}

glm::vec3 UI::rayToObject(glm::vec3 _ray) {
	//return glm::vec3(FLT_MAX);
	return glm::vec3(objPos);
}

void UI::setVBOandEBO(std::string msg) {
	// Generates Shader object using shaders ui.vert and ui.frag
	delete shaderProgram;
	
	name = msg;
	shaderProgram = new Shader("ui.vert", "ui.frag");
	
	shaderProgram->Activate();

	VAO.Bind();


	// Setting VBO and EBO
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(verticesUI);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);
	
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(VertexUI), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(VertexUI), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 4, GL_FLOAT, sizeof(VertexUI), (void*)(6 * sizeof(float)));
	VAO.LinkAttribI(VBO, 3, 2, GL_UNSIGNED_INT, sizeof(VertexUI), (void*)(10 * sizeof(float)));


	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;

	// Assign all the relevant information to the shader.
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}

		textures[i].texUnit(*shaderProgram, (type + num).c_str(), textures[i].unit);
		textures[i].Bind();
	}
	glUniform1i(glGetUniformLocation(shaderProgram->ID, "useTex"), numDiffuse);

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void UI::draw(glm::vec3 _lightPos, glm::vec4 _lightColor) {
	// Activating the shader program and binding the VAO so OpenGL knows what we're trying to do.
	shaderProgram->Activate();
	VAO.Bind();

	if (isWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Assigning all relevant info to the shader.
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "backgroundColor"), backgroundColor.a, backgroundColor.g, backgroundColor.b, backgroundColor.a);

	// Draw the actual mesh
	glDrawElements(triangleType, indices.size(), GL_UNSIGNED_INT, 0);

	// Drawing normals if applicable.
	if (normalShaderProgram != nullptr)
		drawNormals(_lightPos, _lightColor);
}


// Sets the string to something else.
// Kind of a ineffcient/busy function. Probably should split it up to make it look a little cleaner.
void UI::setNewString(std::string _code, bool _removeFormatting) {
	// Setting the texture coords.
	glm::vec3 texCoord1 = glm::vec3(-1.f, 1.f, 0.f);
	glm::vec3 texCoord2 = glm::vec3(1.f, 1.f, 0.f);
	glm::vec3 texCoord3 = glm::vec3(-1.f, -1.f, 0.f);
	glm::vec3 texCoord4 = glm::vec3(1.f, -1.f, 0.f);

	// Setting color;
	glm::vec4 clr = color;

	// Helper variables to track character offsets.
	xCount = 0.f;
	yCount = 0.f;
	int indicesCount = 0;

	// Getting lengths.
	int crntTextLength = verticesUI.size() / 4;
	int newTextLength = _code.length();
	
	for (int i = 0; i < newTextLength; i++) {
		char c = _code[i];
		if (int(c) == 10) {
			yCount += 1.f;
			xCount = 0.f;
			continue;
		}
		// Setting character spacings.
		float xLeft = textOffset.x + characterScale * 2.f * xCount;
		float xRight = textOffset.x + characterScale * 2.f * (xCount + 1.f);
		float yUp = textOffset.y - characterScale * 2.f * yCount;
		float yDown = textOffset.y - characterScale * 2.f * (yCount + 1.f);
		// Incrementing xCount.
		xCount += 1.f;

		// Setting character positions.
		glm::vec3 pos1 = glm::vec3(xLeft, yUp, 0.f);
		glm::vec3 pos2 = glm::vec3(xRight, yUp, 0.f);
		glm::vec3 pos3 = glm::vec3(xLeft, yDown, 0.f);
		glm::vec3 pos4 = glm::vec3(xRight, yDown, 0.f);

		// If characters are at the end of the rectangle, move to next line down.
		if (textOffset.x + characterScale * 2.f * (xCount + 1.f) > textOffset.x + 2.f) {
			xCount = 0.f;
			yCount += 1.f;
		}


		// Setting the character to be displayed.
		glm::uvec2 character = dictionary[int(c) % 100];
		if (i < crntTextLength) {
			verticesUI[indicesCount + 0].letter = character;
			verticesUI[indicesCount + 1].letter = character;
			verticesUI[indicesCount + 2].letter = character;
			verticesUI[indicesCount + 3].letter = character;
			if (_removeFormatting) {
				// Setting the character to be displayed.
				verticesUI[indicesCount + 0].pos = pos1;
				verticesUI[indicesCount + 1].pos = pos2;
				verticesUI[indicesCount + 2].pos = pos3;
				verticesUI[indicesCount + 3].pos = pos4;
			}
		}
		else {

			// Adding Vertex info to vector.
			verticesUI.push_back(VertexUI{ pos1, texCoord1, clr, character });
			verticesUI.push_back(VertexUI{ pos2, texCoord2, clr, character });
			verticesUI.push_back(VertexUI{ pos3, texCoord3, clr, character });
			verticesUI.push_back(VertexUI{ pos4, texCoord4, clr, character });

			// Lower left triangle.
			indices.push_back(indicesCount + 0);
			indices.push_back(indicesCount + 2);
			indices.push_back(indicesCount + 3);
			// Upper right triangle.
			indices.push_back(indicesCount + 0);
			indices.push_back(indicesCount + 3);
			indices.push_back(indicesCount + 1);
		}
		// Increment indices counter by 4 to move to next letter square.
		indicesCount += 4;
	}

	// Polishing off removing previous text.
	if (text.length() > _code.length())
		for (int i = 0; i < text.length() - _code.length(); i++) {
			glm::uvec2 space = dictionary[0];
			verticesUI[indicesCount + 0].letter = space;
			verticesUI[indicesCount + 1].letter = space;
			verticesUI[indicesCount + 2].letter = space;
			verticesUI[indicesCount + 3].letter = space;
		}


	// Finally, setting text equal to the new string (_code) and binding the vertexUI to the VAO. 
	text = _code;
	setVBOandEBO("UI");
}

// Sets the string to something else.
// Kind of a ineffcient/busy function. Probably should split it up to make it look a little cleaner.
void UI::setNewString(std::string _code) {
	// Setting the texture coords.
	glm::vec3 texCoord1 = glm::vec3(-1.f, 1.f, 0.f);
	glm::vec3 texCoord2 = glm::vec3(1.f, 1.f, 0.f);
	glm::vec3 texCoord3 = glm::vec3(-1.f, -1.f, 0.f);
	glm::vec3 texCoord4 = glm::vec3(1.f, -1.f, 0.f);

	// Setting color;
	glm::vec4 clr = color;

	// Helper variables to track character offsets.
	xCount = 0.f;
	yCount = 0.f;
	int indicesCount = 0;

	// Getting lengths.
	int crntTextLength = verticesUI.size() / 4;
	int newTextLength = _code.length();

	int vertCount = 0;

	if (crntTextLength > newTextLength) { // If the old string is longer.
		for (int i = 0; i < crntTextLength; i++) {
			if (i < newTextLength) {
				// Incrementing xCount.
				xCount += 1.f;

				// If characters are at the end of the rectangle, move to next line down.
				if (textOffset.x + characterScale * 2.f * (xCount + 1.f) > textOffset.x + 2.f) {
					xCount = 0.f;
					yCount += 1.f;
				}

				char c = _code[i];
				if (int(c) == 10) {
					yCount += 1.f;
					xCount = 0.f;
					continue;
				}
				// Setting the character to be displayed.
				glm::uvec2 character = dictionary[int(c) % 100];
				verticesUI[vertCount + 0].letter = character;
				verticesUI[vertCount + 1].letter = character;
				verticesUI[vertCount + 2].letter = character;
				verticesUI[vertCount + 3].letter = character;

				realLetters++;
			}
			else {
				// Setting the character to be displayed.
				glm::uvec2 character = dictionary[0];
				verticesUI[vertCount + 0].letter = character;
				verticesUI[vertCount + 1].letter = character;
				verticesUI[vertCount + 2].letter = character;
				verticesUI[vertCount + 3].letter = character;
			}
			vertCount += 4;
		}
	}
	else { // If the new string is longer.
		for (int i = 0; i < newTextLength; i++) {
			char c = _code[i];
			if (int(c) == 10) {
				yCount += 1.f;
				xCount = 0.f;
				continue;
			}
			// Setting character spacings.
			float xLeft = textOffset.x + characterScale * 2.f * xCount;
			float xRight = textOffset.x + characterScale * 2.f * (xCount + 1.f);
			float yUp = textOffset.y - characterScale * 2.f * yCount;
			float yDown = textOffset.y - characterScale * 2.f * (yCount + 1.f);
			// Incrementing xCount.
			xCount += 1.f;

			// If characters are at the end of the rectangle, move to next line down.
			if (textOffset.x + characterScale * 2.f * (xCount + 1.f) > textOffset.x + 2.f) {
				xCount = 0.f;
				yCount += 1.f;
			}


			// Setting the character to be displayed.
			glm::uvec2 character = dictionary[int(c) % 100];
			if (i < crntTextLength) {
				// Setting the character to be displayed.
				verticesUI[indicesCount + 0].letter = character;
				verticesUI[indicesCount + 1].letter = character;
				verticesUI[indicesCount + 2].letter = character;
				verticesUI[indicesCount + 3].letter = character;
			}
			else {
				// Setting character positions.
				glm::vec3 pos1 = glm::vec3(xLeft, yUp, 0.f);
				glm::vec3 pos2 = glm::vec3(xRight, yUp, 0.f);
				glm::vec3 pos3 = glm::vec3(xLeft, yDown, 0.f);
				glm::vec3 pos4 = glm::vec3(xRight, yDown, 0.f);

				// Adding Vertex info to vector.
				verticesUI.push_back(VertexUI{ pos1, texCoord1, clr, character });
				verticesUI.push_back(VertexUI{ pos2, texCoord2, clr, character });
				verticesUI.push_back(VertexUI{ pos3, texCoord3, clr, character });
				verticesUI.push_back(VertexUI{ pos4, texCoord4, clr, character });

				// Lower left triangle.
				indices.push_back(indicesCount + 0);
				indices.push_back(indicesCount + 2);
				indices.push_back(indicesCount + 3);
				// Upper right triangle.
				indices.push_back(indicesCount + 0);
				indices.push_back(indicesCount + 3);
				indices.push_back(indicesCount + 1);
			}
			// Increment indices counter by 4 to move to next letter square.
			indicesCount += 4;
		}
	}

	// Finally, setting text equal to the new string (_code) and binding the vertexUI to the VAO. 
	text = _code;
	setVBOandEBO("UI");
}

// Basically the same as setNewString, except it works specifically with doubles (or any floating point numbers)
// to get an exact precision so you only get the characters you want.
void UI::setNewNumber(double _num, int _precision) {
	// To get a string with as many whole numbers as required + '.' + n many decimals.
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
	setNewString(shortString);
}

// Appends current string with anther string.
void UI::appendString(std::string _code) {
	//text += _code;
	//setNewString(text);
	// Setting the texture coords.
	glm::vec3 texCoord1 = glm::vec3(-1.f, 1.f, 0.f);
	glm::vec3 texCoord2 = glm::vec3(1.f, 1.f, 0.f);
	glm::vec3 texCoord3 = glm::vec3(-1.f, -1.f, 0.f);
	glm::vec3 texCoord4 = glm::vec3(1.f, -1.f, 0.f);

	// Setting color;
	glm::vec4 clr = color;

	int vertsSize = (indices.size() / 6);
	int indicesCount = (indices.size() / 6) * 4;
	int crntTextLength = text.length();
	int vertexIndex = crntTextLength * 4;
	int newTextLength = _code.length();

	// Basically loops through and adds all the new characters in, but if the square for the character
	// was already made, and blanked out previously, it just writes the character into that new slot
	// instead of wiping out the whole string and remaking it with text += + _code.
	for (int i = 0; i < newTextLength; i++) {
		char c = _code[i];
		if (int(c) == 10) {
			yCount += 1.f;
			xCount = 0.f;
			continue;
		}

		// Setting character spacings.
		float xLeft = textOffset.x + characterScale * 2.f * xCount;
		float xRight = textOffset.x + characterScale * 2.f * (xCount + 1.f);
		float yUp = textOffset.y - characterScale * 2.f * yCount;
		float yDown = textOffset.y - characterScale * 2.f * (yCount + 1.f);
		// Incrementing xCount.
		xCount += 1.f;

		// If characters are at the end of the rectangle, move to next line down.
		if (textOffset.x + characterScale * 2.f * (xCount + 1.f) > textOffset.x + 2.f) {
			xCount = 0.f;
			yCount += 1.f;
		}

		// Setting character positions.
		glm::vec3 pos1 = glm::vec3(xLeft, yUp, 0.f);
		glm::vec3 pos2 = glm::vec3(xRight, yUp, 0.f);
		glm::vec3 pos3 = glm::vec3(xLeft, yDown, 0.f);
		glm::vec3 pos4 = glm::vec3(xRight, yDown, 0.f);

		// Setting the character to be displayed.
		glm::uvec2 character = dictionary[int(c) % 100];
		if (i < vertsSize - crntTextLength) {
			// Setting the character to be displayed.
			verticesUI[vertexIndex + 0].letter = character;
			verticesUI[vertexIndex + 1].letter = character;
			verticesUI[vertexIndex + 2].letter = character;
			verticesUI[vertexIndex + 3].letter = character;
			vertexIndex += 4;
		}
		else {
			// Setting character positions.
			glm::vec3 pos1 = glm::vec3(xLeft, yUp, 0.f);
			glm::vec3 pos2 = glm::vec3(xRight, yUp, 0.f);
			glm::vec3 pos3 = glm::vec3(xLeft, yDown, 0.f);
			glm::vec3 pos4 = glm::vec3(xRight, yDown, 0.f);

			// Adding Vertex info to vector.
			verticesUI.push_back(VertexUI{ pos1, texCoord1, clr, character });
			verticesUI.push_back(VertexUI{ pos2, texCoord2, clr, character });
			verticesUI.push_back(VertexUI{ pos3, texCoord3, clr, character });
			verticesUI.push_back(VertexUI{ pos4, texCoord4, clr, character });

			// Lower left triangle.
			indices.push_back(indicesCount + 0);
			indices.push_back(indicesCount + 2);
			indices.push_back(indicesCount + 3);
			// Upper right triangle.
			indices.push_back(indicesCount + 0);
			indices.push_back(indicesCount + 3);
			indices.push_back(indicesCount + 1);

			// Increment indices counter by 4 to move to next letter square.
			indicesCount += 4;
		}
	}

	// Finally, setting text equal to the new string (_code) and binding the vertexUI to the VAO. 
	text += _code;
	setVBOandEBO("UI");
}

// Appends current string with float to given precision.
void UI::appendNumber(double _num, int _precision) {
	// To get a string with as many whole numbers as required + '.' + n many decimals.
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

	appendString(shortString);
}

// Sets background color of text.
void UI::setColor(glm::vec4 _color) {
	backgroundColor = _color;
}

std::string UI::getString() {
	return text;
}

// Creates a new set of Vertex's and their associated indices to send to the Object
// class for drawing.
void UI::genTriangles() {
	genOctahedron();
	setVBOandEBO("UI");
}

// Generates the basic retangle with added characters.
void UI::genOctahedron() {

	// Clearing verts and inds.
	verticesUI.clear();
	indices.clear();

	// Setting text.
	setNewString(text);

	// Setting transformation matrix.
	//glm::vec3 scalingVec = glm::vec3(objScale, objScale, 0.f);
	glm::vec3 screenPos = glm::vec3(objPos.x, objPos.y, 0.f);
	//model = glm::translate(glm::mat4(1.f), screenPos - scalingVec);
	model = glm::translate(glm::mat4(1.f), screenPos);
	model = glm::scale(model, radi);
}

// Yee old destructor.
UI::~UI() {
}

// Sets the screen coordinates of the UI object.
void UI::setScreenPos(glm::vec2 _screenCoords) {
	objPos = glm::vec3(_screenCoords, objPos.z);
	//model = glm::translate(glm::mat4(1.f), glm::vec3(_screenCoords - scale, 0.0f));
	model = glm::translate(glm::mat4(1.f), objPos);
	model = glm::scale(model, radi);
}

// Sets the X and Y scaling factors for the UI objects transformation.
void UI::setScale(glm::vec3 _radi) {
	radi = _radi;
	//model = glm::translate(glm::mat4(1.f), objPos - glm::vec3(scale, 0.0f));
	model = glm::translate(glm::mat4(1.f), objPos);
	model = glm::scale(model, radi);
}


glm::vec3 UI::getScale() {
	return radi;
}

glm::vec3 UI::isRayTouching(glm::vec3 _rayStart, glm::vec3 _rayDir) {
	if (isCursorTouching(camera->getCursorPos()))
		return camera->cameraPos;
	return glm::vec3(FLT_MAX);
}

// Returns whether 2D coordinates are touching the UI Object.
bool UI::isCursorTouching(glm::vec2 _cursorPos)
{
	// Barycentric test.
	glm::vec3 p = glm::vec3(_cursorPos, 0.f);
	for (int i = 0; i < indices.size(); i+=3) {
		glm::vec3 vec_1 = glm::vec3(model * glm::vec4(verticesUI[indices[i + 0]].pos, 1.f));
		glm::vec3 vec_2 = glm::vec3(model * glm::vec4(verticesUI[indices[i + 1]].pos, 1.f));
		glm::vec3 vec_3 = glm::vec3(model * glm::vec4(verticesUI[indices[i + 2]].pos, 1.f));

		// Compute vectors        
		glm::vec3 v0 = vec_3 - vec_1;
		glm::vec3 v1 = vec_2 - vec_1;
		glm::vec3 v2 = p - vec_1;

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
		bool result = ((u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.01f));

		if (result)
			return result;

	}
	return false;
}

void UI::setCharacterColor(glm::vec4 _color) {
	color = _color;
	genTriangles();
}