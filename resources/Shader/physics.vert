#version 330

// attributes per position:
// - position
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texcoords;

// uniforms
uniform mat4 modelViewProjectionMatrix;
// uniform mat4 modelMatrix;

uniform uint useTexture;
uniform sampler2D theTexture;

// out to fragment shader
smooth out vec3 vColor;
smooth out vec3 vPosition;
smooth out vec2 vTexCoords;

void main( ){
	vec4 newPosition = vec4( position, 1.0);
	
	// vertex pos in eye coords
// 	vec4 vertPos = modelMatrix * vec4( position, 1.0);
// 	vec4 vertPos = modelMatrix * newPosition;
// 	vec3 vertEyePos = vertPos.xyz/ vertPos.w;
	
// 	vPosition = vertEyePos;
	
	// transform geometry
// 	gl_Position = modelViewProjectionMatrix * vec4( position, 1.0);
// 	gl_Position = modelViewProjectionMatrix * newPosition;
	vColor = color;
	vPosition = position;
	vTexCoords = texcoords;
	mat4 m1;
	gl_Position = modelViewProjectionMatrix * newPosition;
}