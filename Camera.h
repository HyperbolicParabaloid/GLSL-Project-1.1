#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

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

#include"shaderClass.h"

// Camera class for handling the view matrix.
class Camera
{
private:
	GLFWwindow* window;

	glm::mat4 view;
	glm::mat4 projection;

	// For constructing view;
	glm::vec3 cameraUp;
	glm::vec2 screenDimensions;

	float lastX, lastY, yaw, pitch;	// Keeping track of the motion.
	bool firstMouse = true, motionEnabled = false, flyingMouseLock = false, stopFlying = false, currentlyFlying = false;

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	float cameraSpeed = 2.5f; // Sensitivity of WASD movements
	float sensitivity = 0.1f; // Sensitivity of mouse movements
	float flySpeedScaler = 5.f;	// Scaler to make fly-in speed faster

	void processInput();	// For processing the WASD movement
	void mouse_callback();	// For processing the mouse movement

public:
	glm::vec3 cameraForward;
	glm::vec3 cameraPos;
	Camera(GLFWwindow* window, glm::vec2 screenDimensions, glm::vec3 _cameraPos, glm::vec3 _cameraForward, glm::vec3 _cameraUp);	// Constructor

	void set_projection(float angle, float aspectRatio, float nearClip, float farClip);

	void motion_enabled(bool state);	// For toggling the motion capture
	void set_camera_speed(float _cameraSpeed);	// For setting the WASD movement sensitivity
	void set_camera_sensitivity(float _cameraSense);	// For setting the mouse movement sensitivity
	void set_fly_speed_scaler(float _flySpeedScaler);	// For setting the motion scaler for the camera's motion/turning speed

	void track_movement();	// Tracks movements of player
	glm::mat4 get_view();	// Returns the view matrix

	bool lookAtObj(glm::vec3 _objPos);
	bool fly_to(glm::vec3 newPos, glm::vec3 newForward, bool lockCursorMovement);	// Specifying a position, forward and option to lock camera
	bool fly_to(glm::vec3 newPos);	// Specifying a position
	bool setMousePos(glm::vec3 _newForward, bool _lockCursorMovement, bool _snap);	// forward and option to lock camera

	void camMatrixForShader(Shader& _newShader, const char* uniformName);	// Sets the view matrix for the camera whatever shader program and uniform name are provided
	//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif