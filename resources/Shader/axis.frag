#version 330 core
flat in vec4 to_FragColor;
out vec4 frag_Color;

void main(){
	frag_Color = to_FragColor;
};
