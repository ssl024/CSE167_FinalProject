#include "WaterShader.h"



/*
	WaterShader

		Constructor for water shader.  Calls parent ProgramShader constructor.
*/
WaterShader::WaterShader(const char * vertex_file_path, const char * fragment_file_path) : ShaderProgram(vertex_file_path, fragment_file_path)
{
	saveAllUniformLocations();
}


WaterShader::~WaterShader()
{
}

void WaterShader::loadProjectionMatrix(glm::mat4 projectionMatrix)
{
	loadMatrix4(locationProjectionMatrix, projectionMatrix);
}

void WaterShader::loadViewMatrix(glm::mat4 viewMatrix)
{
	loadMatrix4(locationViewMatrix, viewMatrix);
}

void WaterShader::loadModelMatrix(glm::mat4 modelMatrix)
{
	loadMatrix4(locationModelMatrix, modelMatrix);
}

void WaterShader::loadClipPlane(glm::vec4 planeVector)
{
	loadVector4(locationPlane, planeVector);
}

void WaterShader::loadCameraPosition(glm::vec3 cameraPosition)
{
	loadVector3(locationCameraPosition, cameraPosition);
}

void WaterShader::loadMoveFactor(float moveFactor)
{
	loadFloat(locationMoveFactor, moveFactor);
}

void WaterShader::connectTextureUnits()
{
	loadInt(locationReflectionTexture, 0);
	loadInt(locationRefractionTexture, 1);
	loadInt(locationdudvMap, 2);
}

void WaterShader::saveAllUniformLocations()
{
	this->locationProjectionMatrix = getUniformLocation("projectionMatrix");
	this->locationViewMatrix = getUniformLocation("viewMatrix");
	this->locationModelMatrix = getUniformLocation("modelMatrix");
	this->locationPlane = getUniformLocation("clippingPlane");
	this->locationReflectionTexture = getUniformLocation("reflectionTexture");
	this->locationRefractionTexture = getUniformLocation("refractionTexture");
	this->locationdudvMap = getUniformLocation("dudvMap");
	this->locationMoveFactor = getUniformLocation("moveFactor");
	this->locationCameraPosition = getUniformLocation("cameraPosition");
}