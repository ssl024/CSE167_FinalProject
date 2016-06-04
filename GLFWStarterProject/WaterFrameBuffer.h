#ifndef _WATERFRAMEBUFFER_H_
#define _WATERFRAMEBUFFER_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class WaterFrameBuffer
{
public:
	static const int REFLECTION_WIDTH = 320;
	static const int REFLECTION_HEIGHT = 180;
	static const int REFRACTION_WIDTH = 1280;
	static const int REFRACTION_HEIGHT = 720;

	WaterFrameBuffer();
	~WaterFrameBuffer();

	void bindReflectionFrameBuffer();

	void bindRefractionFrameBuffer();

	void unbindCurrentFrameBuffer();

	GLuint getReflectionTexture();

	GLuint getRefractionTexture();

	GLuint getRefractionDepthTexture();


private:

	GLuint reflectionFrameBuffer;
	GLuint reflectionTexture;
	GLuint reflectionDepthBuffer;

	GLuint refractionFrameBuffer;
	GLuint refractionTexture;
	GLuint refractionDepthTexture;

	void initializeReflectionFrameBuffer();

	void initializeRefractionFrameBuffer();

	void bindFrameBuffer(GLuint frameBuffer, int width, int height);

	GLuint createFrameBuffer();

	GLuint createTextureAttachment(int width, int height);

	GLuint createDepthTextureAttachment(int width, int height);

	GLuint createDepthBufferAttachment(int width, int height);
};

#endif