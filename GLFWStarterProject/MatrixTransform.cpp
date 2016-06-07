#include "MatrixTransform.h"

MatrixTransform::MatrixTransform(glm::mat4 M)
{
	this->M = M;
	this->updateMatrix = glm::mat4(1.0f);
}

MatrixTransform::~MatrixTransform() {}

void MatrixTransform::update(glm::mat4 C)
{
	glm::mat4 newC = C * updateMatrix * M;

	Group::update(newC);
}

void MatrixTransform::setUpdateMatrix(glm::mat4 updateMatrix)
{
	this->updateMatrix = updateMatrix;
}