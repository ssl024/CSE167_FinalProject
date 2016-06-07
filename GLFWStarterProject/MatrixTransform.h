#ifndef _MATRIX_TRANSFORM_H
#define _MATRIX_TRANSFORM_H

#include "Group.h"

class MatrixTransform : public Group {

private:
	glm::mat4 M;

	glm::mat4 updateMatrix;

public:
	MatrixTransform(glm::mat4 M);
	~MatrixTransform();

	void update(glm::mat4 C);

	void setUpdateMatrix(glm::mat4 updateMatrix);
};

#endif