#ifndef OBJECT_CLASS_H
#define OBJECT_CLASS_H

#define PI 3.1415926538

#include <iostream>
#include <fstream>
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

#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include"shaderClass.h"
#include"Camera.h"

/*
This class is for creating basic objects. It's meant to be a parent class.
*/
class Object
{
private:
	GLFWwindow* window;


	bool doNormalArrows;

	int samePosCount;
	std::vector<glm::vec3> velocityHistory;
	std::vector<glm::vec3> posHistory;

protected:
	std::vector <Texture> textures;
	bool randomColor;
	int seed;

public:
	int level;
	bool smooth;
	glm::vec3 radi;
	std::string name;

	VAO VAO;
	Shader* shaderProgram;
	Shader* normalShaderProgram;
	Camera* camera;
	Object(GLFWwindow* _window, glm::vec3 _objPos, float _objScale, glm::vec4 color, std::vector <Texture>& _textures, Camera* _camera);
	~Object();
	
	virtual void draw(glm::vec3 _lightPos, glm::vec4 _lightColor);
	virtual void drawNormals(glm::vec3 _lightPos, glm::vec4 _lightColor);
	virtual void setNormalsVBOandEBO();
	
	void setVBOandEBO(std::vector <Vertex>& _vertices, std::vector <GLuint>& _indices, std::string msg);
	
	virtual void setVBOandEBO(std::string msg);
	virtual void setLevel(int _level);
	virtual void doRandomColors(bool _randomColor);
	bool getRandomColor();
	virtual void smoothSurface(bool _isSmooth);
	
	
	glm::vec3 pointOnPlane(glm::vec3 planePoint, glm::vec3 planeNormal, glm::vec3 rayStart, glm::vec3 rayDirection);
	bool barycentricInterpolation(glm::vec3 _vert0, glm::vec3 _vert1, glm::vec3 _vert2, glm::vec3 _p);

	virtual glm::vec3 isRayTouching(glm::vec3 _rayStart, glm::vec3 _rayDir);
	virtual bool isCursorTouching(glm::vec2 _cursorPos);
	virtual void setColor(glm::vec4 _color);
	
	void setNewPos(glm::vec3 _objPos);
	void rotate(float rotationDegreeAngle, glm::vec3 axisOfRotation);
	void setScale(glm::vec3 _radi);
	void setScale(float _scale);
	
	void moveFirstVertex();

	void pointAt(glm::vec3 _direction, bool _isTopPointing);

	void hotRealoadShader();

	float newrand(glm::vec2 co);
	float noise(glm::vec2 n);

	void toggleNormalArrows();

	std::vector <Triangle> triangles;
	std::vector <Vertex> vertices;
	std::vector <VertexUI> verticesUI;
	std::vector <GLuint> indices;

	GLuint triangleType = GL_TRIANGLES;
	glm::vec4 color;
	float objScale;
	float objRadius = 1.f;
	glm::mat4 rotationMatrix;
	glm::mat4 model;
	glm::vec3 objPos;

	glm::vec3 pastObjPos;

	// Physics elements
	float deltaTime;
	float mass;
	glm::vec3 velocity;
	float rotationalVelocity;
	glm::vec3 rotationalAxis;

	virtual glm::vec3 rayToObject(glm::vec3 _ray);

	bool isTouching(Object* obj);
	bool isTouching(Triangle* tri);
	bool isTouching(Triangle* tri, int index, glm::vec3 *_p11, glm::vec3 *_p12, glm::vec3 *_p13, glm::vec3* _p21, glm::vec3* _p22, glm::vec3* _p23);
	bool barycentricInterpolation(Triangle* tri, glm::vec3 p);
	bool triangleIntersection(Triangle* tri, int index, glm::vec3* _p11, glm::vec3* _p12, glm::vec3* _p13, glm::vec3* _p21, glm::vec3* _p22, glm::vec3* _p23);
	bool triangleIntersection(Triangle* tri);
	glm::vec3 getIntersection(Object* obj);
	
	bool pointProjection(Triangle* triangle, glm::vec3 planePoint0, glm::vec3 planeNormal, glm::vec3 rayStart, glm::vec3 rayDirection, glm::vec3 *_p);
	glm::vec3 pointProjection(Triangle* triangle, glm::vec3 planePoint0, glm::vec3 planeNormal, glm::vec3 rayStart, glm::vec3 rayDirection);

	glm::vec3 isPointInsideModel(glm::vec3 p);

	void applyForce(Force f);
	void reflectAbout(Force f);

	void getNextPos();

	bool doPhysics = false;
	bool isSolid = false;

	bool isWireframe = false;
};

#endif
