#include "WaterTile.h"

#include "Window.h"

const float WaterTile::TILE_SIZE = 50.0f;

const float WaterTile::WATER_SPEED = 0.0001f;

GLfloat waterVertices[] = { -1, -1, 0,
							-1, 1, 0,
							1, -1, 0,
							1, -1, 0,
							-1, 1, 0,
							1, 1, 0 };

WaterTile::WaterTile(float centerX, float centerZ, float height, WaterShader * waterShader, WaterFrameBuffer * waterFB)
{
	this->x = centerX;
	this->z = centerZ;
	this->height = height;
	this->waterShader = waterShader;
	this->waterFB = waterFB;

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(TILE_SIZE, 1.0f, TILE_SIZE));
	model = glm::translate(scale, glm::vec3(x, height, z));

	loadVAO();
}


WaterTile::~WaterTile()
{
}

void WaterTile::draw(Camera * camera, glm::vec4 clipPlane)
{
	//prepareRender(camera);

	moveFactor += WATER_SPEED;
	if (moveFactor > 1.0f)
		moveFactor = 0.0f;

	// Activate texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterFB->getReflectionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, waterFB->getRefractionTexture());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, waterFB->getdudvMapTexture());

	// Activate shader program
	waterShader->start();

	glm::mat4 view = camera->getLookAt();
	glm::mat4 projection = Window::P;

	waterShader->loadModelMatrix(model);
	waterShader->loadViewMatrix(view);
	waterShader->loadProjectionMatrix(projection);
	waterShader->loadClipPlane(clipPlane);
	waterShader->loadCameraPosition(camera->getPosition());
	waterShader->loadMoveFactor(moveFactor);


	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	unbind();
}

void WaterTile::loadVAO()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Using the VAO
	glBindVertexArray(VAO);

	// VBO will take up an index of VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), &waterVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (x,y,z)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive vertex attributes. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

					 // Enable VBO 0
	glEnableVertexAttribArray(0);

	// Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO
	glBindVertexArray(0);
}


float WaterTile::getHeight()
{
	return this->height;
}

float WaterTile::getX()
{
	return this->x;
}

float WaterTile::getZ()
{
	return this->z;
}

void WaterTile::unbind()
{
	glBindVertexArray(0);
	waterShader->stop();
}