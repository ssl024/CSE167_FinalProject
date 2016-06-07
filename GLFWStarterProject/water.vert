#version 330 core

layout (location = 0) in vec3 position;

out vec4 clipSpace;
out vec2 textureCoordinates;
out vec3 toCameraVector;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

// Used for the Fresnel Effect
uniform vec3 cameraPosition;

// Plane used for clipping above/underneath water
uniform vec4 clippingPlane;

// Creates tiling
const float tiling = 1.0;

void main() {
	clipSpace = projectionMatrix * viewMatrix * modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
	gl_Position = clipSpace;

	vec4 worldPosition = modelMatrix * vec4(position.x, 0.0, position.y, 1.0);

	gl_ClipDistance[0] = dot(worldPosition, clippingPlane);

	textureCoordinates = vec2(position.x/2.0 + 0.5, position.y/2.0 + 0.5) * tiling;

	toCameraVector = cameraPosition - worldPosition.xyz;
}