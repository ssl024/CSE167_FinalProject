#ifndef _MODELLOADER_H_
#define _MODELLOADER_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "OBJObject.h"

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	OBJObject * loadToVAO(float positions[], int positionsLength, int dimensions);

private:

	std::vector<GLuint> vaos;
	std::vector<GLuint> vbos;

	int createVAO();

	void storeDataInAttributeList(int attributeNumber, int coordinateSize, float data[], int dataLength)
	{

	}

};

#endif