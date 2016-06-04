#ifndef _SHADERPROGRAM_H_
#define _SHADERPROGRAM_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <glm/mat4x4.hpp> // glm::mat4

class ShaderProgram
{
public:
	ShaderProgram(const char * vertex_file_path, const char * fragment_file_path);
	virtual ~ShaderProgram();

	void start();
	void stop();

	GLuint getShaderProgram();

private:
	GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);

protected:
	GLuint programID;

	GLint getUniformLocation(GLchar * uniformName);

	void loadMatrix4(GLint location, glm::mat4 matrix);

};

#endif