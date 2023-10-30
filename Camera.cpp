#include "Camera.h"

// Constructor takes in a window and the screen dimensions to initialize view
// matrix, with default values for cameraUp, cameraPos and cameraForward being
// assumed to be vec3(0.f, 1.f, 0.f), vec3(0.f, 0.5f, 2.f) and vec3(0.f, 0.f, -1.f)
Camera::Camera(GLFWwindow* _window, glm::vec2 screenDimensions, glm::vec3 _cameraPos, glm::vec3 _cameraForward, glm::vec3 _cameraUp) {
	// Assinging values to the prominant vectors.
	cameraPos = _cameraPos;
	cameraForward = _cameraForward;
	cameraUp = _cameraUp;

	// Initalizing window (pressently cannot be changed)
	window = _window;

	// Setting some defaults for sense and speed.
	cameraSpeed = 2.5f;
	sensitivity = 0.1f;

	// Have to initialize yaw to be -90 degrees or it'll have you turned to the right when you start
	// tracking mouse movements.
	yaw = -90;
	pitch = 0;
}

// This function takes mouse inputs and converts it into camera rotation.
void Camera::mouse_callback()
{
	// 2D position of the mousse on the screen.
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// If we've stopped and want to start tracking again, we want to set these like this or the camera
	// will snap when we start again.
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	// Getting the offset.
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	// Offset multiplied by sense.
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Clamping the values so they can't get too large.
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// Finally, getting a new direction vection and setting cameraForward.
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraForward = glm::normalize(direction);
}

// Processes the WASAD movement inputs in a sort of flying-motion based on the cameraForward vector.
void Camera::processInput()
{
	// Making sure framerate is constant among all framerates.
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	float speed = cameraSpeed * deltaTime;

	// Maping key inputs to motion.
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += speed * cameraForward;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= speed * cameraForward;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraForward, cameraUp)) * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraForward, cameraUp)) * speed;
}

// Toggles the cursor lock/movement capturing for WASD/cursor postion.
void Camera::motion_enabled() {
	motionEnabled = !motionEnabled;

	// Setting "firstMouse" to true so whenever I stop and start the camera won't snap.
	firstMouse = true;
	if (motionEnabled)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else 
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

// returns view matrix.
glm::mat4 Camera::get_view() {
	view = glm::lookAt(cameraPos, cameraPos + cameraForward, cameraUp);
	return view;
}

// For setting the WASD movement sensitivity
void Camera::set_camera_speed(float _cameraSpeed) {
	cameraSpeed = _cameraSpeed;
}

// For setting the mouse movement sensitivity
void Camera::set_camera_sensitivity(float _cameraSense) {
	sensitivity = _cameraSense;
}

// Tracks movements of player
void Camera::track_movement() {
	if (motionEnabled) {
		mouse_callback();
		processInput();
	}
}


