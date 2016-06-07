#ifndef _OBJOBJECT_H_
#define _OBJOBJECT_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "Camera.h"

struct Vertex {
	glm::vec3 position, normal;
};

class OBJObject
{
private:
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<int> faces;
	std::vector<Vertex> posNorm;

	// Max of vertices
	float max[3];
	// Min of vertices
	float min[3];

	GLuint shaderProgram;

	glm::mat4 toWorld;

public:
	OBJObject(const char* filepath);
	~OBJObject();

	void parse(const char* filepath);

	GLuint VBO, VAO, EBO, NBO;
	float angle;

	void draw();
	void draw(glm::mat4 transformMatrix);
	void update();

	void setShaderProgram(GLuint shaderProgram);
	GLuint getShaderProgram();
};

#endif

