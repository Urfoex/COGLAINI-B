#version 330 core

uniform mat4 projectionViewMatrix;
uniform mat4 modelMatrix;

uniform uint numberOfLights;

uniform uniformLights{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 Position;
	vec3 Attenuation;
} Lights[8];

in vec3 in_Position;
// uniform vec4 in_Color_Diffuse;

void main(){
//  Atten = 1/( att0 + att1 * d + att2 * d*d)

	gl_Position = projectionViewMatrix*modelMatrix*vec4(in_Position,1.0);
};
