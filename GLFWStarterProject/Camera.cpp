#include "Camera.h"

#include <iostream> // DEBUG

Camera::Camera()
{
	this->position = glm::vec3(0.0f, 0.0f, 20.0f);
	this->distance = 20.0f;
	this->direction = glm::vec3(0.0f, 0.0f, 0.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
	this->position = position;
	this->direction = direction;
	this->up = up;
}


Camera::~Camera()
{
}

void Camera::update()
{
	float horizontalDistance = calculateHorizontalDistance();
	float verticalDistance = calculateVerticalDistance();
	calculateCameraPosition(horizontalDistance, verticalDistance);

	this->yaw = 180.0f - angleOrbit;
}

glm::vec3 Camera::getPosition()
{
	return this->position;
}

glm::vec3 Camera::getDirection()
{
	return this->direction;
}

glm::vec3 Camera::getUp()
{
	return this->up;
}

glm::mat4 Camera::getLookAt()
{
	return glm::lookAt(position, direction, up);
}

float Camera::getPitch()
{
	return this->pitch;
}

float Camera::getYaw()
{
	return this->yaw;
}

float Camera::getRoll()
{
	return this->roll;
}

void Camera::key_callback(int key, int scancode, int action, int mods) {}

void Camera::mouse_button_callback(int button, int action, int mods, double xPos, double yPos)
{
	// If mouse is pressed
	if (action == GLFW_PRESS)
	{
		// Right mouse button is pressed
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			rightMouseDown = true;

			startRightMouseY = (float)yPos;
		}

		// Left mouse button is pressed
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			leftMouseDown = true;

			startLeftMouseX = (float)xPos;
		}
	}
	
	// If mouse is released
	if (action == GLFW_RELEASE)
	{
		// Right mouse button is released
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			rightMouseDown = false;
		}

		// Left mouse button is released
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			leftMouseDown = false;
		}
	}
	
}

void Camera::cursor_position_callback(double xpos, double ypos)
{
	// Right mouse button is held down so calculate pitch
	if (rightMouseDown)
	{
		endRightMouseY = (float)ypos;

		// Calculate the distance mouse traveled in Y axis
		float deltaY = endRightMouseY - startRightMouseY;

		// The ending point is the new starting point
		startRightMouseY = endRightMouseY;

		calculatePitch(deltaY);
	}

	if (leftMouseDown)
	{
		endLeftMouseX = (float)xpos;

		// Calculate the distance mouse traveled in X axis
		float deltaX = endLeftMouseX - startLeftMouseX;

		// The ending point is the new starting point
		startLeftMouseX = endLeftMouseX;

		calculateAngleOrbit(deltaX);
	}
}

void Camera::scroll_callback(double xoffset, double yoffset)
{
	// Zoom
	calculateZoom(yoffset);
}

void Camera::calculateCameraPosition(float horizontalDistance, float verticalDistance)
{
	float offsetX = horizontalDistance * glm::sin(glm::radians(angleOrbit));
	float offsetZ = horizontalDistance * glm::cos(glm::radians(angleOrbit));

	position.x = -offsetX;
	position.y = verticalDistance;
	position.z = -offsetZ;

	// Camera is always facing origin
	direction = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - position);
}

void Camera::calculateZoom(double yoffset)
{
	// Calculate how much to zoom
	float zoomLevel = (float)yoffset * ZOOM_RATE;

	// Set distance flooring it at minimum zoom level
	distance -= zoomLevel;
	if (distance < MIN_ZOOM)
		distance = MIN_ZOOM;
}

void Camera::calculatePitch(float offset)
{
	float pitchChange = offset * PITCH_RATE;
	pitch -= pitchChange;
}

void Camera::calculateAngleOrbit(float offset)
{
	float orbitChange = offset * ORBIT_RATE;
	angleOrbit += orbitChange;
}

float Camera::calculateHorizontalDistance()
{
	return distance * glm::cos(glm::radians(pitch));
}

float Camera::calculateVerticalDistance()
{
	return distance * glm::sin(glm::radians(pitch));
}