#version 330 core

in vec2 UV;		//Interpolated values from the vertex shaders
out vec3 color;

uniform sampler2D textureSampler;

void main(){
	color = texture( textureSampler, UV ).rgb;		//color = color of the texture at the specified UV
}