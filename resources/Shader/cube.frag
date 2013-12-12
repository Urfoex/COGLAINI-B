#version 330

smooth in vec3 vColor;
smooth in vec3 vPosition;
smooth in vec2 vTexCoords;

uniform uint useTexture;
uniform sampler2D theTexture;

struct Light{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 attenuation;
};

layout(std140) uniform Lights{
	Light light[8];
} lights;


layout(std140) uniform Material{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
} material;


out vec4 vFragmentColor;


void main(){
	vec4 matDiffuse2 = material.diffuse;
	vec4 matDiffuse = vec4(vPosition, 1.0);
// 	vec4 matDiffuse = vec4(1.0,0.0,0.0, 1.0);
	if( useTexture != uint(0)){
		matDiffuse = texture(theTexture, vTexCoords);
	}
	vFragmentColor = matDiffuse;
}