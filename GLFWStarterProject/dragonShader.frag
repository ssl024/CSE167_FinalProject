#version 330 core

// This is a sample fragment shader.

in vec3 FragPos;
in vec3 Normal;

out vec4 color;

// Camera
uniform vec3 cameraPosition;

void main()
{
	float intensity;

	vec3 lightDirection = FragPos - vec3(100.0, 100.0, 100.0);

	intensity = dot(normalize(lightDirection), normalize(Normal));

	if (intensity > 0.95)
		color = vec4(1.0, 0.5, 0.5, 1.0);
	else if(intensity > 0.60)
		color = vec4(0.6, 0.3, 0.3, 1.0);
	else if(intensity > 0.20)
		color = vec4(0.4, 0.2, 0.2, 1.0);
	else
		color = vec4(0.0, 0.0, 0.0, 1.0);

	float edgeChecker = dot(normalize(cameraPosition), normalize(Normal));
	if(-0.50 < edgeChecker && edgeChecker < 0.50)
	{
	    color = vec4(0.0, 0.0, 0.0, 1.0);
	}
}
