#ifndef _WATERTILE_H_
#define _WATERTILE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "WaterShader.h"
#include "Camera.h"
#include "WaterFrameBuffer.h"

class WaterTile
{
public:

	static const float TILE_SIZE;
	static const float WATER_SPEED;

	WaterTile(float centerX, float centerZ, float height, WaterShader * waterShader, WaterFrameBuffer * waterFB);
	~WaterTile();

	void draw(Camera * camera, glm::vec4 clipPlane);

	float getHeight();
	float getX();
	float getZ();


private:
	GLuint VAO, VBO;

	glm::mat4 model;

	// Shader this water tile will use
	WaterShader * waterShader;

	WaterFrameBuffer * waterFB;

	float height;
	float x, z;

	float moveFactor = 0.0f;

	void loadVAO();
	//void prepareRender(Camera * camera);
	void unbind();
};

#endif _WATERTILE_H_