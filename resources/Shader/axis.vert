#version 330 core

uniform mat4 mvpMatrix;

layout (location = 0) in vec3 in_Position;
flat out vec4 to_FragColor;

void main(){
	gl_Position = mvpMatrix*vec4(in_Position, 1.0);
    to_FragColor = vec4(in_Position , 1.0);
};
