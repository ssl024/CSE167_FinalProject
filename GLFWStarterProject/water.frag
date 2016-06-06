#version 330 core

in vec4 clipSpace;

in vec2 textureCoordinates;

out vec4 color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

uniform sampler2D dudvMap;

// Sample the dudvMap differently depending on time to get wave effect
uniform float moveFactor;

// How strong the distortion is
const float waveStrength = 0.03;

void main() {

	// Makes water coordinates x/y
	// Divide 2 and add 0.5 to make bottom left 0,0 and top right 1,1
	vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;

	// Map distortion to texture and we only care about red/green values
	vec2 distortion1 = (texture(dudvMap, vec2(textureCoordinates.x + moveFactor, textureCoordinates.y)).rg * 2.0 - 1.0) * waveStrength;
	vec2 distortion2 = (texture(dudvMap, vec2(textureCoordinates.x + moveFactor, textureCoordinates.y + 0.5)).rg * 2.0 - 1.0) * waveStrength;
	vec2 totalDistortion = distortion1 + distortion2;


	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);

	// Add the distortion to reflection/refraction
	reflectTexCoords += totalDistortion;
	refractTexCoords += totalDistortion;

	// Clamp reflection and refraction to prevent edge of screen texture problems
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture, refractTexCoords);

	color = mix(reflectColor, refractColor, 0.5);
	color = mix(color, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
}