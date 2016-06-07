#ifndef _GEODE_H
#define _GEODE_H

#include "Node.h"
#include "OBJObject.h"

class Geode : public Node {

private:
	OBJObject *toDraw;
	GLuint shaderProgram;

	glm::mat4 M;

public:
	Geode(OBJObject* toDraw, GLuint shaderProgram);
	Geode(OBJObject* toDraw, glm::mat4 M, GLuint shaderProgram);
	~Geode();

	void update(glm::mat4 C);

	void draw();

	glm::mat4 getM();

};

#endif