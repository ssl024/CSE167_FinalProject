#version 330 core
layout (location = 0) in vec3 position;
out vec3 FragPos;

uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 MVP2;

void main()
{
	gl_Position = MVP * vec4(position, 1.0f);
	FragPos = vec3(model * vec4(position, 1.0f)); 
} 