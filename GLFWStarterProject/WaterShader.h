#ifndef _WATERSHADER_H_
#define _WATERSHADER_H_

#include "ShaderProgram.h"
#include <glm/mat4x4.hpp> // glm::mat4

class WaterShader :
	public ShaderProgram
{
public:
	WaterShader(const char * vertex_file_path, const char * fragment_file_path);
	~WaterShader();

	void loadProjectionMatrix(glm::mat4 projectionMatrix);
	void loadViewMatrix(glm::mat4 viewMatrix);
	void loadModelMatrix(glm::mat4 modelMatrix);

	void loadClipPlane(glm::vec4 planeVector);

	void loadCameraPosition(glm::vec3 cameraPosition);

	void loadMoveFactor(float moveFactor);

	void connectTextureUnits();
private:
	GLint locationModelMatrix;
	GLint locationViewMatrix;
	GLint locationProjectionMatrix;
	GLint locationCameraPosition;

	// Clipping plane
	GLint locationPlane;

	GLint locationReflectionTexture;
	GLint locationRefractionTexture;

	GLint locationdudvMap;
	GLint locationMoveFactor;

	/*
		saveAllUniformLocations

			Gets all the uniform locations and saves them as class attributes.
		Uniform names have to be hard coded.
	*/
	void saveAllUniformLocations();
};

#endif