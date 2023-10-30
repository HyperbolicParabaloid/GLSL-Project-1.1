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

// Camera class for handling the view matrix.
class Camera
{
private:
	GLFWwindow* window;

	glm::mat4 view;

	// For constructing view;
	glm::vec3 cameraPos;
	glm::vec3 cameraForward;
	glm::vec3 cameraUp;
	glm::vec2 screenDimensions;

	float lastX, lastY, yaw, pitch;	// Keeping track of the motion.
	bool firstMouse = true, motionEnabled = false, flyingMouseLock = false, stopFlying = false, currentlyFlying = false;

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	float cameraSpeed = 2.5f; // Sensitivity of WASD movements
	float sensitivity = 0.1f; // Sensitivity of mouse movements

	void processInput();	// For processing the WASD movement
	void mouse_callback();	// For processing the mouse movement

public:
	Camera(GLFWwindow* window, glm::vec2 screenDimensions, glm::vec3 _cameraPos, glm::vec3 _cameraForward, glm::vec3 _cameraUp);	// Constructor

	void motion_enabled(bool state);	// For toggling the motion capture
	void set_camera_speed(float _cameraSpeed);	// For setting the WASD movement sensitivity
	void set_camera_sensitivity(float _cameraSense);	// For setting the mouse movement sensitivity

	void track_movement();	// Tracks movements of player
	glm::mat4 get_view();	// Returns the view matrix

	bool fly_to(glm::vec3 newPos, glm::vec3 newForward);
	bool fly_to(glm::vec3 newPos);
};

