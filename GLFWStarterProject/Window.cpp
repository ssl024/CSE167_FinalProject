#include "window.h"

const char* window_title = "GLFW Starter Project";

// Objects
Cube * cube;
OBJObject * cylinderObj;
OBJObject * podObj;
OBJObject * bearObj;
WeddingCake * weddingCake;

// Camera
Camera * camera;

// Skybox
SkyBox * skyBox;

// Water
WaterTile * water;

// Frame Buffer
WaterFrameBuffer * waterFB;

// Shader Programs
ShaderProgram * cubeShader;
ShaderProgram * skyBoxShader;
WaterShader * waterShader;
ShaderProgram * toonShader;

// Flags
bool stop = false;

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
	// Camera with default parameters
	camera = new Camera();

	// Objects
	cube = new Cube();
	cylinderObj = new OBJObject("cylinder.obj");
	podObj = new OBJObject("pod.obj");
	bearObj = new OBJObject("bear.obj");

	// Sky box faces
	std::vector<const GLchar*> faces;
	// I had to rotate clockwise to get the correct orientations
	faces.push_back("cloudtop_ft.ppm"); // right
	faces.push_back("cloudtop_bk.ppm"); // left
	faces.push_back("cloudtop_up.ppm");
	faces.push_back("cloudtop_dn.ppm");
	faces.push_back("cloudtop_rt.ppm"); // back
	faces.push_back("cloudtop_lf.ppm"); // front 

	// Create sky box object
	skyBox = new SkyBox(faces);

	// Load the shader program. Similar to the .obj objects, different platforms expect a different directory for files
#ifdef _WIN32 // Windows (both 32 and 64 bit versions)
	cubeShader = new ShaderProgram("shader.vert", "shader.frag");
	skyBoxShader = new ShaderProgram("skyBoxShader.vert", "skyBoxShader.frag");
	waterShader = new WaterShader("water.vert", "water.frag");
	toonShader = new ShaderProgram("toonShader.vert", "toonShader.frag");
#else // Not windows
	shaderProgram = LoadShaders("shader.vert", "shader.frag");
#endif

	// Object shaders
	cylinderObj->setShaderProgram(toonShader->getShaderProgram());
	podObj->setShaderProgram(toonShader->getShaderProgram());
	bearObj->setShaderProgram(toonShader->getShaderProgram());

	// Create wedding cake
	weddingCake = new WeddingCake(cylinderObj, podObj, bearObj, toonShader->getShaderProgram());

	// Set skybox shader
	skyBox->setSkyBoxShader(skyBoxShader->getShaderProgram());

	// Create water frame buffer
	waterFB = new WaterFrameBuffer("waterdudv.ppm");

	// Create water
	water = new WaterTile(0.0f, 0.0f, 0.0f, waterShader, waterFB);

	
}

void Window::clean_up()
{
	// Delete objects
	delete(cube);
	delete(skyBox);

	// Delete shaders
	delete(cubeShader);
	delete(skyBoxShader);
	delete(waterShader);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);

		if (camera != nullptr)
		{
			V = camera->getLookAt();
		}
		else
		{
			// Initialize default
			glm::vec3 position(0.0f, 0.0f, 20.0f);
			glm::vec3 direction(0.0f, 0.0f, 0.0f);
			glm::vec3 up(0.0f, 1.0f, 0.0f);

			V = glm::lookAt(position, direction, up);
		}
	}
}

void Window::idle_callback()
{
	camera->update();
	V = camera->getLookAt();

	// Update wedding cake
	if(!stop)
		weddingCake->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CLIP_DISTANCE0);

	

	// Get reflection
	waterFB->bindReflectionFrameBuffer();

	// Move camera below water
	float distance = 2 * (camera->getPosition().y - water->getHeight());
	camera->setPosition(glm::vec3(camera->getPosition().x, camera->getPosition().y - distance, camera->getPosition().z));

	skyBox->draw();

	// Cube
	cubeShader->start();
	cube->draw(cubeShader->getShaderProgram());
	cubeShader->stop();

	// Wedding cake
	toonShader->start();
	weddingCake->draw();
	toonShader->stop();

	water->draw(camera, glm::vec4(0.0f, 1.0f, 0.0f, -water->getHeight() - 0.2f));

	// Move camera back to normal
	camera->setPosition(glm::vec3(camera->getPosition().x, camera->getPosition().y + distance, camera->getPosition().z));

	// Get refraction
	waterFB->bindRefractionFrameBuffer();

	skyBox->draw();

	// Cube
	cubeShader->start();
	cube->draw(cubeShader->getShaderProgram());
	cubeShader->stop();

	// Wedding cake
	toonShader->start();
	weddingCake->draw();
	toonShader->stop();

	water->draw(camera, glm::vec4(0.0f, -1.0f, 0.0f, water->getHeight() + 0.2f));

	// Get entire scene
	waterFB->unbindCurrentFrameBuffer();
	glDisable(GL_CLIP_DISTANCE0);
	

	skyBox->draw();

	// Cube
	cubeShader->start();
	cube->draw(cubeShader->getShaderProgram());
	cubeShader->stop();

	// Wedding cake
	toonShader->start();
	weddingCake->draw();
	toonShader->stop();

	water->draw(camera, glm::vec4(0.0f, -1.0f, 0.0f, 100.0f));

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Inform camera of key
	camera->key_callback(key, scancode, action, mods);

	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		// Stop key
		if (key == GLFW_KEY_S)
		{
			stop = !stop;
		}
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Position of the mouse
	double xPos, yPos;

	// Get mouse position
	glfwGetCursorPos(window, &xPos, &yPos);

	// Inform camera of mouse
	camera->mouse_button_callback(button, action, mods, xPos, yPos);
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Inform camera of cursor position
	camera->cursor_position_callback(xpos, ypos);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Inform camera of scroll
	camera->scroll_callback(xoffset, yoffset);
}

Camera * Window::getCamera()
{
	return camera;
}