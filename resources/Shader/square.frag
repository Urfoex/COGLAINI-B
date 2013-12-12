#version 150 core
in vec3 to_FragColor;
out vec4 frag_Color;

void main(){
	frag_Color = vec4( 1.0, 1.0, 1.0, 1.0);
//	frag_Color = vec4( to_FragColor, 1.0);
};
