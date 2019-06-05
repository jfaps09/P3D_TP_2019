#version 440 core

layout(location = 0) in vec3 vertexPos_model;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNorm_model;

out vec2 UV;
out vec3 Position_world;
out vec3 Normal_cam;
out vec3 vPositionEyeSpace;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

void main(){
	gl_Position =  MVP * vec4(vertexPos_model,1);

	// Posição do vértice em coordenadas do olho.
	vPositionEyeSpace = (V * M * vec4(vertexPos_model, 1.0)).xyz;
	
	//Position of the vertex, in worldspace : M * position
	Position_world = (M * vec4(vertexPos_model,1)).xyz;
	
	//Vertex normal
	Normal_cam = ( V * M * vec4(vertexNorm_model,0)).xyz;
	UV = vertexUV;
}

