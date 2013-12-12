#version 330 core
uniform vec4 in_Color_Diffuse;
uniform vec4 in_Color_Specular;
uniform vec4 in_Color_Ambient;
uniform vec4 in_Color_Emit;
out vec4 frag_Color;

void main(){
	frag_Color = in_Color_Diffuse;
};
