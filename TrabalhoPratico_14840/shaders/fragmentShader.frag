#version 330 core

in vec2 UV;
in vec3 Position_world;
in vec3 Normal_cam;
in vec3 EyeDirection_cam;
in vec3 LightDirection_cam;

out vec3 color;

uniform vec3 random;
uniform sampler2D textureSampler;
uniform mat4 MV;
uniform vec3 lightPos;

void main(){
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 40.0f;
	
	vec3 DiffuseColor = texture( textureSampler, UV ).rgb * random;
	vec3 AmbientColor = vec3(0.15,0.15,0.15) * DiffuseColor;
	vec3 SpecularColor = vec3(0.2,0.2,0.2);

	//Calculate distance to the light source
	float distance = length( lightPos - Position_world );
	vec3 n = normalize( Normal_cam );
	vec3 l = normalize( LightDirection_cam );	//Direction of the light

	//Cosine of the angle between the normal and the light direction, 
	//- light is at the vertical of the triangle -> 1
	//- light is perpendicular to the triangle -> 0
	//- light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );

	vec3 EyeVec = normalize(EyeDirection_cam);

	//Direction in which the triangle reflects the light
	vec3 ReflectVec = reflect(-l,n);

	//Cosine of the angle between the Eye vector and the Reflect vector,
	//- Looking into the reflection -> 1
	//- Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( EyeVec,ReflectVec ), 0,1 );
	
	color = AmbientColor +
			DiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
			SpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
}