#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class Camera
{
public:
	const float ZOOM_RATE = 2.0f;
	const float PITCH_RATE = 0.1f;
	const float MIN_ZOOM = 3.0f;
	const float ORBIT_RATE = 0.3f;

	Camera();
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up);
	~Camera();

	void update();

	glm::vec3 getPosition();
	glm::vec3 getDirection();
	glm::vec3 getUp();

	void setPosition(glm::vec3 newPosition);

	glm::mat4 getLookAt();

	float getPitch();
	float getYaw();
	float getRoll();

	// Key callback from Window to Camera
	void key_callback(int key, int scancode, int action, int mods);

	// Mouse callback from Window to Camera with mouse position
	// (0,0) is in the upper left corner
	void mouse_button_callback(int button, int action, int mods, double xPos, double yPos);

	// Cursor position callback from Window to Camera
	// (0,0) is in the upper left corner
	void cursor_position_callback(double xpos, double ypos);

	// Scroll callback from Window to Camera
	void scroll_callback(double xoffset, double yoffset);

private:
	// Y position upon clicking right mouse and holding left mouse button
	float startRightMouseY;
	float endRightMouseY;
	// X position upon clicking left mouse and holding left mouse button
	float startLeftMouseX;
	float endLeftMouseX;

	bool rightMouseDown = false;
	bool leftMouseDown = false;

	glm::vec3 position;		// e  | Position of camera
	glm::vec3 direction;	// d  | This is where the camera looks at
	glm::vec3 up;			// up | What orientation "up" is

	float distance = 0.0f;
	float angleOrbit = 0.0f;

	// Angle between ground and camera
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;

	/*
		calculateCameraPosition

			Calculates the camera's vector3 position using distances on the x,y plane

		horizontalDistance - Distance camera is from origin on x axis
		verticalDistance - Distance camera is from origin on y axis
	*/
	void calculateCameraPosition(float horizontalDistance, float verticalDistance);

	/*
		calculateZoom

			Calculates how much the camera should zoom by taking
		direction parameter from scroll callback.

		yoffset - Direction of the scroll
	*/
	void calculateZoom(double yoffset);

	/*
		calculatePitch

			Calculates how much the camera should pitch by taking the distance
		the mouse traveled in the y direction.

		offset - Distance mouse traveled in y direction
	*/
	void calculatePitch(float offset);

	/*
		calculateAngleOrbit

			Calculates how much the camera should orbit around by taking the
		ditance the mouse traveled in the x direction.

		offset - Distance mouse traveled in x direction
	*/
	void calculateAngleOrbit(float offset);

	/*
		calculateHorizontalDistance

			Calculates how far the camera is horizontally from the origin.

		Returns the distance.
	*/
	float calculateHorizontalDistance();

	/*
		calculateVerticalDistance	

			Calculates how far the camera is vertically from the origin.

		Returns the distance.
	*/
	float calculateVerticalDistance();
};

#endif _CAMERA_H_