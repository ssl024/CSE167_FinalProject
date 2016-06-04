#version 330 core

layout (location = 0) in vec3 position;

//out vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform mat4 MVP;


void main() {

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
	//gl_Position = MVP * vec4(position.x, 0.0, position.y, 1.0);
	//textureCoords = vec2(position.x/2.0 + 0.5, position.y/2.0 + 0.5);
 
}