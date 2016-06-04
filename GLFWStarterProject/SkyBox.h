#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>   // include math functions, such as sin, cos, M_PI
#include <iostream> // allow c++ style console printouts

class SkyBox {

private:

	GLuint skyBoxVAO, skyBoxVBO;
	GLuint textureId;
	GLuint skyBoxShader;

	unsigned char* loadPPM(const char* filename, int& width, int& height);
	GLuint loadSkyBox(std::vector<const GLchar*> faces);

public:
	SkyBox(std::vector<const GLchar*> faces);
	~SkyBox();

	void draw();
	void setSkyBoxShader(GLuint skyBoxShader);
};

#endif