#ifndef _WATERTILE_H_
#define _WATERTILE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "WaterShader.h"
#include "Camera.h"

class WaterTile
{
public:

	static const float TILE_SIZE;

	WaterTile(float centerX, float centerZ, float height, WaterShader * waterShader);
	~WaterTile();

	void draw(Camera * camera);

	float getHeight();
	float getX();
	float getZ();


private:
	GLuint VAO, VBO;

	glm::mat4 model;

	// Shader this water tile will use
	WaterShader * waterShader;

	float height;
	float x, z;

	void loadVAO();
	void prepareRender(Camera * camera);
	void unbind();
};

#endif _WATERTILE_H_