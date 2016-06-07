#version 330 core

in vec4 clipSpace;
in vec2 textureCoordinates;
in vec3 toCameraVector;

out vec4 color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

uniform sampler2D dudvMap;
uniform sampler2D normalMap;


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

	/* Other distortion technique
	vec2 distortedTexCoords = texture(dudvMap, vec2(textureCoordinates.x + moveFactor, textureCoordinates.y)).rg * 0.1;
	distortedTexCoords = textureCoordinates + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
	vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;
	*/

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

	// Calculate how refractive water should be
	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));
	// Change how reflective water is
	refractiveFactor = pow(refractiveFactor, 5.0);

	// Normal map
	vec4 normalMapColor = texture(normalMap, totalDistortion);
	// Extract normals from map
	vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g * 2.0 - 1.0);
	normal = normalize(normal);

	color = mix(reflectColor, refractColor, refractiveFactor);
	// Make water bluer
	color = mix(color, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
}