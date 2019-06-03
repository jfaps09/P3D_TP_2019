#version 440 core

layout(location = 0) in vec3 vertexPos_model;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNorm_model;

out vec2 UV;
out vec3 Position_world;
out vec3 Normal_cam;
out vec3 EyeDirection_cam;
out vec3 LightDirection_cam;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightPos;

void main(){
	gl_Position =  MVP * vec4(vertexPos_model,1);
	
	//Position of the vertex, in worldspace : M * position
	Position_world = (M * vec4(vertexPos_model,1)).xyz;
	
	//Vector that goes from the vertex to the camera
	vec3 vertexPos_cam = ( V * M * vec4(vertexPos_model,1)).xyz;
	EyeDirection_cam = vec3(0,0,0) - vertexPos_cam;

	//Vector that goes from the vertex to the light
	vec3 LightPos_cam = ( V * vec4(lightPos,1)).xyz;
	LightDirection_cam = LightPos_cam + EyeDirection_cam;
	
	//Vertex normal
	Normal_cam = ( V * M * vec4(vertexNorm_model,0)).xyz;
	UV = vertexUV;
}

