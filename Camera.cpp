#include "Camera.h"

//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	std::cout << "offset = (" << xoffset << ", " << yoffset << ")\n";
//}

// Constructor takes in a window and the screen dimensions to initialize view
// matrix, with default values for cameraUp, cameraPos and cameraForward being
// assumed to be vec3(0.f, 1.f, 0.f), vec3(0.f, 0.5f, 2.f) and vec3(0.f, 0.f, -1.f)
Camera::Camera(GLFWwindow* _window, glm::vec2 _screenDimensions, glm::vec3 _cameraPos, glm::vec3 _cameraForward, glm::vec3 _cameraUp) {
	// Assinging values to the prominant vectors.
	cameraPos = _cameraPos;
	cameraForward = _cameraForward;
	cameraUp = _cameraUp;
	screenDimensions = _screenDimensions;

	// Initalizing window (pressently cannot be changed)
	window = _window;

	// Setting some defaults for sense and speed.
	cameraSpeed = 2.5f;
	sensitivity = 0.1f;

	// Have to initialize yaw to be -90 degrees or it'll have you turned to the right when you start
	// tracking mouse movements.
	yaw = -90;
	pitch = 0;

	//glfwSetScrollCallback(window, scroll_callback);
}

void Camera::set_projection(float angle, float aspectRatio, float nearClip, float farClip) {
	projection = glm::perspective(angle, aspectRatio, nearClip, farClip);
}

// This function takes mouse inputs and converts it into camera rotation.
void Camera::mouse_callback()
{
	// 2D position of the mousse on the screen.
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Taking what I know of the cameraForward vector we can use asin and atan2 to get the pitch and yaw.
	// I want to do this so if I change the cameraForward vector somewhere else this function won't cause
	// the view to snap back to where it was previously looking.
	//
	// TL:DR, this function will ONLY track the delta of the mouse position, A.K.A., a change in the cursor,
	// it won't effect the cameraForward vector in any other case.
	pitch = glm::degrees(asin(cameraForward.y));
	yaw = glm::degrees(atan2(cameraForward.z, cameraForward.x));

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

	// This is to lock out the cameraForward change until
	if (flyingMouseLock)
		return;

	// Finally, getting a new direction vector and setting cameraForward.
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraForward = direction;

}

// Processes the WASAD movement inputs in a sort of flying-motion based on the cameraForward vector.
void Camera::processInput()
{
	// Making sure framerate is constant among all framerates.
	float speed = cameraSpeed * deltaTime;

	// Changed this a little to make it more like normal Minecraft creative mode flying.
	glm::vec3 temp = cameraPos;
	glm::vec3 right = glm::normalize(glm::cross(cameraForward, cameraUp));
	glm::vec3 forward = glm::normalize(glm::cross(cameraUp, right));

	// Maping key inputs to motion.
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += speed * forward;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= speed * forward;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= right * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += right * speed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraPos += glm::normalize(cameraUp) * speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraPos -= glm::normalize(cameraUp) * speed;

	// Added in a break to stop the flying in motion if I want to at any point.
	if (temp != cameraPos && currentlyFlying)
		stopFlying = true;
	else 
		stopFlying = false;
}

// Toggles the cursor lock/movement capturing for WASD/cursor postion.
void Camera::motion_enabled(bool state) {
	motionEnabled = state;

	// Setting "firstMouse" to true so whenever I stop and start the camera won't snap.
	firstMouse = true;
	if (motionEnabled)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else 
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

// returns view matrix.
glm::mat4 Camera::get_view() {
	// Doing this to keep motion consistent, otherwise when I do something initially like fly_to() the
	// initial delta time will be huge causing unintended aggressive motion.
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	view = projection * glm::lookAt(cameraPos, cameraPos + cameraForward, cameraUp);

	return view;
}

// Sets the view matrix for the camera whatever shader program and uniform name are provided.
void Camera::camMatrixForShader(Shader& _newShader, const char* uniformName) {
	GLuint uniView = glGetUniformLocation(_newShader.ID, uniformName);
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

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

// Flies the camera to a specified location and faces the camera to new newly specified direction
bool Camera::fly_to(glm::vec3 _newPos, glm::vec3 _newForward, bool lockCursorMovement) {

	bool mouseIsDone = setMousePos(_newForward, lockCursorMovement);

	// Making sure framerate is constant among all framerates.
	// NOTE: if we don't have delta time updated in view or something of that nature, we'll get
	// a massive initial delta time which will send the camera way off in another direction.
	// To fix this, whenever get_view() is called we update delta-time there.
	float speed = cameraSpeed * deltaTime * flySpeedScaler;

	// Setting delta
	glm::vec3 deltaPos = (_newPos - cameraPos);

	// Variable to control the distance at which the camera's position will snap to the newPos
	// so it doesn't keep going at infinitesimally smaller steps and never get there.
	float snapInPlaceDistance = 0.01f;

	// Setting new values for cameraPosition
	// ____________________________________
	if (abs(cameraPos.x - _newPos.x) < snapInPlaceDistance)
		cameraPos.x = _newPos.x;
	else
		cameraPos.x += deltaPos.x * speed;
	// For y
	if (abs(cameraPos.y - _newPos.y) < snapInPlaceDistance)
		cameraPos.y = _newPos.y;
	else
		cameraPos.y += deltaPos.y * speed;
	// For z
	if (abs(cameraPos.z - _newPos.z) < snapInPlaceDistance)
		cameraPos.z = _newPos.z;
	else
		cameraPos.z += deltaPos.z * speed;

	if (_newPos == cameraPos && mouseIsDone)
		return true;
	else
		return false;
}

// flys the camera to a specified location
bool Camera::fly_to(glm::vec3 _newPos) {
	//std::cout << "----------------------\nDOING IT NOW!!!\n--------------------\n";
	//std::cout << "cameraPos = (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")\n";
	currentlyFlying = true;

	if (_newPos == cameraPos || stopFlying) {
		return true;
	}

	// Making sure framerate is constant among all framerates.
	// NOTE: if we don't have delta time updated in view or something of that nature, we'll get
	// a massive initial delta time which will send the camera way off in another direction.
	// To fix this, whenever get_view() is called we update delta-time there.
	float speed = cameraSpeed * deltaTime * flySpeedScaler;

	glm::vec3 deltaPos = (_newPos - cameraPos);

	// Variable to control the distance at which the camera's position will snap to the newPos
	// so it doesn't keep going at infinitesimally smaller steps and never get there.
	float snapInPlaceDistance = 0.01f;

	if (abs(cameraPos.x - _newPos.x) < snapInPlaceDistance)
		cameraPos.x = _newPos.x;
	else
		cameraPos.x += deltaPos.x * speed;
	// For y
	if (abs(cameraPos.y - _newPos.y) < snapInPlaceDistance)
		cameraPos.y = _newPos.y;
	else
		cameraPos.y += deltaPos.y * speed;
	// For z
	if (abs(cameraPos.z - _newPos.z) < snapInPlaceDistance)
		cameraPos.z = _newPos.z;
	else
		cameraPos.z += deltaPos.z * speed;

	//glfwSetCursorPos

	return false;
}

// Turns the camera to aim at the specified vector
bool Camera::setMousePos(glm::vec3 _newForward, bool lockCursorMovement) {
	// This values controls at what point the cameraForward vector will snap onto the target,
	// without it the function will run essentially forever getting infinitely closer but never
	// reaching it due to decaying size of the deltaForward and floating point errors.
	float snapInPlaceDistance = 0.001f;

	// Normalizing the _newForward vector to make sure the delta's work out.
	_newForward = glm::normalize(_newForward);

	// Making sure framerate is constant among all framerates.
	float speed = cameraSpeed * deltaTime * flySpeedScaler;

	// Getting new delta of the position.
	glm::vec3 deltaForward = (_newForward - cameraForward) * speed;

	// Setting new cameraForward position for:
	// For x
	if (abs(cameraForward.x - _newForward.x) < snapInPlaceDistance)
		cameraForward.x = _newForward.x;
	else
		cameraForward.x += deltaForward.x;
	// For y
	if (abs(cameraForward.y - _newForward.y) < snapInPlaceDistance)
		cameraForward.y = _newForward.y;
	else
		cameraForward.y += deltaForward.y;
	// For z
	if (abs(cameraForward.z - _newForward.z) < snapInPlaceDistance)
		cameraForward.z = _newForward.z;
	else
		cameraForward.z += deltaForward.z;

	// Finally, checking if the camerForward vector matches the _newForward and if so, returning true.
	if (_newForward == cameraForward) {
		flyingMouseLock = false;
		return true;
	}
	else {
		if (lockCursorMovement)
			flyingMouseLock = true;
		return false;
	}
}

// For setting the motion scaler for the camera's motion/turning speed
void Camera::set_fly_speed_scaler(float _flySpeedScaler) {
	flySpeedScaler = _flySpeedScaler;
}

