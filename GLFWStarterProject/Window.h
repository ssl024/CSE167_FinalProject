#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "ShaderProgram.h"
#include "WaterShader.h"
#include "Camera.h"
#include "SkyBox.h"
#include "WaterTile.h"

class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P;
	static glm::mat4 V;
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	// Mouse callback
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	// Cursor position callback
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	// Scroll callback
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif
