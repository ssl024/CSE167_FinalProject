#include "Geode.h"

Geode::Geode(OBJObject* toDraw, GLuint shaderProgram)
{
	this->toDraw = toDraw;
	this->M = glm::mat4(1.0f);
	this->shaderProgram = shaderProgram;
}

Geode::Geode(OBJObject* toDraw, glm::mat4 M, GLuint shaderProgram)
{
	this->toDraw = toDraw;
	this->M = M;
	this->shaderProgram = shaderProgram;
}

Geode::~Geode() {}

void Geode::update(glm::mat4 C)
{
	this->M = C;
}

void Geode::draw()
{
	toDraw->setShaderProgram(shaderProgram);
	toDraw->draw(M);
}

glm::mat4 Geode::getM()
{
	return M;
}