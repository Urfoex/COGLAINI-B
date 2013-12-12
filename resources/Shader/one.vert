#version 150 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 in_Position;
out vec3 to_FragColor;

void main(){
	gl_Position = projectionMatrix*viewMatrix*modelMatrix*vec4(in_Position, 1.0);
//	gl_Position = projectionMatrix*viewMatrix*modelMatrix*vec4(in_Position,  0.0 , 1.0);
    to_FragColor = vec3(in_Position.xy, 0.3);//in_Position;
};
