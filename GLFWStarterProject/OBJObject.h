#ifndef _OBJOBJECT_H_
#define _OBJOBJECT_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <vector>
#include <string>

class OBJObject
{
public:
	OBJObject(GLuint objVAO, int vertexCount);
	~OBJObject();

private:

	// VAO ID
	GLuint objVAO;

	// How many vertices this object has
	int vertexCount;
};

#endif