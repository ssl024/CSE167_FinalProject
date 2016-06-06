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
#include <glm/vec3.hpp>

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

	void loadInt(GLint location, GLint number);

	void loadMatrix4(GLint location, glm::mat4 matrix);

	void loadVector3(GLint location, glm::vec3 vec3);

	void loadVector4(GLint location, glm::vec4 vector4);

	void loadFloat(GLint location, float floatValue);

};

#endif