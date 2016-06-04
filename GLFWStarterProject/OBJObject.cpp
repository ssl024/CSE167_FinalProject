#include "OBJObject.h"



OBJObject::OBJObject(GLuint objVAO, int vertexCount)
{
	this->objVAO = objVAO;
	this->vertexCount = vertexCount;
}


OBJObject::~OBJObject()
{
}
